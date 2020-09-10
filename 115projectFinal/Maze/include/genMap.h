#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

#ifndef GENMAP_H
#define GENMAP_H

#define N 20

class genMap
{
    public:
        genMap();
        virtual ~genMap();
        void genMapUtil(int mat[N][N], int level);
        void fillPlain(int mat[N][N]);
        void mountains(int k, int l, int m, int n, int a[][N]);
        void beach(int s, int mat[][N]);
        void desert(int h,int w, int mat[][N]);
        void forest(int h,int w, int mat[][N]);
        void saveMap(int mat[N][N], string filename);

    protected:

    private:
        //for 10x10 there are 5 concentric squares in our matrix
        int pattern[5] ={3,3,2,2,1};

};

#endif // GENMAP_H
