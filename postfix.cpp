#include "stack.hpp"
// #include "templateList.cpp"
#include <string>

//This class uses a string from the user input, converts that string from
//infix form to postfix, and then evaluates that postfix algebraically.
//my own stack class is used in this implementation. The bonus portion was
//attempted, but commented out.
class AlgebraicExpression{

public:

    string in;
    string post;

    AlgebraicExpression(string input)
    {
        in = input;
    }

    //checks whether character on stack is an operator
    bool isOperator(char c)
    {
        if(c == '+' || c == '-' || c == '*' || c == '/')
            return true;
        else
            return false;
    }
    //checks if character is an operand
    bool isOperand(char c)
    {
        if(!isOperator(c) && c != '(' && c != ')')
            return true;
        else
            return false;
    }
    //used to compare operators for successful order of operations
    int compareOperators(char op1, char op2)
    {
        if((op1 == '*' || op1 == '/') && (op2 == '+' || op2 == '-'))
            return -1;
        else if((op1 == '+' || op1 == '-') && (op2 == '*' || op2 == '/'))
            return 1;
        else
            return 0;
    }
    //converst an infix string to a postfix string with the help of a stack
    string infixToPostfix()
    {
        Stack<char> operatorStack;
        post = "";
        //converts string to array of char*
        const char* ptr = in.c_str();

        while(*ptr != '\0') //checks for end of string
        {
            if(isOperand(*ptr))
                //skip over operands, add to postfix string
                post += *ptr;
            else if(isOperator(*ptr)){
                //if no more operands for appropriate operation, push operators into string
                while(!operatorStack.isEmpty() && operatorStack.top() != '(' && compareOperators(operatorStack.top(), *ptr) <= 0){
                    post += operatorStack.top();
                    operatorStack.pop();
                }
                //if operator is found, push onto stack
                operatorStack.push(*ptr);
            }
            else if(*ptr == '(')
                operatorStack.push(*ptr);
            else if(*ptr == ')'){
                while(!operatorStack.isEmpty()){
                    //gets rid of not need parentheses
                    if(operatorStack.top() == '('){
                        operatorStack.pop();
                        break;
                    }
                    post += operatorStack.top();
                    operatorStack.pop();
                }
            }
            ptr++;
        }
        while(!operatorStack.isEmpty()){
            //empty stack into postfix string
            post += operatorStack.top();
            operatorStack.pop();
        }
        cout << "Postfix is: " << post << endl;
        return post;
    }

    bool noVariable()
    {
        int length = post.length();
        for(int i = 0; i<length; i++){
            if(!isdigit(post[i]) && !isOperator(post[i]))
                return false;
        }
        return true;
    }
    //helper function that evaluates given algebraic expression
    int evaluate(int op1, int op2, char operator_)
    {
        switch(operator_){
            case '*':
                return op2 * op1;
            case '/':
                if(op1 != 0)
                    return op2 / op1;
                else
                    cout << "No dividing by 0!" << endl;
            case '+':
                return op2 + op1;
            case '-':
                return op2 - op1;
            default:
                return 0;
        }
    }
    //uses another stack to evaluate total postfix
    int evaluatePostfix()
    {
        //converst post string to char* array
        const char* ptr = post.c_str();
        Stack<char> operandStack;
        int result;
        char c;
        //checks for end of string
        while(*ptr != '\0'){
            c = *ptr;
            if(isdigit(c))
                //pushes operand onto stack as an int
                operandStack.push(c-'0');
            else{
                //get operands
                int op1 = operandStack.top();
                operandStack.pop();
                int op2 = operandStack.top();
                operandStack.pop();
                //if c is an operator, initiate evaluation
                result = evaluate(op1, op2, c);
                operandStack.push(result);
            }
            //iterate forward
            ptr++;
        }
        return result;
    }
};

string get_choice()
{
    string choice;
    cin >> choice;
    if(choice != "IN" && choice != "in")
        throw "Incorrect input: " + choice + " is not an available choice.\n";
    return choice;
}

// bool checkForRepeat(LList<AlgebraicExpression> lst, string input)
// {
//     bool found = false;
//     Node<AlgebraicExpression>* tmp = lst.head;
//     while(tmp != NULL)
//     {
//         if(input == tmp->info.in)
//             found = true;
//     }
//
//     return found;
// }

int main(){
    string in_="";
    string post_;
    string c;
    char cont;
    AlgebraicExpression expression(in_);
    // bool repeat = false;
    // LList<AlgebraicExpression> lst;
    do{
        bool valid_input = false;
        do{
            try{
                cout << "Would you like to input an infix expression? (IN): ";
                c = get_choice();
                valid_input = true;
            }
            catch(const char* msg){
                cerr << msg << endl;
                valid_input = false;
            }
        } while(!valid_input);
        if(c == "IN" || c == "in"){
            cout << "Input an infix expression: ";
            cin >> in_;
            AlgebraicExpression* expression = new AlgebraicExpression(in_);
            // lst.append(expression);
            // repeat = checkForRepeat(lst, in_);
            cout << "Converting infix to postfix expression...\n";
            post_ = expression->infixToPostfix();
            cout << "Evaluating postfix...\n"
                 << "Evaluation of postfix: " << post_ << " = " << expression->evaluatePostfix() << endl;
        cout << "Go again? (Y/N): ";
        cin >> cont;
        expression->in = "";
        delete expression;
        }
    } while(cont == 'Y' || cont == 'y');
    return 0;
}
