#include <iostream>
#include <string>

#include "Operators.h"
#include "Map.h"
#include "Postfix.h"
#include "Plot.h"

using namespace std;

int main(){
	OPERATORS::Initialize();
	Map<string, double> vars("Vars");
	Map<string, Postfix*> funcs("Funcs");
	vars.add("ans", 0);
	vars.add("pi", 3.14159265358979323846);
	vars.add("e", 2.71828182845904523536);
	string input = "";
	while(true){
		getline(cin, input);
		if(input == "exit" || input == "q") break;
		if(input.find("plot") == -1){
			if(input.find('=') == -1){
				Postfix expression(input, &vars);
				vars.get("ans") = expression.evaluate();
				cout << "ans -> " << vars.get("ans") << endl;
			}
			else{
				string parts[2] = {"", ""};
				int index = 0, part = 0;
				while(index<input.length()){
					if(input[index] == '=') ++part;
					else parts[part] += input[index];
					++index;
				}
				if(parts[0].find('y') == 0){
					if(funcs.contains(parts[0])){
						delete funcs.get(parts[0]);
						funcs.get(parts[0]) = new Postfix(parts[1], &vars);
					}
					else funcs.add(parts[0], new Postfix(parts[1], &vars));
					cout << parts[0] << " -> " << parts[1] << endl;
				}
				else{
					Postfix expr(parts[1], &vars);
					double val = expr.evaluate();
					if(vars.contains(parts[0])) vars.get(parts[0]) = val;
					else vars.add(parts[0], val);
					cout << parts[0] << " -> " << val << endl;
				}
			}
		}
		else {
			string parts[3] = {"", "", ""};
			for(int i=0, j=0; i<input.length() && j < 3; ++i){
				if(input[i] == ' ') ++j;
				else parts[j]+=input[i];
			}
			Postfix xRange(parts[1], &vars);
			Postfix yRange(parts[2], &vars);
			Array<Postfix*> fs;
			funcs.toArray(fs);
			//for(int i=0; i<fs.size; ++i) fs[i]->print();
			Plot::Plot(&fs, xRange.evaluate(), yRange.evaluate());
		}
	}
	Array<Postfix*> fs;
	funcs.toArray(fs);
	for(int i=0; i<fs.size; ++i) delete fs[i];
	return 0;
}