#ifndef COORDSTRANSLATION_H
#define COORDSTRANSLATION_H

namespace fem
{
	// AKT is AKT of 2x2x2 by local coords,globalPointsAKT is global coordinates of body's 20 points in local numeration order
	void traslate(vector<vector<double>>& AKT, vector<vector<double>>& globalPointsAKT);

	// move all the points diagonally by adding offset value to all coordinates
	void moveAKT(vector<vector<double>>& AKT, double offset);
}

#endif // COORDSTRANSLATION_H
