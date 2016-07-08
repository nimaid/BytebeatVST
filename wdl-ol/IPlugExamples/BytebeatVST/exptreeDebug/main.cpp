#include <iostream>
#include "../TreeExpressions.h"
#include <string>
#include <cstdint>

using namespace std;

int main()
{
    string expression;
    ExpressionTree formulaTree;
    uint32_t input;

    cout << "Please enter expression: ";
    cin >> expression;
    cout << "\nYou entered: " << expression << endl;

    formulaTree.build(expression);

    while(true)
    {
        cout << "\nEnter a var for equ " << expression << ": ";
        cin >> input;
        cout << "\nResult: " << formulaTree.evaluate(input);
    }

    return 0;
}
