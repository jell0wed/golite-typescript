#include <golite/for.h>
#include <golite/utils.h>
#include <sstream>

std::string golite::For::toGoLite(int indent) {
    std::stringstream ss;
    ss << golite::Utils::indent(indent) << "for";
    if(left_simple_ || right_simple_) {
        ss << "( ";
        if(left_simple_) {
            ss << left_simple_->toGoLite(0);
        }
        ss << "; ";

        if(expression_) {
            ss << expression_->toGoLite(0);
        }
        ss << "; ";

        if(right_simple_) {
            ss << right_simple_->toGoLite(0);
        }
        ss << ")";
    } else if(expression_) {
        ss << expression_->toGoLite(0);
    }
    ss << " {" << std::endl;
    for(Statement* statement : block_->getStatements()) {
        ss << statement->toGoLite(indent+1) << std::endl;
    }
    ss << "}";
    return ss.str();
}