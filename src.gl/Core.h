#pragma once
////////////////////////////////////////////////////////////////////////////////
#define _USE_MATH_DEFINES
#include <iostream> 
#include <vector> 
#include <string> 
#include <stdio.h>
#include <glew.h>
#include <wglew.h>
#include <GL/glut.h>// Header File For The GLUT Library 
#include <time.h>
#include <mmsystem.h>
#include <windows.h>
using namespace std;
#include "VecMath.h"
#include "glsl.h"
///////////////////////////////////////////
#include "Bmp.h"
///////////////////////////////////////////
extern int SCREEN_SIZE_X;
extern int SCREEN_SIZE_Y; 
////////////////////////////////////////////////////////////////////////////////
#define _USE_MATH_DEFINES
#define loopi(start_l,end_l,step_l) for ( int i=start_l;i<end_l;i+=step_l )
#define loopi(start_l,end_l) for ( int i=start_l;i<end_l;++i )
#define loopj(start_l,end_l,step_l) for ( int j=start_l;j<end_l;j+=step_l )
#define loopj(start_l,end_l) for ( int j=start_l;j<end_l;++j )
#define loopk(start_l,end_l,step_l) for ( int k=start_l;k<end_l;k+=step_l )
#define loopk(start_l,end_l) for ( int k=start_l;k<end_l;++k )
#define loopm(start_l,end_l) for ( int m=start_l;m<end_l;++m )
#define loopl(start_l,end_l) for ( int l=start_l;l<end_l;++l )
#define loopn(start_l,end_l) for ( int n=start_l;n<end_l;++n )
#define loop(var_l,start_l,end_l) for ( int var_l=start_l;var_l<end_l;++var_l )
#define loops(a_l,start_l,end_l,step_l) for ( a_l = start_l;a_l<end_l;a_l+=step_l )

/*
#ifndef byte
#define byte unsigned char
#endif
*/
#ifndef ushort
#define ushort unsigned short
#endif

#ifndef uint
#define uint unsigned int
#endif

#ifndef uchar
#define uchar unsigned char
#endif

////////////////////////////////////////////////////////////////////////////////
class Keyboard
{
	public:

	bool  key [256]; // actual
	bool  key2[256]; // before

	Keyboard(){ int a; loop(a,0,256) key[a] = key2[a]=0; }

	bool KeyDn(char a)//key down
	{
		return key[a];
	}
	bool KeyPr(char a)//pressed
	{
		return ((!key2[a]) && key[a] );
	}
	bool KeyUp(char a)//released
	{
		return ((!key[a]) && key2[a] );
	}
	void update()
	{
		int a;loop( a,0,256 ) key2[a] = key[a];
	}
};
////////////////////////////////////////////////////////////////////////////////
class Mouse
{
	public:

	bool  button[256];
	bool  button2[256];
	float mouseX,mouseY;
	float mouseX2,mouseY2;
	float mouseDX,mouseDY;

	Mouse()
	{ 
		int a; loop(a,0,256) button[a] = button2[a]=0; 
		mouseX=mouseY=mouseDX=mouseDY= 0;
	}
	void update()
	{
		mouseDX=mouseX-mouseX2;mouseX2=mouseX;
		mouseDY=mouseY-mouseY2;mouseY2=mouseY;
		int a;loop( a,0,256 ) button2[a] = button[a];
	}
};
////////////////////////////////////////////////////////////////////////////////
class ScreenA
{
	public:
	ScreenA()
	{
		//pos -8.63 9.91 63.02 rot 30.27 -12.89 0.00 lookat  -8.51 9.97 62.81
		pos=vec3f(8.63*4, -9.91*4, -63.02*4);
		rot=vec3f(30.27, -12.89, 0.00);
	};

	float screentime;
	int	 window_width;
	int	 window_height;
	int	 window2_width;
	int	 window2_height;
	int	 window3_width;
	int	 window3_height;
	bool fullscreen;

	matrix44 camera;

	vec3f pos,rot,lookat;
};
////////////////////////////////////////////////////////////////////////////////
void CoreInit(void (GLUTCALLBACK *drawFunc)(void),int argc,char **argv);

double CoreCubicInterpolate(
   double y0,double y1,
   double y2,double y3,
   double mu);
char*  CoreReadFile(char* name);
GLuint CoreNewFloat16Tex(int width,int height,float* buffer,bool alpha);
GLuint CoreNewChar8Tex(int width,int height,uchar* buffer,bool alpha);
GLuint CoreNewFloat32Tex(int width,int height,float* buffer,bool alpha);
GLuint CoreTextureFromBmp(Bmp &bmp);
void CoreScreenshot(int counter,char* prefix=NULL);
void CoreDrawQuad();
void CoreKeyMouse();
////////////////////////////////////////////////////////////////////////////////
extern Keyboard		keyboard;
extern Mouse		mouse;
extern ScreenA		screen;
////////////////////////////////////////////////////////////////////////////////
extern int handle_window1;
extern int handle_window2;
extern int handle_window3;
////////////////////////////////////////////////////////////////////////////////
