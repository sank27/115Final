#ifndef PATHFINDER_H
#define PATHFINDER_H
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <player.h>

using namespace std;

#define N 20

//popping queue helper function
//https://stackoverflow.com/questions/48487678/compilation-error-void-value-not-ignored-as-it-ought-to-be-in-stdqueuepop
template<class T>
int popget(T& queue)
{
    int ret = std::move(queue.front());
    queue.pop();

    return ret;
}

class pathfinder
{
    public:
        Player *P;
        queue<int> actionsToTake;                    // will player actions for shortest path
        int pX,pY ;                                  // storing the previous locations

        // queue node used in BFS
        struct Node
        {
            // (x, y) represents coordinates of a cell in matrix
            int x, y;

            // parent stores the parent Node of the current cell
            // It will have only one entry i.e. parent node
            vector<Node> parent;

            // As we are using struct as a key in a std::set,
            // we need to overload below operators

            bool const operator==(const Node& ob) const
            {
                return x == ob.x && y == ob.y;
            }

            bool operator<(const Node& ob) const
            {
                return x < ob.x || (x == ob.x && y < ob.y);
            }
        };

        bool shortPathUtility(int mat[N][N], int sx, int sy, int dx, int dy);   // calls findpath function
        bool findPath(int matrix[][N], Node src, Node dst, Node &curr);         // using BFS to find the shortest path
        int printPath(vector<Node> path);                                       // prints the path and calls moveIt
        void moveIt(int x, int y);                                              // generates the actions based on previous location
        void getActions(queue<int> &act);                                                // returns all the actions we need to take
        bool isValid(int matrix[N][N],int x, int y);

        pathfinder(Player *p);
        virtual ~pathfinder();

    protected:

    private:
};

#endif // PATHFINDER_H

