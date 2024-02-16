/**
* @file
* Prueba de evaluador de expresiones aritméticas
* @author Erwin Meza Vega <emezav@unicauca.edu.co>
* @copyright MIT License
*/
#include<iostream>
#include <iomanip>

using std::cout;
using std::endl;

#include "expression.h"

int main() {

	// Uso del evaluador de expresiones.

	expression expr("x^3 - 2*x^2 -x + 1"); //Crear la expresión
	cout << "f(x) = " << expr.str() << endl; //Obtener la representación en texto
	cout << "rpn: " << expr.rpnstr() << endl; //Obtener la presentación RPN en texto
	cout << "f(-1.0) = " << expr.eval({{"x", -1.0f}})<<endl; //Evaluar para x
	cout << "f(-0.5) = " << expr(-0.5f)<<endl; //Evaluar (suponiendo que la expresión está en términos de "x")
	cout << "===" << endl;

	expression expr2("2*a + 1"); //Crear la expresión
	cout << "f(x) = " << expr2.str() << endl; //Obtener la representación en texto
	cout << "rpn: " << expr2.rpnstr() << endl; //Obtener la presentación RPN en texto
	cout << "f(x = -1.0) = " << expr2.eval({{"a", -1.0f}})<<endl; //Evaluar para x
	cout << "f(-10.0) = " << expr2({{"a", -10.0f}})<<endl; //Evaluar para x
	cout << "f(10.0) = " << expr2(10.0f)<<endl; //Evaluar (suponiendo que la expresión está en términos de "x")
	cout << "===" << endl;

	expression expr3("e^~x - ln(x)"); //Crear la expresión
	cout << "f(x) = " << expr3.str() << endl; //Obtener la representación en texto
	cout << "rpn: " << expr3.rpnstr() << endl; //Obtener la presentación RPN en texto
	cout << "f(1.0) = " << expr3(1.0)<<endl; //Evaluar (suponiendo que la expresión está en términos de "x")
	cout << "f(1.5) = " << expr3(1.5)<<endl; //Evaluar (suponiendo que la expresión está en términos de "x")
	cout << "===" << endl;

	expression expr4("e^~(x^2) - x");  //Crear la expresión
	cout << "f(x) = " << expr4.str() << endl; //Obtener la representación en texto
	cout << "rpn: " << expr4.rpnstr() << endl; //Obtener la presentación RPN en texto
	cout << "f(-0.5) = " << expr4(-0.5f)<<endl; //Evaluar (suponiendo que la expresión está en términos de "x")
	cout << "f(0.5) = " << expr4(0.5f)<<endl; //Evaluar (suponiendo que la expresión está en términos de "x")
	cout << "===" << endl;

	expression expr5("x^4 - 6*x^3 + 12*x^2 - 10*x + 3");  //Crear la expresión
	cout << "f(x) = " << expr5.str() << endl; //Obtener la representación en texto
	cout << "rpn: " << expr5.rpnstr() << endl; //Obtener la presentación RPN en texto
	cout << "f(3.0) = " << expr5(3.0f)<<endl; //Evaluar (suponiendo que la expresión está en términos de "x")
	cout << "f(1.0) = " << expr5(1.0f)<<endl; //Evaluar (suponiendo que la expresión está en términos de "x")
	cout << "f(0.0) = " << expr5(0.0f)<<endl; //Evaluar (suponiendo que la expresión está en términos de "x")
	cout << "===" << endl;

	expression expr6("e"); //Crear la expresión
	cout << "f(x) = " << expr6.str() << endl; //Obtener la representación en texto
	cout << "rpn: " << expr6.rpnstr() << endl; //Obtener la presentación RPN en texto
	cout << "f() = " << expr6()<<endl;//Evaluar con variables predeternimadas (e, pi)
	cout << "===" << endl;

	expression expr7("pi"); //Crear la expresión
	cout << expr7.str() << endl; //Obtener la representación en texto
	cout << expr7.rpnstr() << endl; //Obtener la presentación RPN en texto
	cout << expr7()<<endl; //Evaluar con variables predeternimadas (e, pi)
	cout << "===" << endl;

	expression expr8("~pi"); //Crear la expresión
	cout << "f(x) = " << expr8.str() << endl; //Obtener la representación en texto
	cout << "rpn: " << expr8.rpnstr() << endl; //Obtener la presentación RPN en texto
	cout << "f() = " << expr8()<<endl; //Evaluar con variables predeternimadas (e, pi)
	cout << "===" << endl;

	expression expr9("sin(x)"); //Crear la expresión
	cout << "f(x) = " << expr9.str() << endl; //Obtener la representación en texto
	cout << "rpn: " << expr9.rpnstr() << endl; //Obtener la presentación RPN en texto
	cout << "f() = " << expr9(3.1416 / 2)<<endl; //Evaluar
	cout << "===" << endl;

	expression expr10("~7*e^~x + sin(tan(x^3) + cos(x - pi))");
	cout << "f(x) = " << expr10.str() << endl; //Obtener la representación en texto
	cout << "rpn: " << expr10.rpnstr() << endl; //Obtener la presentación RPN en texto
	cout << "f(pi/2) = " <<expr10(3.1416 / 2)<<endl; //Evaluar
	cout << "===" << endl;

	return 0;
}
