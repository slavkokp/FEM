#ifndef STIFFNESSMATRIX_H
#define STIFFNESSMATRIX_H

namespace fem
{
	// permutations - vector containing the points in which dfi is calculated
	vector<vector<vector<double>>> createDFIABG(vector<vector<double>>& permutations);


	// node - gaussian node, der - derivative(0 - alpha, 1 - beta, 2 - gamma), coord - {x,y,z}, i - actual node in geometry, jacobianMatrices27 - 27 3x3 matrices for node number "element"
	vector<vector<vector<double>>> createJacobian(vector<vector<int>>& NT, vector<vector<double>>& AKT, vector<vector<vector<double>>>& DFIABG, int element);

	vector<double> createJacobianValuesForElement(vector<vector<vector<double>>>& jacobiansForElement);

	// the same structure as the DFIABG
	vector<vector<vector<double>>> createDFIXYZ(vector<vector<vector<double>>>& jacobiansForElement, vector<vector<vector<double>>>& DFIABG);


	// for single finite element
	vector<vector<double>> createAe11(vector<vector<vector<double>>>& DFIXYZ, vector<double>& jacobianValuesForElement, std::map<vector<double>, int>& permutationIndex);


	// for single finite element
	vector<vector<double>> createAe22(vector<vector<vector<double>>>& DFIXYZ, vector<double>& jacobianValuesForElement, std::map<vector<double>, int>& permutationIndex);


	// for single finite element
	vector<vector<double>> createAe33(vector<vector<vector<double>>>& DFIXYZ, vector<double>& jacobianValuesForElement, std::map<vector<double>, int>& permutationIndex);


	// for single finite element
	vector<vector<double>> createAe12(vector<vector<vector<double>>>& DFIXYZ, vector<double>& jacobianValuesForElement, std::map<vector<double>, int>& permutationIndex);


	// for single finite element
	vector<vector<double>> createAe13(vector<vector<vector<double>>>& DFIXYZ, vector<double>& jacobianValuesForElement, std::map<vector<double>, int>& permutationIndex);


	// for single finite element
	vector<vector<double>> createAe23(vector<vector<vector<double>>>& DFIXYZ, vector<double>& jacobianValuesForElement, std::map<vector<double>, int>& permutationIndex);


	// MG for element of size: 60 x 60
	vector<vector<double>> createMGE(vector<vector<vector<double>>>& jacobiansForElement, vector<vector<vector<double>>>& DFIABG, std::map<vector<double>, int>& permutationIndex);

}

#endif // STIFFNESSMATRIX_H