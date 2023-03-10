#ifndef EXPRESSION_H
#define EXPRESSION_H

/**
* @file
* Arithmethic expression evaluator
* C++ implementation of the Shunting Yard  Algorithm by E.W. Dijkstra.
* Implementación en C++ del algoritmo Shunting Yard propuesto por E. W. Dijkstra.
* Adaptado para aceptar variables, funciones personalizadas y el operador unario de negación "~"
* @author Erwin Meza Vega <emezav@unicauca.edu.co>
* @copyright MIT License
*/

#include <cfloat>
#include <cmath>
#include <functional>
#include <list>
#include <map>
#include <stack>
#include <string>
#include <sstream>
#include <vector>

#include <bits/stdc++.h>


/*
 Shunting Yard Algorithm by Edsger Wybe Dijkstra (E. W. Dijkstra)
 (Wikipedia Version)
  https://en.wikipedia.org/wiki/Shunting_yard_algorithm

	while there are tokens to be read:
	  read a token
	  if the token is:
	  - a number:
	    put it into the output queue
	  - a function:
	    push it onto the operator stack 
	  - an operator o1:
	   while (
	     there is an operator o2 at the top of the operator stack which is not a left parenthesis, 
	     and (o2 has greater precedence than o1 or (o1 and o2 have the same precedence and o1 is left-associative))
	    ):
	       pop o2 from the operator stack into the output queue
	   push o1 onto the operator stack
	  - a left parenthesis (i.e. "("):
	    push it onto the operator stack
	  - a right parenthesis (i.e. ")"):
	    while the operator at the top of the operator stack is not a left parenthesis:
	      {assert the operator stack is not empty}
	       // If the stack runs out without finding a left parenthesis, then there are mismatched parenthesis. 
	        pop the operator from the operator stack into the output queue
	        {assert there is a left parenthesis at the top of the operator stack}
		pop the left parenthesis from the operator stack and discard it
		if there is a function token at the top of the operator stack, then:
			pop the function from the operator stack into the output queue
		
	// After the while loop, pop the remaining items from the operator stack into the output queue. 
	while there are tokens on the operator stack:
		// If the operator token on the top of the stack is a parenthesis, then there are mismatched parenthesis. 
	    {assert the operator on top of the stack is not a (left) parenthesis}
        pop the operator from the operator stack onto the output queue
*/

using std::function;
using std::list;
using std::map;
using std::stack;
using std::string;
using std::vector;
using std::ostringstream;


/**
* @brief Función personalizada sobre una variable real
*/
struct CustomFunction {
	string name; /*!< Nombre de la función */
	function<double(double)> f; /*!< Función a aplicar a la variable */
	
	/**
	* @brief Evalúa la función en x
	* @param x Valor de la variable
	* @return Valor de la función en x
	*/
	double operator()(double x) {
		return f(x);
	}
};

/**
* @brief Variable de la expresión aritmética
*/
struct Variable {
	string name; /*!< Nombre de la variable */
	double val;	/*!< Valor de la variable */
};

/**
* @brief Expresión aritmética de una variable
*/
class Expression {
public: 
	/**
	* @brief Crea una expresión a partir de una cadena de caracteres
	* @param exprText Texto de la expresión en notación infija
	*/
	Expression(string exprText): Expression(exprText, defaultFunctions()) {
	}
	
	/**
	* @brief Crea una expresión a partir de una cadena de caracteres y un conjunto de funciones personalizadas
	* @param exprText Texto de la expresión en notación infija
	* @param funcs Vector de funciones personalizadas
	*/
	Expression(string exprText, vector<CustomFunction> funcs): text(exprText), functions(funcs) {
		balanced = false;
		
		//Remove spaces
		text.erase(std::remove(text.begin(),text.end(), ' '), text.end());
		//Separate into tokens
		tokens = getTokens();
		
		//Create RPN expression
		rpn = getRPN(tokens);
		
		//Save string of the original expression
		ostringstream tokens_oss;
		for (auto & t: tokens) {
			tokens_oss << t << " ";
		}
		tokensStr = tokens_oss.str();
		
		//Save string of the RPN expression
		ostringstream rpn_oss;
		for (auto & t: rpn) {
			rpn_oss << t << " ";
		}
		rpnStr = rpn_oss.str();
		
	}
	
	/**
	* @brief Funciones predeterminadas
	* @return Arreglo de funciones predeterminadas
	*/
	vector<CustomFunction> defaultFunctions() {
		return {
				{ "sin" , 
					[](double x) -> double {
						return sin(x);
					}
				},
				{ "cos" , 
					[](double x) -> double {
						return cos(x);
					}
				},
				{ "tan" , 
					[](double x) -> double {
						return tan(x);
					}
				},
				{ "ln" , 
					[](double x) -> double {
						return log(x);
					}
				},
				{ "log" , 
					[](double x) -> double {
						return log10(x);
					}
				},
				{ "exp" , 
					[](double x) -> double {
						return exp(x);
					}
				},
				{ "sqrt" , 
					[](double x) -> double {
						return sqrt(x);
					}
				},
				{ "abs" , 
					[](double x) -> double {
						return abs(x);
					}
				}
		};
	}
	
	/**
	* @brief Variables predeterminadas
	* @return Vector de variables predeterminadas
	*/
	vector<Variable> defaultVariables() {
		return {
			{"pi", 3.141592654f},
			{"e",  2.718281828f }
		};
	}
	
	/**
	* @brief Verifica si la expresión está bien balanceada
	* @return verdadero si los paréntesis están balanceados, falso en caso contrario
	*/
	bool checkParenthesis() {
		int cnt = 0;
		
		size_t pos = 0;
		
		while (pos < text.length() && cnt >= 0) {
			if (text[pos] == '(') { 
				cnt ++; 
			}
			else if (text[pos] == ')') { 
				cnt--;
			}
			pos++;
		}
		
		balanced = (cnt == 0);
		
		return balanced;
	}
	
	/**
	* @brief Retorna la condición de balance de paréntesis
	* @return Verdadero si los paréntesis están balanceados
	*/
	bool isBalanced() {
		return balanced;
	}
	
	/**
	* @brief Verifica si una cadena es un operador aritmético válido
	* @param str Cadena a verificar
	* @return Verdadero si la cadena es un operador aritmético válido
	*/
	bool isOperator(string str) {
		return (
				str == "+"
				|| str == "-"
				|| str == "*"
				|| str == "/"
				|| str == "^"
				|| str == "~"
				);
	}
	
	/**
	* @brief Verifica si una cadena es un número válido
	* @param str Cadena a verificar
	* @return Verdadero si la cadena representa un número válido
	*/
	bool isNumber(string str)
	{
		char* p;
		strtod(str.c_str(), &p);
		return *p == 0;
	}
	
	/**
	* @brief Verifica la asociatividad a la izquierda de un operador
	* @param str Operador
	* @return Verdadero si el operador tiene asociatividad a la izquierda
	*/
	bool isLeftAssociative(string str) {
		return (
				str == "+"
				|| str == "-"
				|| str == "*"
				|| str == "/"
				);
	}
	
	/**
	* @brief Verifica si un operador es binario
	* @param str Operador
	* @return Verdadero si el operador es binario, e.g. requiere dos operandos
	*/
	bool isBinaryOperator(string str) {
		return (
				str == "+"
				|| str == "-"
				|| str == "*"
				|| str == "/"
				|| str == "^"
				);
	}
	
	/** 
	* @brief Verifica si un operador es unario
	* @param str Operador
	* @return Verdadero si el operador es unario, e.g. requiere un operando
	*/
	bool isUnaryOperator(string str) {
		return (str == "~");
	}
	
	/**
	* @brief Verifica y obtiene un valor a partir de una cadena
	* @param str Cadena a evaluar
	* @param val Referencia a la variable que almacena el valor de salida
	* @return Verdadero si se puede transformar el texto en un numero
	*/
	bool getNumber(string str, double & val) {
		char* p;
		if (str.length() > 0 && str[0] == '~') {
			str[0] = '-';
		}
		val = strtod(str.c_str(), &p);
		return *p == 0;
	}
	
	/**
	* @brief Convierte un valor real a cadena
	* @param val Valor a convertir
	* @return Valor convertido a cadena de caracteres
	*/
	string numberToString(double val) {
		ostringstream oss;
		
		oss<< val;
		
		return oss.str();
	}
	
	/**
	* @brief Retorna la representación en cadena de la expresión
	* @return Representación en cadena de la expresión
	*/
	string str() {
		return tokensStr;
	}
	
	/**
	* @brief Retorna la representación en cadena de la expresión en RPN
	* @return Representación en RPN (Reverse Polish Notation) de la expresión
	*/
	string rpnstr() {
		return rpnStr;
	}
	
	/**
	* @brief Calcula el resultado de una operación binaria
	* @param a Primer operando
	* @param b Segundo operando
	* @param op Operando
	* @return Resultado de la operación, NAN si no es válida
	*/
	double calculate(double a, double b, string op){
		if (op == "+") {
			return a + b;
		}else if (op == "-") {
			return a - b;
		}else if (op == "*") {
			return a * b;
		}else if (op == "/") {
			return a / b;
		}else if (op == "^") {
			return pow(a, b);
		}
		return NAN;
	}
	
	/**
	* @brief Calcula el resultado de una operación unaria
	* @param x Valor al cual se aplica la operación
    * @param op Operando
	* @return Resultado de la operación, NAN si no es válida
	*/
	double calculateUnary(double x, string op) {
		if (op == "~") {
			return -1.0 * x;
		}
		return NAN;
	}
	
	/**
	* @brief Calcula la precedencia de un operador
	* @param op Operador
	* @return Precedencia (1 para suma y resta, 2 para producto y división, 3 para potencia y negación
	*/
	int precedence(string op){
		if(op == "+"||op == "-") {
			return 1;
		}
		else if(op == "*"||op == "/" ) {
			return 2;
		}
		else if (op == "^"|| op == "~" ) {
			return 3;
		}
		return 0;
	}
	
	/**
	* @brief Determina si un nombre dado corresponde a una variable, obteniendo su valor
	* @param name Nombre de la variable
	* @param vars Arreglo de variables definidas
	* @param val Referencia a la variable que almacena el valor obtenido
	* @return Verdadero si el nombre corresponde a una variable, valor almacenado en val.
	*/
	bool isVariable(string name, vector<Variable> & vars, double & val) {
		if (isOperator(name)) {
			return false;
		}
		for (auto & v: vars) {
			if (v.name == name) {
				val = v.val;
				return true;
			}else if ("~" + v.name ==  name) {
				val = -1.0 * v.val;
				return true;
			}
		}
		return false;
	}
	
	/**
	* @brief Verifica si un nombre dado corresponde a una función
	* @param name Nombre a verificar
	* @param func Referencia a la función obtenida, si existe
	* @return Verdadero si la función está definida, falso en caso contrario
	*/
	bool isFunction(string name, function<double(double)> & func) {
		for (auto & f : functions) {
			if (f.name == name) {
				func = f;
				return true;
			}
		}
		
		return false;
	}
	
	/**
	* @brief Separa los elementos de la expresión
	* @return Vector de cadenas con los elementos separados
	*/
	vector<string> getTokens() {
		
		vector<string> tokens;

		string token;
		string opchar;
		
		size_t pos = 0;
		size_t nextPos;
		
		
		// Verificar parentesis
		if (!checkParenthesis()) {
			return tokens;
		}
		
		nextPos = string::npos;
		
		while ((nextPos = text.find_first_of("()+-*/^~", pos)) != string::npos) {
			if (nextPos != string::npos) {
				opchar =  std::string(1, text[nextPos]);
				
				token = text.substr(pos, nextPos - pos);
				if (token.length() > 0) {
					tokens.push_back(token);
				}
				tokens.push_back(opchar);
			}else {
				token = text.substr(pos, nextPos - pos);
				tokens.push_back(token);
			}
			pos = nextPos;
			if (pos != string::npos) {
				pos++;
			}
		}
		token = text.substr(pos, nextPos);
		if (token.length() > 0) {
			tokens.push_back(token);
		}
		return tokens;
	}
	
	/**
	* @brief Obtiene la representación RPN de la expresión original
	* @param tokens Vector de elementos separados
	* @return Vector de elementos de la expresión en Notación Reversa Polaca (RPN)
	*/
	vector<string> getRPN(vector<string> tokens) {

		vector<string> input(tokens);
		vector<string> rpn;
		stack<string> operators;
		double val;
		function<double(double)> func;
		string op;
		
		string token;
		
		size_t i;
		//Shunting Yard Algorithm
		
		//while there are tokens to be read:
		
		for (i = 0; i < input.size(); i++)  {
			//Read a token
			token = input[i];
			//If the token is:
			//a number:
			if (getNumber(token, val)) {
				//put it into the output queue
				rpn.push_back(numberToString(val));
			}
			//a function:
			else if (isFunction(token, func)) {
				//push it into the operator stack
				operators.push(token);
			}
			//an operator o1:
			else if (isOperator(token)) {
				//while (
				//there is an operator o2 at the top of the operator stack which is not a left parenthesis, 
				//and (o2 has greater precedence than o1 or (o1 and o2 have the same precedence and o1 is left-associative))
				//)
				while (!operators.empty() && operators.top() != "("
					   && (precedence(operators.top()) > precedence(token) || 
						   (precedence(operators.top()) == precedence(token) && isLeftAssociative(token)))) {
					//pop o2 from the operator stack into the output queue
					op = operators.top();
					operators.pop();
					
					rpn.push_back(op);
				}
				//push o1 onto the operators stack
				operators.push(token);
			}
			//a left parenthesis i.e. "("
			else if (token == "(") {
				// push it onto the operator stack
				operators.push(token);
			}
			//a right parenthesis (i.e. ")")
			else if (token == ")") {
				//while the operator at the top of the operator stack is not a left parenthesis:
				//{assert the operator stack is not empty}
				//// If the stack runs out without finding a left parenthesis, then there are mismatched parenthesis. 
				
				while (!operators.empty() && operators.top() != "(") {
					//pop the operator from the operator stack into the output queue
					op = operators.top();
					operators.pop();
					rpn.push_back(op);
				}
				//{assert there is a left parenthesis at the top of the operator stack}
				//pop the left parenthesis from the operator stack and discard it
				
				if (!operators.empty() && operators.top() == "(") {
					operators.pop();
				}
				
				//if there is a function token at the top of the operator stack, then:
				//pop the function from the operator stack into the output queue
				if (!operators.empty() && isFunction(operators.top(), func)) {
					op = operators.top();
					operators.pop();
					rpn.push_back(op);
				}
			}
			//Variable! colocarla en la salida
			else {
				//Variable
				rpn.push_back(token);
			}
		}
		
		/* 
		After the while loop, pop the remaining items from the operator stack into the output queue. 
		while there are tokens on the operator stack:
		*/
		while (!operators.empty()) {
			// If the operator token on the top of the stack is a parenthesis, then there are mismatched parenthesis. 
			// Ya se ha verificado el balance de parentesis
			//{assert the operator on top of the stack is not a (left) parenthesis}
			//pop the operator from the operator stack onto the output queue
			op = operators.top();
			operators.pop();
			if (op != "(") {
				rpn.push_back(op);
			}
		}
		
		// rpn almacena la representación RPN de la expresión
		return rpn;
	}
	
	/**
	* @brief Sobrecarga del operador ()
	* @return Resultado de evaluar la expresión
	*/
	double operator()() {
		return eval();
	}
	
	/**
	* @brief Sobrecarga del operador () sobre x
	* @param val Valor de la variable x de la expresión
	* @return Resultado de evaluar la expresión
	*/
	double operator()(double val) {
		return eval(val);
	}
	
	/**
	* @brief Sobrecarga del operador () sobre variables
	* @param vars Arreglo de variables a reemplazar
	* @return Resultado de evaluar la expresión
	*/
	double operator()(vector<Variable> vars) {
		return eval(vars);
	}
	
	/**
	* @brief Evalúa la expresión usando las variables por defecto
	* @return Resultado de evaluar la expresión
	*/
	double eval() {
		vector<Variable> vars = defaultVariables();
		return eval(vars);
	}
	
	/**
	* @brief Evalúa la expresión para un valor de x
	* @param val Valor para una única variable llamada x
	* @return Resultado de la expresión
	*/
	double eval(double val) {
		
		vector<Variable> vars = defaultVariables();
		bool updated = false;
		
		for (auto & v: vars) {
			if (v.name == "x") {
				v.val = val;
				updated = true;
			}
		}
		
		if (!updated) {
			vars.push_back({"x", val});
		}
		
		return eval(vars);
	}
	
	/**
	* @brief Evalúa la expresión para una un más variables
	* @param vars Vector de variables
	* @return Resultado de la expresión
	*/
	double eval(vector<Variable> vars) {
		return evalRPN(rpn, vars);
	}
	

	/**
	* @brief Evalúa la expresión para una un más variables y una o más funciones
	* @param rpn Expresión (vector de cadenas) en RPN
	* @param vars Vector de variables
	* @return Resultado de la expresión
	*/
	double evalRPN(vector<string> rpn, vector<Variable> vars) {
		double result = NAN;
		
		stack<double> values;
		
		vector<string> items = rpn;
		function<double(double)> func;
		double val;

		
		for (auto & item: rpn) {
			if (getNumber(item, val)) {
				values.push(val);
			}
			else if (isFunction(item, func)) {
				//Verificar si existe un valor en la pila
				if (values.empty()) {
					return result;
				}
				//Obtener el valor del tope de la pila
				val = values.top();
				//Eliminar el valor obtenido de la pila
				values.pop();
				//Evaluar la funcion y almacenar el resultado en la pila
				values.push(func(val));
			}
			else if (isOperator(item)) {
				if (isBinaryOperator(item) && values.size() >= 2) {
					//Obtener b, a de la pila (b esta en el tope)
					double b = values.top();
					values.pop();
					double a = values.top();
					values.pop();
					//Calcular y almacenar el resultado en la pila
					values.push(calculate(a, b, item));
				}else if (isUnaryOperator(item) && values.size() > 0) {
					//Obtener un valor del tope de la pila
					val = values.top();
					values.pop();
					//Calcular y almacenar el resultado en la pila
					values.push(calculateUnary(val, item));
				}
			}
			else if (isVariable(item, vars, val)) {
				//Almacenar el valor de la variable en la pila
				values.push(val);
			}
		}
		//Al final de la ejecucion, el tope de la pila contiene el resultado
		if (values.size() == 1) {
			result = values.top();
			values.pop();
		}
		
		return result;
	}
	
private:
	string text; /*!< Texto original de la expresión */
	vector<CustomFunction> functions; /*!< Vector de funciones personalizadas */
	vector<string> tokens; /*!< Elementos separados de la expresión */
	string tokensStr; /*!< Cadena de texto de la expresión */
	vector<string> rpn; /*!< Elementos separados de la expresión en RPN */
	string rpnStr; /*!< Cadena de texto de la expresión  en RPN */
	bool balanced; /*!< Verdadero si los parentesis se encuentran balanceados */
};

#endif
