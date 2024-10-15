#ifndef MATRIX_H
#define MATRIX_H

namespace fem
{
    //symmetrical n diagonal matrix
    class SMatrix
    {
    public:
        SMatrix(int height, int width, int ng);

        double& at(int i, int j);
        double& atGE(int i, int j);

        // makes matrix stored as non symmetrical n diagonal matrix in order for it to be valid for gaussian elimination
        void makeValidForGE();

        // col height
        int getHeight()const;

        // row length
        int getWidth()const;

        int getNg()const;
    private:
        vector<vector<double>> rawData;
        //number of not null diagonals
        int ng;
        int height;
        int width;

    };
}

#endif // MATRIX_H