/******************************************
**** Computer Graphics CSCI 4229/5229
**** Final Project: Arcade games (Car & bowling)
**** Author     : Shekhar 
**** References : Most of the code is taken from Prof.Schreuder example codes 
**** I have referred google and online documentation as well for most of the opengl implementations
********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "CSCIx229.h"
#include "bowling.h"

//  OpenGL with prototypes for glext
#define GL_GLxOffsetT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int axes=0;       //  Display axes
int mode=1;       //  Projection mode
int move = 1;	  // Move Light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fieldOfView = 75;       //  Field of view (for perspective)
double aspect=1;     //  aspectect ratio
double dim=30.0;   //  Size of world
int light=1;      //  Lighting

int Change_light = 0;
int i = 0;

float Ex = 0.0, Ey = 1.0,Ez=1000; // Initial eye coordinates
double lz=-1; //for the eye look ahead
float acceleration = 10;

int x_delta=0, y_delta=0, z_delta=0;
int FlagC=0;
int DistanceC[7];
int variableC = 0;
int iter = 0;
int view = 0;
int mercedes = 0;
int carVal = 2;
int environmentCar = 1; 
int varCarRotate = 0;
//LightValues
int one       =   1;  // Unit value
int distance  =   500;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
float shinyvector[1] = {0.0};
#define PI 3.14159

// Textures
unsigned int texture_2D[30];
bool speedState[256];

enum gameCurrentView
{
	gameCurrentView1,
	gameCurrentView2, 
	gameCurrentView3, 
	gameCurrentView4,
	gameCurrentView5
}SCREEN;

int SEIZE_RESIZE_WINDOW =1;

/************rain/hail parameters ************/ 

#define _MAX_ELEMENTS 50000
#define CLIMATE_RAIN	1
#define	CLIMATE_HAIL	3


float decreaseSpeed = 1.5;
float increaseSpeed = 0;
float zoom = -40.0;
float hailsize = 0.1;

int single_particle;
int falli;

//floor colors
float r = 0.0;
float g = 1.0;
float b = 0.0;

typedef struct {
  int alive;	// is the particle alive?
  float scope;	// particle scopespan
  float decay; // decay
  // color
  float red;
  float green;
  float blue;
  // Position/direction
  float xpos;
  float ypos;
  float zpos;
  // increaseSpeed/Direction, only goes down in y dir
  float velocity;
  // Gravity
  float gravity;
}elements;

// Paticle System
elements elementArray[_MAX_ELEMENTS];

/******************** bowling game parameters (start)************/

int BOWLING_GAME_FLAG = 0;
int roll_range           = 600;
int maxExplosion   = 60;
int explode_position        = 540;

//Azimuth
int th1=0;
//Elevation
int ph1=0;

//Projection variables
int mode1 = 1;
int fieldOfView1=60;
double aspect1=1;
double dim1=50.0;

//first person location
double xOffset = 0;
double yOffset = 2.0;
double zOffset = -60.0;

int roll = 600;
int axes1=0;       //  Display axes
double idleArry[6] = {0.0,0.0,0.0,0.0,0.0,0.0};
int window_width = 600;
int window_height = 600;
int left_click_down = 0;
int saved_th = 0;
int saved_ph = 0;
int light1 = 1;

int inc1       =  10;  // Ball increment
int local1     =   0;  // Local Viewer Model

unsigned int side = 0;// side texture
unsigned int end = 0; // end texture
unsigned int floor_texture = 0; // floor texture
unsigned int arrow_texture = 0; // floor texture
unsigned int mural_texture[4] = {0,0,0,0};
unsigned int wall_texture = 0; // wall texture
unsigned int ball_texture = 0; // bowling ball texture
unsigned int cieling_texture = 0; // bowling ball texture
unsigned int duct_texture = 0;

double AX = 0; // x-coordinate of where the camera is looking
double AY = 0; // y-coordinate of where the camera is looking
double AZ = 0; // z-coordinate of where the camera is looking

double UX = 0; // x-coordinate of the up vector
double UY = 1; // y-coordinate of the up vector
double UZ = 0; // z-coordinate of the up vector

int pole_position[8] = {0,0,0,0,0,0,0,0};
double explosion[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
double reset[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
double bowling_ball_z[8] = {-25.0,-25.0,-25.0,-25.0,-25.0,-25.0,-25.0,-25.0};
int ball_ph[8] = {0,0,0,0,0,0,0,0};
int pin_x[8] = {19, 0, -17, -36, -53, -72, -89, -108};
float colors[8][4] = {
{1.0, 0.1, 0.6 , 1.0},
{0.1, 1.0, 0.6 , 1.0},
{0.8, 1.0, 1.0 , 1.0},
{0.4, 0.7, 1.0 , 1.0},
{0.3, 1.0, 0.3 , 1.0},
{0.2, 0.3, 0.6 , 1.0},
{0.7, 0.4, 0.5 , 1.0},
{1.0, 1.0, 0.3 , 1.0}
};
/******************** bowling game parameters (end)************/


/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*ch++);
}


/**** bowling game functions (start) ***/ 

void checkOffsets()
{
    if(xOffset > 29.5)
        xOffset = 29.5;
    else if(xOffset < -105.5)
        xOffset = -105.5;
    if(zOffset > 120)
        zOffset = 120;
    else if(zOffset < -60)
        zOffset = -60;
    if(yOffset > 28)
        yOffset = 28;
    else if(yOffset < 2)
        yOffset = 2;
}

/*********mouse rotation*****/ 
double mouse_rotation(double delta, double mid)
{
  return 180 * (delta / mid);
}
 

/*
*  Draw vertex in polar coordinates with normal
*/
static void Vertex1(double th1,double ph1)
{
  double x = Sin(th1)*Cos(ph1);
  double y = Cos(th1)*Cos(ph1);
  double z =         Sin(ph1);
  //  For a sphere at the origin, the position
  //  and normal vectors are the same
  glNormal3d(x,y,z);
  glVertex3d(x,y,z);
}

/*
* balll is for ball in the bowling game
*/

static void ball1(double x,double y,double z,double r, float color[])
 {
   int th1,ph1;
   float Emission[]  = {color[0],color[1],color[2],1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(color[0],color[1],color[2]);
   float shinyvec[1] = {4};
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,color);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph1=-90;ph1<90;ph1+=inc1)
   {
     glBegin(GL_QUAD_STRIP);
     for (th1=0;th1<=360;th1+=2*inc1)
     {
       Vertex1(th1,ph1);
       Vertex1(th1,ph1+inc1);
     }
     glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
 }

 /*
 * bowling x - the way x co-ordinates change with respect to change in Rotation
 */
 double bowling_ball_x(int lane)
 {
   int shift = lane % 2;
   if( bowling_ball_z[lane] > -20.0)
   {
     return -36.0 * (lane/2) + 24 + (!shift? 0: - 19.0);
   }
   else
   {
     return -36.0 * (lane/2) + 15 + (!shift? 0.25: -1.25);
   }
 }

 
 /*
 * bowling y - the way y co-ordinates change with respect to change in Rotation
 */
 double bowling_ball_y(int lane)
 {
   if( bowling_ball_z[lane] > -5.0)
   {
     return 1;
   }
   if(bowling_ball_z[lane] < -9 && bowling_ball_z[lane] > -11)
   {
     return -3.0;
   }
   if( bowling_ball_z[lane] > 110)
   {
     return -0.5*(bowling_ball_z[lane]-110);
   }
   else
   {
     return 3.7;
   }
 }
 
 
 /*
 * gobowl : roll range of the ball once rolled
 */
  void gobowl(int lane)
  {
    if(bowling_ball_z[lane] > -28.0)
    {
      return;
    }
    else
    {
      bowling_ball_z[lane] = -2.0;
    }
  }

  /*
  * mouse : mouse scroll change 
  */
  void mouse(int val, int state, int x, int y)
  {
	  if(BOWLING_GAME_FLAG){
    // Wheel reports as val 3(scroll up) and val 4(scroll down)
    if ((val == 3) || (val == 4)) // It's a wheel event
    {
      if (val == 3)
      dim1 += 0.1;
      else if(dim1>1)
      dim1 -= 0.1;
    }
    if ((val == 0) && (state == GLUT_DOWN || state == GLUT_UP))
    {
      if (state == GLUT_DOWN)
      left_click_down = 1;
      else if (state == GLUT_UP)
      left_click_down = 0;
      double midx = (window_width / 2);
      double midy = (window_height / 2);
      double x_delta = (midx - x);
      double y_delta = (midy - y);
      saved_th = th1 - mouse_rotation(x_delta , midx);
      saved_ph = ph1 - mouse_rotation(y_delta , midy);
    }
    //  Update projection
    Project(fieldOfView1,aspect1,dim1);
    //  Tell GLUT it is necessary to redisplay the scene
	  glutPostRedisplay();}
  }

  /*
  *motionmouse provides the updated value of projection after scroll
  */
  void motionmouse(int x, int y)
  {
	  if(BOWLING_GAME_FLAG){
    if (left_click_down)
    {
      double midx = (window_width / 2);
      double midy = (window_height / 2);
      double x_delta = (midx - x);
      double y_delta = (midy - y);
      th1 = mouse_rotation(x_delta , midx) + saved_th;
      ph1 = mouse_rotation(y_delta , midy) + saved_ph;
      th1 %= 360;
      ph1 %= 360;
    }
    //  Update projection
    Project(fieldOfView1,aspect1,dim1);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
	  }
  }

  /*
  *explode_and_reset : rest the value after the roll is done
  */
  void explode_and_reset(int lane)
  {
    double explosion_change = 12.0;
    if (explosion[lane] > 0.1)
    {
      if(explosion[lane] < 360.0)
      {
        explosion[lane] += explosion_change;
      }
      else
      {
        explosion[lane] = 0.0;
        reset[lane] = 30.0;
      }
    }
    if(reset[lane] > 0.0)
    {
      reset[lane] -= 0.1;
    }
  }

  /*
  * roll_dat_ball is the function to update the position of ball during rolling
  */
  void roll_dat_ball(int lane)
  {
    if(bowling_ball_z[lane] > -3.0)
    {
      bowling_ball_z[lane] += 0.8;
      ball_ph[lane] += 32;
      ball_ph[lane] %= 360;
    }
    if(bowling_ball_z[lane] > 105.0)
    {
      explosion[lane] += 1.0;
    }
    if(bowling_ball_z[lane] > 110.0)
    {
      bowling_ball_z[lane] = -10.0;
    }
    if(bowling_ball_z[lane] < -9 && bowling_ball_z[lane] > -11)
    {
      if(reset[lane] < 1.0 && explosion[lane] < 0.1)
      {
        bowling_ball_z[lane] = -20;
      }
    }
    if(bowling_ball_z[lane] < -19.5 && bowling_ball_z[lane] > -28.9)
    {
      bowling_ball_z[lane] -= 0.3;
      ball_ph[lane] -= 12;
      ball_ph[lane] %= 360;
    }
  }

/**** bowling game functions (end) ***/ 
 
/*
* Vertex change for racing game
*/ 
static void Vertex(double th,double ph, double r,double g,double b)
{
   glColor3f(r,g,b); // Setting color to yellow
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}


/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph,1,1,1);
         Vertex(th,ph+inc,1,1,1);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

/*
* drawTree : draws the tree cordinate with respect to the textureVal passed
*/
static void drawTree(int textureVal)
{
	const int d=5;
	int th,ph;

	float white[] = {1,1,1,1};
	float black[] = {0,0,0,1};
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvector);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

	//  Save transformation
	glPushMatrix();
	//  Offset and scale
	glTranslatef(-1.2,1.3,0);
	glScalef(0.3,0.6,0.4);

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,texture_2D[textureVal]);

	for (ph=-90;ph<90;ph+=d)
	{	
		glBegin(GL_QUAD_STRIP);
		for (th=0;th<=360;th+=d)
		{
			glTexCoord2f(sin(th),0);
			Vertex(th,ph,0.6,0.8,0.2);
			glTexCoord2f(cos(th),1);
			Vertex(th,ph+d,0.6,0.8,0.2);
		}
			glEnd();
	}
	glPopMatrix(); 
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslated(-0.3,-0.6,0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,texture_2D[1]);
	//Start bark of tree
	glBegin(GL_QUADS);
	// Front
	glNormal3f(0,0,1);
	glColor3f(0.4,0.25,0.13);
	glTexCoord2f(0,0);glVertex3f(-1,0.1, 0.1);
	glTexCoord2f(1,0);glVertex3f(-0.8,0.1, 0.1);
	glTexCoord2f(1,1);glVertex3f(-0.8,1.5, 0.1);
	glTexCoord2f(0,1);glVertex3f(-1,1.5, 0.1);
	//  Back
	glNormal3f(0,0,-1);
	glColor3f(0.4,0.25,0.13);
	glTexCoord2f(0,0);glVertex3f(-0.8,0.1,0.1);
	glTexCoord2f(1,0);glVertex3f(-1,0.1,-0.1);
	glTexCoord2f(1,1);glVertex3f(-1,1.5,-0.1);
	glTexCoord2f(0,1);glVertex3f(-0.8,1.5,-0.1);
	//  Right
	glColor3f(0.4,0.25,0.13);
	glNormal3f(1,0,0);
	glTexCoord2f(0,0);glVertex3f(-0.8,0.1,0.1);
	glTexCoord2f(1,0);glVertex3f(-0.8,0.1,-0.1);
	glTexCoord2f(1,1);glVertex3f(-0.8,1.5,-0.1);
	glTexCoord2f(0,1);glVertex3f(-0.8,1.5,0.1);
	//  Left
	glColor3f(0.4,0.25,0.13);
	glNormal3f(-1,0,0);
	glTexCoord2f(0,0);glVertex3f(-1,0.1,-0.1);
	glTexCoord2f(1,0);glVertex3f(-1,0.1,0.1);
	glTexCoord2f(1,1);glVertex3f(-1,1.5,0.1);
	glTexCoord2f(0,1);glVertex3f(-1,1.5,-0.1);
	//  Top
	glColor3f(0.4,0.25,0.13);
	glNormal3f(0,1,0);
	glTexCoord2f(0,0);glVertex3f(-1,1.5,0.1);
	glTexCoord2f(1,0);glVertex3f(-0.8,1.5,0.1);
	glTexCoord2f(1,1);glVertex3f(-0.8,1.5,-0.1);
	glTexCoord2f(0,1);glVertex3f(-1,1.5,-0.1); 
	//  Bottom
	glColor3f(0.4,0.25,0.13);
	glNormal3f(0,-1,0);
	glTexCoord2f(0,0);glVertex3f(-1,0.1,-0.1);
	glTexCoord2f(1,0);glVertex3f(-0.8,0.1,-0.1);
	glTexCoord2f(1,1);glVertex3f(-0.8,0.1,0.1);
	glTexCoord2f(0,1);glVertex3f(-1,0.1,0.1);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
	

/**************************
* drawing the house
**************************/

static void drawHouse( int x, int y, int z,
                        int dx, int dy, int dz,
                        double th, int cval){


	//  Set specular color to white
	float white[] = {1,1,1,1};
	float black[] = {0,0,0,1};
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);



	glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	// body of the house
    glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,texture_2D[cval]);

	glBegin(GL_QUADS);
	glNormal3f( 0, 0, 1);
	glTexCoord2f(0,0);	glVertex3f(-1,-1, 1);
	glTexCoord2f(1,0);  glVertex3f(+1,-1, 1);
	glTexCoord2f(1,1);	glVertex3f(+1,+1, 1);
	glTexCoord2f(0,1);	glVertex3f(-1,+1, 1);

	glNormal3f( 0, 0, -1);
	glTexCoord2f(0,0);	glVertex3f(-1,-1, -1);
	glTexCoord2f(1,0);	glVertex3f(+1,-1, -1);
	glTexCoord2f(1,1);	glVertex3f(+1,+1, -1);
	glTexCoord2f(0,1);	glVertex3f(-1,+1, -1);

	glNormal3f( 0, 1, 0);
	glTexCoord2f(0,0);	glVertex3f(-1, +1, -1);
	glTexCoord2f(1,0);	glVertex3f(+1, +1, -1);
	glTexCoord2f(1,1);	glVertex3f(+1, +1, +1);
	glTexCoord2f(0,1);	glVertex3f(-1, +1, +1);

	glNormal3f( 0, -1, 0);
	glTexCoord2f(0,0);	glVertex3f(-1, -1, -1);
	glTexCoord2f(1,0);	glVertex3f(+1, -1, -1);
	glTexCoord2f(1,1);	glVertex3f(+1, -1, +1);
	glTexCoord2f(0,1);	glVertex3f(-1, -1, +1);


	glNormal3f( 1, 0, 0);
	glTexCoord2f(0,0);	glVertex3f(+1, -1, -1);
	glTexCoord2f(1,0);	glVertex3f(+1, +1, -1);
	glTexCoord2f(1,1);	glVertex3f(+1, +1, +1);
	glTexCoord2f(0,1);	glVertex3f(+1, -1, +1);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(0,0);	glVertex3f(-1, -1, -1);
	glTexCoord2f(1,0);	glVertex3f(-1, +1, -1);
	glTexCoord2f(1,1);	glVertex3f(-1, +1, +1);
	glTexCoord2f(0,1);	glVertex3f(-1, -1, +1);

	glEnd();

	// Draw ROof
	glBindTexture(GL_TEXTURE_2D,texture_2D[cval+1]);
	glBegin(GL_TRIANGLES);

	glNormal3f(0,0,+1);
	glTexCoord2f(0,0);	glVertex3f(+1.5,+0.75,+1.25);
	glTexCoord2f(1,0);	glVertex3f(-1.5,+0.75,+1.25);
	glTexCoord2f(0.5,1);glVertex3f(0,+2,+0.625);


	glNormal3f(0,0,-1);
	glTexCoord2f(0,0);	glVertex3f(+1.5,+0.75,-1.25);
	glTexCoord2f(1,0);	glVertex3f(-1.5,+0.75,-1.25);
	glTexCoord2f(0.5,1);glVertex3f(0,+2,-0.625);
	glEnd();


	// Draw Side Quads of the roof
	glBindTexture(GL_TEXTURE_2D,texture_2D[cval+1]);
	glBegin(GL_QUADS);


	glNormal3f(+1,+1,0);
	glTexCoord2f(0,0);	glVertex3f(+1.5,+0.75,-1.25);
	glTexCoord2f(1,0);	glVertex3f(+1.5,+0.75,+1.25);
	glTexCoord2f(1,1);	glVertex3f(0,+2,+0.625);
	glTexCoord2f(0,1);	glVertex3f(0,+2,-0.625);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,texture_2D[cval+1]);
	glBegin(GL_QUADS);

	glNormal3f(-1,-1,0);
	glTexCoord2f(0,0);	glVertex3f(-1.5,+0.75,-1.25);
	glTexCoord2f(1,0);	glVertex3f(-1.5,+0.75,+1.25);
	glTexCoord2f(1,1);	glVertex3f(0,+2,+0.625);
	glTexCoord2f(0,1);	glVertex3f(0,+2,-0.625);

	glEnd();


	glBindTexture(GL_TEXTURE_2D,texture_2D[cval+1]);
	glBegin(GL_QUADS);

	glNormal3f(0,+1,0);
	glTexCoord2f(0,0);	glVertex3f(+1.5,+0.75,-1.25);
	glTexCoord2f(1,0);	glVertex3f(+1.5,+0.75,+1.25);
	glTexCoord2f(1,1);	glVertex3f(-1.5,+0.75,+1.25);
	glTexCoord2f(0,1);	glVertex3f(-1.5,+0.75,-1.25);

	glEnd();
	

	// Draw Chimney
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,texture_2D[17]);

	glBegin(GL_QUADS);
	// Left

	glNormal3f(1, 0, 0);
	glTexCoord2f(0,0);	glVertex3f(+.5,+1,+.25);
	glTexCoord2f(1,0);	glVertex3f(+.5,+1,-.25);
 	glTexCoord2f(1,1);	glVertex3f(+.5,+2,-.25);
 	glTexCoord2f(0,1);	glVertex3f(+.5,+2,+.25);
	
	// Right
	glNormal3f(+1, 0, 0);
	glTexCoord2f(0,0);	glVertex3f(+1,+1,+.25);
	glTexCoord2f(1,0);	glVertex3f(+1,+1,-.25);
	glTexCoord2f(1,1);	glVertex3f(+1,+2,-.25);
	glTexCoord2f(0,1);	glVertex3f(+1,+2,+.25);
	
	// Back
	glNormal3f(0, 0, -1);
	glTexCoord2f(0,0);	glVertex3f(+.5,+1,-.25);
	glTexCoord2f(1,0);	glVertex3f(+.5,+2,-.25);
	glTexCoord2f(1,1);	glVertex3f(+1,+2,-.25);
	glTexCoord2f(0,1);	glVertex3f(+1,+1,-.25);
	
	// Front
	glNormal3f(0, 0, +1);
	glTexCoord2f(0,0);	glVertex3f(+.5,+1,+.25);
	glTexCoord2f(1,0);	glVertex3f(+.5,+2,+.25);
	glTexCoord2f(1,1);	glVertex3f(+1,+2,+.25);
	glTexCoord2f(0,1);	glVertex3f(+1,+1,+.25);
	
	// Top
	glNormal3f(0, +1, 0);
	glTexCoord2f(0,0);	glVertex3f(+.5,+2,+.25);
	glTexCoord2f(1,0);	glVertex3f(+.5,+2,-.25);
	glTexCoord2f(1,1);	glVertex3f(+1,+2,-.25);
	glTexCoord2f(0,1);	glVertex3f(+1,+2,+.25);
	glEnd();

	
	// Door
	glBindTexture(GL_TEXTURE_2D,texture_2D[18]);

	glBegin(GL_QUADS);
	glNormal3f(0, 0, +0.4);
	glTexCoord2f(0,0);  glVertex3f(+.2,-1,1.01);
	glTexCoord2f(1,0);	glVertex3f(-.2,-1,1.01);
	glTexCoord2f(1,1);	glVertex3f(-.2,0,1.01);
	glTexCoord2f(0,1);	glVertex3f(+.2,0,1.01);
	glEnd();

	glDisable(GL_TEXTURE_2D);

}

/*
* draw the road
*/
static void drawRoad()
{
	float white[] = {1,1,1,1};
	float black[] = {0,0,0,1};
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvector);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1,-1);
	glBegin(GL_QUADS);
	glColor3f(0.415, 0.415, 0.415);
	glNormal3f(0,1,0);
	glVertex3f(1,-10,-5000);
	glVertex3f(1,-10,5000);         //a long road
	glVertex3f(65,-10,5000);
	glVertex3f(65,-10,-5000);

	glColor3f(0.974, 0.974, 1.000);
	glVertex3f(-1,-10,-5000);       //a median
	glVertex3f(-1,-10,5000);
	glVertex3f(1,-10,5000);
	glVertex3f(1,-10,-5000);

	glColor3f(0.415, 0.415, 0.415);
	glVertex3f(-65,-10,-5000);
	glVertex3f(-65,-10,5000);         //a long road
	glVertex3f(-1,-10,5000);
	glVertex3f(-1,-10,-5000); 
	glEnd();
	glDisable(GL_POLYGON_OFFSET_FILL);
}

/*
* Draw road2 perpendicular to the one we have already drawn
*/
static void drawRoad2()
{
	float white[] = {1,1,1,1};
	float black[] = {0,0,0,1};
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvector);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1,-1);
	glBegin(GL_QUADS);
	glColor3f(0.415, 0.415, 0.415);
	glNormal3f(0,1,0);
	glVertex3f(-5000,-14,1);
	glVertex3f(0,-14,1);         //a long road
	glVertex3f(0,-14,65);
	glVertex3f(-5000,-14,65);

	glColor3f(0.974, 0.974, 1.000);
	glVertex3f(-5000,-14,-1);       //a median
	glVertex3f(0,-14,-1);
	glVertex3f(0,-14,1);
	glVertex3f(-5000,-14,1);

	glColor3f(0.415, 0.415, 0.415);
	glVertex3f(-5000,-14,-65);
	glVertex3f(0,-14,-65);         //a long road
	glVertex3f(0,-14,-1);
	glVertex3f(-5000,-14,-1); 
	glEnd();
	glDisable(GL_POLYGON_OFFSET_FILL);
}



/*
* Draw skybox
*/
static void drawSkyBox(double dim) //skycube
{
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   //  Sides
   if(environmentCar ==1)
   glBindTexture(GL_TEXTURE_2D,texture_2D[2]);
   else    
   glBindTexture(GL_TEXTURE_2D,texture_2D[15]);

   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-dim,0,+dim);
   glTexCoord2f(1,0); glVertex3f(-dim,0,-dim);
   glTexCoord2f(1,1); glVertex3f(-dim,+dim,-dim);
   glTexCoord2f(0,1); glVertex3f(-dim,+dim,+dim);
  
   glTexCoord2f(1,0); glVertex3f(-dim,0,-dim);
   glTexCoord2f(0,0); glVertex3f(+dim,0,-dim);
   glTexCoord2f(0,1); glVertex3f(+dim,+dim,-dim);
   glTexCoord2f(1,1); glVertex3f(-dim,+dim,-dim);
   
   glTexCoord2f(0,0); glVertex3f(+dim,0,-dim);
   glTexCoord2f(1,0); glVertex3f(+dim,0,+dim);
   glTexCoord2f(1,1); glVertex3f(+dim,+dim,+dim);
   glTexCoord2f(0,1); glVertex3f(+dim,+dim,-dim);
   
   glTexCoord2f(0,0); glVertex3f(+dim,0,+dim);
   glTexCoord2f(1,0); glVertex3f(-dim,0,+dim);
   glTexCoord2f(1,1); glVertex3f(-dim,+dim,+dim);
   glTexCoord2f(0,1); glVertex3f(+dim,+dim,+dim);
   glEnd();

   //  Top and bottom

   glColor3f(1,1,1); 
   if(environmentCar ==1)
   	glBindTexture(GL_TEXTURE_2D,texture_2D[4]);
   else    
   	glBindTexture(GL_TEXTURE_2D,texture_2D[14]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-dim, 0,+dim);
   glTexCoord2f(0,1); glVertex3f(+dim, 0,+dim);
   glTexCoord2f(1,1); glVertex3f(+dim, 0,-dim);
   glTexCoord2f(1,0); glVertex3f(-dim, 0,-dim);
   glEnd();
   glDisable(GL_TEXTURE_2D);


   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture_2D[3]);
   glBegin(GL_QUADS);
   glTexCoord2f(1,0); glVertex3f(+dim,+dim,-dim);
   glTexCoord2f(0,0); glVertex3f(+dim,+dim,+dim);
   glTexCoord2f(0,1); glVertex3f(-dim,+dim,+dim);
   glTexCoord2f(1,1); glVertex3f(-dim,+dim,-dim);
   glEnd();

}

/*
* Draw road sign
*/
static void drawRoadSign()
{
	
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,texture_2D[5]);
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glTexCoord2f(1,0); glVertex3f(-1,8.5,0);
	glTexCoord2f(1,0); glVertex3f(1,8.5,0);
	glTexCoord2f(1,1); glVertex3f(1,10,0);
	glTexCoord2f(0,1); glVertex3f(-1,10,0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,texture_2D[6]);
	glPushMatrix();
	glTranslated(0,-1,0);
	glScaled(0.1,10,0.1);
	glBegin(GL_QUAD_STRIP);
	for (th=0;th<=360;th+=15)
	{  
		glTexCoord2f(Sin(i),0);
		glNormal3f(Cos(th),1,Sin(th));
		glVertex3f(Cos(th),1,Sin(th));
		glTexCoord2f(Sin(i),1);
		glNormal3f(Cos(th),0,Sin(th));
		glVertex3f(Cos(th),0,Sin(th));
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

/*
* draw pedestrian Crossing
*/
static void drawPedestrianCrossing()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,texture_2D[7]);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1,-1);
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2f(0,1); glVertex3f(-62,-8.55,-15);
	glTexCoord2f(0,0); glVertex3f(-62,-8.55,15);         
	glTexCoord2f(1,0); glVertex3f(62,-8.55,15);
	glTexCoord2f(1,1); glVertex3f(62,-8.55,-15); 
	glEnd();
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_TEXTURE_2D);   
}

/*
* draw trafficBlocks
*/
static void drawTrafficBlocks()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,texture_2D[8]);
	glBegin(GL_QUADS);
	glColor3f(1,1,1);     
	glNormal3f(0,5.28,5.28);
	glTexCoord2f(0.0  ,0.0);glVertex3f(-1.2, 0.8, 1.2); 
	glTexCoord2f(1.0  ,0.0);glVertex3f(1.2, 0.8, 1.2);
	glTexCoord2f(1.0  ,1.0);glVertex3f(1.0, 2.0, 0.0);
	glTexCoord2f(0.0  ,1.0);glVertex3f(-1.0, 2.0, 0.0);
	  
	glColor3f(1,1,1);     // white
	 glNormal3f(0,-5.28,5.28);
	glTexCoord2f(0.0  ,0.0);glVertex3f(1.0, 2.0, 0.0); 
	glTexCoord2f(1.0  ,0.0);glVertex3f(-1.0, 2.0, 0.0); 
	glTexCoord2f(1.0  ,1.0);glVertex3f( -1.2, 0.8, -1.2); 
	glTexCoord2f(0.0  ,1.0);glVertex3f( 1.2, 0.8, -1.2); 
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

/*
* A simple Circle shape
*/
static void drawCircle()
{
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0,0,1);
	glVertex3f(0,0,0);
	for (th=0;th<=360;th+=15)
	{
		glNormal3f(0,0,1);
		glVertex3f(Cos(th),Sin(th),0);
	}
	glEnd();
}

/*
* Draw a cube
*/
static void drawCube(double x, double y, double z)
{
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glTexCoord2f(0,0); glVertex3f(0,0,0);
	glTexCoord2f(0,1); glVertex3f(0,y,0);
	glTexCoord2f(1,1); glVertex3f(x,y,0);
	glTexCoord2f(1,0); glVertex3f(x,0,0);

	glNormal3f(1,0,0);
	glTexCoord2f(0,0); glVertex3f(x,0,z);
	glTexCoord2f(0,1); glVertex3f(x,0,0);
	glTexCoord2f(1,1); glVertex3f(x,y,0);
	glTexCoord2f(1,0); glVertex3f(x,y,z);

	glNormal3f(-1,0,0);
	glTexCoord2f(0,0); glVertex3f(0,0,z);
	glTexCoord2f(0,1); glVertex3f(0,y,z);
	glTexCoord2f(1,1); glVertex3f(0,y,0);
	glTexCoord2f(1,0); glVertex3f(0,0,0);

	glNormal3f(0,0,-1);
	glTexCoord2f(0,0); glVertex3f(0,0,z);
	glTexCoord2f(1,0); glVertex3f(x,0,z);
	glTexCoord2f(1,1); glVertex3f(x,y,z);
	glTexCoord2f(0,1); glVertex3f(0,y,z);

	glNormal3f(0,1,0);
	glTexCoord2f(0,0); glVertex3f(0,y,0);
	glTexCoord2f(0,1); glVertex3f(0,y,z);
	glTexCoord2f(1,1); glVertex3f(x,y,z);
	glTexCoord2f(1,0); glVertex3f(x,y,0);

	glNormal3f(0,-1,0);
	glTexCoord2f(0,1); glVertex3f(0,0,z);
	glTexCoord2f(0,0); glVertex3f(0,0,0);
	glTexCoord2f(1,0); glVertex3f(x,0,0);
	glTexCoord2f(1,1); glVertex3f(x,0,z);
	
	glEnd();
}


/*
* Acceeration control values
*/
static void specialKey()
{
	if (speedState['z'] || speedState['Z'])
       acceleration+=1;
    if (speedState['x']||speedState['X'] )
       acceleration-=1;

}

/*
* generalkeys to determine collision detection
*/
static void generalKey()
{

	if(speedState[GLUT_KEY_RIGHT] && SCREEN == gameCurrentView3)
	{
		
		FlagC = 0;
		for (i=0; i<7; i++)
		{

			if(DistanceC[i] < 14)
			{
				//printf("Collison Detected\n");
				FlagC = 1;	
				SCREEN = gameCurrentView4;		
			
			}

		}
		if(FlagC != 1)
			Ex += acceleration;
	}
	else if(speedState[GLUT_KEY_LEFT] && SCREEN == gameCurrentView3)
	{
		FlagC = 0;
		for (i=0; i<7; i++)
		{

			if(DistanceC[i] < 14)
			{
				//printf("Collison Detected\n");
				FlagC = 1;
				SCREEN = gameCurrentView4;		
				
			}

		}
		if(FlagC != 1)
		Ex -= acceleration;
	}
	else if(speedState[GLUT_KEY_UP] && SCREEN == gameCurrentView3)
	{
		FlagC = 0;
		for (i=0; i<7; i++)
		{

			if(DistanceC[i] < 14)
			{
				//printf("Collison Detected\n");
				FlagC = 1;		
				SCREEN = gameCurrentView4;		
			}

		}
		if(FlagC != 1)
		Ez-= acceleration;
	}
	else if(speedState[GLUT_KEY_DOWN] && SCREEN == gameCurrentView3)
	{
		FlagC = 0;
		for (i=0; i<7; i++)
		{

			if(DistanceC[i] < 14)
			{
				//printf("Collison Detected\n");
				FlagC = 1;		
				SCREEN = gameCurrentView4;		
		
			}

		}
		if(FlagC != 1)
		Ez+= acceleration;
	}
}

/*
* draw  simple one side traffic signal with the pole
*/

static void drawTrafficSignal()
{
	float white[] = {1,1,1,1};
	float black[] = {0,0,0,1};
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvector);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,texture_2D[9]);

	glColor3f(0.7,0.7,0.7);
	drawCube(0.5,7,1);
	glPushMatrix();
	glTranslated(0,7,0);  
	drawCube(2,3,1);
	glPopMatrix();
	if(Change_light<=90)
	{
		glDisable(GL_TEXTURE_2D); 
		glColor3f(1,0,0);
		glPushMatrix();
		glTranslated(2,9.3,2); 
		glScaled(0.3,0.3,0.3);
		drawCircle();
		glPopMatrix();
		Change_light++;
	}
	glEnable(GL_TEXTURE_2D);
	if(Change_light<=180&&Change_light>90)
	{
		glDisable(GL_TEXTURE_2D); 
		glColor3f(1.1,1,0);
		glPushMatrix();
		glTranslated(2,8.3,2); 
		glScaled(0.3,0.3,0.3);
		drawCircle();
		glPopMatrix();
		Change_light++;
	}
	else
	{
	glEnable(GL_TEXTURE_2D);
	}
	glEnable(GL_TEXTURE_2D);
	if(Change_light<265&&Change_light>180)
	{
		glDisable(GL_TEXTURE_2D); 
		glColor3f(0,1,0);
		glPushMatrix();
		glTranslated(2,7.3,2); 
		glScaled(0.3,0.3,0.3);
		drawCircle();
		glPopMatrix();
		Change_light++;
	}
	if(Change_light==265)
	{
		Change_light=0;
	}
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_2D);
}

/*
* draw a building
*/
static void drawBuilding()
{
	float white[] = {1,1,1,1};
	float black[] = {0,0,0,1};
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvector);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,texture_2D[10]);
	drawCube(20,10,5);
	glDisable(GL_TEXTURE_2D);
}

/*
* load the object of the car to a call list
*/
static void getCarObj()
{

	glPushMatrix();
	glTranslatef(0,-10.5,995);
	glRotatef(90, 1, 0, 0);
	glRotatef(180, 0, 1, 0);
	glRotatef(varCarRotate,0,0,1);
	glScaled(1,1,1);
	glCallList(mercedes);
	glPopMatrix();
}


/*
* display the first Page
*/
static void displayFirstPage(){

	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glWindowPos2i(400,550);
	Print("Welcome to Arcade Games!!",2);
	glWindowPos2i(80,500);
	Print("Games:",2);
	glWindowPos2i(80,450);
	Print("1.Car Driving Test",2);
	glWindowPos2i(480,450);
	Print("2.Bowling Game ",2);
	glWindowPos2i(80,350);
	Print("Controls:",2);
	glWindowPos2i(80,300);
	Print("Press @ toggle between two different games at any time",2);
	glWindowPos2i(80,250);
	Print("To continue with 1st game press SPACE",2);
	glWindowPos2i(80,200);
	Print("ESC: To exit game",2);

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,texture_2D[16]);
	glBegin(GL_QUADS);                
	glColor3f(1,1,1); 
	glNormal3f( 0, 0, 1);
	glTexCoord2f(0,0);glVertex3f(-55,-30,-35);
	glTexCoord2f(1,0);glVertex3f(+55,-30,-35);
	glTexCoord2f(1,1);glVertex3f(+55,+30,-35);
	glTexCoord2f(0,1);glVertex3f(-55,+30,-35);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

/*
* Display the car - this function is usually used to show the car in 3D rotating perspective before game 
*/
static void displayCar()
{
	glClearColor(0.6, 0.75, 0.85, 1); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//  Enable Z-buffering in OpenGL
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glWindowPos2i(680,50);
	Print("Press Enter to start the game",1);
	glPushMatrix();
	glTranslated(0,11,0);
	getCarObj();
	glPopMatrix();  

}

/*
* displayEnvironmentMenu : depending on the environment chosen : display desert or denver environment textures
*/
static void displayEnvironmentMenu()
{
	glClearColor(0.6, 0.75, 0.85, 1); // sky color is light blue
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glWindowPos2i(100,500);
	//  Enable Z-buffering in OpenGL
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glLoadIdentity();
	// Set the look at position for perspective projection
	gluLookAt(Ex,Ey,Ez,Ex,Ey,Ez+lz,0,1,0);
	displayCar();

	glWindowPos2i(680,550);
	Print("Car for Driving test: Mercedes-Benz SLS AMG ",1);
	glWindowPos2i(680,500);
	Print("Environment to drive the car:",1);
	glWindowPos2i(680,450);
	Print("1.Denver",1);
	glWindowPos2i(880,450);
	Print("2.Desert",1);
	glWindowPos2i(680,400);
	Print("Press 1 or 2 to select the environment",1);
	
	glWindowPos2i(60,450);
	Print("Controls:",2);
	glWindowPos2i(60,400);
	Print("Up arrow : Move Forward ",1);
	glWindowPos2i(60,350);
	Print("Down arrow : Move Backward ",1);
	glWindowPos2i(60,300);
	Print("Right arrow: Move towards right ",1);
	glWindowPos2i(60,250);
	Print("Left arrow: Move towards left ",1);
	glWindowPos2i(60,200);
	Print("+/-: Zoom in/Zoom out ",1);
	glWindowPos2i(60,150);
	Print("ESC: To exit game",1);

	if(environmentCar == 1)
	{
	glWindowPos2i(680,450);
	Print("1.Denver (Selected)",1);
	glWindowPos2i(880,450);
	Print("2.Desert",1);
	glWindowPos2i(680,400);		
	}
	else if(environmentCar == 2)
	{	
	glWindowPos2i(680,450);
	Print("1.Denver            ",1);
	glWindowPos2i(880,450);
	Print("2.Desert  (Selected)",1);
	glWindowPos2i(680,400);
	}
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,texture_2D[11]);
	glBegin(GL_QUADS);                
	glColor3f(1,1,1); 
	glNormal3f( 0, 0, 1);
	glTexCoord2f(0,0);glVertex3f(-1300,-1000,200);
	glTexCoord2f(1,0);glVertex3f(+1300,-1000,200);
	glTexCoord2f(1,1);glVertex3f(+1300,+1000,200);
	glTexCoord2f(0,1);glVertex3f(-1300,+1000,200);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

/*
* Display the crash of the car
*/
static void displayCrash()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	Ex=0;Ey=1;Ez=1000;
	glLoadIdentity();
	gluLookAt(Ex,Ey,Ez,Ex,Ey,Ez+lz,0,1,0);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	glWindowPos2i(100,400);
	Print("You Failed the Driving License Test",1);
	glWindowPos2i(100,350);
	Print("Press r/R to restart or Esc to exit the game",1);
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture_2D[12]);
	glBegin(GL_QUADS);                
	glColor3f(1,1,1); 
	glNormal3f( 0, 0, 1);
	glTexCoord2f(0,0);glVertex3f(-1300,-1000,200);
	glTexCoord2f(1,0);glVertex3f(+1300,-1000,200);
	glTexCoord2f(1,1);glVertex3f(+1300,+1000,200);
	glTexCoord2f(0,1);glVertex3f(-1300,+1000,200);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

/*********************************
 ***  Adapted from Example 7 given on Moodle
 ***  Draw a cylinder
 ***     at (x,y,z)
 ***     dimentions (dx,dy,dz)
 ***     rotated th about the z axis
 ***     given top & bottom color
 ***     given side color
 ************************************/

void drawCylinder(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, char lcol, char scol) {
   double i;
   double radius = 0.3, height = 0.2;

   glPushMatrix();
   //  Transform cube
   glTranslated(x,y,z);
   glRotated(th,0,0,1);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture_2D[1]);
   
   double k,l,m;
   /* Tube of Cylinder */
   glBegin(GL_QUAD_STRIP);
      for(i = 0.0; i <= 2.1*PI; i+=.05){
	 k = radius * cos(i); l = height; m = radius * sin(i);
   	 glNormal3d(k,l,m);
   	 glTexCoord2d(th,0);
         glVertex3d(radius * cos(i), height, radius * sin(i));

	 k = radius * cos(i); l = -height; m = radius * sin(i);
   	 glNormal3d(k,l,m);
   	 glTexCoord2d(th/360.0,ph/180.0+0.5);
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
   glEnd();

   //Set colors
   if (lcol == 'r'){
      glColor3f(1.0, 0.0, 0.0); //Set color to red
   } else if (lcol == 'g') {
      glColor3f(0.0, 1.0, 0.0); //Set color to green
   } else if (lcol == 'b') {
      glColor3f(0.0, 0.0, 1.0); //Set color to blue
   } else if (lcol == 'w') {
      glColor3f(1.0, 1.0, 1.0); //Set color to white
   } else {
      glColor3f(0.8, 0.8, 0.8); //Default color to gray
   }

   /* Top of Cylinder */
   glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, height, 0.0);

      for(i = 0.0; i < 2*PI*radius*4; i+=.125) {
         glVertex3d(radius * cos(i), height, radius * sin(i));
      }
   glEnd();

   /* Bottom of Cylinder */
   glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, -height, 0.0);

      for(i = 0.0; i < 2*PI*radius*4; i+=.125) {
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
   glEnd();

   glPopMatrix();
}

/*
* draw the traffic cones
*/
static void drawCone(){
glPushMatrix();
float radius=1;
float h=1;
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture_2D[25]);
float t,s;
float i=0.05;
float k,l,m;
for(s=0.0;s<1.0;s+=i)
{
    for(t=0.0;t<=1.0;t+=i)
    {       
        float r=((h-t)/h)*radius;
        glBegin(GL_POLYGON);
k = r*cos(2*M_PI*s); l=t; m=r*sin(2*M_PI*s);
glNormal3d(k,l,m);
        glTexCoord2f(s,t);
        glVertex3f(r*cos(2*M_PI*s),t,r*sin(2*M_PI*s));
k = r*cos(2*M_PI*(s+i)); l=t; m=r*sin(2*M_PI*(s+i));
glNormal3d(k,l,m);
        glTexCoord2f(s+i,t);
        glVertex3f(r*cos(2*M_PI*(s+i)),t,r*sin(2*M_PI*(s+i)));
k = r*cos(2*M_PI*(s+i)); l=t+i; m=r*sin(2*M_PI*(s+i));
glNormal3d(k,l,m);
        glTexCoord2f(s+i,t+i);
        glVertex3f(r*cos(2*M_PI*(s+i)),(t+i),r*sin(2*M_PI*(s+i)));
k = r*cos(2*M_PI*s); l=t+i; m=r*sin(2*M_PI*s);
glNormal3d(k,l,m);
        glTexCoord2f(s,t+i);
        glVertex3f(r*cos(2*M_PI*s),(t+i),r*sin(2*M_PI*s));
        glEnd();
    }
}

glDisable(GL_TEXTURE_2D);
glPopMatrix();
}


/*************************** CLIMATE_RAIN/hailhelper functions (start)***************/

// Initialize/Reset elements - give them their attributes
void initelements(int i) {
    elementArray[i].alive = 1;
    elementArray[i].scope = 1.0;
    elementArray[i].decay = (float)(rand()%10)/(1000.0f+0.003f);

    elementArray[i].xpos = (float) (rand() % 2000) - 1000;
    elementArray[i].ypos = 10.0;
    elementArray[i].zpos = (float) (rand() % 2000) - 1000;

    elementArray[i].red = 0.5;
    elementArray[i].green = 0.5;
    elementArray[i].blue = 1.0;

    elementArray[i].velocity = increaseSpeed;
    elementArray[i].gravity = -0.8;//-0.8;

}

/*
* Initialize all the particles with some random location to start from.
*/
void init( ) {

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

    // Initialize elements
    for (single_particle = 0; single_particle < _MAX_ELEMENTS; single_particle++) {
        initelements(single_particle);
    }
}


/*
*CLIMATE_RAIN drawn 
*/
void drawCLIMATE_RAIN() {
  float x, y, z;
  for (single_particle = 0; single_particle < _MAX_ELEMENTS; single_particle=single_particle+2) {
    if (elementArray[single_particle].alive == 1) {
      x = elementArray[single_particle].xpos;
      y = elementArray[single_particle].ypos;
      z = elementArray[single_particle].zpos + zoom;

      // Draw elements
      glColor3f(0.5, 0.5, 1.0);
      glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x, y+0.5, z);
      glEnd();

      // Adjust decreaseSpeed for speed!
      elementArray[single_particle].ypos += elementArray[single_particle].velocity / (decreaseSpeed*1000);
      elementArray[single_particle].velocity += elementArray[single_particle].gravity;
      // Decay
      elementArray[single_particle].scope -= elementArray[single_particle].decay;

      if (elementArray[single_particle].ypos <= -10) {
        elementArray[single_particle].scope = -1.0;
      }
      //Revive
      if (elementArray[single_particle].scope < 0.0) {
        initelements(single_particle);
      }
    }
  }
}

/*
*CLIMATE_HAIL drawn 
*/
void drawCLIMATE_HAIL() {
  float x, y, z;

  for (single_particle = 0; single_particle < _MAX_ELEMENTS; single_particle=single_particle+2) {
    if (elementArray[single_particle].alive == 1) {
      x = elementArray[single_particle].xpos;
      y = elementArray[single_particle].ypos;
      z = elementArray[single_particle].zpos + zoom;

      // Draw elements
      glColor3f(0.8, 0.8, 0.9);
      glBegin(GL_QUADS);
        // Front
        glVertex3f(x-hailsize, y-hailsize, z+hailsize); // lower left
        glVertex3f(x-hailsize, y+hailsize, z+hailsize); // upper left
        glVertex3f(x+hailsize, y+hailsize, z+hailsize); // upper right
        glVertex3f(x+hailsize, y-hailsize, z+hailsize); // lower left
        //Left
        glVertex3f(x-hailsize, y-hailsize, z+hailsize);
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z+hailsize);
        // Back
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        //Right
        glVertex3f(x+hailsize, y+hailsize, z+hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z+hailsize);
        //Top
        glVertex3f(x-hailsize, y+hailsize, z+hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z+hailsize);
        //Bottom
        glVertex3f(x-hailsize, y-hailsize, z+hailsize);
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z+hailsize);
      glEnd();

      // Update values
      //Move
      if (elementArray[single_particle].ypos <= -10) {
        elementArray[single_particle].velocity = elementArray[single_particle].velocity * -1.0;
      }
      elementArray[single_particle].ypos += elementArray[single_particle].velocity / (decreaseSpeed*1000); // * 1000
      elementArray[single_particle].velocity += elementArray[single_particle].gravity;

      // Decay
      elementArray[single_particle].scope -= elementArray[single_particle].decay;

      //Revive
      if (elementArray[single_particle].scope < 0.0) {
        initelements(single_particle);
      }
    }
  }
}


/*************************CLIMATE_RAIN/hail/storm (end)*******************************/
/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
static void display()
{
if(BOWLING_GAME_FLAG)
{
    if(SEIZE_RESIZE_WINDOW)
    glutReshapeWindow(600,600);
    int i;
    double fall;
    const double len=10.0;  //  Length of axes
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);
    //  Undo previous transformations
    glLoadIdentity();

    if(mode1 == 0)
    {
        //Set up perspective projection
        double Ex = 2*dim1*Sin(th1)*Cos(ph1);
        double Ey = 2*dim1*Sin(ph1);
        double Ez = -2*dim1*Cos(th1)*Cos(ph1);
        gluLookAt(Ex,Ey,Ez ,0,0,0, 0,Cos(ph1),0);
    }
    else
    {
        //Set up first person projection
        double Cx = -2*dim1*Sin(th1)*Cos(ph1) + xOffset;
        double Cy = 2*dim1*Sin(ph1) + yOffset;
        double Cz = 2*dim1*Cos(th1)*Cos(ph1) + zOffset;
        gluLookAt(xOffset,yOffset,zOffset ,Cx,Cy,Cz, 0,Cos(ph1),0);
    }

    //  Light switch
    if (light1)
    {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.1,0.1,0.1,1.0};
      float Diffuse[]   = {0.3,0.3,0.3,1};
      float Specular[]  = {0.2,0.2,0.2,1};
      float white[]     = {1,1,1,1};
      //  Light direction
      float Position[6][4]  = {
        {68*Cos(idleArry[0])-38,30,20 , 1},
        {68*Cos(idleArry[1])-38,31,40 , 1},
        {68*Cos(idleArry[2])-38,32,60 , 1},
        {68*Cos(idleArry[3])-38,33,80 , 1},
        {68*Cos(idleArry[4])-38,34,100 , 1},
        {bowling_ball_x(4),bowling_ball_y(4),bowling_ball_z[4] , 1},
      };
      ball1(Position[0][0],Position[0][1],Position[0][2],1,white);

      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Enable lighting with normalization
      glEnable(GL_NORMALIZE);

      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local1);

      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0

      //  Set ambient, diffuse, specular components and position of light 0
      glEnable(GL_LIGHT0);
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position[0]);
      glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,32.0f);
      glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    }
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    wall(33.5,0,-30,10,12,10,0,180,wall_texture);
    wall(33.5,0,0,10,12,10,0,180,wall_texture);
    wall(33.5,0,30,10,12,10,0,180,wall_texture);
    wall(33.5,0,60,10,12,10,0,180,wall_texture);
    wall(33.5,0,90,10,12,10,0,180,wall_texture);
    wall(33.5,0,120,10,12,10,0,180,wall_texture);
    mural(-2,0,-10, 12,12,10 , 0, 0, mural_texture[0], light1);
    mural(-38,0,-10, 12,12,10 , 0, 0, mural_texture[1], light1);
    mural(-74,0,-10, 12,12,10 , 0, 0, mural_texture[2], light1);
    mural(-110,0,-10, 12,12,10 , 0, 0, mural_texture[3], light1);
    for(i = 0 ; i < 8 ; i++)
    {
      fall = explosion[i] > 180 ? (double)explosion[i]/180.0 : 0;
      fall = fall*fall;
      pins(pin_x[i],reset[i] - fall,0,1,1,1,0,explosion[i]);
    }
    double_lane(0,0,0,1,1,1,0,0,floor_texture, cieling_texture, duct_texture);
    double_lane(-36,0,0,1,1,1,0,0,floor_texture, cieling_texture, duct_texture);
    double_lane(-72,0,0,1,1,1,0,0,floor_texture, cieling_texture, duct_texture);
    double_lane(-108,0,0,1,1,1,0,0,floor_texture, cieling_texture, duct_texture);
    wall(-110.5,0,-60,10,12,10,0,0,wall_texture);
    wall(-110.5,0,-30,10,12,10,0,0,wall_texture);
    wall(-110.5,0,0,10,12,10,0,0,wall_texture);
    wall(-110.5,0,30,10,12,10,0,0,wall_texture);
    wall(-110.5,0,60,10,12,10,0,0,wall_texture);
    wall(-110.5,0,90,10,12,10,0,0,wall_texture);
    for(i = 0 ; i < 8 ; i++)
    {
      bowling_ball(bowling_ball_x(i),bowling_ball_y(i),bowling_ball_z[i],1,1,1,ball_ph[i],ball_texture, colors[i]);
    }

    //enable textures


    //  disbale lighting from here on
    if (light1)
    {
      glDisable(GL_LIGHTING);
    }
    //  Draw axes
    glColor3f(1,1,1);
    if (axes1)
    {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
    }

      glWindowPos2i(5,5);
      if(mode1 == 0)
      {
          Print("Projection Type: Perspective");
          glWindowPos2i(5,25);
          Print("Dimension: %.0f", dim1);
      }
      else
      {
          Print("Projection Type: First Person");
          glWindowPos2i(5,25);
          Print("User Location: { %.3f, %.3f, %.3f }", xOffset, yOffset, zOffset);
      }
	
}
else {
	if(SEIZE_RESIZE_WINDOW){
		glutReshapeWindow(1200,800);
	}
	  generalKey();
	specialKey();

	//  Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		
	glClearColor(0.6, 0.75, 0.85, 1); // sky color is light blue

	
	if(SCREEN == gameCurrentView1)
	{
		displayFirstPage();
	}

	if(SCREEN == gameCurrentView2)
	{
		displayEnvironmentMenu();
	}

	else if(SCREEN == gameCurrentView3)
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		//  Enable Z-buffering in OpenGL
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		//  Undo previous transformations
		glLoadIdentity();


		gluLookAt(Ex,Ey,Ez,Ex,Ey,Ez+lz,0,1,0);


		glShadeModel(GL_SMOOTH);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		glPushMatrix();
		glTranslatef(-Ex,0, Ez);
		drawSkyBox(1000); 
		glPopMatrix();
		
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);

		//  Light switch
		if (light)
		{
		//  Translate intensity to color vectors
			float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
			float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
			float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
			//  Light position
			float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
			//  Draw light position as ball (still no lighting here)
			glColor3f(1,1,1);
			ball(Position[0],Position[1],Position[2] , 0.1);
			//  OpenGL should normalize normal vectors
			glEnable(GL_NORMALIZE);
			//  Enable lighting
			glEnable(GL_LIGHTING);
			//  Location of viewer for specular calculations
			glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
			//  glColor sets ambient and diffuse color materials
			glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
			glEnable(GL_COLOR_MATERIAL);
			//  Enable light 0
			glEnable(GL_LIGHT0);
			//  Set ambient, diffuse, specular components and position of light 0
			glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
			glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
			glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
			glLightfv(GL_LIGHT0,GL_POSITION,Position);
		}
		else
			glDisable(GL_LIGHTING);

		if(carVal == MERCEDES_BENZ)
		{
		 glPushMatrix();
         glTranslatef(	Ex,-10, Ez+lz-25);
         glRotatef(90, 1, 0, 0);
         glRotatef(180, 0, 1, 0);
         glColor3f(1,1,1);
         glScalef(5,5,5);
		 glCallList(mercedes);
		 glPopMatrix();
		}

		for(i=0; i<=1000; i+=250)
		{
			glPushMatrix();
			glTranslated(-600,-15,50+i);
			glScaled(10,15,10);
			drawBuilding();
			glPopMatrix();
		}

		for(i=0; i<=1000; i+=250)
		{
			glPushMatrix();
			glTranslated(+1200,-15,50-i);
			glScaled(50,55,50);
			drawHouse(+1000,-15,50+i ,65,65,65,0,23);
			glPopMatrix();
		}
		

		drawRoad();
		drawRoad2();
	
		for(i=0; i<=1000; i+=400)
		{
			glPushMatrix();
			glTranslated(-74,-10,i);
			glScaled(10,10,10);
			drawTrafficSignal();
			glPopMatrix();
		}

		for(i=0; i<=1000; i+=400)
		{
			glPolygonOffset(-1,-1);
			glPushMatrix();
			glTranslated(0,-1.4,i+40);
			drawPedestrianCrossing();
			glPopMatrix();
		}
		if(environmentCar ==1){
		// Draw Treees 
		for(i=0; i<=1000; i+=40)
		{
			glPushMatrix();
			glTranslated(-50,10,i);
			glScaled(25,25,25);
			if(!(i%3))
			drawTree(13);
			else
			drawTree(0);
			glPopMatrix();
		}

		for(i=0; i<=1000; i+=40)
		{
			glPushMatrix();
			glTranslated(90, 5,i);
			glScaled(25,25,25);
			if(!(i%3))
			drawTree(13);
			else
			drawTree(0);	
			glPopMatrix();
		}
		
		}
		for(i=0; i<=1000; i+=400)
		{
			glPushMatrix();
			glTranslated(68,0,i);
			glScaled(10,10,10);
			drawRoadSign();
			glPopMatrix();
		}

		// Which elements
		  if (falli == CLIMATE_RAIN) {
		    drawCLIMATE_RAIN();
		  }else if (falli == CLIMATE_HAIL) {
		    drawCLIMATE_HAIL();
		  }
		for (i = 0; i<1000; i+=153)
		{

			if((i%2)==0)
			{
				glPushMatrix();
				glTranslated(-16,-10,i);
				glScaled(8,5,4);
				drawTrafficBlocks();
				glPopMatrix();
			}
			else
			{
				glPushMatrix();
				glTranslated(16,-10,i);
				glScaled(7,8,6);
				drawCone();
				glPopMatrix();
			}

		glPushMatrix();
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST); 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //so that bounding circle is transparent
		glColor4f(1,1,1,0);
		glTranslated(-16,-8,i);
		glutSolidSphere(10, 95, 95); //bounding circle for collision detection
		glPopMatrix();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		
		}

		variableC = 0;

		while(variableC < 1000)
		{

			for(i=0;i<7;i++)
			{
				if(variableC%2==0)
					x_delta=Ex+16;
				else
					x_delta=Ex-16;

			y_delta= -5;
			z_delta=((Ez+lz-21)-variableC);
			DistanceC [i]= sqrt(x_delta*x_delta + y_delta*y_delta + z_delta*z_delta); 
			variableC +=153;
			}
		}

	}

	else if(SCREEN == gameCurrentView4)
	{
		displayCrash();		
	}


   //  Render the scene and make it visible
   ErrCheck("display");
   
	}
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this function for continuous animation
 */
void idle()
{
  if(BOWLING_GAME_FLAG){
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    int change = 2.0;
    int lane;
    idleArry[0] = fmod(90*t,360);
    idleArry[1] = fmod(45*t,360);
    idleArry[2] = fmod(30*t,360);
    idleArry[3] = fmod(22.5*t,360);
    idleArry[4] = fmod(18*t,360);
    idleArry[5] = fmod(15*t,360);
    //If rolling
    if(roll < roll_range)
    {
      roll += 1*change;
      ph1 -= (((6.0*change)*360) / roll_range) ;
      ph1 %= 360;
      zOffset += ((change*120.0) / roll_range);
      yOffset = 3 - 1.5*Sin(ph1);
      explosion[3] += zOffset > 105.0 ? 1.0 : 0.0 ;
      if(zOffset >= 120.0)
      {
        ph1 = 0;
        zOffset = 0;
        yOffset = 3;
        xOffset = -31.0;
        roll = roll_range;
      }
    }
    for(lane = 0; lane < 8 ; lane++)
    {
      explode_and_reset(lane);
      roll_dat_ball(lane);
    }
    glutPostRedisplay();
  }
  else{ //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
     	//printf("In Idle\n");
   //  Tell GLUT it is necessary to redisplay the scene
   varCarRotate = 0.1*glutGet(GLUT_ELAPSED_TIME);
   glutPostRedisplay();
  }
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
if(BOWLING_GAME_FLAG){
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
    th1 += 5;
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
    th1 -= 5;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
    ph1 += 5;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
    ph1 -= 5;
    //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_UP)
    dim1 += 0.1;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_DOWN && dim1>1)
    dim1 -= 0.1;
    //  Keep angles to +/-360 degrees
    th1 %= 360;
    ph1 %= 360;
    //  Update projection
    Project(fieldOfView1,aspect1,dim1);
}
else {
		speedState[key]=true;

	if(key == GLUT_KEY_PAGE_UP)
		Ey += 0.5;

	else if (key == GLUT_KEY_PAGE_DOWN && Ey > 1)
		Ey -= 0.5;

	//  Update projection
	Project(fieldOfView,aspect,dim);
	
	}
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is Released
 */
void specialUp(int key,int x,int y)
{
   if(!BOWLING_GAME_FLAG){
	speedState[key]=false;

	if(key == GLUT_KEY_PAGE_UP)
		Ey += 0.5;

	else if (key == GLUT_KEY_PAGE_DOWN && Ey > 1)
		Ey -= 0.5;

	//  Update projection
	Project(fieldOfView,aspect,dim);
	//  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();}
}

/*
 *  GLUT calls this routine when a key is Released
 */

void keyUp(unsigned char ch,int x,int y)
{if(!BOWLING_GAME_FLAG){
	speedState[ch]=false;
	//  Reproject
	Project(fieldOfView,aspect,dim);
	//  Animate if requested
	glutIdleFunc(move?idle:NULL);
	//  Tell GLUT it is necessary to redisplay the scene
glutPostRedisplay();}
}


/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
	if(ch == '@')
	{
		BOWLING_GAME_FLAG^=1;
	}
	if(ch == '%')
 	{
		SEIZE_RESIZE_WINDOW^=1;
	}
	if(BOWLING_GAME_FLAG)
	{
    //  Exit on ESC
    if (ch == 27)
    exit(0);
    //  Reset view angle
    else if (ch == '0')
    {
      th1 = ph1 = 0;
      xOffset = 0;
      yOffset = 2.0;
      zOffset = -60.0;
    }
    else if (ch == '1')
    {
      gobowl(0);
    }
    else if (ch == '2')
    {
      gobowl(1);
    }
    else if (ch == '3')
    {
      gobowl(2);
    }
    else if (ch == '4')
    {
      gobowl(3);
    }
    else if (ch == '5')
    {
      gobowl(4);
    }
    else if (ch == '6')
    {
      gobowl(5);
    }
    else if (ch == '7')
    {
      gobowl(6);
    }
    else if (ch == '8')
    {
      gobowl(7);
    }
    //  Toggle axes
    else if (ch == 'x' || ch == 'X')
    axes1 = 1-axes1;
    //  Toggle lighting
    else if (ch == 'l' || ch == 'L')
    light1 = 1-light1;
    //  Switch projection mode
    else if (ch == 'p' || ch == 'P')
    {
mode1 ^=1;
 xOffset = 0;
 yOffset = 2.0;
zOffset = -60.0;
    }
    //  Change field of view angle
    else if (ch == '-' && ch>1)
    fieldOfView1--;
    else if (ch == '+' && ch<179)
    fieldOfView1++;
    else if (mode1 != 0 && ch == 'r')
    {
      roll = 0;
      xOffset = -31.0;
      yOffset = 4.0;
      zOffset = 0.0;
      th1 = 0;
      ph1  = 180;
    }
    //FIRST PERSON NAVIGATION WITH WASD
    else if(ch == 'w' || ch == 'W')
    {
        xOffset -= 2*Sin(th1);
        zOffset += 2*Cos(th1);
        checkOffsets();
    }
    else if(ch == 's' || ch == 'S')
    {
        xOffset += 2*Sin(th1);
        zOffset -= 2*Cos(th1);
        checkOffsets();
    }
    else if(ch == 'a' || ch == 'A')
    {
        xOffset -= 2*Sin(th1-90);
        zOffset += 2*Cos(th1-90);
        checkOffsets();
    }
    else if(ch == 'd' || ch == 'D')
    {
        xOffset += 2*Sin(th1-90);
        zOffset -= 2*Cos(th1-90);
        checkOffsets();
    }
    else if(ch == '[')
    {
        yOffset += 1;
        checkOffsets();
    }
    else if(ch == ']')
    {
        yOffset -= 1;
        checkOffsets();
    }

    //  Reproject
    Project(fieldOfView1,aspect1,dim1);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
  }else{
   
	
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1 && SCREEN == gameCurrentView3)
      fieldOfView--;
   else if (ch == '+' && ch<179 && SCREEN == gameCurrentView3)
      fieldOfView++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   
   // Car Game Controls 
  	else if (ch == ' ')
  		SCREEN = gameCurrentView2;
  	else if (ch == '1')
  		environmentCar = 1;
  	else if (ch == '2')
  		environmentCar = 2;
  	else if (ch == 13 && SCREEN == gameCurrentView2)
  		SCREEN = gameCurrentView3;
  	else if ((ch == 'r' || ch == 'R') && ((SCREEN == gameCurrentView4) || (SCREEN == gameCurrentView3)) )
  	{
		SCREEN = gameCurrentView2;
		Ex = 0.0;
		Ey = 1.0;
		Ez=1000; 
		carVal = 2;
		environmentCar = 1; 
		FlagC = 0;
  	}

   //weather control 
  else  if (ch == '!') { // CLIMATE_RAIN
    falli = CLIMATE_RAIN;
    glutPostRedisplay();
  }
  else if (ch == 'h') { // CLIMATE_HAIL
    falli = CLIMATE_HAIL;
    glutPostRedisplay();
  }
  else if (ch =='#')
  {
    falli =0;
  }
  else if (ch == '=') { //really '+' - make CLIMATE_HAIL bigger
    hailsize += 0.01;
  }
  else if (ch == '/') { // make hail smaller
    if (hailsize > 0.1) hailsize -= 0.01;
  }
  else if (ch == ',') { // really '<' slow down
    if (decreaseSpeed > 4.0) decreaseSpeed += 0.01;
  }
  else if (ch == '.') { // really '>' speed up
    if (decreaseSpeed > 1.0) decreaseSpeed -= 0.01;
  }

   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(fieldOfView,aspect,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   if(BOWLING_GAME_FLAG)
   {	   
    window_width = width;
    window_height = height;
    //  Ratio of the width to the height of the window
    aspect1 = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    //  Set projection
    Project(fieldOfView1,aspect1,dim1);
   } else{
   //  Ratio of the width to the height of the window
   aspect = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
  Project(fieldOfView,aspect,dim);
	}
}



/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
	//  Initialize GLUT
	glutInit(&argc,argv);
	//  Request double buffered, true color window with Z buffering at 600x600
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);   
	if(BOWLING_GAME_FLAG)
   	 {
		    glutInitWindowSize(window_width,window_height);
	 }else 
	glutInitWindowSize(1200,800);
	glutCreateWindow("Final Project : Shekhar");
	SCREEN = gameCurrentView1;
	init(); // initialize the weather conditions
	//  Set callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyUp);
    glutMouseFunc(mouse);
    glutMotionFunc(motionmouse);
	glutIdleFunc(idle);
	
	// load textures for bowing game 
    floor_texture = LoadTexBMP("floor.bmp");
    arrow_texture = LoadTexBMP("arrows.bmp");
    mural_texture[0] = LoadTexBMP("1.bmp");
    mural_texture[1] = LoadTexBMP("2.bmp");
    mural_texture[2] = LoadTexBMP("3.bmp");
    mural_texture[3] = LoadTexBMP("4.bmp");
    wall_texture = LoadTexBMP("wall3.bmp");
    ball_texture = LoadTexBMP("ball.bmp");
    cieling_texture = LoadTexBMP("cieling.bmp");
    duct_texture = LoadTexBMP("duct.bmp");
	
	// load textures for car game 
	mercedes = LoadOBJ("sls_amg.obj");
	texture_2D[0] = LoadTexBMP("leaves.bmp");
	texture_2D[1] = LoadTexBMP("barkTexture.bmp");
	texture_2D[2] = LoadTexBMP("far_city_mountains.bmp");
	texture_2D[3] = LoadTexBMP("cloudTexture.bmp");
	texture_2D[4] = LoadTexBMP("grass.bmp");
	texture_2D[5] = LoadTexBMP("s.bmp");
	texture_2D[6] = LoadTexBMP("PoleTexture.bmp");
	texture_2D[7] = LoadTexBMP("pedestrianCrossingTexture.bmp");
	texture_2D[8] = LoadTexBMP("orange_white.bmp");
	texture_2D[9] = LoadTexBMP("trafficPole1.bmp");
	texture_2D[10] = LoadTexBMP("building.bmp");
	texture_2D[11] = LoadTexBMP("car_selection_bkg.bmp");
	texture_2D[12] = LoadTexBMP("car_crash1.bmp");
	texture_2D[13] = LoadTexBMP("autum2.bmp");
	texture_2D[14] = LoadTexBMP("desertTexture.bmp");
	texture_2D[15] = LoadTexBMP("desert.bmp");
    texture_2D[16] = LoadTexBMP("Games_Baground.bmp");
        
	texture_2D[17] = LoadTexBMP("chimney.bmp");
    texture_2D[18] = LoadTexBMP("door1.bmp");
	texture_2D[19] = LoadTexBMP("wall1.bmp");
	texture_2D[20] = LoadTexBMP("rf2.bmp");
	texture_2D[21] = LoadTexBMP("wall2.bmp");
	texture_2D[22] = LoadTexBMP("rf2.bmp");
	texture_2D[23] = LoadTexBMP("wall3.bmp");
	texture_2D[24] = LoadTexBMP("rf3.bmp");
    texture_2D[25] = LoadTexBMP("low1.bmp");
       
	
	//  Pass control to GLUT so it can interact with the user
	ErrCheck("init");
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}

