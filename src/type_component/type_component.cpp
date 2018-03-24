#include <golite/type_component.h>
#include <golite/utils.h>
#include <sstream>
#include <iostream>
#include <golite/type_reference.h>
#include <golite/program.h>
#include <golite/func.h>

std::string golite::TypeComponent::toGoLite(int indent) {
    std::stringstream ss;
    if(children_.empty()) {
        throw std::runtime_error("Cannot prettify an empty type component");
    }

    for(size_t i = children_.size(); i > 0; i--) {
        ss << children_[i-1]->toGoLite(indent);
    }
    return ss.str();
}

std::string golite::TypeComponent::toGoLiteMin() {
    std::stringstream ss;
    if(children_.empty()) {
        throw std::runtime_error("Cannot min prettify an empty type component");
    }

    for(size_t i = children_.size(); i > 0; i--) {
        ss << children_[i-1]->toGoLiteMin();
    }
    return ss.str();
}

void golite::TypeComponent::addChild(golite::TypeComposite *type_composite) {
    children_.push_back(type_composite);
}

int golite::TypeComponent::getLine() {
    if(children_.empty()) {
        throw std::runtime_error("Cannot get line of an empty type component");
    }
    return children_.front()->getLine();
}

void golite::TypeComponent::weedingPass() {
    for(TypeComposite* type_composite : children_) {
        type_composite->weedingPass();
    }
}

bool golite::TypeComponent::isCompatible(TypeComponent *type_component) {

    // If the number of composite types is different they they are not compatible
    if(children_.size() != type_component->children_.size()) {
        return false;
    }

    for(size_t i=0; i < children_.size(); i++) {
        if(!children_[i]->isCompatible(type_component->children_[i])) {
            return false;
        }
    }
    return true;
}

void golite::TypeComponent::symbolTablePass(SymbolTable *root) {
    for(TypeComposite* child : children_) {
        child->symbolTablePass(root);
    }
}

bool golite::TypeComponent::isBool() {
    return isCompatible(golite::Program::BOOL_BUILTIN_TYPE->getTypeComponent());
}

bool golite::TypeComponent::isInt() {
    return isCompatible(golite::Program::INT_BUILTIN_TYPE->getTypeComponent());
}

bool golite::TypeComponent::isInteger() {
    return isInt() || isRune();
}

bool golite::TypeComponent::isFloat64() {
    return isCompatible(golite::Program::FLOAT64_BUILTIN_TYPE->getTypeComponent());
}

bool golite::TypeComponent::isRune() {
    return isCompatible(golite::Program::RUNE_BUILTIN_TYPE->getTypeComponent());
}

bool golite::TypeComponent::isString() {
    return isCompatible(golite::Program::STRING_BUILTIN_TYPE->getTypeComponent());
}

bool golite::TypeComponent::isInfer() {
    return isCompatible(golite::Program::INFER_TYPE->getTypeComponent());
}

bool golite::TypeComponent::isVoid() {
    return isCompatible(golite::Program::VOID_TYPE->getTypeComponent())
           || isCompatible(golite::Program::UNMAPPED_TYPE->getTypeComponent());
}

bool golite::TypeComponent::isComparable() {
    for(TypeComposite* type_composite : children_) {
        if(!type_composite->isComparable()) {
            return false;
        }
    }
    return true;
}

bool golite::TypeComponent::isOrdered() {
    return isInt() || isFloat64() || isRune() || isString();
}

bool golite::TypeComponent::isNumeric() {
    return isInt() || isFloat64() || isRune();
}

bool golite::TypeComponent::resolvesTo(Declarable* declarable) {
    if(children_.size() != 1 || declarable->getTypeComponent()->children_.size() != 1) {
        return false;
    }
    return children_.front()->resolvesTo(declarable);
}

std::string golite::TypeComponent::toPrettySymbol() {
    std::stringstream ss;
    if(children_.empty()) {
        throw std::runtime_error("Cannot prettify symbol an empty type component");
    }

    for(size_t i = children_.size(); i > 0; i--) {
        ss << children_[i-1]->toPrettySymbol();
    }
    return ss.str();
}

bool golite::TypeComponent::isRecursive(Type* type) {
    if(children_.empty()) {
        throw std::runtime_error("Cannot check if type component is recursive because children list is empty");
    }

    for(TypeComposite* type_composite : children_) {
        if(!type_composite->isRecursive(type)) {
            return false;
        }
    }
    return true;
}

bool golite::TypeComponent::resolvesToBool() {
    return resolvesTo(golite::Program::BOOL_BUILTIN_TYPE);
}

bool golite::TypeComponent::resolvesToFloat64() {
    return resolvesTo(golite::Program::FLOAT64_BUILTIN_TYPE);
}

bool golite::TypeComponent::resolvesToInt() {
    return resolvesTo(golite::Program::INT_BUILTIN_TYPE);
}

bool golite::TypeComponent::resolvesToInteger() {
    return resolvesToInt() || resolvesToRune();
}

bool golite::TypeComponent::resolvesToString() {
    return resolvesTo(golite::Program::STRING_BUILTIN_TYPE);
}

bool golite::TypeComponent::resolvesToRune() {
    return resolvesTo(golite::Program::RUNE_BUILTIN_TYPE);
}

bool golite::TypeComponent::resolvesToComparable() {
    std::vector<TypeComposite*> resolved = resolveChildren();
    for(TypeComposite* type_composite : resolved) {
        if(!type_composite->resolvesToComparable()) {
            return false;
        }
    }
    return true;
}

bool golite::TypeComponent::resolvesToOrdered() {
    return resolvesToInt() || resolvesToFloat64() || resolvesToRune() || resolvesToString();
}

bool golite::TypeComponent::resolvesToNumeric() {
    return resolvesToInt() || resolvesToFloat64() || resolvesToRune();
}

std::vector<golite::TypeComposite*> golite::TypeComponent::resolveChildren() {
    std::vector<golite::TypeComposite*> children;
    for(size_t i=0; i < children_.size(); i++) {
        if(i == children_.size() - 1) {
            std::vector<golite::TypeComposite*> resolved_children = children_[i]->resolveChildren();
            children.insert(children.end(), resolved_children.begin(), resolved_children.end());
        } else {
            children.push_back(children_[i]);
        }
    }
    return children;
}

bool golite::TypeComponent::isBaseType() {
    return isInt()
           || isFloat64()
           || isBool()
           || isString()
           || isRune();
}

bool golite::TypeComponent::resolvesToBaseType() {
    return resolvesToInt()
           || resolvesToFloat64()
           || resolvesToBool()
           || resolvesToString()
           || resolvesToRune();
}

golite::TypeComponent* golite::TypeComponent::resolveFunc() {
    if(children_.empty()) {
        throw std::runtime_error("Cannot resolve func because children is empty");
    }
    if(!children_.front()->isFunc()) {
        throw std::runtime_error("Cannot resolve func because front child is not a func");
    }
    golite::Func* func = static_cast<Func*>(children_.front());
    return func->getTypeComponent();
}

std::string golite::TypeComponent::toTypeScript(int indent) {
    std::stringstream ss;
    std::stringstream ss_start;
    std::stringstream ss_end;
    for(TypeComposite* child : children_) {
        if(child->isArray() || child->isSlice()) {
            ss_start << "Array<";
            ss_end << ">";
        } else if(child->isTypeReference() || child->isStruct() || child->isPointer()) {
            ss_end << child->toTypeScript(indent);
        }
    }
    ss << ss_start.str() << ss_end.str();
    return ss.str();
}

std::string golite::TypeComponent::toTypeScriptDefaultValue() {
    if(isInt()) {
        return "0";
    } else if (isFloat64()) {
        return "0.0";
    } else if(isString()) {
        return "\"\"";
    } else if(isRune()) {
        return "'\\u0000'";
    } else if(isBool()) {
        return "false";
    } else if(isSlice() || isArray()) {
        return "[]";
    } else if(isStruct()) {
        return "{}";
    } else {
        throw std::runtime_error("Unhandled default type script value");
    }
}

bool golite::TypeComponent::isArray() {
    return !children_.empty() && children_.back()->isArray();
}

bool golite::TypeComponent::isStruct() {
    return !children_.empty() && children_.back()->isStruct();
}

bool golite::TypeComponent::isSlice() {
    return !children_.empty() && children_.back()->isSlice();
}

bool golite::TypeComponent::isTypeReference() {
    return !children_.empty() && children_.back()->isTypeReference();
}

bool golite::TypeComponent::isPointer() {
    return !children_.empty() && children_.back()->isPointer();
}
