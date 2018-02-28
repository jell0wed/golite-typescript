#ifndef GOLITE_ARRAY_H
#define GOLITE_ARRAY_H

#include <golite/type_composite.h>
#include <golite/literal.h>

namespace golite {
    class Array : public TypeComposite {
    private:
        golite::Literal<int>* size_ = nullptr;
    public:
        Array(golite::Literal<int>* size) : size_(size) {}

        /**
         * Get size
         * @return size
         */
        golite::Literal<int>* getSize() const { return size_; }

        /**
         * @see TypeComponent::getLine()
         */
        std::string toGoLite(int indent);

        /**
         * @see TypeComposite::getLine()
         */
        int getLine();

        /**
         * @see TypeComposite::weedingPass()
         */
        void weedingPass(bool check_break, bool check_continue);
    };
}

#endif