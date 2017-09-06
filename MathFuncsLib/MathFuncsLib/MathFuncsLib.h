// MathFuncsLib.h
// https://docs.microsoft.com/en-us/cpp/windows/walkthrough-creating-and-using-a-static-library-cpp

namespace MathFuncs
{
	class MyMathFuncs
	{
	public:
		// Returns a + b
		static double Add(double a, double b);

		// Returns a - b
		static double Subtract(double a, double b);

		// Returns a * b
		static double Multiply(double a, double b);

		// Returns a / b
		static double Divide(double a, double b);
	};
}