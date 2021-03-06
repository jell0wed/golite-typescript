#ifndef GOLITE_EXPRESSIONS_FUNCTION_CALL_H
#define GOLITE_EXPRESSIONS_FUNCTION_CALL_H

#include <vector>
#include <golite/primary.h>
#include <golite/function.h>
#include <golite/type.h>

namespace golite {
    class FunctionCall : public Primary {
    private:
        std::vector<Expression*> args_;
        int line_;
    public:
        FunctionCall(int line) : line_(line) {}

        /**
         * Set arguments
         * @param args
         */
        void setArgs(std::vector<Expression*> args) { args_ = args; }

        /**
         * @see Expression::toGoLite(int)
         */
        std::string toGoLite(int indent);

        /**
         * Get line number
         * @return line number
         */
        int getLine() { return line_; }

        /**
         * @see Expression::isFunctionCall()
         */
        bool isFunctionCall() { return true; }

        /**
         * @see Expression::weedingPass()
         */
        void weedingPass();

        /**
         * @see Expression::typeCheck()
         */
        golite::TypeComponent* typeCheck();

        /**
         * @see Statement::symbolTablePass()
         */
        void symbolTablePass(SymbolTable* root);

        /**
         * Check parameters
         * @param params
         */
        void checkParams(golite::Function* function);

        /**
         * Check parameters
         * @param params
         */
        void checkParams(golite::Type* type);

        /**
         * @see Expression::toTypeScript()
         */
        std::string toTypeScript(int indent);

        /**
         * @see Expression::toTypeScriptInitializer()
         */
        std::string toTypeScriptInitializer(int indent);

        /**
         * Get arguments
         * @return args
         */
        std::vector<golite::Expression*> getArgs() const { return args_; }
    };
}

#endif
