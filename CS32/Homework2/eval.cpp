//  @file: eval.cpp

#include <iostream>
#include <stack>
#include <string>

using namespace std;

int evaluate(const string& infix, const bool values[], string& postfix, bool& result) throw(char);
// Evaluates a boolean expression
// Precondition: infix is an infix boolean expression consisting of
//   the digits 0 through 9, the operators |, &, and !, and parentheses,
//   with embedded blanks allowed for readability. 
// Postcondition: If infix is a syntactically valid infix boolean
//   expression, then postfix is set to the postfix form of that
//   expression, result is set to the value of the expression (where
//   each digit k represents element k of the values array), and
//   the function returns zero.  If infix is not a syntactically
//   valid expression, the function returns 1.  (In that case,
//   postfix may or may not be changed, but result must be unchanged.)


int precedence(const char& oper);
// Evaluates the precedence of an operator using an array of operators
// stored in order of precedence. Higher index means higher precedence.


int precedence(const char& oper)
{
    const int size = 3;
    char opArr[size] = { '|', '&', '!' };
    
    for (int i = 0; i < size ; i++) 
    {
        if (oper == opArr[i])
            return i;
    }
    
    return -1;
}

int evaluate(const string& infix, const bool values[], string& postfix, bool& result) throw(char)
{
    
    if (infix.empty())
        return 1; // Checking for empty expression.
    
    postfix.clear(); // Clearing postfix string.
    
    // Infix to postfix conversion.
    stack<char> operatorStack;
    
    int length = static_cast<int>(infix.length());
    
    try 
    {
        int opCount = 0;
        char last = ' ';
        for (int i = 0; i < length; i++) 
        {
            char ch = infix[i];
            
            
            if (i > 0)
                last = infix[i-1];
            
                switch (ch) 
                {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        while (i < length-1) 
                        {
                            if (infix[i+1] == ' ')
                            {
                                i++;
                                continue;
                            }
                            
                            else if (infix[i+1] == '!')
                                throw ch; // Checking for digit followed by !
                            
                            else
                                break;
                        }
                        postfix += ch;
                        break;
                    
                    case '(':
                        operatorStack.push(ch);
                        break;
                    
                    case '!':
                        while ( (!operatorStack.empty()) && operatorStack.top() != '(' ) 
                        {
                            if (precedence(ch) <= precedence(operatorStack.top()))
                            {
                                postfix += operatorStack.top();
                                operatorStack.pop();
                            }
                            
                            else
                                break;
                        }
                        
                        operatorStack.push(ch);
                        opCount++;
                        break;

                    case '&':
                    case '|':
                        if (last == '!' || last == '&' || last == '|' || last == '(' ) 
                            throw ch; // Throw an exception if invalid operation.
                        
                        while ( (!operatorStack.empty()) && operatorStack.top() != '(' ) 
                        { // Making sure that stack is not empty before checking operator
                          // precedence.
                            if (precedence(ch) <= precedence(operatorStack.top()))
                            {
                                postfix += operatorStack.top();
                                operatorStack.pop();
                            }
                            
                            else
                                break;
                        }
                        
                        operatorStack.push(ch);
                        opCount++;
                        break;
                        
                    case ')':
                    
                        while ( (!operatorStack.empty()) && operatorStack.top() != '(') 
                        {
                            postfix += operatorStack.top();
                            operatorStack.pop();
                        }
                        
                        if (operatorStack.empty() || last == '(' )
                            throw ch; // Throw an exception if invalid operation or empty
                                      // evaluation.
                        
                        operatorStack.pop();
                        break;
                        
                    case ' ':
                        break;
                    
                        
                    default: // Throw an exception if invalid char entered.
                        throw ch;
                        break;
                }
                
            }
        
        char err = 'E';
        
        if (postfix.empty()) // Checking for empty postfix. This could occur if
            throw err;       // the infix statement consisted of blank spaces. 
                
        else if ( opCount < 1 && ( postfix.size()>1 ) )
            throw err; // Throw an exception if postfix contains multiple operands but no operators.
        
        while (!(operatorStack.empty())) 
        {
            if (operatorStack.top() == '(')
                throw err; // Throw an exception if invalid operation.
            postfix += operatorStack.top();
            operatorStack.pop();
        }
        
        last = infix[length-1]; // Set last equal to last char in infix string.
                                // We already verified that our infix string was
                                // not empty at the beginning of the function.
        
        switch (last) 
        {
            case '(':
            case '&':
            case '|':
            case '!':
                throw last; // Throw an exception if last infix char is an operator
                break;      // with nothing to operate on!
                
            default:
                break;
        }
        
    }
        
        catch (char ch) 
    {
        return 1;
    }
    
    // Begin postfix evaluation.
    
    stack<char> operandStack;
    
    length = static_cast<int>(postfix.length());
    
    for (int i = 0; i < length; i++) 
    {
        char ch = postfix[i];
        char operand1;
        char operand2;
        bool eval;
        
        switch (ch) 
        {
                
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                eval = values[ch - '0']; // Converting ASCII char to int digit.
                operandStack.push(eval);
                break;
            
            case '!':
                operand2 = operandStack.top();
                operandStack.pop();
                eval = (!operand2);
                operandStack.push(eval);
                break;
                
            case '&':
                operand2 = operandStack.top();
                operandStack.pop();
                operand1 = operandStack.top();
                operandStack.pop();
                eval = (operand2 && operand1);
                operandStack.push(eval);
                break;
                
            case '|':
                operand2 = operandStack.top();
                operandStack.pop();
                operand1 = operandStack.top();
                operandStack.pop();
                eval = (operand2 || operand1);
                operandStack.push(eval);
                break;
                
            default:
                return 1;
                break;
        }
    }
    
    int n = static_cast<int>(operandStack.size());
    
    if( n == 1 )
    {
        result = operandStack.top();
        return 0;
    }
    
    else
    {
        return 1; // If anything else goes wrong.
    }

    
}
