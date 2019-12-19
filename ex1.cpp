
#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#define new new(_NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define new new
#endif
#include "ex1.h"
#include <iostream>
#include "Expression.h"
#include <string>
#include <stack>
#include <queue>
#include <stdlib.h>
#include <map>
#include <algorithm>


Value::Value() { }
Value::Value(double num) {
    number = num;
}
double Value::calculate() {
    return number;
}
Value:: ~Value(){}
Variable::Variable(string name1, double number1) {
    name = name1;
    val = number1;
}
double Variable::calculate() {
    return val;
}

Variable& Variable:: operator++() { // before- prefix
    val++;
    //return *new Variable(name, val - 1);
    return *this;
}
Variable& Variable:: operator++(int) { //after- postfix
    val++;
    return *this;
}
Variable& Variable:: operator--() { // before- prefix
    val++;
    //return *new Variable(name, val + 1);
    return *this;
}
Variable& Variable:: operator--(int) { //after- postfix
    val--;
    return *this;
}
Variable& Variable:: operator+=(double num) {
    double newNum = val + num;
    return *new Variable(name, newNum);
}
Variable& Variable:: operator-=(double num) {
    double newNum = val - num;
    return *new Variable(name, newNum);
}
BinaryOperator::BinaryOperator(Expression *l, Expression *r) {
    this->left = l;
    this->right = r;
}
BinaryOperator::~BinaryOperator() {
    delete right;
    delete left;
}
Plus::Plus(Expression* left, Expression* right) : BinaryOperator(left, right) {

}
Plus::Plus(double left, double right) : BinaryOperator(new Value(left), new Value(right)) {

}
Plus::Plus(Expression* left, double right) : BinaryOperator(left, new Value(right)) {

}
Plus::Plus(double left, Expression* right) : BinaryOperator(new Value(left), right) {

}
double Plus:: calculate() {
    return left->calculate() + right->calculate();
}


Minus::Minus(Expression* left, Expression* right) : BinaryOperator(left, right) {

}
Minus::Minus(double left, double right) : BinaryOperator(new Value(left), new Value(right)) {

}
Minus::Minus(Expression* left, double right) : BinaryOperator(left, new Value(right)) {

}
Minus::Minus(double left, Expression* right) : BinaryOperator(new Value(left), right) {

}
double Minus:: calculate() {
    return left->calculate() - right->calculate();
}

Mul::Mul(Expression* left, Expression* right) : BinaryOperator(left, right) {

}
Mul::Mul(double left, double right) : BinaryOperator(new Value(left), new Value(right)) {

}
Mul::Mul(Expression* left, double right) : BinaryOperator(left, new Value(right)) {

}
Mul::Mul(double left, Expression* right) : BinaryOperator(new Value(left), right) {

}
double Mul::calculate() {
    return left->calculate() * right->calculate();
}


Div::Div(Expression* left, Expression* right) : BinaryOperator(left, right) {

}
Div::Div(double left, double right) : BinaryOperator(new Value(left), new Value(right)) {

}
Div::Div(Expression* left, double right) : BinaryOperator(left, new Value(right)) {

}
Div::Div(double left, Expression* right) : BinaryOperator(new Value(left), right) {

}
double Div:: calculate() {
    if (right->calculate() == 0.0) {
        const char* messege = "dividing by zero";
        throw messege;

    }
    return left->calculate() / right->calculate();
}


UnaryOperator::UnaryOperator(Expression* exp1) {
    this->exp = exp1;
}
UnaryOperator::~UnaryOperator() {
    delete exp;
}


UPlus::UPlus(Expression* exp) : UnaryOperator(exp) {

}
UPlus::UPlus(double exp) : UnaryOperator(new Value(exp)) {

}
double UPlus::calculate() {
    return exp->calculate();
}

UMinus::UMinus(Expression* exp) : UnaryOperator(exp) {

}
UMinus::UMinus(double exp) : UnaryOperator(new Value(exp)) {

}
double UMinus::calculate() {
    return (-1)*exp->calculate();
}


Interpreter::Interpreter() {

}
Expression* Interpreter::interpret(string str) {
    int i = 0;
    int k;
    int balance = 0;
    stack<char> signStack;
    queue<string> numbersQ;
    int flag = 1;
    int flag2 = 0;
    int length = str.length();

    //checking the correction of the infix:
    //checking the parnthesis and invalid operators
    for (k = 0; k < length; k++) {
        if (str[k] == '(') {
            balance++;
            if (k + 1 < length && (str[k + 1] == '*' || str[k + 1] == '/')) {
                throw "invalid input";
            }
        }
        if (str[k] == ')') {
            balance--;
            if (k - 1 > 0 && (str[k - 1] == '*' || str[k - 1] == '/' || str[k - 1] == '+' || str[k - 1] == '-')) {
                throw "invalid input";
            }
        }
        if (balance < 0) {
            throw "invalid input";
        }
    }
    if (balance != 0) {
        throw "invalid input";
    }




    while (i < length) {
        if (str[i] == '.' && flag == 1) { // if we have a dot
            flag = 0;
        }
        if ((str[i] == '+') && flag == 1) { // if we have a +
            //a check to determine if its unary operator or not
            int flag3 = 0;
            if (i == 0) {
                flag3 = 1;
            }
            if (i != 0 && str[i - 1] == '(') {
                flag3 = 1;
            }


            if (signStack.size() != 0) {
                int topStackPriority = getPriority(signStack.top());
                if (topStackPriority == 0) {
                    if (flag3 != 1) {
                        signStack.push(str[i]);
                    }
                    if (flag3 == 1) {
                        signStack.push('%');
                    }
                }
                else {
                    while (topStackPriority == 2 || topStackPriority == 1) {
                        char signOnTop = signStack.top();
                        string s;
                        s.push_back(signOnTop);
                        numbersQ.push(s);
                        signStack.pop();
                        if (signStack.size() != 0) {
                            topStackPriority = getPriority(signStack.top());
                        }
                        else {
                            break;
                        }
                    }
                    if (flag3 != 1) {
                        signStack.push(str[i]);
                    }
                    if (flag3 == 1) {
                        signStack.push('&');
                    }
                }
            }
            else {
                if (flag3 != 1) {
                    signStack.push(str[i]);
                }
                if (flag3 == 1) {
                    signStack.push('&');
                }
            }

            flag = 0;
        }
        if (str[i] == '-' && flag == 1) { // if we have a -
            //a check to determine if its unary operator or not
            int flag3 = 0;
            if (i == 0) {
                flag3 = 1;
            }
            if (i != 0 && str[i - 1] == '(') {
                flag3 = 1;
            }
            if (signStack.size() != 0) {
                int topStackPriority = getPriority(signStack.top());
                if (topStackPriority == 0) {
                    if (flag3 != 1) {
                        signStack.push(str[i]);
                    }
                    if (flag3 == 1) {
                        signStack.push('&');
                    }
                }
                else {
                    while (topStackPriority == 2 || topStackPriority == 1) {
                        char signOnTop = signStack.top();
                        string s;
                        s.push_back(signOnTop);
                        numbersQ.push(s);
                        signStack.pop();
                        if (signStack.size() != 0) {
                            topStackPriority = getPriority(signStack.top());
                        }
                        else {
                            break;
                        }
                    }
                    if (flag3 != 1) {
                        signStack.push(str[i]);
                    }
                    if (flag3 == 1) {
                        signStack.push('&');
                    }
                }
            }
            else {
                if (flag3 != 1) {
                    signStack.push(str[i]);
                }
                if (flag3 == 1) {
                    signStack.push('&');
                }
            }
            flag = 0;
        }
        if (str[i] == '*' && flag == 1) { // if we have a *
            if (signStack.size() != 0) {
                int topStackPriority = getPriority(signStack.top());
                if (topStackPriority != 2) {
                    signStack.push(str[i]);
                }
                else {
                    while (topStackPriority == 2) {
                        char signOnTop = signStack.top();
                        string s;
                        s.push_back(signOnTop);
                        numbersQ.push(s);
                        signStack.pop();
                        if (signStack.size() != 0) {
                            topStackPriority = getPriority(signStack.top());
                        }
                        else {
                            break;
                        }
                    }
                    signStack.push(str[i]);
                }
            }
            else {
                signStack.push(str[i]);
            }
            flag = 0;
        }
        if (str[i] == '/' && flag == 1) { // if we have a /
            if (signStack.size() != 0) {
                int topStackPriority = getPriority(signStack.top());
                if (topStackPriority != 2) {
                    signStack.push(str[i]);
                }
                else {
                    while (topStackPriority == 2) {
                        char signOnTop = signStack.top();
                        string s;
                        s.push_back(signOnTop);
                        numbersQ.push(s);
                        signStack.pop();
                        if (signStack.size() != 0) {
                            topStackPriority = getPriority(signStack.top());
                        }
                        else {
                            break;
                        }
                    }
                    signStack.push(str[i]);
                }
            }
            else {
                signStack.push(str[i]);
            }
            flag = 0;
        }
        if (str[i] == '(' && flag == 1) { // if we have a (
            signStack.push(str[i]);
            flag = 0;
        }
        if (str[i] == ')' && flag == 1) { // if we have a )
            while (signStack.top() != '(') {
                char signOnTop = signStack.top();
                string s;
                s.push_back(signOnTop);
                numbersQ.push(s);
                signStack.pop();
            }
            signStack.pop(); // getting rid of the '('
            flag = 0;
        }
        if ((str[i] >= '0' && str[i] <= '9') && flag == 1) {
            // if its a number
            // we should read the string untill its not a dot or number
            // and then transfer the string we got into double
            int j;
            int counter = 0;
            j = i;

            while (((str[i] >= '0' && str[i] <= '9') || str[i] == '.' || str[i] == '(' || str[i] == ')') && i < length &&flag2 == 0) {// while we have number or dot we can keep reading
                i++;
                counter++;
                if (str[i] == ')') {
                    flag2 = 1;
                }
            }
            string subStr;
            subStr = str.substr(j, counter);
            numbersQ.push(subStr);
            if (flag2 == 1) {
                while (signStack.top() != '(') {
                    char signOnTop = signStack.top();
                    string s;
                    s.push_back(signOnTop);
                    numbersQ.push(s);
                    signStack.pop();
                }
            }
            flag2 = 0;
            flag = 0;
            i--;
        }
        if ((str[i] >= 97 && str[i] <= 122)) { // if its a variable
            int j = i;
            int counter = 0;
            while (str[i] != '+' && str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '(' && str[i] != ')') {


                counter++;
                i++;
            }
            i--;
            string s = str.substr(j, counter);

            if (map.find(s) == map.end()) {
                throw "illegal math expression";
            }

            double d = map[s];
            std::string varAsString = std::to_string(d);
            numbersQ.push(varAsString);
        }


        flag = 1;
        i++;
    }
    //out of the loop on the string
    while (signStack.size() != 0) {
        char signOnTop = signStack.top();
        string s;
        s.push_back(signOnTop);
        numbersQ.push(s);
        signStack.pop();
    }

    //now we have the infix equation in postfix form, its all in the queue
    // i need to transfer the postfix into an Expression.
    //printQ(numbersQ);

    Expression* e = transformPostfixToExpression(numbersQ);

    return e;
}

int Interpreter::getPriority(char C)
{
    if (C == '-' || C == '+' || C == '%' || C == '&')
        return 1;
    else if (C == '*' || C == '/')
        return 2;
    return 0;
}
void Interpreter::printQ(queue<string> Q) {
    cout << " the postfix of the equation is: ";
    while (Q.size() != 0) {
        cout << Q.front();
        Q.pop();
    }
    cout << endl;
}

Expression* Interpreter::transformPostfixToExpression(queue<string>Q) {
    stack<Expression*> S;
    while (Q.size() != 0) {
        int token = isOperator(Q.front());
        if (token == 0) { // if the token is a number
            string str = Q.front();
            double number = stod(str);
            Value* val = new Value(number);
            S.push(val);
        }
        if (token == 1 || token == 2 || token == 3 || token == 4) { // if the token is a binary operator
            // poping the two numbers from stack //

            if (S.size() < 2) {
                throw "invalid input";
            }

            Expression* exp1 = S.top();
            S.pop();
            Expression* exp2 = S.top();
            S.pop();

            switch (token) // separte to specifc operator
            {
                case 1: // if its Plus '+'
                {Expression* exp = new Plus(exp1, exp2);
                    S.push(exp); }
                    break;
                case 2: // if its Minus '-'
                {Expression* exp = new Minus(exp2, exp1);
                    S.push(exp); }
                    break;
                case 3:// if its Mult '*'
                {Expression* exp = new Mul(exp1, exp2);
                    S.push(exp); }
                    break;
                case 4: // if its Div '/'
                {Expression* exp = new Div(exp2, exp1);
                    S.push(exp); }
                    break;
                default:
                    break;
            }

        }
        if (token == 5 || token == 6) {  //if unary operator
            Expression* exp1 = S.top();
            S.pop();
            switch (token)
            {
                case 5:
                {Expression* exp5 = new UPlus(exp1);
                    S.push(exp5); }

                    break;
                case 6:
                {Expression* exp6 = new UMinus(exp1);
                    S.push(exp6); }
                    break;

                default:
                    break;
            }
        }
        Q.pop();
    }

    Expression* expAnswer = S.top();
    return expAnswer;
}

int Interpreter::isOperator(string x)
{
    switch (x[0]) {
        case '+':
            return 1;
            break;
        case '-':
            return 2;
            break;
        case '*':
            return 3;
            break;
        case '/':
            return 4;
            break;
        case '%':
            return 5;
            break;
        case '&':
            return 6;
            break;
    }


    return 0; // then its a number or variable
}

void Interpreter::setVariables(string str) {
    int lengthOfStr = str.length();
    int i = 0;
    while (i < lengthOfStr) {
        string nameOfVar;
        string valueOfVar;
        int counter = 0;
        int j = i;
        while (str[i] != '=') {// taking the name of the var
            counter++;
            i++;
        }
        nameOfVar = str.substr(j, counter);
        counter = 0;
        i++;
        j = i;
        while (str[i] != ';'&& i < lengthOfStr) {//taking the value of the var
            counter++;
            i++;
        }
        valueOfVar = str.substr(j, counter);
        if (is_number(valueOfVar) == false) {
            const char* messege = "illegal variable assignment!";
            throw messege;
        }
        else {
            double val = stod(valueOfVar);
            map[nameOfVar] = val;
            i++;
        }
    }
}
int Interpreter::possibleNum(string val) {
    size_t i = 0;
    int flag = 0;
    while (i < val.length()) {
        if (val[i] <= '0' && val[i] >= '9') {
            flag = 1;
        }
        i++;
    }
    if (flag == 1) {
        return 1;
    }
    return 0;
}

bool Interpreter::is_number(const std::string& s)
{
    char* end = 0;
    double val = strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0' && val != 99999;
}

