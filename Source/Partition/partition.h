#ifndef PARTITION_H
#define PARTITION_H

namespace fem
{
	vector<vector<double>> createAKT(vector<double> xyzLength, vector<int> xyzQuantity);

	vector<vector<int>> createNT(vector<vector<double>>& AKT, vector<int>& xyzQuantity);

	// donut
	vector<vector<double>> createClosedAKT(vector<double> xyzLength, vector<int> xyzQuantity);

	// donut
	vector<vector<int>> createClosedNT(vector<vector<double>>& AKT, vector<int>& xyzQuantity, vector<vector<int>>& NT, bool close);
}

#endif // PARTITION_H
