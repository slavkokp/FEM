#ifndef PSI_H
#define PSI_H

namespace fem
{
	// node 0 to 3
	double psi4(double eta, double tau, int nodeLocalNumber);

	// node 4, 6
	double psi57(double eta, double tau, int nodeLocalNumber);

	// node 5, 7
	double psi68(double eta, double tau, int nodeLocalNumber);

	// unified interface
	double psii(double eta, double tau, int nodeLocalNumber, int i);
}

#endif // PSI_H
