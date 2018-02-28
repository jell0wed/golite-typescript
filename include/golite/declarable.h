#ifndef GOLITE_STATEMENTS_DECLARABLES_DECLARABLE_H
#define GOLITE_STATEMENTS_DECLARABLES_DECLARABLE_H

#include <string>

namespace golite {
    /**
     * Parent class of elements that can be created on the global scope (program scope)
     *
     * The following types can are declarable on the global scope:
     * - Types
     * - Variables
     * - Functions
     */
    class Declarable {
    public:

        /**
         * Convert code to golite
         * @param indent
         * @return code
         */
        virtual std::string toGoLite(int indent) = 0;

        /**
         * Function overrides this function
         * @return true for functions
         */
        virtual bool isFunction() { return false; }

        /**
         * Variable overrides this function
         * @return true for variable statements
         */
        virtual bool isDecVariable() { return false; }

        /**
         * @see Statement::weedingPass()
         */
        virtual void weedingPass(bool check_break, bool check_continue) = 0;
    };
}

#endif
