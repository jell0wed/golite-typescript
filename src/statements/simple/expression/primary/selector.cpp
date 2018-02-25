#include <golite/selector.h>
#include <golite/utils.h>
#include <sstream>

std::string golite::Selector::toGoLite(int indent) {
    std::stringstream ss;
    ss << "." << identifier_->toGoLite(0);
    return ss.str();
}
