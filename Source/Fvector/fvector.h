#ifndef FVECTOR_H
#define FVECTOR_H

namespace fem
{
	vector<vector<vector<double>>> createDPSIET();
	vector<vector<vector<double>>> createDPSIXYZ(Matrix3& DPSIET, vector<vector<double>>& AKT, vector<vector<int>>& NT, int finiteElementIndex, int side);

	// for 1 powered element from ZPi
	vector<double> createFe1(Matrix3& DPSIET, vector<vector<double>>& AKT, vector<vector<int>>& NT, vector<int>& ZPi, std::map<vector<double>, int>& permutationIndex);
	vector<double> createFe2(Matrix3& DPSIET, vector<vector<double>>& AKT, vector<vector<int>>& NT, vector<int>& ZPi, std::map<vector<double>, int>& permutationIndex);
	vector<double> createFe3(Matrix3& DPSIET, vector<vector<double>>& AKT, vector<vector<int>>& NT, vector<int>& ZPi, std::map<vector<double>, int>& permutationIndex);
	vector<double> createFE(Matrix3& DPSIET, vector<vector<double>>& AKT, vector<vector<int>>& NT, vector<int>& ZPi, std::map<vector<double>, int>& permutationIndex);
}

#endif // FVECTOR_H
