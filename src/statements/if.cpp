#include <golite/if.h>
#include <golite/utils.h>
#include <sstream>
#include <iostream>
#include <golite/primary_expression.h>
#include <golite/identifier.h>

std::string golite::If::toGoLite(int indent) {
    std::stringstream ss;

    // If statement
    ss << golite::Utils::indent(indent) << "if ";
    if(simple_) {
        ss << simple_->toGoLite(0) << "; ";
    }
    ss << expression_->toGoLite(0) << " {";

    // Statement body
    if(!block_->getStatements().empty()) {
        ss << std::endl;
        for(Statement* statement : block_->getStatements()) {
            ss << statement->toGoLite(indent+1) << std::endl;
        }
        ss << golite::Utils::indent(indent);
    }
    ss << "}";

    // Else if statements
    for(If* else_if : else_if_) {

        ss << " else if ";
        if(else_if->simple_) {
            ss << else_if->simple_->toGoLite(0) << "; ";
        }
        ss << else_if->expression_->toGoLite(0) << " {";

        // Statement body
        if(!else_if->block_->getStatements().empty()) {
            ss << std::endl;
            for(Statement* statement : else_if->block_->getStatements()) {
                ss << statement->toGoLite(indent+1) << std::endl;
            }
            ss << golite::Utils::indent(indent);
        }
        ss << "}";
    }

    // Else statement
    if(else_) {
        ss << " else {";
        if(!else_->getStatements().empty()) {
            ss << std::endl;
            for(Statement* statement : else_->getStatements()) {
                ss << statement->toGoLite(indent+1) << std::endl;
            }
            ss << golite::Utils::indent(indent);
        }
        ss << "}";
    }

    return ss.str();
}

void golite::If::weedingPass(bool check_break, bool check_continue) {
    if(simple_) {
        if(simple_->isExpression()) {
            golite::Expression* expression = static_cast<Expression*>(simple_);
            if(expression->isBlank()) {
                golite::Utils::error_message("If statement initial statement cannot be a blank identifier",
                                             simple_->getLine());
            }
        }
        simple_->weedingPass(false, false);
    }

    if(expression_->isBlank()) {
        golite::Utils::error_message("If statement expression cannot be a blank identifier", expression_->getLine());
    }
    expression_->weedingPass(false, false);

    for(If* else_if_stmt : else_if_) {
        else_if_stmt->weedingPass(check_break, check_continue);
    }

    if(else_) {
        else_->weedingPass(check_break, check_continue);
    }

    block_->weedingPass(check_break, check_continue);
}

void golite::If::symbolTablePass(SymbolTable *root) {
    if(this->simple_) {
        this->simple_->symbolTablePass(root);
    }

    this->expression_->symbolTablePass(root);

    for(golite::If* else_if_stmt : this->else_if_) {
        else_if_stmt->symbolTablePass(root);
    }

    if(this->else_) {
        this->else_->symbolTablePass(root);
    }

    SymbolTable* if_symbol_table = new SymbolTable();
    root->addChild(if_symbol_table);
    this->block_->symbolTablePass(if_symbol_table);
}