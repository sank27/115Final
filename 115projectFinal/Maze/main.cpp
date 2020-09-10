/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <string.h>
#include <CommonThings.h>
#include <Maze.h>
#include <iostream>
#include <Timer.h>
#include <player.h>

#include <vector>
#include <set>
#include <queue>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

#include <wall.h>
#include <math.h>

#include <fstream>
#include <sstream>

#include <pathfinder.h>
#include <genMap.h>

/* GLUT callback Handlers */

//#define N 8    //N is defined in pathfinder.h

using namespace std;

Maze *M = new Maze(N);                             // Set Maze grid size

Player *Human = new Player(new int[6]{ 2,3,4,5,6,7 },6);   // create player with grids it can visit
Player *Fish = new Player(new int[1]{ 8 },1);   // create player with grids it can visit
Player *Frog = new Player(new int[5]{ 3,4,5,7,8 },5);
Player *Bird = new Player(new int[7]{ 1,2,3,4,5,7,8 },7);


Player *P = &(*Human);             //Active player
pathfinder *F = new pathfinder(P); //Active pathfinder

Player *players[4] = {Human,Fish,Frog,Bird};

string filename = "map.txt";                 // stores name of our file

bool displayEverything = true;

int choice;                                  // the mode user wants to play in
int level;                                   // the level user wants to play in
string line,obj;                             // will store the kind of object

genMap *g = new genMap();                    // we create a genmap object

bool inProgress;                             // will keep track if a player is moving
                                             // when there are more players we should make this
                                             // an attribute of player class

wall Forest[1000];                            // wall with number of bricks
wall O[1000];                                 // wall with number of oceans
wall B[1000];                                 // wall with number of beaches
wall High[1000];                              // wall with number of high mountains
wall Med[1000];                               // wall with number of medium mountains
wall Foot[1000];                              // wall with number of foothills
wall D[1000];                                 // wall with number of desert

int totalForest = 0;                           // keep track of forest
int totalOceans = 0;                          // keep track of oceans
int totalBeaches = 0;                         // keep track of beaches
int totalHM = 0;                         // keep track of high mts
int totalMM = 0;                         // keep track of medium mts
int totalFH = 0;                         // keep track of foothills
int totalD = 0;                         // keep track of deserts

Timer *T0 = new Timer();                     // animation timer

int mat[N][N];                               // constructing a matrix to internally map the maze
queue<int> act;                    // will player actions for shortest path

float wWidth, wHeight;                       // display window width and Height
int xPos,yPos;                               // Viewport mapping


void display(void);                          // Main Display : this runs in a loop
void playerActions();


void resize(int width, int height)           // resizing case on the window
{
    wWidth = width;
    wHeight = height;

    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void init()
{
    glEnable(GL_COLOR_MATERIAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0,0.0,0.0,0.0);
    gluOrtho2D(0, wWidth, 0, wHeight);

    T0->Start();                                        // set timer to 0

    glEnable(GL_BLEND);                                 //display images with transparent
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    T0->Start();                                        // set timer to 0
    glEnable(GL_BLEND);                                 //display images with transparent
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    M->loadBackgroundImage("images/background.jpg");           // Load maze background image

    fstream myfile; // to read in a file

    while(choice != 1 && choice != 2){
    cout << "What would you like to do? " << endl;
    cout << "1. Choose a Map" << endl;
    cout << "2. Load last played Map" << endl;
    cout << "Please enter 1 or 2" << endl;
    cin >> choice;
    } //end of while

    if(choice == 1)
    {

    while(level != 1 && level != 2){
    cout << "What Level would You like to play on? " << endl;
    cout << "1. Level 1"<< endl;
    cout << "2. Level 2" << endl;
    cout << "Please enter 1 or 2" << endl;
    cin >> level;
    }

    g->genMapUtil(mat,level);

    for(int x = 0; x < N; x++)
    for(int y = 0; y < N; y++)
    {

        if(mat[y][x] == 1){ // if a high mountain
            High[totalHM].wallInit(M->getGridSize(),"images/high.png"); // Load high mountain image
            High[totalHM].placeWall(x,y); // place the high mountain
            totalHM++; // increment the amount of high mountains
            }
        else if(mat[y][x] == 2){ // if a medium mountain
            Med[totalMM].wallInit(M->getGridSize(),"images/medium.png"); // Load medium mountain image
            Med[totalMM].placeWall(x,y); // place the medium mountain
            totalMM++; // increment the amount of medium mountains
            }
        else if(mat[y][x] == 3){ // if a foothill
            Foot[totalFH].wallInit(M->getGridSize(),"images/foot.png"); // Load foothill image
            Foot[totalFH].placeWall(x,y); // place the foothill
            totalFH++; // increment the amount of foothills
            }
        else if(mat[y][x] == 5){ // if a forest
            Forest[totalForest].wallInit(M->getGridSize(),"images/forest.png"); // Load forest image for 1st map
            Forest[totalForest].placeWall(x,y); // place the forest
            totalForest++; // increment the amount of forest grids
            }
        else if(mat[y][x] == 6){ // if a desert
            D[totalD].wallInit(M->getGridSize(),"images/d.png"); // Load desert image
            D[totalD].placeWall(x,y); // place the desert
            totalD++; // increment the amount of deserts
            }
        else if(mat[y][x] == 7){ // if a beach
            B[totalBeaches].wallInit(M->getGridSize(),"images/b.png"); // Load beach image
            B[totalBeaches].placeWall(x,y); // place the beach
            totalBeaches++; // increment the amount of beach
            }
        else if(mat[y][x] == 8){ // if a ocean
            O[totalOceans].wallInit(M->getGridSize(),"images/ocean.png"); // Load ocean image
            O[totalOceans].placeWall(x,y); // place the ocean
            totalOceans++; // increment the amount of oceans
            }
    }

    //saving the generated map
    g->saveMap(mat,filename);

    }
    else if(choice == 2)
    {
    myfile.open(filename);
    if (myfile.is_open()){

    cout << "Opened map file" << endl;

    int xCoor, yCoor;

    g->fillPlain(mat); // we fill our matrix with 4s for plain tiles

    while ( getline (myfile,line) ){ // while their is input from file
        stringstream ss(line);
        ss >> obj >> xCoor >> yCoor; // populate object with x and y fields

        if(obj == "highMt"){ // if a high mountain
            High[totalHM].wallInit(M->getGridSize(),"images/high.png"); // Load high mountain image
            High[totalHM].placeWall(xCoor,yCoor); // place the high mountain
            totalHM++; // increment the amount of high mountains
            mat[yCoor][xCoor] = 1; //Assigning the internal matrix
            }
        else if(obj == "mediumMt"){ // if a medium mountain
            Med[totalMM].wallInit(M->getGridSize(),"images/medium.png"); // Load medium mountain image
            Med[totalMM].placeWall(xCoor,yCoor); // place the medium mountain
            totalMM++; // increment the amount of medium mountains
            mat[yCoor][xCoor] = 2;
            }
        else if(obj == "foothill"){ // if a foothill
            Foot[totalFH].wallInit(M->getGridSize(),"images/foot.png"); // Load foothill image
            Foot[totalFH].placeWall(xCoor,yCoor); // place the foothill
            totalFH++; // increment the amount of foothills
            mat[yCoor][xCoor] = 3;
            }
        else if(obj == "forest"){ // if a forest
            Forest[totalForest].wallInit(M->getGridSize(),"images/forest.png"); // Load forest image for 1st map
            Forest[totalForest].placeWall(xCoor,yCoor); // place the forest
            totalForest++; // increment the amount of forest grids
            mat[yCoor][xCoor] = 5;
            }
        else if(obj == "desert"){ // if a desert
            D[totalD].wallInit(M->getGridSize(),"images/d.png"); // Load desert image
            D[totalD].placeWall(xCoor,yCoor); // place the desert
            totalD++; // increment the amount of deserts
            mat[yCoor][xCoor] = 6;
            }
        else if(obj == "beach"){ // if a beach
            B[totalBeaches].wallInit(M->getGridSize(),"images/b.png"); // Load beach image
            B[totalBeaches].placeWall(xCoor,yCoor); // place the beach
            totalBeaches++; // increment the amount of beach
            mat[yCoor][xCoor] = 7;
            }
        else if(obj == "ocean"){ // if a ocean
            O[totalOceans].wallInit(M->getGridSize(),"images/ocean.png"); // Load ocean image
            O[totalOceans].placeWall(xCoor,yCoor); // place the ocean
            totalOceans++; // increment the amount of oceans
            mat[yCoor][xCoor] = 8;
            }
        }//end of while
    }//end of file open check
    else{

        cout<<"Unable to open file"<<endl;
        displayEverything = false;
    }
    }//end of else if

    Human->initPlayer(M->getGridSize(),"images/human.png",6);   // initialize player pass grid size,image and number of frames
    Human->x = 17;
    Human->y = 1;
    Human->placePlayer(17,1);                                // Place player

    Fish->initPlayer(M->getGridSize(),"images/fish.png",3);   // initialize player pass grid size,image and number of frames
    Fish->x = 1;
    Fish->y = 1;
    Fish->placePlayer(1,1);                                // Place player

    Frog->initPlayer(M->getGridSize(),"images/frog.png",3);   // initialize player pass grid size,image and number of frames
    Frog->x = 7;
    Frog->y = 7;
    Frog->placePlayer(7,7);                                // Place player

    Bird->initPlayer(M->getGridSize(),"images/bird.png",3);   // initialize player pass grid size,image and number of frames
    Bird->x = 14;
    Bird->y = 8;
    Bird->placePlayer(14,8);                                // Place player

}


// Function to display everything
void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT);        // clear display screen

        glPushMatrix();
         M->drawBackground();           // Display Background
        glPopMatrix();

        // Drawing things on the map

        for(int i=0; i<totalOceans;i++)
        {
           O[i].drawWall();
        }

        for(int i=0; i<totalBeaches;i++)
        {
           B[i].drawWall();
        }
        for(int i=0; i<totalHM;i++)
        {
           High[i].drawWall();
        }
        for(int i=0; i<totalMM;i++)
        {
           Med[i].drawWall();
        }
        for(int i=0; i<totalFH;i++)
        {
           Foot[i].drawWall();
        }
        for(int i=0; i<totalD;i++)
        {
           D[i].drawWall();
        }
        for(int i=0; i<totalForest;i++)
        {
           Forest[i].drawWall();
        }


        glPushMatrix();
            M->drawGrid();                   // Draw the grid
        glPopMatrix();

        glPushMatrix();
            Human->drawplayer();             // Draw the Human
        glPopMatrix();

        glPushMatrix();
            Fish->drawplayer();              // Draw the Fish
        glPopMatrix();

        glPushMatrix();
            Frog->drawplayer();              // Draw the Frog
        glPopMatrix();

        glPushMatrix();
            Bird->drawplayer();              // Draw the Bird
        glPopMatrix();

        glPushMatrix();
             playerActions();                // Draw Player move actions
        glPopMatrix();


    glutSwapBuffers();
}

void playerActions()
{
     // Your path code is here
     //cout<<"Mouse Click location : "<< xPos<<" "<<yPos <<endl;
     // here is player moving example
     if(P->action ==1)
     {P->movePlayer("up",6);
    // Sample of how to get player location
    //(remove this after you figure it out)
    // print only when you moving up
   // cout<<"Player Location  :  "<<P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
     }
     else if(P->action ==2)P->movePlayer("down",6);
     else if(P->action ==3)P->movePlayer("left",6);
     else if(P->action ==4)P->movePlayer("right",6);
     else P->movePlayer("stand",6);
}

void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case ' ':
             //P->shootArrow();           // Shoot (optional )
        break;
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
    }
    glutPostRedisplay();
}


 void GetOGLPos(int x, int y)          // Mouse map coordinates
{
    //cout<<"\nMouse Event"<<endl;

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    xPos =(int) (posX *M->getGridSize()/2 +M->getGridSize()/2); // update mouse position X
    yPos =(int) (posY *M->getGridSize()/2 +M->getGridSize()/2);

    cout<<"Clicked on "<< xPos<<" "<<yPos <<endl;   // print out grid value

    //cout<<"Player Location  :  "<<P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl; // print out the player location also

    if(inProgress == false)
    {

    for(int ply = 0; ply < 4; ply++)
    {
        if(players[ply]->x == xPos && players[ply]->y == yPos)
        {

            cout<<"You clicked on a player"<<endl;

            P = &(*players[ply]);
            F = new pathfinder(P);

        }
    }

//    P = &(*Human);
//    F = new pathfinder(P);

    //The Original Maze is opposite of internal Maze
    int sx = P->getPlayerLoc().y;
    int sy = P->getPlayerLoc().x;
    int dx = yPos;
    int dy = xPos;

    bool pathPossible = F->shortPathUtility(mat, sx, sy, dx, dy);

    if(pathPossible == true)
    {
        F->getActions(act);
        //store the final position of the player for next time
        P->x = xPos;
        P->y = yPos;
    }

    //cout<<"TOTAL ACTIONS: "<<act.size()<<endl;
    P->steps = 1;
    inProgress = true;
    }
    else{
        cout<<"\nWait till current action completes!"<<endl;
    }

}

void idle(void)
{

   //Your Code in this section
    if(T0->GetTicks()>500)
      {
        // This is to make sure player move one unit
        // And change direction
        if(P->steps >=P->unitWidth)
          {
            P->steps =0;
            // if player is off the center of grid, place it right
            // This may make you player jumpy a bit
            // you can get rid of this line if your player not messing your path
            P->placePlayer(P->getPlayerLoc().x,P->getPlayerLoc().y);
            //P->action = (rand()%4)+1; // your code should plug here
            //P->action = 3;

            if(!act.empty())
            {
                int a = popget(act);


                cout<<"Next Action: "<<a<<endl;
                P->action = a;
            }
            else
            {
                P->action = 6; //to make the player stand
                inProgress = false;
            }

            //P->steps =0;
            //cout<<"\nSteps:"<<P->steps<<endl;
          }

         T0->Reset();
      }
    glutPostRedisplay();
}


void mouse(int btn, int state, int x, int y){

    switch(btn){
        case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);
             }
            break;


      case GLUT_RIGHT_BUTTON:

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);
            }
            break;
    }
     glutPostRedisplay();
};

void Specialkeys(int key, int x, int y)
{
    //cout<<"Player Location  :  "<<P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
    switch(key)
    {
    case GLUT_KEY_UP:
        P->action =1;
         break;

    case GLUT_KEY_DOWN:
        P->action =2;
         break;

    case GLUT_KEY_LEFT:
        P->action =3;
         break;

    case GLUT_KEY_RIGHT:
        P->action =4;
         break;
   }
   // comment this after you debug


  glutPostRedisplay();

}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (700, 700);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Maze");                    //program title
   init();

   //if we couldn't load map from the file we don't display anything
   if(displayEverything)
   {
   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutSpecialFunc(Specialkeys);
   glutMouseFunc(mouse);
   glutIdleFunc(idle);
   glutMainLoop();
   }

   return EXIT_SUCCESS;
}
