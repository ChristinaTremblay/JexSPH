
#include <stdio.h>
#include <iostream>
#include <Eigen\Dense>

//#define Eigen_Test

#ifdef Eigen_Test

int main() {

	Eigen::MatrixXd mtx(2, 2);

	mtx << 3, 2.5, -1, 6;

	std::cout << mtx << std::endl;

	system("pause");

}

#endif