#include "genMap.h"

genMap::genMap()
{
    //ctor
}

genMap::~genMap()
{
    //dtor
}

void genMap::mountains(int k, int l, int m, int n, int a[][N]) //must give dimensions except first one
{
    int i, pos = 0;

    /*  k - starting row index
        m - ending row index
        l - starting column index
        n - ending column index
        i - iterator
        p - position in our pattern
    */

    while (k < m && l < n) {
        /* Print the first row from
               the remaining rows */
        for (i = l; i < n; ++i) {
            a[k][i] = pattern[pos];
            //cout << a[k][i] << " ";
        }
        k++;

        /* Print the last column
         from the remaining columns */
        for (i = k; i < m; ++i) {
            a[i][n - 1] = pattern[pos];
            //cout << a[i][n - 1] << " ";
        }
        n--;

        /* Print the last row from
                the remaining rows */
        if (k < m) {
            for (i = n - 1; i >= l; --i) {
                a[m - 1][i] = pattern[pos];
                //cout << a[m - 1][i] << " ";
            }
            m--;
        }

        /* Print the first column from
                   the remaining columns */
        if (l < n) {
            for (i = m - 1; i >= k; --i) {
                a[i][l] = pattern[pos];
                //cout << a[i][l] << " ";
            }
            l++;
        }
        pos++;
        cout<<endl;
    }
}

void genMap::beach(int s, int mat[][N]){
    int i = 0, r = 0; // i is iterator, r is starting row
    while(s > 2){
    //if(r%2 == 0 && r>0)
        s-=1;

    //Boundary for ocean ends at 9,0 (s-2)
    for(i = 0; i<=s-2; i++)
        mat[r][i] = 8;

    if(s-2 == 0)
    {
        mat[r][0] = 7;
        mat[r][1] = 7;
        mat[r][2] = 7;
    }
    else
    {
        mat[r][i] = 7;
        mat[r][i+1] = 7;
    }



    r++;
    }
}

void genMap::desert(int h,int w, int mat[][N]){ //h is for height, w is for width
    for(int i = N - h; i < N ; i++ ){
        for(int j = 0; j < w; j++)
        if(mat[i][j]==4) //4 means plain
            mat[i][j] = 6;
    }
}

void genMap::forest(int h,int w, int mat[][N]){ //h is for height, w is for width
    for(int i = N - h; i < N ; i++ ){
        for(int j = N - w ; j < N; j++)
        if(mat[i][j]==4) //4 means plain
            mat[i][j] = 5;
    }
}

void genMap::fillPlain(int mat[N][N])
{
    for(int i=0; i<N; i++)
    for(int j=0; j<N; j++)
    {
        mat[i][j] = 4;
    }
}


void genMap::saveMap(int mat[N][N], string filename)
{
  ofstream myfile;
  myfile.open (filename);
    cout<<"\nSaving map to file.\n";

    if (myfile.is_open()) {
            cout<<"File is open"<<endl;
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {

            if (mat[y][x] == 1) { // if a high mountain
                myfile << "highMt "<< x << " " << y << "\n";
            } else if (mat[y][x] == 2) { // if a medium mountain
                myfile << "mediumMt " << x << " " << y << endl;
            } else if (mat[y][x] == 3) { // if a foothill
                myfile << "foothill " << x << " " << y << endl;
            } else if (mat[y][x] == 5) { // if a forest
                myfile << "forest " << x << " " << y << endl;
            } else if (mat[y][x] == 6) { // if a desert
                myfile << "desert " << x << " " << y << endl;
            } else if (mat[y][x] == 7) { // if a beach
                myfile << "beach " << x << " " << y << endl;
            } else if (mat[y][x] == 8) { // if a ocean
                myfile << "ocean " << x << " " << y << endl;
            }

        }
    } else {
        cout << "Couldn't open file." << endl;
    }

    myfile.close();
}


void genMap::genMapUtil(int mat[N][N],int level)
{
    //We want to print in a spiral 7x7 to 17x17
    //printf("Generating Maze Matrix\n");

    /*
    For a 5x5 = 25, outside box is 16. inner is 8, then 1 is left.
    8 = 2^3
    16 = 2^4
    */

    /*
    int counter = 0;
    for(int i=0; i<N; i++)
    for(int j=0; j<N; j++)
    {
        mat[i][j] = 4;
        counter++;
    }*/


    fillPlain(mat);

    //printInternalMaze(mat);

    //cout<<"\nPrinting Spiral Matrix"<<endl;

    int cs;

    if (level == 1)
    {
        cs = 9; //increase this to make mountain larger
    }
    else
        cs = 10; //increase this to make mountain larger

    int start = 7;
    int beachSize = 12;

    mountains(start, start, start+cs, start+cs, mat);

    beach(beachSize, mat);

    desert(7,6, mat);

    forest(14,6, mat);

}
