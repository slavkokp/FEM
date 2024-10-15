#include "pch.h"
#include "InputValidator.h"

namespace fem
{
	bool InputValidator::validateZPU(const vector<vector<int>>& ZPU, int finiteElemsCount)
	{
		bool valid = !ZPU.empty() && std::find_if(ZPU.begin(), ZPU.end(), [&finiteElemsCount](const vector<int>& zpuEl) {return zpuEl[0] > finiteElemsCount - 1; }) == ZPU.end();
		return valid;
	}
}