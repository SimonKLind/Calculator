#ifndef POSTFIX_H
#define POSTFIX_H

#include <string>

#include "Stack.h"
#include "Operators.h"
#include "Array.h"

class Postfix{

	Map<std::string, double> *vars = nullptr;
	Stack<Operator*> opStack;
	Array<Operator*> operators;
	Array<double> numbers;

	void pushNumber(std::string &num){
		if(num != ""){
			numbers.add(std::stod(num));
			operators.add(&Num);
			num = "";
		}
	}

	void handleString(std::string &expr, int &currentIndex){
		std::string s = "";
		while(expr[currentIndex] >= 97 && expr[currentIndex] <=122) s+=expr[currentIndex++];
		--currentIndex;
		if(s == "x") operators.add(&X);
		else if(Operators.contains(s)){
			Operator *temp = &Operators.get(s);
			while(opStack.size > 0 && opStack.peek()->precedence > temp->precedence) operators.add(opStack.pop());
			opStack.push(temp);
		}
		else if(vars->contains(s)){
			operators.add(&Num);
			numbers.add(vars->get(s));
		}
	}

public:

	Postfix(){}

	Postfix(Postfix &other){
		vars = other.vars;
		opStack = other.opStack;
		operators = other.operators;
		numbers = other.numbers;
	}

	Postfix(std::string expr, Map<std::string, double> *vars){
		this->vars = vars;
		int currentIndex = 0;
		std::string number = "";
		char c;
		while(currentIndex<expr.length()){
			c = expr[currentIndex];
			if(c >= 48 && c <= 57 || c == '.') number+=c;
			else{
				pushNumber(number);
				if(c == '(') opStack.push(&lPar);
				else if(c == ')'){
					while(opStack.size > 0 && opStack.peek()->precedence != -1) operators.add(opStack.pop());
					opStack.pop();
				}
				else if(c >= 97 && c <= 122) handleString(expr, currentIndex);
				else if(Operators.contains(std::string(1, c))){
					Operator *temp = &Operators.get(std::string(1, c));
					while(opStack.size > 0 && opStack.peek()->precedence > temp->precedence) operators.add(opStack.pop());
					opStack.push(temp);
				}
			}
			++currentIndex;
		}
		pushNumber(number);
		while(opStack.size > 0) (opStack.peek()->precedence == -1) ? (void)opStack.pop() : operators.add(opStack.pop());
		//print();
	}

	void print(){
		int numIndex = 0;
		for(int i=0; i<operators(); ++i){
			if(operators[i]->precedence == -2) std::cout << numbers[numIndex++] << "; ";
			else std::cout << operators[i]->precedence << ", ";
		}
		std::cout << '\n';
	}

	double evaluate(double x = 0){
		int numIndex = 0;
		double arg1, arg2;
		Stack<double> numStack;
		for(int i=0; i<operators(); ++i){
			if(operators[i]->precedence == -2) numStack.push(numbers[numIndex++]);
			else if(operators[i]->precedence == -3) numStack.push(x);
			else{
				(operators[i]->precedence == 4) ? arg2 = 0 : arg2 = numStack.pop();
				arg1 = numStack.pop();
				numStack.push(operators[i]->operate(arg1, arg2));
			}
		}
		return numStack.pop();
	}
};

#endif