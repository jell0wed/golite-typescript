#include <golite/selector.h>
#include <golite/utils.h>
#include <sstream>

std::string golite::Selector::toGoLite(int indent) {
    std::stringstream ss;
    ss << golite::Utils::indent(indent) << "." << identifier_->toGoLite(0);
    return ss.str();
}

int golite::Selector::getLine() {
    return identifier_->getLine();
}

void golite::Selector::weedingPass(bool, bool) {
    if(identifier_->isBlank()) {
        golite::Utils::error_message("Selector cannot be a blank identifier", identifier_->getLine());
    }
    identifier_->weedingPass(false, false);
}

void golite::Selector::symbolTablePass(SymbolTable *root) {
    // typecheck checks that the field selected is valid
}