#include <golite/symbol_table.h>
#include <golite/declarable.h>
#include <golite/utils.h>

void golite::SymbolTable::addChild(SymbolTable *table) {
    table->parent_ = this;
    this->children_.push_back(table);
}

void golite::SymbolTable::putSymbol(std::string name, golite::Declarable *decl) {
    this->entries_.insert(std::pair<std::string, golite::Declarable*>(name, decl));
    // TODO : check what to do with existing entries ? probably only need to replace
}

golite::Declarable* golite::SymbolTable::getSymbol(std::string name, bool search_in_parent) {
    SymbolTable* curr_sym_table = this;
    std::map<std::string, golite::Declarable*>::iterator found;
    while(curr_sym_table) {
        found = curr_sym_table->entries_.find(name);
        if(found != curr_sym_table->entries_.end()) {
            return (*found).second;
        } else if(!search_in_parent) {
            return nullptr;
        }
        curr_sym_table = curr_sym_table->parent_;
    }

    return nullptr;
}

bool golite::SymbolTable::hasSymbol(std::string name, bool search_in_parent) {
    return this->getSymbol(name, search_in_parent) != nullptr;
}

std::string golite::SymbolTable::prettyPrint(int indent) {
    std::stringstream ss;
    ss << golite::Utils::indent(indent) << "{" << std::endl;
    indent++;

    // show entries
    for(std::map<std::string, Declarable*>::value_type kv : this->entries_) {
        ss << golite::Utils::indent(indent) << kv.first << " [" << kv.second->toPrettySymbol() << "]" << " = " << std::endl;
    }

    // show nested symbol table
    for(SymbolTable* child : this->children_) {
        ss << child->prettyPrint(indent) << std::endl;
    }

    indent--;
    ss << golite::Utils::indent(indent) << "}" << std::endl;
    return ss.str();
}