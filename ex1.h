
#include "Expression.h"
#include <string>
#include <queue>
#include <map>
#include <iostream>
class Value : public Expression {
private:
    double number;
public:
    Value();
    Value(double num);
    ~Value();
    double calculate();
};
class Variable : public Expression {
private:
    string name;
    double val;
public:
    Variable(string name1, double number1);
    double calculate();
    Variable& operator++();
    Variable& operator++(int);
    Variable& operator--();
    Variable& operator--(int);
    Variable& operator+=(double num);
    Variable& operator-=(double num);

};
class BinaryOperator : public Expression {
protected:
    Expression *right;
    Expression *left;

public:
    BinaryOperator(Expression *l, Expression *r);
    ~BinaryOperator();
};
class Plus : public BinaryOperator {
public:
    Plus(Expression* left, Expression* right);
    Plus(double left, double right);
    Plus(Expression* left, double right);
    Plus(double left, Expression* right);
    double calculate();
};

class Mul : public BinaryOperator {
public:
    Mul(Expression* left, Expression* right);
    Mul(double left, double right);
    Mul(Expression* left, double right);
    Mul(double left, Expression* right);
    double calculate();
};
class Minus : public BinaryOperator {
public:
    Minus(Expression* left, Expression* right);
    Minus(double left, double right);
    Minus(Expression* left, double right);
    Minus(double left, Expression* right);
    double calculate();
};

class Div : public BinaryOperator {
public:
    Div(Expression* left, Expression* right);
    Div(double left, double right);
    Div(Expression* left, double right);
    Div(double left, Expression* right);
    double calculate();
};
class UnaryOperator : public Expression {
protected:
    Expression* exp;
public:
    UnaryOperator(Expression* exp1);
    ~UnaryOperator();

};
class UPlus : public UnaryOperator {
public:
    UPlus(Expression* exp);
    UPlus(double exp);
    double calculate();

};

class UMinus :public UnaryOperator {
public:
    UMinus(Expression* exp);
    UMinus(double exp);
    double calculate();
};
class Interpreter {
private:
    std::map<string, double> map;
public:
    Interpreter();
    Expression* interpret(string str);
    int getPriority(char C);
    void printQ(queue<string> Q);
    Expression* transformPostfixToExpression(queue<string>Q);
    int isOperator(string x);
    void setVariables(string str);
    int possibleNum(string val);
    bool is_number(const std::string& s);

};

