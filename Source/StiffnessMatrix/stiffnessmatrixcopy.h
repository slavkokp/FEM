#ifndef STIFFNESSMATRIX_H
#define STIFFNESSMATRIX_H


vector<vector<vector<double>>> createDFIABG();


// node - gaussian node, der - derivative(0 - alpha, 1 - beta, 2 - gamma), coord - {x,y,z}, i - actual node in geometry, jacobianMatrices27 - 27 3x3 matrices for node number "element"
vector<vector<vector<double>>> createJacobian(vector<vector<int>>& NT, vector<vector<double>>& AKT, vector<vector<vector<double>>>& DFIABG, int element);


// the same structure as the DFIABG
vector<vector<vector<vector<double>>>> createDFIXYZ(vector<vector<vector<vector<double>>>>& jacobiansForElement, vector<vector<vector<double>>>& DFIABG, int finiteElementsQuantity);


// for each finite element
vector<vector<vector<double>>> createAe11(vector<vector<vector<vector<double>>>>& DFIXYZ, vector<vector<double>>& jacobianValuesForElement,
                                          int finiteElementsQuantity, std::map<vector<double>, int>& permutationIndex);


// for each finite element
vector<vector<vector<double>>> createAe22(vector<vector<vector<vector<double>>>>& DFIXYZ, vector<vector<double>>& jacobianValuesForElement,
                                          int finiteElementsQuantity, std::map<vector<double>, int>& permutationIndex);


// for each finite element
vector<vector<vector<double>>> createAe33(vector<vector<vector<vector<double>>>>& DFIXYZ, vector<vector<double>>& jacobianValuesForElement,
                                          int finiteElementsQuantity, std::map<vector<double>, int>& permutationIndex);


// for each finite element
vector<vector<vector<double>>> createAe12(vector<vector<vector<vector<double>>>>& DFIXYZ, vector<vector<double>>& jacobianValuesForElement,
                                          int finiteElementsQuantity, std::map<vector<double>, int>& permutationIndex);


// for each finite element
vector<vector<vector<double>>> createAe13(vector<vector<vector<vector<double>>>>& DFIXYZ, vector<vector<double>>& jacobianValuesForElement,
                                          int finiteElementsQuantity, std::map<vector<double>, int>& permutationIndex);


// for each finite element
vector<vector<vector<double>>> createAe23(vector<vector<vector<vector<double>>>>& DFIXYZ, vector<vector<double>>& jacobianValuesForElement,
                                          int finiteElementsQuantity, std::map<vector<double>, int>& permutationIndex);


// MGE of size: finiteElementsQuantity x 60 x 60
vector<vector<vector<double>>> createMGE(vector<vector<vector<double>>>& allAe11,
                                         vector<vector<vector<double>>>& allAe22,
                                         vector<vector<vector<double>>>& allAe33,
                                         vector<vector<vector<double>>>& allAe12,
                                         vector<vector<vector<double>>>& allAe13,
                                         vector<vector<vector<double>>>& allAe23,
                                         int finiteElementsQuantity);

#endif // STIFFNESSMATRIX_H
