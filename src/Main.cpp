/////////////////////////////////////////////
//
// Mesh Simplification Tutorial
//
// (C) by Sven Forstmann in 2014
//
// License : MIT
// http://opensource.org/licenses/MIT
/////////////////////////////////////////////
// Mathlib included from 
// http://sourceforge.net/projects/nebuladevice/
///////////////////////////////////////////
int SCREEN_SIZE_X=1536;
int SCREEN_SIZE_Y=768;
/////////////////////////////////////////////
#include "Core.h"
#include "OBJ.h"
#include "Simplify.h"
///////////////////////////////////////////
//
// Mesh to Simplify
//
//OBJ obj("../data/bunny.obj",100);
OBJ obj("../data/wall.obj",1);
//OBJ obj("../data/sphere.obj",4.0/256.0);
///////////////////////////////////////////
extern float fps;
void DrawScene();
///////////////////////////////////////////
int main(int argc, char **argv) 
{ 
	// Setup Mesh for Simplification
	
	// Simplify::vertices[].p : vertex position
	// Simplify::triangles[].v[0..2] : vertex indices
	
	loopi(0,obj.objects[0].points.size())
	{
		Simplify::Vertex v;
		v.p=obj.objects[0].points[i];
		Simplify::vertices.push_back(v);
	}
	loopi(0,obj.objects[0].triangles.size())
	{
		Simplify::Triangle t;
		loopj(0,3)t.v[j]=obj.objects[0].triangles[i].points[j];
		Simplify::triangles.push_back(t);
	}
	printf("Input: %d triangles %d vertices\n",Simplify::triangles.size(),Simplify::vertices.size());
	
	// Simplification start
	
	Simplify::simplify_mesh(20000);
	Simplify::write_obj("../test_out.obj");
	
	// Simplification done;	
	// Visualize the Result
	
	printf("Output: %d triangles %d vertices\n",Simplify::triangles.size(),Simplify::vertices.size());
	
	loopi(0,Simplify::vertices.size())
	{
		obj.objects[0].points[i]=Simplify::vertices[i].p;
	}
	int ofs=0;
	loopi(0,Simplify::triangles.size())
	{
		if(!Simplify::triangles[i].deleted)
		{
			obj.objects[0].triangles[ofs] = obj.objects[0].triangles[i];
			obj.objects[0].triangles[ofs].material=0;
			loopj(0,3) obj.objects[0].triangles[ofs].points[j]=Simplify::triangles[i].v[j];
			ofs++;
		}
	}
	obj.objects[0].triangles.resize(ofs);
	
	CoreInit(DrawScene,argc, argv);
	try
	{
		glutMainLoop();
	}
	catch (const char* msg)
	{
		printf("exiting\n");
	}
}
///////////////////////////////////////////
void DrawScene()
{
	CoreKeyMouse();

	glViewport(0,0,screen.window_width,screen.window_height);
	glClearDepth(1.0f);
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)screen.window_width / (GLfloat) screen.window_height, 0.1, 10000.0);
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();
	
	// Position
	glRotatef(screen.rot.z, 0,0,1);
	glRotatef(screen.rot.y, 1,0,0);
	glRotatef(screen.rot.x, 0,1,0);
	glTranslatef(screen.pos.x,screen.pos.y,screen.pos.z);
		
	obj.draw(vec3f(0,0,0) , vec3f(0,0,0) );

	static int frame=0;frame++;
	if((frame&31)==0)
	{
		char txt[1000];int tcount=obj.objects[0].triangles.size();
		sprintf(txt,"Triangles:%d FPS: %f MT/S: %f",tcount,fps,float(tcount)*fps/1000000);
		glutSetWindowTitle(txt);
	}

	printf("pos %2.2f %2.2f %2.2f rot %2.2f %2.2f %2.2f \r",		
		screen.pos.x,screen.pos.y,screen.pos.z,
		screen.rot.x,screen.rot.y,screen.rot.z	);

	glDisable(GL_BLEND);

	glutSwapBuffers();
}
///////////////////////////////////////////
