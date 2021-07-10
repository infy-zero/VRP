#include <iostream>

using namespace std;
class C
{
public:
	int a = -1;
};
class A
{
public:
	int a = 1;
	int b[2] = { 1,2 };
	C c;
	void print() {
		cout << a << endl << b[0] << "," << b[1] << endl << c.a << endl;
	}
};
void change(A a);
A copy(A a)
{
	change(a);
	a.a = 100;
	return a;
}

void change(A a)
{
	a.a = 2;
	a.b[0] = -1;
	a.c.a = 1;
}
void test() {
	A a;
	a.print();


	copy(a).print();

}