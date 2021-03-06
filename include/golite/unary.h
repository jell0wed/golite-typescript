#ifndef GOLITE_EXPRESSIONS_UNARY_H
#define GOLITE_EXPRESSIONS_UNARY_H

#include <golite/expression.h>

namespace golite {

    /**
     * Class representing unary expression
     * unary_expression: <operator> <expr>
     * operator:
     *     -
     *     !
     *     +
     *     ^
     */
    class Unary : public Expression {
    public:
        /**
         * All kinds of unary operators
         */
        enum KIND {
            MINUS,
            NOT,
            PLUS,
            XOR
        };

        Unary(Expression* operand, KIND kind) : operand_(operand), kind_(kind) {}

        /**
         * Get operand
         * @return operand
         */
        Expression* getOperand() const { return operand_; }

        /**
         * Get kind
         * @return kind
         */
        KIND getKind() const { kind_; }

        /**
         * @see Expression::toGoLite(int)
         */
        std::string toGoLite(int indent);

        /**
         * Get line number
         */
        int getLine();

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
         * @see Expression::isUnary()
         */
        bool isUnary() { return true; }

        /**
         * @see Expression::toTypeScript()
         */
        std::string toTypeScript(int indent);

        /**
         * @see Expression::toTypeScriptInitializer()
         */
        std::string toTypeScriptInitializer(int indent);
    private:
        Expression* operand_ = nullptr;
        KIND kind_;
    };
}

#endif
