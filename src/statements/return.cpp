#include <golite/return.h>
#include <golite/utils.h>
#include <sstream>
#include <golite/identifier.h>
#include <golite/primary_expression.h>

std::string golite::Return::toGoLite(int indent) {
    std::stringstream ss;
    ss << golite::Utils::indent(indent) << "return";
    if(expression_) {
        ss << " " << expression_->toGoLite(0);
    }
    ss << ";";
    return ss.str();
}

void golite::Return::weedingPass(bool check_break, bool check_continue) {
    if(expression_) {
        if(expression_->isIdentifier()) {
            golite::PrimaryExpression* primary_expression = static_cast<PrimaryExpression*>(expression_);
            golite::Identifier* identifier = static_cast<Identifier*>(primary_expression->lastChild());
            if(identifier->isBlank()) {
                golite::Utils::error_message("A return statement cannot return a blank identifier", identifier->getLine());
            }
        }
        expression_->weedingPass(check_break, check_continue);
    }
}
