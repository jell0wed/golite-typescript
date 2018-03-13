#include <golite/function.h>
#include <golite/utils.h>
#include <golite/pretty_helper.h>
#include <golite/program.h>

std::string golite::Function::toGoLite(int indent) {
    std::stringstream ss;
    ss << golite::Utils::indent(indent) << "func " << identifier_->toGoLite(0)
       << "(" << golite::Pretty::implodeParams(params_, indent) << ") ";
    if(!type_component_->isVoid()) {
        ss << type_component_->toGoLite(indent) << " ";
    }
    ss << "{";
    if(!block_->getStatements().empty()) {
        ss << std::endl;
        for(Statement* statement : block_->getStatements()) {
            ss << statement->toGoLite(indent+1) << std::endl;
        }
        ss << golite::Utils::indent(indent);
    }
    ss << "}";
    return ss.str();
}

void golite::Function::weedingPass(bool check_break, bool check_continue) {
    identifier_->weedingPass();
    for(FunctionParam* function_param : params_) {
        function_param->weedingPass();
    }
    type_component_->weedingPass();
    block_->weedingPass(check_break, check_continue);
}

void golite::Function::typeCheck() {
    block_->typeCheck();
    block_->checkReturn(this);
    if(!type_component_->isVoid() && !block_->isTerminating()) {
        golite::Utils::error_message("Function " + identifier_->toGoLite(0) + " is not terminating",
                                     identifier_->getLine());
    }
}

void golite::Function::symbolTablePass(golite::SymbolTable *root) {
    if(root->hasSymbol(this->identifier_->getName(), false)) {
        golite::Utils::error_message("Function name " + identifier_->toGoLite(0) + " redeclared in this block",
                                     identifier_->getLine());
    }

    root->putSymbol(this->identifier_->getName(), this);
    SymbolTable* function_block_table = new SymbolTable(root);
    for(FunctionParam* param : params_) {
        param->symbolTablePass(function_block_table);
    }

    if(!type_component_->isVoid()) {
        type_component_->symbolTablePass(root);
    }

    this->block_->symbolTablePass(function_block_table);
}

std::string golite::Function::toPrettySymbol() {
    std::stringstream ss;
    ss << identifier_->getName() << " [function] = ";
    ss << type_component_->toPrettySymbol();
    return ss.str();
}

std::vector<golite::FunctionParam*> golite::Function::getParamsSeparated() {
    std::vector<FunctionParam*> separated_params;
    for(FunctionParam* param : params_) {
        for(Identifier* identifier : param->getIdentifier()) {
            FunctionParam* p = new FunctionParam();
            p->setIdentifiers({identifier});
            p->setTypeComponent(param->getTypeComponent());
            separated_params.push_back(p);
        }
    }
    return separated_params;
}