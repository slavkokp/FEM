#include "pch.h"
#include "globalconsts.h"

namespace fem
{
	const vector<double> GlobalConsts::c = { 5.0 / 9, 8.0 / 9, 5.0 / 9 };

	const vector<double> GlobalConsts::xg = { -sqrt(0.6), 0, sqrt(0.6) };

	double GlobalConsts::e = 110;
	double GlobalConsts::nu = 0.35;
	double GlobalConsts::lambda = e / ((1 + nu) * (1 - 2 * nu));
	double GlobalConsts::mu = e / (2 * (1 + nu));

	void GlobalConsts::recalculateLambdaMu()
	{
		lambda = e / ((1 + nu) * (1 - 2 * nu));
		mu = e / (2 * (1 + nu));
	}
}