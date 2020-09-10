/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <Player.h>

Timer *T = new Timer();                             // Set Timer for animation

Player::Player(int* arr, int size)
{
    //ctor
    playerDir = "up";
    arrowLoc.x = 0.2;
    arrowLoc.y =0.5;
    arrAngle =0;
    arrowStatus = false;
    livePlayer = true;
    action= 0;

    a = arr;
    s = size;
}

Player::~Player()
{
    //dtor
}

void Player::printVals(){
        for(int i = 0; i<s; i++)
            cout<<a[i]<<endl;
    }

bool Player::isThere(int val){
        for(int i=0; i<s; i++)
            if(a[i]==val)
                return true;
        return false;
    }

void Player::drawArrow()
{

if(arrowStatus){
   glColor3f(1.0,1.0,1.0);

     if(T->GetTicks()>10)
        {
            if(arrowLoc.x>-1+unitWidth/2 && arrowLoc.x<1-unitWidth/2)
                arrowLoc.x += t*arrXdir;
            else arrowStatus = false;
            if(arrowLoc.y>-1+unitWidth/2 && arrowLoc.y<1-unitWidth/2)
                arrowLoc.y += t*arrYdir;
            else arrowStatus = false;
           T->Reset();
        }
    glTranslatef(arrowLoc.x ,arrowLoc.y,0.0);
    glRotated(arrAngle,0,0,1);
    glBindTexture(GL_TEXTURE_2D,arrowTex);
    glScaled(1.0/(float)gridSize,1.0/(float)gridSize,1);

    glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(1,0);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(1,1);
        glVertex3f(-1,-1,0.0f);

        glTexCoord2f(0,1);
        glVertex3f(1,-1,0.0f);
     glEnd();
  }
}

void Player::shootArrow()
{   if(livePlayer){
    if(!arrowStatus)
    {
      arrowStatus=true;

      if(strcmp(playerDir, "left")==0)
      {
        arrAngle = -90;
        arrXdir=-1;
        arrYdir=0;
        arrowLoc.x = plyLoc.x-unitWidth/2;
        arrowLoc.y = plyLoc.y;
      }
      if(strcmp(playerDir, "right")==0)
      {
        arrAngle =90;
        arrXdir=1;
        arrYdir=0;
        arrowLoc.x = plyLoc.x+unitWidth/2;
        arrowLoc.y = plyLoc.y;
      }
      if(strcmp(playerDir, "up")==0)
      {
        arrXdir=0;
        arrYdir=1;
        arrAngle =180;

        arrowLoc.y = plyLoc.y+unitWidth/2;
        arrowLoc.x = plyLoc.x;
      }
      if(strcmp(playerDir, "down")==0)
      {
        arrXdir=0;
        arrYdir=-1;
        arrAngle= 0;

        arrowLoc.y = plyLoc.y-unitWidth/2;
        arrowLoc.x = plyLoc.x;
      }
     }
    }
}


void Player::loadArrowImage(char* fileName)
{
    arrowTex = TextureLoader(fileName);
}

GridLoc Player::getArrowLoc()
{
   GridLoc val;

    val.x = (int)(ceil((arrowLoc.x +(1-unitWidth))/unitWidth));
    val.y = (int)(ceil((arrowLoc.y +(1-unitWidth))/unitWidth));

   return val;
}


void Player::drawplayer()
{
    if(livePlayer)
    {

    glColor3f(1.0,1.0,1.0);

    glTranslatef(plyLoc.x ,plyLoc.y,0.0);

    glBindTexture(GL_TEXTURE_2D,plyTex);
    glScaled(1.0/(float)gridSize,1.0/(float)gridSize,1);


    glBegin(GL_QUADS);
        glTexCoord2f(xmin,ymin);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(xmax,ymin);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(xmax,ymax);
        glVertex3f(-1,-1,0.0f);

        glTexCoord2f(xmin,ymax);
        glVertex3f(1,-1,0.0f);
     glEnd();
    }
}


void Player::initPlayer(int gSize,char *FileName,int frms)
{
    gridSize = gSize;
    frames = frms;

    xmax =1/(float)frames;
    ymax =0.25;
    xmin =0;
    ymin =0;

    unitWidth = (float)2/gridSize;
    stepsPerMove =frames;

    T->Start();
    plyLoc.x= -unitWidth;
    plyLoc.y= -unitWidth;

    t= unitWidth/stepsPerMove;
    steps =0;
    plyTex = TextureLoader(FileName);
}

void Player::placePlayer(int x, int y)
{
    plyLoc.x =  converter(x,y).x;
    plyLoc.y =  converter(x,y).y;
}

void Player::movePlayer(char* dir, int frames)
{
   /*
   if(steps >= unitWidth) {
   placePlayer(getPlayerLoc().x,getPlayerLoc().y);
   steps =0;
   }
   */

  if(strcmp(dir, "left")==0)
   {   playerDir = "left";

       if(T->GetTicks()>100)
        {   steps +=t;
            if(plyLoc.x>-1+unitWidth/2)
              {
                 steps<unitWidth? plyLoc.x -= t:NULL;
              }
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin += 1/(float)frames;
            xmax += 1/(float)frames;
            ymin =0.75;
            ymax =1.0;
            T->Reset();
          }  // end if
       }

   else if(strcmp(dir, "right")==0)
   {
        playerDir = "right";

        if(T->GetTicks()>100)
        {
            steps +=t;
            if(plyLoc.x<1-unitWidth/2)
            steps<unitWidth?plyLoc.x += t:NULL;

            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin +=1/(float)frames;
            xmax +=1/(float)frames;
            ymin =0.5;
            ymax =0.75;
            T->Reset();
        }  // end if
   }


   else if(strcmp(dir, "up")==0)
   {
        playerDir = "up";

        if(T->GetTicks()>100)
        {
            steps +=t;
            if(plyLoc.y< 1-unitWidth/2)
            steps<unitWidth?plyLoc.y += t:NULL;

            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin +=1/(float)frames;
            xmax +=1/(float)frames;

            ymin =0.25;
            ymax =0.50;
            T->Reset();
         }  // end if

    }

   else if(strcmp(dir, "down")==0)
   {
         playerDir = "down";

        if(T->GetTicks()>100)
        {
            steps +=t;
            if(plyLoc.y > -1+unitWidth/2)
                steps<unitWidth?plyLoc.y -= t:NULL;
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin +=1/(float)frames;
            xmax +=1/(float)frames;
            ymin =0.0;
            ymax =0.25;
            T->Reset();
        }  // end if

     }


    drawplayer();
}

GridLoc Player::getPlayerLoc()
{
   GridLoc val;

   val.x = (int)(ceil((plyLoc.x +(1-unitWidth))/unitWidth));
   val.y = (int)(ceil((plyLoc.y +(1-unitWidth))/unitWidth));

   return val;

}

loc Player::converter(int x, int y)
{
  loc val;
        x+=1;
        y+=1;

       val.x = -1-unitWidth/2+(unitWidth)*x;
       val.y = -1-unitWidth/2+(unitWidth)*y;
       return val;
}
