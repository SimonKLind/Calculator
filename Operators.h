#ifndef OPERATORS_H
#define OPERATORS_H

#include <cmath>

#include "Map.h"

class Operator{
	typedef double(*Func)(double, double);
public:
	int precedence;
	Func operate;
	Operator() {}
	Operator(Func Operate, int Precedence): operate(Operate), precedence(Precedence){}
};

Map<std::string, Operator> Operators;


Operator lPar = Operator([](double dummy1, double dummy2) ->  double { return 0; }, -1);
Operator Num = Operator([](double dummy1, double dummy2) ->  double { return 0; }, -2);
Operator X = Operator([](double dummy1, double dummy2) ->  double { return 0; }, -3);

namespace OPERATORS{
	const double pi = 3.14159265358979323846;
	void Initialize(){
		Operators.add("+", Operator([](double arg1, double arg2) -> double { return arg1+arg2; }, 1));
		Operators.add("-", Operator([](double arg1, double arg2) -> double { return arg1-arg2; }, 1));
		Operators.add("/", Operator([](double arg1, double arg2) -> double { return arg1/arg2; }, 2));
		Operators.add("*", Operator([](double arg1, double arg2) -> double { return arg1*arg2; }, 2));
		Operators.add("^", Operator([](double arg1, double arg2) -> double { return pow(arg1, arg2); }, 3));
		Operators.add("sin", Operator([](double arg, double dummy) -> double { return sin(arg); }, 4));
		Operators.add("cos", Operator([](double arg, double dummy) -> double { return cos(arg); }, 4));
		Operators.add("tan", Operator([](double arg, double dummy) -> double { return tan(arg); }, 4));
		Operators.add("asin", Operator([](double arg, double dummy) -> double { return asin(arg); }, 4));
		Operators.add("acos", Operator([](double arg, double dummy) -> double { return acos(arg); }, 4));
		Operators.add("atan", Operator([](double arg, double dummy) -> double { return atan(arg); }, 4));
		Operators.add("sqrt", Operator([](double arg, double dummy) -> double { return sqrt(arg); }, 4));
		Operators.add("log", Operator([](double arg, double dummy) -> double { return log10(arg); }, 4));
		Operators.add("ln", Operator([](double arg, double dummy) -> double { return log(arg); }, 4));
		Operators.add("abs", Operator([](double arg, double dummy) -> double { return (arg<0) ? -arg : arg; }, 4));
		Operators.add("rad", Operator([](double arg, double dummy) -> double { return arg*(pi/180); }, 4));
		Operators.add("deg", Operator([](double arg, double dummy) -> double { return arg*(180/pi); }, 4));
		Operators.add("!", Operator([](double arg, double dummy) -> double { return -arg; }, 4));
	}
}

#endif