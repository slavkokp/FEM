#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QScreen>
#include <QtGui/QFontDatabase>
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include "datamodifier.h"
#include "StiffnessMatrix/stiffnessmatrixcopy.h"
#include "Fvector/fvector.h"
#include "Partition/partition.h"
#include "Math/permutations.h"
#include "Math/determinant.h"
#include "GlobalMatrix/globalmatrixcopy.h"
#include "gaussianelimination.h"
#include "Math/round.h"
//#include "ModifiedGaussianElimination/ModifiedGaussianElimination.h"
//#include "Matrix/Matrix.h"



// mainwindow and datamodifier do not contain logic(probably), pch - precompiled header with some constants
/*
     MAIN ISSUES:
     1) code for stiffness matirx and fvector f should be identical(have same idea but different dimensions (3 vs 2)) BUT
        functions createMGE creates stiffness matrices for all finite elements and createFE creates Fvector only for 1 powered element(not for all powered elements at once)
        which is why loop is used in main and functions inside the stiffnessmatrix and fvector share the same difference
     2) gaussian elemination should be replaced, MGE matrices should be stored and treated as symmetrical, globalMG matrix should be stored as n diagonal symmetrical and treated accordingly
     3) partition(akt and np creation) is shit and should be redone according to new WOLK rules
     4) parameters(constants)  {e, nu, lambda, mu} which are used in stiffness matrix creation are not set globally instead there are 6 functions which use them:
     createAe11
     createAe22
     createAe33
     createAe12
     createAe13
     createAe23
     which all use their local set of constants which have equall value. obviously I should fix that
     5) there may be some more issues that I can't remember (or am not AWARE of)
*/





int main(int argc, char *argv[])
{
    // input begin

    // figure dimensions
    vector<double> xyzLengths(3);
    xyzLengths = { 4, 2, 4 };

    //partitions
    vector<int> xyzQuantity(3);
    xyzQuantity = { 2, 1, 2 };

    // to press use negative power and to stretch positive (dzero ne testiv)
    // powered sides, list of <elemNumber, sideNumber, POWER> elemNumber begins from 0, side number begins from 1
    vector<vector<int>> ZP;
    ZP = {{2, 6, -20}, {3, 6, -20}};


    //fixed sides, list of pairs <elemNumber, sideNumber> elemNumber begins from 0, side number begins from 1
    vector<vector<int>> ZU;
    ZU = {{0, 5}, {1, 5}};

    //test input for 1 finite element // hz if works
    //xyzLengths = {2,2,2};
    //xyzQuantity = {1,1,1};
    //ZP = {{0, 6, -1}};
    //ZU = {{0, 5}};
    //input end

    int finiteElementsQuantity = xyzQuantity[0] * xyzQuantity[1] * xyzQuantity[2];

    auto AKT = createAKT(xyzLengths, xyzQuantity);
    auto NT = createNT(AKT, xyzQuantity);
    auto DFIABG = createDFIABG();
    vector<vector<vector<vector<double>>>> jacobiansForElement(finiteElementsQuantity);
    for (int i = 0; i < finiteElementsQuantity; ++i)
    {
        jacobiansForElement[i] = createJacobian(NT, AKT, DFIABG, i);
    }

    vector<vector<double>> jacobianValuesForElement(finiteElementsQuantity, vector<double>(27));
    for (int i = 0; i < finiteElementsQuantity; ++i)
    {
        for (int j = 0; j < 27; ++j)
        {
            jacobianValuesForElement[i][j] = getDeterminant(jacobiansForElement[i][j]);
        }
    }

    auto DFIXYZ = createDFIXYZ(jacobiansForElement, DFIABG, finiteElementsQuantity);


    vector<vector<double>> permutations = getTwentySevenGaussianNodes();
    std::map<vector<double>, int> permutationIndex;
    for (int i = 0; i < 27; ++i)
    {
        permutationIndex[permutations[i]] = i;
    }



    auto allAe11 = createAe11(DFIXYZ, jacobianValuesForElement, finiteElementsQuantity, permutationIndex);
    auto allAe22 = createAe22(DFIXYZ, jacobianValuesForElement, finiteElementsQuantity, permutationIndex);
    auto allAe33 = createAe33(DFIXYZ, jacobianValuesForElement, finiteElementsQuantity, permutationIndex);
    auto allAe12 = createAe12(DFIXYZ, jacobianValuesForElement, finiteElementsQuantity, permutationIndex);
    auto allAe13 = createAe13(DFIXYZ, jacobianValuesForElement, finiteElementsQuantity, permutationIndex);
    auto allAe23 = createAe23(DFIXYZ, jacobianValuesForElement, finiteElementsQuantity, permutationIndex);

    auto allMGE = createMGE(allAe11, allAe22, allAe33, allAe12, allAe13, allAe23, finiteElementsQuantity);

    auto DPSIET = createDPSIET();

    vector<vector<double>> permutations2d = getNineGaussianNodes();
    std::map<vector<double>, int> permutationIndex2d;
    for (int i = 0; i < 9; ++i)
    {
        permutationIndex2d[permutations2d[i]] = i;
    }

    // FE[60] for every pair in ZP
    Matrix allFE = Matrix(ZP.size(), 0);
    for (int i = 0; i < ZP.size(); ++i)
    {
        allFE[i] = createFE(DPSIET, AKT, NT, ZP[i], permutationIndex2d);
    }

    // total nodes quantity
    size_t nqp = AKT[0].size();

    //making all MGE full matrices (tmp to be usable with current gaussian elimination)
    for (int q = 0; q < allMGE.size(); ++q)
    {
        for (int i = 0; i < allMGE[0].size(); ++i)
        {
            for (int j = i; j < allMGE[0][0].size(); ++j)
            {
                allMGE[q][j][i] = allMGE[q][i][j];
            }
        }
    }

    Matrix globalMG = Matrix(nqp * 3, nqp * 3);
    vector<double> globalF = vector<double>(nqp * 3, 0);
    initGlobals(globalMG, globalF, allMGE, allFE, ZP, NT);
    fixateSides(globalMG, ZU, NT);
    //tmp making matrix full
//    for (int i = 0; i < globalMG.size(); ++i)
//    {
//        for (int j = i; j < globalMG.size(); ++j)
//        {
//            globalMG[j][i] = globalMG[i][j];
//        }
//    }


    //debug file output

    std::ofstream f;
    f.open("outputGlobalOld.txt");
    for (int i = 0; i < globalMG.size(); ++i)
    {
        for (int j = 0; j < globalMG.size(); ++j)
        {
            f << round_up(globalMG[i][j], 2) << '\t';
            //f << globalMG[i][j] << '\t';
        }
        if(i != globalMG.size() - 1)
        {
            f << '\n';
        }
    }
    //f << "\n\n\n----------------------------------------------------------------------\n\n\n";

    f.close();

//    std::ofstream f123;
//    f123.open("outputF.txt");
//    for (int i = 0; i < globalF.size(); ++i)
//    {
//        f123 << round_up(globalF[i], 2) << '\t';
//        //f123 << globalF[i] << '\t';
//    }
//    f123.close();




    std::ofstream f1;
    f1.open("outputSep.txt");
    for (int m = 0; m < allMGE.size(); ++m)
    {
        for (int i = 0; i < allMGE[0].size(); ++i)
        {
            for (int j = 0; j < allMGE[0][0].size(); ++j)
            {
                f1 << round_up(allMGE[m][i][j], 2) << '\t';
                //f << globalMG[i][j] << '\t';
            }
            f1 << '\n';
        }
        f1 << "\n\n\n----------------------------------------------------------------------\n\n\n";
    }
    for (int i = 0; i < ZP.size(); ++i)
    {
        for (int j = 0; j < allFE[0].size(); ++j)
        {
            f1 << round_up(allFE[i][j], 2) << '\t';
        }
        f1 << "\n\n\n----------------------------------------------------------------------\n\n\n";
    }

    f1.close();




    // coef to multiply result values so it would be easier to notice
    double coef = 1;
    auto finalRes = GaussianElimination::solve(globalMG, globalF);
    for (int i = 0; i < AKT[0].size(); ++i)
    {
        AKT[0][i] += coef * finalRes[i * 3];
        AKT[1][i] += coef * finalRes[i * 3 + 1];
        AKT[2][i]+= coef * finalRes[i * 3 + 2];
    }

    //vector<vector<double>> testInput{{1, 3, 5}, {4, 0.5, 2}, {3, 33, 1}}; vector<double> b{22,11,72}; // test gaussian elimination

    // create MGE[0] array string
//    vector<QString> MGEStrings(MGE[0].size(), "");
//    for (int i = 0; i < MGE[0].size(); ++i)
//    {
//        for (int j = 0; j < MGE[0].size(); ++j)
//        {
//            MGEStrings[i] += QString::number(MGE[0][i][j]) + "\t";
//        }
//        MGEStrings[i] += "\n";
//    }
//    QString mgeStringsConcat = std::accumulate(MGEStrings.begin(), MGEStrings.end(), QString(""));

    // create NT array string
//    vector<QString> NTStrings(NT[0].size(), "");
//    for (int i = 0; i < NT[0].size(); ++i)
//    {
//        NTStrings[i] += QString::number(i) + ") ";
//        for (int j = 0; j < 20; ++j)
//        {
//            NTStrings[i] += QString::number(NT[j][i]) + " ";
//        }
//        NTStrings[i] += "\n";
//    }
//    QString allstrings = std::accumulate(NTStrings.begin(), NTStrings.end(), QString(""));

    // print into file elements MGE from code above or single MGE component
//    QFile file("output.txt");
//    int finiteElementIndex = 0;
//    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
//    {
//        QTextStream stream(&file);
//        bool right = true;
//        for (int i = 0; i < 20; ++i)
//        {
//            for (int j = 0; j < 20; ++j)
//            {
//                stream << allAe22[finiteElementIndex][i][j] << '\t';
//                right = right && (allAe22[finiteElementIndex][i][j] == allAe22[finiteElementIndex][j][i]);
//            }
//            stream << '\n';
//        }
//        stream << '\n' << right << '\n';
//        //stream << mgeStringsConcat;
//    }
//    file.close();

    //visualization
    QApplication app(argc, argv);
    QMessageBox box;
    box.setText("success");
    box.exec();
    Q3DScatter *graph = new Q3DScatter();
    QWidget *container = QWidget::createWindowContainer(graph);

    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->addWidget(container);
    DataModifier *modifier = new DataModifier(graph, AKT);
    widget->show();
    delete modifier;
    return app.exec();
}
