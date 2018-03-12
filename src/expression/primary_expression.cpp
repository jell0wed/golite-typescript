#include <golite/primary_expression.h>
#include <golite/utils.h>
#include <golite/identifier.h>
#include <sstream>
#include <iostream>
#include <golite/selector.h>
#include <golite/parenthesis.h>
#include <golite/declarable.h>

void golite::PrimaryExpression::addChild(golite::Primary *child) {
    children_.push_back(child);
}

std::string golite::PrimaryExpression::toGoLite(int indent) {
    std::stringstream ss;
    ss << golite::Utils::indent(indent);
    for(Expression* primary : children_) {
        ss << primary->toGoLite(0);
    }
    return ss.str();
}

int golite::PrimaryExpression::getLine() {
    if(children_.empty()) {
        throw std::runtime_error("Cannot get line number of an empty primary");
    }
    return children_.front()->getLine();
}

bool golite::PrimaryExpression::isFunctionCall() {
    if(children_.empty()) {
        throw std::runtime_error("Cannot check if primary expression is a function call because children list is empty");
    }
    return children_.back()->isFunctionCall();
}

bool golite::PrimaryExpression::isIdentifier() {
    // TODO Check if we can resolve inside parenthesis. Refactor required if applied
    // TODO Or maybe create another method called resolvesToIdentifier()
    if(children_.empty()) {
        throw std::runtime_error("Cannot check if primary expression is an identifier because children list is empty");
    }
    if(children_.size() != 1) {
        return false;
    }
    return children_.back()->isIdentifier();
}

bool golite::PrimaryExpression::isBlank() {
    if(!isIdentifier()) {
        return false;
    }
    return children_.back()->isBlank();
}

void golite::PrimaryExpression::weedingPass() {
    if (children_.empty()) {
        throw std::runtime_error("Cannot perform weeding pass on primary expression because children list is empty");
    }

    if (children_.front()->isBlank() && children_.size() > 1) {
        golite::Utils::error_message("Blank identifier cannot be accessed", children_.front()->getLine());
    }

    for (Primary *primary : children_) {
        primary->weedingPass();
    }
}

golite::TypeComponent* golite::PrimaryExpression::typeCheck() {
    if(children_.empty()) {
        throw std::runtime_error("Cannot perform type checking on a primary expression with an empty list of children");
    }

    // Get the type of the first element
    Primary* base_expression = nullptr;
    std::vector<golite::TypeComposite*> type_composites;
    for(size_t i = 0; i < children_.size(); i++) {
        TypeComponent* child_type = children_[i]->typeCheck();
        if(children_[i]->isSelector()) {
            if(base_expression->isIdentifier()) {
                // TODO
            } else if(base_expression->isSelector()) {
                // TODO
            } else {
                golite::Utils::error_message("Cannot access member of a non identifier "
                                             + base_expression->toGoLite(0), children_[i]->getLine());
            }
            base_expression = children_[i];

        } else if(children_[i]->isIdentifier()) {
            std::vector<TypeComposite*> type_children = child_type->resolveChildren();
            type_composites.insert(type_composites.end(), type_children.begin(), type_children.end());
            base_expression = children_[i];

        } else if(children_[i]->isLiteral()) {
            std::vector<TypeComposite*> type_children = child_type->getChildren();
            base_expression = children_[i];

        } else if(children_[i]->isIndex()) {
            if(base_expression->isIdentifier() || base_expression->isAppend() || base_expression->isParenthesis()) {
                // TODO Compare stack type
            } else {
                golite::Utils::error_message("Cannot access index of " + base_expression->toGoLite(0),
                                             children_[i]->getLine());
            }

        } else if(children_[i]->isFunctionCall()) {
            if(children_[i-1]->isIdentifier()) {
                Identifier *identifier = static_cast<Identifier *>(children_[i - 1]);
                Declarable *id_declarable = identifier->getSymbolTableEntry();
                if (!id_declarable->isFunction()) {
                    golite::Utils::error_message("Cannot call a non-function identifier " + children_[i-1]->toGoLite(0),
                                                 children_[i]->getLine());
                }
            } else if(children_[i-1]->isParenthesis()) {
                Parenthesis *parenthesis = static_cast<Parenthesis*>(children_[i-1]);
                Expression* par_expr = parenthesis->resolveExpression();
                if(!par_expr->isIdentifier()) {
                    golite::Utils::error_message("Cannot call a non-function parenthesis expression "
                                                 + children_[i-1]->toGoLite(0), children_[i]->getLine());
                }
                PrimaryExpression* primary_expression = static_cast<PrimaryExpression*>(par_expr);
                Identifier* identifier = static_cast<Identifier*>(primary_expression->getChildren().front());
                Declarable *id_declarable = identifier->getSymbolTableEntry();
                if (!id_declarable->isFunction()) {
                    golite::Utils::error_message("Cannot call a non-function parenthesis identifier "
                                                 + identifier->toGoLite(0), children_[i]->getLine());
                }
            } else {
                golite::Utils::error_message("Invalid function call", children_[i]->getLine());
            }

        } else if(children_[i]->isAppend()) {
            std::vector<TypeComposite*> type_children = child_type->resolveChildren();
            type_composites.insert(type_composites.end(), type_children.begin(), type_children.end());
            base_expression = children_[i];

        } else if(children_[i]->isParenthesis()) {
            base_expression = children_[i];

        } else {
            throw std::runtime_error("Unhandled type check for an unrecognized child type");
        }
    }
    return children_.front()->typeCheck();
}

void golite::PrimaryExpression::symbolTablePass(SymbolTable *root) {
    if(children_.empty()) {
        throw std::runtime_error("Cannot perform symbol table pass on a primary expression with an empty list of children");
    }
    children_.front()->symbolTablePass(root);
    // The rest of the primary expression children are handled by the type checking pass
}

bool golite::PrimaryExpression::isParenthesis() {
    if(children_.empty()) {
        throw std::runtime_error("Cannot check if primary expression is a parenthesis because children list is empty");
    }
    if(children_.size() != 1) {
        return false;
    }
    return children_.back()->isParenthesis();
}