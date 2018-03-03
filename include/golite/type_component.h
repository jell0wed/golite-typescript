#ifndef GOLITE_TYPE_COMPONENT_H
#define GOLITE_TYPE_COMPONENT_H

#include <string>
#include <vector>
#include <golite/type_composite.h>

namespace golite {
    class TypeComponent {
    private:
        std::vector<golite::TypeComposite*> children_;

    public:

        /**
         * Convert to GoLite code
         * @param indent
         * @return golite code
         */
        virtual std::string toGoLite(int indent);

        /**
         * Add child
         * @param type_composite
         */
        void addChild(golite::TypeComposite* type_composite);

        /**
         * Get line number
         * @return line number
         */
        int getLine();

        /**
         * Perform a weeding pass
         */
        virtual void weedingPass();

        /**
         * Get children vector
         * @return children vector
         */
        std::vector<golite::TypeComposite*> getChildren() { return children_; }

        /**
         * Check if type is int
         * @return true if it is
         */
        bool isInt();

        /**
         * Check if type is float
         * @return true if it is
         */
        bool isFloat64();

        /**
         * Check if type is boolean
         * @return true if it is
         */
        bool isBool();

        /**
         * Check if type is string
         * @return true if it is
         */
        bool isString();

        /**
         * Check if type is rune
         * @return true if it is
         */
        bool isRune();
    };
}

#endif