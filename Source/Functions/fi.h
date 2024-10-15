#ifndef FI_H
#define FI_H

namespace fem
{
	// fi nodeLocalNumber from 0 to 7
	double fi8(double alpha, double beta, double gamma, int nodeLocalNumber);

	// fi nodeLocalNumber from 8 to 19
	double fi12(double alpha, double beta, double gamma, int nodeLocalNumber);

	// nodeLocalNumber == i so I don't pass any i
	double fii(double alpha, double beta, double gamma, int nodeLocalNumber);
}

#endif // FI_H