#include <golite/unary.h>
#include <golite/utils.h>
#include <sstream>
#include <golite/identifier.h>
#include <golite/primary_expression.h>

std::string golite::Unary::toGoLite(int indent) {
    std::stringstream ss;
    ss << golite::Utils::indent(indent) << "(";
    switch (kind_) {
        case MINUS:
            ss << "-";
            break;
        case NOT:
            ss << "!";
            break;
        case PLUS:
            ss << "+";
            break;
        case XOR:
            ss << "^";
            break;
    }
    ss << operand_->toGoLite(0) << ")";
    return ss.str();
}

int golite::Unary::getLine() {
    return operand_->getLine();
}

void golite::Unary::weedingPass(bool, bool) {
    if(operand_->isBlank()) {
        golite::Utils::error_message("Unary operand cannot be a blank identifier", operand_->getLine());
    }
    operand_->weedingPass(false, false);
}

void golite::Unary::symbolTablePass(SymbolTable *root) {
    this->operand_->symbolTablePass(root);
}