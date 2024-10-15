#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

namespace fem
{
	using std::vector;

	class InputValidator
	{
	public:
		static bool validateZPU(const vector<vector<int>>& ZPU, int cubesCount);
	};
}

#endif // INPUTVALIDATOR_H