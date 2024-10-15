#ifndef DELTAFI_H
#define DELTAFI_H

namespace fem
{
	// dfi/dalpha nodeLocalNumber from 0 to 7
	double dfia8(double alpha, double beta, double gamma, int nodeLocalNumber);

	// dfi/dalpha nodeLocalNumber from 8 to 19
	double dfia12(double alpha, double beta, double gamma, int nodeLocalNumber);

	// dfi/dbeta nodeLocalNumber from 0 to 7
	double dfib8(double alpha, double beta, double gamma, int nodeLocalNumber);

	// dfi/dbeta nodeLocalNumber from 8 to 19
	double dfib12(double alpha, double beta, double gamma, int nodeLocalNumber);

	// dfi/dgamma nodeLocalNumber from 0 to 7
	double dfig8(double alpha, double beta, double gamma, int nodeLocalNumber);

	// dfi/dgamma nodeLocalNumber from 8 to 19
	double dfig12(double alpha, double beta, double gamma, int nodeLocalNumber);
}

#endif // DELTAFI_H
