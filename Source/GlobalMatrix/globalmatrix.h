#ifndef GLOBALMATRIX_H
#define GLOBALMATRIX_H

namespace fem
{
	class SMatrix;

	void initGlobals(Matrix& matrix, vector<double>& vecF, Matrix3& allMGE, Matrix& allFE, vector<vector<int>>& ZP, vector<vector<int>>& NT);

	void fixateSides(SMatrix& globalMatrix, vector<vector<int>>& ZU, vector<vector<int>>& NT);

	SMatrix createGlobalStiffnessMatrix(vector<vector<int>>& NT, vector<vector<double>>& AKT, vector<vector<vector<double>>>& DFIABG, std::map<vector<double>, int>& permutationIndex, int globalMatrixSize, int ng, int finiteElementsQuantity);

	vector<double> createGlobalVectorF(vector<vector<int>>& NT, vector<vector<double>>& AKT, vector<vector<vector<double>>>& DPSIET, vector<vector<int>>& ZP, std::map<vector<double>, int>& permutationIndex2d, int globalVecFSize);
}

#endif // GLOBALMATRIX_H
