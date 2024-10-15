#ifndef FEMINTERFACE_H
#define FEMINTERFACE_H

namespace fem
{
    using std::vector;

    class FEMInterface
    {
    public:
        FEMInterface();

        void setXyzQuantity(int x, int y, int z);
        void setXyzLengths(double x, double y, double z);

        void setGlobalAKT(const vector<vector<double>>& globalAKT);
        void setZP(const vector<vector<int>>& ZP);
        void setZU(const vector<vector<int>>& ZU);

        // mathematical consts lol
        void setConsts(double e, double nu);
        void setE(double e);
        void setNu(double nu);

        // get AKT provided by user
        const vector<vector<double>>& getGlobalAKT() const;

        // get partitioned AKT
        const vector<vector<double>>& getPartitionedAKT() const;

        // get partitioned AKT with movements applied
        vector<vector<double>> getModifiedAKT() const;
        const vector<double>& getMovements() const;
        const vector<vector<double>>& getTensions() const;
        const vector<vector<int>>& getZU() const;
        const vector<vector<int>>& getZP() const;
        const vector<int>& getXyzQuantity() const;
        const vector<double>& getXyzLengths() const;
        const int getTotalFiniteElementsQuantity() const;

        // AKT size
        int getTotalNodesQuantity() const;

        void calculatePartition();

        // calculates movements
        void calculateMovements();

        void calculateTensions();

        void calculateAll();
    private:
        // to press use negative power and to stretch positive (dzero ne testiv)
        // powered sides, list of <elemNumber, sideNumber, POWER> elemNumber begins from 0, side number begins from 1
        vector<vector<int>> ZP;

        //fixed sides, list of pairs <elemNumber, sideNumber> elemNumber begins from 0, side number begins from 1
        vector<vector<int>> ZU;

        // partitions
        vector<int> xyzQuantity;

        // figure dimensions
        vector<double> xyzLengths;

        // size = pointsCount * 3, structure: x1, y1, z1, x2, y2, z2, ...
        vector<double> movements;

        vector<vector<double>> tensions;

        // AKT recieved from user;
        vector<vector<double>> globalAKT;

        // AKT after partition is performed
        vector<vector<double>> AKT;

        // connection between local and global numeration of AKT nodes
        // format: NT[FE number (0 - npq)][local node number(0 - 19)] = global node number
        vector<vector<int>> NT;

        int finiteElementsQuantity;

        // total nodes quantity
        int nqp;

        // max difference between global node numbers of nodes within single finite element * 3
        // used to store global stiffness matrix elements efficiently (currently useless)
        int ng;

        // coef to multiply movement values to make tendencies easier to spot for stiff materials 
        int coef;
    };
}

#endif // FEMINTERFACE_H