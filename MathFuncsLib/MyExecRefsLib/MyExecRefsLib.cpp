// MyExecRefsLib.cpp
// compile with: cl /EHsc MyExecRefsLib.cpp /link MathFuncsLib.lib
// https://docs.microsoft.com/en-us/cpp/windows/walkthrough-creating-and-using-a-static-library-cpp

#include <iostream>

#include "MathFuncsLib.h"

using namespace std;

int main()
{
	double a = 7.4;
	int b = 99;

	cout << "a + b = " <<
		MathFuncs::MyMathFuncs::Add(a, b) << endl;
	cout << "a - b = " <<
		MathFuncs::MyMathFuncs::Subtract(a, b) << endl;
	cout << "a * b = " <<
		MathFuncs::MyMathFuncs::Multiply(a, b) << endl;
	cout << "a / b = " <<
		MathFuncs::MyMathFuncs::Divide(a, b) << endl;

	return 0;
}