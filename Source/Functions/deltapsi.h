#ifndef DELTAPSI_H
#define DELTAPSI_H

namespace fem
{
	// node 0 to 3
	double dpsieta4(double eta, double tau, int nodeLocalNumber);

	// node 0 to 3
	double dpsitau4(double eta, double tau, int nodeLocalNumber);

	// node 4, 6
	double dpsieta57(double eta, double tau, int nodeLocalNumber);

	// node 4, 6
	double dpsitau57(double eta, double tau, int nodeLocalNumber);

	// node 5, 7
	double dpsieta68(double eta, double tau, int nodeLocalNumber);

	// node 5, 7
	double dpsitau68(double eta, double tau, int nodeLocalNumber);
}

#endif // DELTAPSI_H
