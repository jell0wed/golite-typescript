#include <golite/index.h>
#include <golite/utils.h>
#include <sstream>

std::string golite::Index::toGoLite(int indent) {
    std::stringstream ss;
    ss << "[ " << expression_->toGoLite(0) << " ]";
    return ss.str();
}