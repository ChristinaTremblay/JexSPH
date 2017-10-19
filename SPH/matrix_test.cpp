
#include <iostream>
#include "JexMath.h"

using namespace std;

//#define Matrix_Test

#ifdef Matrix_Test

int main() {

	float a[16];
	for (int i = 0; i < 16; i++)
		a[i] = float(i);

	Jex::Float16 f16(a);

	for (int i = 0; i < 16; i++)
		cout << f16[i] << endl;

	Jex::Mtx4f m(a);
	Jex::Mtx4f n;
	m *= n; 
	m.print_info();

	Jex::Float4 v4(1.0, 2.0, 3.0, 4.0);

	v4 = a * v4;
	v4.print_info();

	system("pause");
}

#endif