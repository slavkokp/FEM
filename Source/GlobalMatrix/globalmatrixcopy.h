#ifndef GLOBALMATRIX_H
#define GLOBALMATRIX_H

void initGlobals(Matrix& matrix, vector<double>& vecF, Matrix3& allMGE, Matrix& allFE, vector<vector<int>>& ZP, vector<vector<int>>& NT);

void fixateSides(Matrix& globalMatrix, vector<vector<int>>& ZU, vector<vector<int>>& NT);

#endif // GLOBALMATRIX_H
