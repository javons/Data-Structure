/**
 * @file proj3.cc
 * @mainpage
 *
 * CISC 2200, Data Structures<br>
 * Project 3: Recognizing Infix Arithmetic Expressions<p>
 *
 * <p> A stack-based program that repeatedly</p>
 * <p> · reads an expression from standard input,</p>
 * <p> · computes the value of that expression, and</p>
 * <p> · prints the expression's value.</p>
 *
 * @author Tianmao Jiang
 * @date   March 28 2016
 **********************************************************************/

/*
 * includes
 */
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
using namespace std;

/**
 * This program only does integer arithmetic (and it's pretty 
 * limited integer arithmetic, with only one-digit operands).
 * If we ever decide to do (say) floating point arithmetic, 
 * simply change the definition of ValueType. Of course, this would 
 * require us to work somewhat harder when dealing with operands.
 */
using ValueType = int;

/**
 * @class IllegalExprException
 * Exception class thrown by illegal infix expression
 */
class IllegalExprException : public runtime_error
{
public:
/**
 * Default constructor.  Pretty much what you'd expect.
 */
    IllegalExprException(const string& message=""):
	runtime_error("Illegal Expression: " + message)
	{
	}
};

/*
 * function prototypes, with doxygen docs
 */

/**
 * Determine the value of an infix expression
 *
 * @param expr a string, supposedly representing an infix expression
 * @return the value of <code>expr</code>
 * @throw IllegalExprException if <code>expr</code> is not a legal 
 * infix expression 
 */
ValueType processExpression(const string& expr) throw
    (IllegalExprException);

/**
 * Is a character a valid response?
 *
 * @param response the first char in the user's response
 * @pre response is a valid character
 * @return true if response is one of the following: 
 * @return 'Y', 'y', 'N', 'n', 'T', 't', 'F', 'f', '1', '0'
 */
bool isValidResponse(char response);

/**
 * Is a character a "yes" response?
 *
 * @param response the first char in the user's response
 * @pre response is a valid response (in the sense of isValidResponse)
 * @return true if Response is one of the following: 
 * @return 'Y', 'y', 'T', 't', '1'
 */
bool isYesResponse(char response);

/**
 * Determine the precedence of an operator.
 *
 * @param op the operator in question
 * @pre op is one of '+', '-', '*', '/'
 * @return · 3 for multiplicative operations
 * @return · 2 for additive operations
 * @return · 1 for illegal operation
 * @return · 0 for a left parenthesis
 */
int precedence(char op);

/**
 * Print a help message if the user wants one. 
 */
void provideHelpIfNecessary(void);

/**
 * Do the binary operation using the top element of an operand stack
 * and the top two elements an operator stack.
 *
 * @param valstack    a stack of operands
 * @param opStack     a stack of operators
 * @throw IllegalExprException	if the operation cannot be executed
 * @post the binary operation at the top of opStack is applied to the 
 * two operands at the top of valstack and this value is pushed to the
 * top of valstack
 */
void execute(stack<ValueType>& valstack, stack<char>& opStack) 
    throw (IllegalExprException);

/**
 * Do a binary operation.
 * 
 * @param operandL	the left operatand
 * @param operation	the operation to be performed
 * @param operandR	the right operatand
 * @throw IllegalExprException	if the operation cannot be executed
 * @return the result of the operation
 */
ValueType doOperation(ValueType operandL, char operation,
		      ValueType operandR)
    throw (IllegalExprException);

/**
 * The usual main function
 */
int main(void)
{
    provideHelpIfNecessary();

    while (true) {
	cout << "Expression? " ;
	string expr;                 // holds the suppose expression
	getline(cin, expr);
	if (expr.length() == 0)
	    break;
	try {
	    ValueType result = processExpression(expr);
	    cout << result << endl;
	} catch (exception& ex) {
	    cerr << ex.what() << endl;
	}
    } // end while
    cout << endl;
} // end main

void provideHelpIfNecessary(void)
{
    while (true) {
	string ans;
	cout << "Do you need help (Y/N)? ";
	cin >> ans;
	if (isValidResponse(ans[0])) {
	    if (isYesResponse(ans[0])) {
		cout << "Enter an infix expression at the prompt.\n"
		    "The program will tell you its value.\n"
		    "To stop the program, just hit \"return\" "
		    "at the prompt.\n";
		cin.ignore ();
		break;
	    }
	    else {
		cin.ignore ();
		break;
	    }
	}
	else {
	    cout << "Response must be \'Y\' or \'N\'.\n\n";
	    continue;
	}
    }
} // end provideHelpIfNecessary

ValueType processExpression(const string& expr) throw
    (IllegalExprException)
{ 
    stack<ValueType> valStack;
    stack<char> opStack;
    for (unsigned int i = 0; i < expr.length(); i++)
        {
	    if (expr[i] == ' ') continue;
            else if (expr[i] == '0'|| expr[i] == '1'|| expr[i] == '2'||
		     expr[i] == '3'|| expr[i] == '4'|| expr[i] == '5'||
		     expr[i] == '6'|| expr[i] == '7'|| expr[i] == '8'||
		     expr[i] == '9') valStack.push(expr[i]-'0');
            else if (expr[i] == '(') opStack.push(expr[i]);

            else if (expr[i] == '*'|| expr[i] == '+'||
		     expr[i] == '-'|| expr[i] == '/') {

                if(opStack.empty()) opStack.push(expr[i]);

                else if (precedence(expr[i]) >
			 precedence(opStack.top()))
                    opStack.push(expr[i]);

                else
                    {
                        if (!opStack.empty() && precedence(expr[i])
			    <= precedence(opStack.top()))
			    execute(valStack,opStack);
                        opStack.push(expr[i]);
                    }
	    }
            else if (expr[i] == ')') {
                while (!opStack.empty() && (opStack.top()) != '(')
		    execute(valStack,opStack);
		if (opStack.empty())
		    throw IllegalExprException
			("missing left parenthesis");
                opStack.pop();
            }
	    else {
		throw IllegalExprException
		    ("illegal character in expression");
	    }
	}

    if (!opStack.empty() && valStack.empty())
	throw IllegalExprException("missing operand");
	
    while (!opStack.empty() && !(valStack.size() == 1)) {
	execute(valStack,opStack);
    }
    
    if (!opStack.empty())
	throw IllegalExprException("missing operand");
    else if (opStack.empty() && valStack.size()>1)
	throw IllegalExprException("missing operator");
    
    return valStack.top();
} // end processExpression

bool isValidResponse(char response)
{
    if(response == 'Y' || response == 'y' || response == 'N' ||
       response == 'n' || response == 'T' || response == 't' ||
       response == 'F' || response == 'f' || response == '1' ||
       response == '0')
	return true;
    else
	return false;
} // end isValidResponse

bool isYesResponse(char response)
{
    if(response == 'Y' || response == 'y' || response == 'T' ||
       response == 't' || response == '1')
	return true;
    else
	return false;
} // end isYesResponse

int precedence(char op)
{
    if (op == '*' || op == '/') return 3;
    else if (op == '+' || op == '-') return 2;
    else if (op == '(') return 0;
    else return 1;
} // end precedence

void execute(stack<ValueType>& valStack, stack<char>& opStack)
    throw (IllegalExprException)
{
    ValueType opR = valStack.top();
    valStack.pop();
    ValueType opL = valStack.top();
    valStack.pop();
    char op = opStack.top();
    opStack.pop();
    if (op == '(')
	throw IllegalExprException("missing right parenthesis");
    ValueType result = doOperation(opL, op, opR);
    valStack.push(result);
} // end execute

ValueType doOperation(ValueType operandL, char operation,
		      ValueType operandR)
    throw (IllegalExprException)
{
    ValueType result;
    if (operation == '*') result = operandL * operandR;
    else if (operation == '/') {
	if (operandR == 0)
	    throw IllegalExprException("division by zero");
	else
	    result = operandL / operandR;
    }
    else if (operation == '+') result = operandL + operandR;
    else if (operation == '-') result = operandL - operandR;
    return result;
} // end doOperation
