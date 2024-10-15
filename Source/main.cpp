#include "pch.h"
#include "StiffnessMatrix/stiffnessmatrix.h"
#include "Fvector/fvector.h"
#include "Partition/partition.h"
#include "Math/permutations.h"
#include "Math/determinant.h"
#include "GlobalMatrix/globalmatrix.h"
#include "gaussianelimination.h"
#include "ModifiedGaussianElimination/ModifiedGaussianElimination.h"
#include "Matrix/Matrix.h"
#include "Math/round.h"
#include "Tension/Tension.h"
#include "Numeration/numeration.h"
#include "Constants/globalconsts.h"
#include "CoordsTranslation/CoordsTranslation.h"

using namespace fem;

void parseCommandLineInput(int argc, char* argv[], vector<double>& xyzLength, vector<int>& xyzQuantity)
{
    xyzLength.clear();
    xyzQuantity.clear();
    for (int i = 1; i < 4; i++)
    {
        xyzLength.push_back(std::stoi(argv[i]));
    }
    for (int i = 4; i < 7; i++)
    {
        xyzQuantity.push_back(std::stod(argv[i]));
    }
    GlobalConsts::e = std::stod(argv[7]);
    GlobalConsts::nu = std::stod(argv[8]);
}

void readZP(const std::string& filename, vector<vector<int>>& ZP)
{
    std::ifstream f;
    f.open(filename);
    ZP = vector<vector<int>>();
    while(!f.eof())
    {
        vector<int> tmp(3);
        f >> tmp[0];
        if (f.eof())
        {
            break;
        }
        f >> tmp[1] >> tmp[2];
        ZP.push_back(tmp);
    }
    f.close();
}

void readZU(const std::string& filename, vector<vector<int>>& ZU)
{
    std::ifstream f;
    f.open(filename);
    ZU = vector<vector<int>>();
    while (!f.eof())
    {
        vector<int> tmp(2);
        f >> tmp[0];
        if (f.eof())
        {
            break;
        }
        f >> tmp[1];
        ZU.push_back(tmp);
    }
    f.close();
}

void readGlobalAKT(const std::string& filename, vector<vector<vector<double>>>& globalAKT)
{
    std::ifstream f;
    f.open(filename);
    for (int k = 0; k < globalAKT.size(); k++)
    {
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                f >> globalAKT[k][j][i];
            }
        }
    }
    f.close();
}

void readGlobalAKT(const std::string& filename, vector<vector<double>>& globalAKT)
{
    std::ifstream f;
    f.open(filename);
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            f >> globalAKT[j][i];
        }
    }
    f.close();
}

void readGlobalNT(const std::string& filename, vector<vector<int>>& globalNT)
{
    std::ifstream f;
    int NTSize = 0;
    f.open(filename);
    f >> NTSize;
    globalNT = vector<vector<int>>(20, vector<int>(NTSize));
    for (int i = 0; i < globalNT[0].size(); i++)
    {
        for (int j = 0; j < 20; j++)
        {
            f >> globalNT[j][i];
        }
    }
    f.close();
}

void copyNT(vector<vector<int>>& NT, int** rawNT)
{
    for (int i = 0; i < NT[0].size(); i++)
    {
        for (int j = 0; j < NT.size(); j++)
        {
            rawNT[i][j] = NT[j][i];
        }
    }
}

void copyAKT(vector<vector<double>>& AKT, double** rawAKT)
{
    for (int i = 0; i < AKT.size(); i++)
    {
        for (int j = 0; j < AKT[0].size(); j++)
        {
            rawAKT[i][j] = AKT[i][j];
        }
    }
}

// AKTS[i][j][k], i - finite elements AKTS count | j - x,y,z | k - global point number in [i] AKT space
vector<vector<double>> uniteAKT(vector<Matrix> AKTS, vector<int>& xyzQuantity) 
{
    vector<vector<double>> newAKT = vector<vector<double>>(3);
    int longRowPointsCount = 1 + 2 * xyzQuantity[0];
    int shortRowPointsCount = 1 + xyzQuantity[0];
    int bigLayerPointsCount = ((longRowPointsCount + shortRowPointsCount) * xyzQuantity[1]) + longRowPointsCount;
    int AKTCount = AKTS.size();
    int pointsInAKT = AKTS[0][0].size();

    // The first AKT
    for (int j = 0; j < pointsInAKT; j++)
    {
        newAKT[0].push_back(AKTS[0][0][j]);
        newAKT[1].push_back(AKTS[0][1][j]);
        newAKT[2].push_back(AKTS[0][2][j]);
    }
    // all AKT between first and last
    for (int i = 1; i < AKTCount - 1; i++)
    {
        for (int j = bigLayerPointsCount; j < pointsInAKT; j++)
        {
            newAKT[0].push_back(AKTS[i][0][j]);
            newAKT[1].push_back(AKTS[i][1][j]);
            newAKT[2].push_back(AKTS[i][2][j]);
        }
    }
    // The last AKT
    for (int j = bigLayerPointsCount; j < pointsInAKT - bigLayerPointsCount; j++)
    {
        newAKT[0].push_back(AKTS[AKTCount - 1][0][j]);
        newAKT[1].push_back(AKTS[AKTCount - 1][1][j]);
        newAKT[2].push_back(AKTS[AKTCount - 1][2][j]);
    }
    return newAKT;
}

int calc(int argc, char *argv[])
{
    // input begin

    // figure dimensions
    vector<double> xyzLengths(3);
    xyzLengths = { 2, 2, 2 };

    //partitions
    vector<int> xyzQuantity(3);
    xyzQuantity = { 2, 2, 2 };

    // to press use negative power and to stretch positive (dzero ne testiv)
    // powered sides, list of <elemNumber, sideNumber, POWER> elemNumber begins from 0, side number begins from 1
    vector<vector<int>> ZP;
    ZP = { {2, 6, 10}, {3, 6, 10} };
    //ZP = { {0, 6, 10} };

    //fixed sides, list of pairs <elemNumber, sideNumber> elemNumber begins from 0, side number begins from 1
    vector<vector<int>> ZU;
    ZU = { {0, 5}, {1, 5} };
    //ZU = { {0, 5} };

    //test input for 1 finite element // hz if works
    //xyzLengths = {2,2,2};
    //xyzQuantity = {1,1,1};
    //ZP = {{0, 6, -1}};
    //ZU = {{0, 5}};

    //input end
    //GlobalConsts::e = std::stod("110");
    //GlobalConsts::nu = std::stod("0.35");
    parseCommandLineInput(argc, argv, xyzLengths, xyzQuantity);
    GlobalConsts::recalculateLambdaMu();
    readZP("ZP.txt", ZP);
    readZU("ZU.txt", ZU);

    int finiteElementsQuantity = xyzQuantity[0] * xyzQuantity[1] * xyzQuantity[2];

    // THIS IS SOME SHIT WITH MULTIPLE AKTs (works but useless)
    /*
    vector<Matrix> AKTS(3);
    vector<vector<vector<double>>> globalAKTS(3, vector<vector<double>>(3, vector < double>(20)));
    //readGlobalAKT("C:/Users/User/source/repos/ComputerVisionWalkInterface/global_AKT_save_test.txt", globalAKTS);
    readGlobalAKT("globalAKT.txt", globalAKTS);
    // local AKT
    for (int i = 0; i < AKTS.size(); i++)
    {
        AKTS[i] = createAKT({2,2,2}, xyzQuantity); // {2,2,2} coz it has to be standard element with local coords from -1 to 1 therefore move later
        moveAKT(AKTS[i], -1);
        traslate(AKTS[i], globalAKTS[i]);
    }
    auto AKTunion = uniteAKT(AKTS, xyzQuantity);
    xyzQuantity[2] *= AKTS.size();
    finiteElementsQuantity = xyzQuantity[0] * xyzQuantity[1] * xyzQuantity[2];
    auto NTunion = createNT(AKTunion, xyzQuantity);
    int pointsCount = AKTunion[0].size();
    for (int j = finiteElementsQuantity - xyzQuantity[0] * xyzQuantity[1]; j < finiteElementsQuantity; j++)
    {
        NTunion[4][j] = NTunion[4][j] % pointsCount;
        NTunion[5][j] = NTunion[5][j] % pointsCount;
        NTunion[6][j] = NTunion[6][j] % pointsCount;
        NTunion[7][j] = NTunion[7][j] % pointsCount;
        NTunion[16][j] = NTunion[16][j] % pointsCount;
        NTunion[17][j] = NTunion[17][j] % pointsCount;
        NTunion[18][j] = NTunion[18][j] % pointsCount;
        NTunion[19][j] = NTunion[19][j] % pointsCount;
    }
    */
    auto AKT = createAKT({2,2,2}, xyzQuantity); // {2,2,2} coz it has to be standard element with local coords from -1 to 1 therefore move later
    moveAKT(AKT, -1);
    //vector<vector<int>> globalNT;
    //readGlobalNT("save_nt.txt", globalNT);

    //vector<vector<vector<double>>> globalAKT(globalNT[0].size(), vector<vector<double>>(3, vector<double>(20)));
    vector<vector<double>> globalAKT(3, vector<double>(20));
    
    readGlobalAKT("globalAKT.txt", globalAKT);

    traslate(AKT, globalAKT);
    //auto AKT = AKTunion; // multiple AKTs
    // total nodes quantity
    size_t nqp = AKT[0].size();
    auto NT = createNT(AKT, xyzQuantity);
    //auto NT = NTunion; // multiple AKTs
    std::ofstream asf;
    asf.open("AKT.txt");
    for (int i = 0; i < AKT.size(); ++i)
    {
        for (int j = 0; j < AKT[0].size(); ++j)
        {
            asf << AKT[i][j] << ' ';
        }
        asf << '\n';
    }
    asf.close();
    asf.open("NT.txt");
    asf << NT[0].size() << '\n';
    for (int i = 0; i < NT[0].size(); ++i)
    {
        for (int j = 0; j < NT.size(); ++j)
        {
            asf << NT[j][i] << ' ';
        }
        asf << '\n';
    }
    asf.close();
    int ng = (NT[6][0] - NT[0][0] + 1) * 3;
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

    int globalMatrixSize = nqp * 3;
    auto globalMG = createGlobalStiffnessMatrix(NT, AKT, DFIABG, permutationIndex, globalMatrixSize, ng, finiteElementsQuantity);
    auto globalF = createGlobalVectorF(NT, AKT, DPSIET, ZP, permutationIndex2d, globalMatrixSize);
    fixateSides(globalMG, ZU, NT);

    //globalMG.makeValidForGE();
//    std::ofstream f;
//    f.open("outputGlobalNewWorking.txt");
//    for (int i = 0; i < globalMG.getHeight(); ++i)
//    {
//        for (int j = 0; j < globalMG.getWidth(); ++j)
//        {
//            f << round_up(globalMG.at(i, j), 2) << '\t';
//            //f << globalMG[i][j] << '\t';
//        }
//        if(i != globalMG.getHeight() - 1)
//        {
//            f << '\n';
//        }
//    }
//    //f << "\n\n\n----------------------------------------------------------------------\n\n\n";

//    f.close();

    // coef to multiply result values so it would be easier to notice
    double coef = 1;
    auto finalRes = ModifiedGaussianElimination::solve(globalMG, globalF);
   /* for (int i = 0; i < finalRes.size(); i++)
    {
        cout << finalRes[i] << '\n';
    }*/
    std::ofstream fres;
    fres.open("Movement.txt");
    for (int j = 0; j < finalRes.size(); j++)
    {
        fres << finalRes[j] << '\n';
    }
    fres.close();

    // tension calculation
    vector<vector<double>> points(Numeration::nodeNumberToLocalCoords.size());
    for (int i = 0; i < points.size(); ++i)
    {
        points[i] = vector<double>(Numeration::nodeNumberToLocalCoords[i].begin(), Numeration::nodeNumberToLocalCoords[i].end());
    }
    auto DFIABGForTension = createDFIABG(points);

    vector<vector<vector<double>>> allSigma(finiteElementsQuantity);
    for (int i = 0; i < finiteElementsQuantity; ++i)
    {
        allSigma[i] = calculateSigmaForElement(NT, AKT, finalRes, DFIABGForTension, i);
    }
    auto mergedSigma = mergeSigmas(NT, allSigma, nqp);
    //---------------------------------------------------------------
    auto tensions = calculateFinalSigma(mergedSigma);
    //---------------------------------------------------------------
    for (int i = 0; i < tensions.size(); ++i)
    {
        sort(tensions[i].begin(), tensions[i].end(), tensionsComparator());
        cout << i << ") " << " " << tensions[i][0] << " " << tensions[i][1] << " " << tensions[i][2] << '\n';
    }


    // modif AKT
    for (int i = 0; i < AKT[0].size(); ++i)
    {
        AKT[0][i] += coef * finalRes[i * 3];
        AKT[1][i] += coef * finalRes[i * 3 + 1];
        AKT[2][i]+= coef * finalRes[i * 3 + 2];
    }

    
    std::ofstream f;
    f.open("AKT.txt");
    for (int i = 0; i < AKT.size(); ++i)
    {
        for (int j = 0; j < AKT[0].size(); ++j)
        {
            f << AKT[i][j] << ' ';
        }
        f << '\n';
    }
    f.close();
    f.open("NT.txt");
    f << NT[0].size() << '\n';
    for (int i = 0; i < NT[0].size(); ++i)
    {
        for (int j = 0; j < NT.size(); ++j)
        {
            f << NT[j][i] << ' ';
        }
        f << '\n';
    }
    f.close();
    f.open("Tensions.txt");
    for (int j = 0; j < tensions[0].size(); j++)
    {
        for (int i = 0; i < tensions.size(); ++i)
        {
            f << tensions[i][j] << ' ';
        }
        f << '\n';
    }
    f.close();
    // good oda print
    /*for (int i = 0; i < 20; ++i)
    {
        f << AKT[0][NT[i][0]] << ' ' << AKT[1][NT[i][0]] << ' ' << AKT[2][NT[i][0]] << '\n';
    }*/

    return 0;
}
