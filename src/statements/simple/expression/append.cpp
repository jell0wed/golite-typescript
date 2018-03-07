#include <golite/append.h>
#include <golite/utils.h>
#include <sstream>
#include <golite/identifier.h>
#include <golite/primary_expression.h>

std::string golite::Append::toGoLite(int indent) {
    std::stringstream ss;
    ss << golite::Utils::indent(indent) << "append("
       << left_expression_->toGoLite(0) << ", "
       << right_expression_->toGoLite(0) << ")";
    return ss.str();
}

int golite::Append::getLine() {
    return left_expression_->getLine();
}

void golite::Append::weedingPass(bool, bool) {
    if(left_expression_->isBlank()) {
        golite::Utils::error_message("Append does not accept left argument to be a blank identifier",
                                     left_expression_->getLine());
    }

    if(right_expression_->isBlank()) {
        golite::Utils::error_message("Append does not accept right argument to be a blank identifier",
                                     right_expression_->getLine());
    }

    left_expression_->weedingPass(false, false);
    right_expression_->weedingPass(false, false);
}

void golite::Append::symbolTablePass(SymbolTable *root) {
    this->left_expression_->symbolTablePass(root);
    this->right_expression_->symbolTablePass(root);
}