#include <golite/variable.h>
#include <golite/utils.h>
#include <golite/pretty_helper.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <golite/program.h>

long golite::Variable::indexOfIdentifier(std::string id) {
    std::vector<golite::Identifier*>::iterator found_id_itt =
            std::find_if(this->identifiers_.begin(), this->identifiers_.end(), [&id](const golite::Identifier* c_id) {
                return c_id->getName() == id;
            });
    if(found_id_itt == this->identifiers_.end()) {
        return -1;
    }

    return std::distance(this->identifiers_.begin(), found_id_itt);
}

void golite::Variable::replaceExpression(int index, Expression *expr) {
    this->expressions_[index] = expr;
}

std::string golite::Variable::toGoLite(int indent) {
    std::stringstream ss;
    ss << golite::Utils::indent(indent) << "var " << golite::Pretty::implodeIdentifiers(identifiers_);
    if(!type_component_->isInfer()) {
        ss << " " << type_component_->toGoLite(indent);
    }
    if(!expressions_.empty()) {
        ss << " = ";
    }
    ss << golite::Pretty::implodeExpressions(expressions_) << ";";
    return ss.str();
}

int golite::Variable::getLine() {
    if(identifiers_.empty()) {
        throw std::runtime_error("Cannot get line number of variable with no identifiers");
    }
    return identifiers_.front()->getLine();
}

void golite::Variable::weedingPass(bool, bool) {
    if(!expressions_.empty() && identifiers_.size() != expressions_.size()) {
        golite::Utils::error_message("Number of left and right elements of variable declaration does not match",
                                     getLine());
    }

    type_component_->weedingPass();
    for(Identifier* identifier : identifiers_) {
        identifier->weedingPass();
    }

    for(Expression* expression : expressions_) {
        if(expression->isBlank()) {
            golite::Utils::error_message("Variable value cannot be a blank identifier", expression->getLine());
        }
        expression->weedingPass();
    }
}

void golite::Variable::typeCheck() {
    if(!expressions_.empty()) {
        if(identifiers_.size() != expressions_.size()) {
            throw std::runtime_error("Cannot perform type checking on variable statement with different number of "
                                             "identifiers and expressions");
        }

        for(size_t i=0; i < identifiers_.size(); i++) {
            TypeComponent* expression_type = expressions_[i]->typeCheck();
            if(!identifiers_[i]->isBlank()) {
                if(type_component_->isInfer()) {
                    identifiers_[i]->updateTypeInSymbolTable(expression_type);
                } else if(!type_component_->isCompatible(expression_type)) {
                    golite::Utils::error_message("Variable " + identifiers_[i]->getName()
                                                 + " expects an expression of type " + type_component_->toGoLite(0)
                                                 + " but given " + expression_type->toGoLite(0),
                                                 identifiers_[i]->getLine());
                }
            }
        }
    }
}

void golite::Variable::symbolTablePass(SymbolTable *root) {
    for(golite::Identifier* id : this->identifiers_) {
        if(!id->isBlank()) {
            // search for an existing symbol in current scope
            if(root->hasSymbol(id->getName(), false)) {
                golite::Utils::error_message("Variable name " + id->getName() + " redeclared in this block", getLine());
            }
            root->putSymbol(id->getName(), this);
        }
    }

    if(!type_component_->isInfer()) {
        type_component_->symbolTablePass(root);
    }

    for(golite::Expression* expression : expressions_) {
        expression->symbolTablePass(root);
    }
}

std::string golite::Variable::toPrettySymbol() {
    std::stringstream ss;
    if(identifiers_.size() != 1) {
        throw std::runtime_error("Symbol table variables must be normalized to one identifier per variable");
    }
    ss << identifiers_.front()->getName() << " [variable]";
    if(type_component_->isInfer()) {
        ss << " -> <infer>";
    } else {
        ss << type_component_->toPrettySymbol();
    }
    return ss.str();
}