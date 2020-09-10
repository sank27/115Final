#include "pathfinder.h"


void printInternalMaze(int matrix[N][N]){
    for(int i = 0 ; i < N; i++)
	    {
	        for(int j = 0; j < N ; j++)
	            cout<< matrix[i][j] << " ";

	        cout<<endl;
	    }
}

///////// Movements Adding to Queue /////////


void pathfinder::moveIt(int x, int y){
    int sx = P->getPlayerLoc().y;
    int sy = P->getPlayerLoc().x;

    if(sx != x || sy != y) //will stop us from taking the first step
    {


        if(!actionsToTake.empty())
        {
            sx = pX;
            sy = pY;
        }
        //Now we pushing it.
        if (y < sy) actionsToTake.push(3);
        if (x > sx) actionsToTake.push(1);
        if (y > sy) actionsToTake.push(4);
        if (x < sx) actionsToTake.push(2);


        cout<<"Total actions: "<<actionsToTake.size()<<endl;
        pX = x;
        pY = y;
        cout<<"Move from ("<<sx<<","<<sy<<") to ("<<x<<","<<y<<")"<<endl;
    }
}


//////////////////////////////////
/////////Shortest Path////////////


// Below arrays details all 4 possible movements from a cell
int row[] = { -1, 0, 0, 1 };
int col[] = { 0, -1, 1, 0 };

// The function returns false if pt is not a valid position
bool pathfinder::isValid(int matrix[N][N],int x, int y)
{
    if(P->isThere(matrix[x][y]) == 0) //can't visit that location
        return false;
	return (x >= 0 && x < N) && (y >= 0 && y < N);
}

// Function to print the complete path from source to destination
int pathfinder::printPath(vector<Node> path)
{
	if (path.size() == 0)
		return 0;

	int len = printPath(path[0].parent) + 1;
	//cout << "(" << path[0].x << ", " << path[0].y << ") ";
	moveIt(path[0].x,path[0].y);
	return len;
}

// Find shortest route in the matrix from source cell (x, y) to
// destination cell (N - 1, N - 1)
bool pathfinder::findPath(int matrix[N][N], Node src, Node dst, Node &curr)
{
    if( P->isThere(matrix[dst.x][dst.y]) == 0) //if the grid is not there
    {
        cout<< matrix[dst.x][dst.y] << " is not there in grid list ";
        P->printVals();
        cout<<endl;
        return false;

    }

	// create a queue and enqueue first node
	queue<Node> q;
	//Node src = {x, y};
	q.push(src);

	// set to check if matrix cell is visited before or not
	set<Node> visited;
	visited.insert(src);

	// run till queue is not empty
	while (!q.empty())
	{
		// pop front node from the queue and process it
		curr = q.front();
		q.pop();

		int i = curr.x;
		int j = curr.y;

		// if destination is found, print the shortest path and
		// return the shortest path length
		if (i == dst.x && j == dst.y)
		{
			return true;
		}

		// get value of the current cell is how many steps we can take
		//int n = matrix[i][j]; // it's 1 for the most basic case

		// check all 4 possible movements from current cell
		// and recur for each valid movement
		for (int k = 0; k < 4; k++)
		{
			// get next position coordinates using value of current cell
			/*
			int x = i + row[k] * n;
			int y = j + col[k] * n;
            */
            int x = i + row[k];
			int y = j + col[k];
			// check if it is possible to go to next position
			// from current position
			if (isValid(matrix,x, y))
			{
				// construct the next cell node
				Node next = { x, y, {curr} };

				// if it not visited yet
				if (!visited.count(next))
				{
					// push it into the queue and mark it as visited
					q.push(next);
					visited.insert(next);
				}
			}
		}
	}

	// return -1 if path is not possible
	return 0;
}

////////////////////////////

pathfinder::pathfinder(Player *p)
{
    P = p;
}

pathfinder::~pathfinder()
{
    //dtor
}

bool pathfinder::shortPathUtility(int mat[N][N], int sx, int sy, int dx, int dy){

	cout<<"Matrix representation of Maze: "<<endl;
	printInternalMaze(mat);

    if(sx == dx && sy == dy)
    {
        cout<<"\nBoth Source and Destinations are same!"<<endl;
    }

	Node curr; // will store the final path by backtracking its parents

    Node src = {sx, sy};
    Node dst = {dx, dy};
	// Find a route in the matrix from source cell (0, 0) to
	// destination cell (N - 1, N - 1)
	bool path = findPath(mat, src, dst, curr);
    int len;
    if(path == true)
    {
		cout << "Finding Shortest path "<<endl;

		len = printPath({curr});
	    len = len - 1;
    	cout << "\nShortest Path length is " << len <<endl;
    	return true;
	} else {
		cout << "Destination not possible";
		return false;
	}
}

void pathfinder::getActions(queue<int> &act){

    while(!actionsToTake.empty()){
        act.push(popget(actionsToTake));
        //cout<< popget(actionsToTake) << endl;
    }
}
