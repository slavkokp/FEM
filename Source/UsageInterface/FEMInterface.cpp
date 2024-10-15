#include "pch.h"
#include "FEMInterface.h"
#include "Constants/globalconsts.h"
#include "Partition/partition.h"
#include "CoordsTranslation/CoordsTranslation.h"
#include "Math/permutations.h"
#include "StiffnessMatrix/stiffnessmatrix.h"
#include "Fvector/fvector.h"
#include "GlobalMatrix/globalmatrix.h"
#include "Matrix/Matrix.h"
#include "ModifiedGaussianElimination/ModifiedGaussianElimination.h"
#include "Numeration/numeration.h"
#include "Tension/Tension.h"

namespace fem
{
	FEMInterface::FEMInterface() :
		coef(1),
		finiteElementsQuantity(0),
		ng(1)
	{
	}

	void FEMInterface::setXyzQuantity(int x, int y, int z)
	{
		this->xyzQuantity = { x, y, z };
		this->finiteElementsQuantity = x * y * z;
	}

	void FEMInterface::setXyzLengths(double x, double y, double z)
	{
		this->xyzLengths = { x, y, z };
	}

	void FEMInterface::setConsts(double e, double nu)
	{
		GlobalConsts::e = e;
		GlobalConsts::nu = nu;
		GlobalConsts::recalculateLambdaMu();
	}

	void FEMInterface::setE(double e)
	{
		GlobalConsts::e = e;
		GlobalConsts::recalculateLambdaMu();
	}

	void FEMInterface::setNu(double nu)
	{
		GlobalConsts::nu = nu;
		GlobalConsts::recalculateLambdaMu();
	}

	void FEMInterface::setGlobalAKT(const vector<vector<double>>& globalAKT)
	{
		this->globalAKT = globalAKT;
	}

	void FEMInterface::setZP(const vector<vector<int>>& ZP)
	{
		this->ZP = ZP;
	}

	void FEMInterface::setZU(const vector<vector<int>>& ZU)
	{
		this->ZU = ZU;
	}

	const vector<vector<double>>& FEMInterface::getGlobalAKT() const
	{
		return this->globalAKT;
	}

	const vector<vector<double>>& FEMInterface::getPartitionedAKT() const
	{
		return this->AKT;
	}

	vector<vector<double>> FEMInterface::getModifiedAKT() const
	{
		vector<vector<double>> modifAKT(3);
		for (int i = 0; i < 3; i++)
		{
			modifAKT[i] = this->AKT[i];
		}
		for (int i = 0; i < AKT[0].size(); ++i)
		{
			modifAKT[0][i] += coef * movements[i * 3];
			modifAKT[1][i] += coef * movements[i * 3 + 1];
			modifAKT[2][i] += coef * movements[i * 3 + 2];
		}
		return modifAKT;
	}

	const vector<double>& FEMInterface::getMovements() const
	{
		return this->movements;
	}

	const vector<vector<double>>& FEMInterface::getTensions() const
	{
		return this->tensions;
	}

	int FEMInterface::getTotalNodesQuantity() const
	{
		return this->nqp;
	}

	const vector<vector<int>>& FEMInterface::getZU() const
	{
		return this->ZU;
	}

	const vector<vector<int>>& FEMInterface::getZP() const
	{
		return this->ZP;
	}

	const vector<int>& FEMInterface::getXyzQuantity() const
	{
		return this->xyzQuantity;
	}

	const vector<double>& FEMInterface::getXyzLengths() const
	{
		return this->xyzLengths;
	}

	const int FEMInterface::getTotalFiniteElementsQuantity() const
	{
		return this->finiteElementsQuantity;
	}

	void FEMInterface::calculatePartition()
	{
		this->AKT = createAKT({ 2,2,2 }, xyzQuantity); // {2,2,2} coz it has to be standard element with local coords from -1 to 1 therefore move later
		moveAKT(AKT, -1);

		traslate(AKT, globalAKT);

		// total nodes quantity
		this->nqp = AKT[0].size();
		this->NT = createNT(AKT, xyzQuantity);

		// TODO: CHANGE THIS TO ACTUAL MAX VALUE (CURRENTLY PASSED BUT NEVER USED)
		this->ng = (NT[6][0] - NT[0][0] + 1) * 3;
	}


	void FEMInterface::calculateMovements()
	{
		vector<vector<double>> permutations = getTwentySevenGaussianNodes();
		auto DFIABG = createDFIABG(permutations);
		vector<vector<vector<vector<double>>>> jacobiansForElement(finiteElementsQuantity);
		for (int i = 0; i < finiteElementsQuantity; ++i)
		{
			jacobiansForElement[i] = createJacobian(NT, AKT, DFIABG, i);
		}

		std::map<vector<double>, int> permutationIndex;
		for (int i = 0; i < 27; ++i)
		{
			permutationIndex[permutations[i]] = i;
		}

		auto DPSIET = createDPSIET();
		vector<vector<double>> permutations2d = getNineGaussianNodes();
		std::map<vector<double>, int> permutationIndex2d;
		for (int i = 0; i < 9; ++i)
		{
			permutationIndex2d[permutations2d[i]] = i;
		}

		int globalMatrixSize = this->nqp * 3;
		auto globalMG = createGlobalStiffnessMatrix(NT, AKT, DFIABG, permutationIndex, globalMatrixSize, ng, finiteElementsQuantity);
		auto globalF = createGlobalVectorF(NT, AKT, DPSIET, ZP, permutationIndex2d, globalMatrixSize);
		fixateSides(globalMG, ZU, NT);
		this->movements = ModifiedGaussianElimination::solve(globalMG, globalF);
	}

	void FEMInterface::calculateTensions()
	{
		vector<vector<double>> points(Numeration::nodeNumberToLocalCoords.size());
		for (int i = 0; i < points.size(); ++i)
		{
			points[i] = vector<double>(Numeration::nodeNumberToLocalCoords[i].begin(), Numeration::nodeNumberToLocalCoords[i].end());
		}
		auto DFIABGForTension = createDFIABG(points);

		vector<vector<vector<double>>> allSigma(finiteElementsQuantity);
		for (int i = 0; i < finiteElementsQuantity; ++i)
		{
			allSigma[i] = calculateSigmaForElement(NT, AKT, this->movements, DFIABGForTension, i);
		}
		auto mergedSigma = mergeSigmas(NT, allSigma, nqp);
		//---------------------------------------------------------------
		this->tensions = calculateFinalSigma(mergedSigma);
		//---------------------------------------------------------------
		for (int i = 0; i < tensions.size(); ++i)
		{
			sort(tensions[i].begin(), tensions[i].end(), tensionsComparator());
		}
	}

	void FEMInterface::calculateAll()
	{
		this->calculatePartition();
		this->calculateMovements();
		this->calculateTensions();
	}
}