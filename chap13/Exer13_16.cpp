#include <iostream>
#include "Exer13_16.h"
using std::cout;
using std::endl;
int main()
{
	numbered a, b = a, c = b;
	f(a); f(b); f(c);
	cout << "Real value of a, b and c" << endl;
	cout << a.mysn << endl;
	cout << b.mysn << endl;
	cout << c.mysn << endl;
	return 0;
}