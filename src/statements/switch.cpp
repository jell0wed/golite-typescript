#include <golite/switch.h>
#include <golite/utils.h>
#include <sstream>
#include <golite/declaration.h>
#include <golite/primary_expression.h>
#include <golite/simple_expression.h>

std::string golite::Switch::toGoLite(int indent) {
    std::stringstream ss;

    // Switch statement
    ss << golite::Utils::indent(indent) << "switch";
    if(simple_) {
        ss << " " << simple_->toGoLite(0) << ";";
    }
    if(expression_) {
        ss << " " << expression_->toGoLite(0);
    }
    ss << " {";

    // Cases
    if(!cases_.empty()) {
        ss << std::endl;
        for(SwitchCase* switch_case : cases_) {
            ss << switch_case->toGoLite(indent+1) << std::endl;
        }
        ss << golite::Utils::indent(indent);
    }
    ss << "}";
    return ss.str();
}

void golite::Switch::weedingPass(bool, bool check_continue) {
    if(simple_) {
        if(simple_->isSimpleExpression()) {
            golite::SimpleExpression* simple_expression = static_cast<SimpleExpression*>(simple_);
            if(simple_expression->getExpression()->isBlank()) {
                golite::Utils::error_message("Switch statement initial statement cannot be a blank identifier",
                                             simple_->getLine());
            }
        }
        simple_->weedingPass(false, false);
    }

    if(expression_) {
        if(expression_->isBlank()) {
            golite::Utils::error_message("Switch statement expression cannot be a blank identifier",
                                         expression_->getLine());
        }
        expression_->weedingPass();
    }

    bool has_default = false;
    for(SwitchCase* switch_case : cases_) {
        if(switch_case->isDefault()) {
            if(has_default) {
                golite::Utils::error_message("Switch statement has more than one default case",
                                             switch_case->getLine());
            }
            has_default = true;
        }
        switch_case->weedingPass(check_continue);
    }
}

void golite::Switch::typeCheck() {
    if (simple_) {
        simple_->typeCheck();
    }

    if (expression_) {
        TypeComponent *type_component_ = expression_->typeCheck();
        // TODO Check if it's booklean
    }

    for (SwitchCase *switch_case : cases_) {
        switch_case->typeCheck();
    }
}

void golite::Switch::symbolTablePass(golite::SymbolTable *root) {
    symbol_table_ = new SymbolTable();
    root->addChild(symbol_table_);

    if(this->simple_) {
        this->simple_->symbolTablePass(symbol_table_);
    }

    if(this->expression_) {
        this->expression_->symbolTablePass(symbol_table_);
    }

    for(golite::SwitchCase* switch_case : this->cases_) {
        switch_case->symbolTablePass(symbol_table_);
    }
}

bool golite::Switch::hasReturn(Declarable* function) {
    bool has_default = false;
    bool has_return = true;
    for(SwitchCase* switch_case : cases_) {
        has_return &= switch_case->hasReturn(function);
        has_default |= switch_case->isDefault();
    }
    return has_default && has_return;
}