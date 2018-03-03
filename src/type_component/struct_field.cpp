#include <golite/struct_field.h>
#include <golite/utils.h>
#include <golite/pretty_helper.h>

std::string golite::StructField::toGoLite(int indent) {
    std::stringstream ss;
    ss << golite::Utils::indent(indent) << golite::Pretty::implodeIdentifiers(identifiers_);
    ss << " " << type_component_->toGoLite(indent) << ";";
    return ss.str();
}

void golite::StructField::weedingPass() {
    for(Identifier* identifier : identifiers_) {
        identifier->weedingPass();
    }
    type_component_->weedingPass();
}