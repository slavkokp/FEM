#include "pch.h"
#include "CoordsTranslation.h"
#include "Functions/fi.h"
#include "Numeration/numeration.h"

namespace fem
{
	void traslate(vector<vector<double>>& AKT, vector<vector<double>>& globalPointsAKT)
	{
		for (int i = 0; i < AKT[0].size(); i++) // points count
		{
			double newCoord[]{ 0, 0, 0 };
			for (int j = 0; j < AKT.size(); j++) // 3
			{
				for (int k = 0; k < 20; k++)
				{
					newCoord[j] += globalPointsAKT[j][k] * fii(AKT[0][i], AKT[1][i], AKT[2][i], k);
				}
			}
			AKT[0][i] = newCoord[0];
			AKT[1][i] = newCoord[1];
			AKT[2][i] = newCoord[2];
		}
	}

	void moveAKT(vector<vector<double>>& AKT, double offset)
	{
		for (int i = 0; i < AKT.size(); i++)
		{
			for (int j = 0; j < AKT[0].size(); j++)
			{
				AKT[i][j] += offset;
			}
		}
	}
}