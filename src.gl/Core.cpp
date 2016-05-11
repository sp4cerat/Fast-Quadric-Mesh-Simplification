////////////////////////////////////////////////////////////////////////////////
#include "Core.h"
////////////////////////////////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////////////////////////////////
Keyboard	keyboard;
Mouse		mouse;
ScreenA		screen;
////////////////////////////////////////////////////////////////////////////////void keyDown1Static(int key, int x, int y)           { KeyPressed(key, x, y,true); }
int handle_window1;
int handle_window2;
int handle_window3;
////////////////////////////////////////////////////////////////////////////////void keyDown1Static(int key, int x, int y)           { KeyPressed(key, x, y,true); }
void CorekeyDown1Static(int key, int x, int y)           { keyboard.key[ key&255 ] =true;  }
void CorekeyDown2Static(unsigned char key, int x, int y) { keyboard.key[ key&255 ] =true;  }
void CorekeyUp1Static(int key, int x, int y)             { keyboard.key[ key&255 ] =false; }
void CorekeyUp2Static(unsigned char key, int x, int y)   { keyboard.key[ key&255 ] =false; }
////////////////////////////////////////////////////////////////////////////////
void CoreMouseMotionStatic (int x,int y)
{
	mouse.mouseX = float(x)/float(screen.window_width);
	mouse.mouseY = float(y)/float(screen.window_height);
}
////////////////////////////////////////////////////////////////////////////////
void CoreMouseButtonStatic(int button_index, int state, int x, int y)
{
	mouse.button[button_index] =  ( state == GLUT_DOWN ) ? true : false;
	CoreMouseMotionStatic (x,y);
}
////////////////////////////////////////////////////////////////////////////////
void CoreToggleFullscreen()
{
	static int win_width=screen.window_width;
	static int win_height=screen.window_height;
	static bool fullscreen=false;

	if(fullscreen)
	{
		glutReshapeWindow(win_width,win_height);

		screen.window_width = win_width;
		screen.window_height = win_height;
	}
	else
	{
		win_width=screen.window_width;
		win_height=screen.window_height;

		glutFullScreen() ;
	}
	fullscreen = (fullscreen) ? false : true;
}
////////////////////////////////////////////////////////////////////////////////
void CoreReshape(int w, int h)
{
    screen.window_width = w;
    screen.window_height = h;
	//printf("%d x %d\n",w,h);;
}
////////////////////////////////////////////////////////////////////////////////
void CoreReshape2(int w, int h)
{
    screen.window2_width = w;
    screen.window2_height = h;
	//printf("%d x %d\n",w,h);;
}
////////////////////////////////////////////////////////////////////////////////
void CoreReshape3(int w, int h)
{
    screen.window3_width = w;
    screen.window3_height = h;
	//printf("%d x %d\n",w,h);;
}
////////////////////////////////////////////////////////////////////////////////
extern int SCREEN_SIZE_X;
extern int SCREEN_SIZE_Y;
void CoreInit(void (GLUTCALLBACK *drawFunc)(void),int argc,char **argv)
{	
	glutInit(&argc, (char**)argv);  
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
	glutInitWindowSize(SCREEN_SIZE_X, SCREEN_SIZE_Y);  
	glutInitWindowPosition(0, 0);  
	glutCreateWindow("Title");
	glutDisplayFunc(drawFunc);
	glutIdleFunc(drawFunc);

	glutReshapeFunc(&CoreReshape);

	glutSpecialFunc(&CorekeyDown1Static);
	glutSpecialUpFunc(&CorekeyUp1Static);
	glutKeyboardFunc(&CorekeyDown2Static);
	glutKeyboardUpFunc(&CorekeyUp2Static);

	glutMotionFunc(&CoreMouseMotionStatic);
	glutPassiveMotionFunc(&CoreMouseMotionStatic);
	glutMouseFunc (&CoreMouseButtonStatic);

	glewInit();

	std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;			//std::cout << "GL_EXTENSIONS: " << glGetString(GL_EXTENSIONS) << std::endl;
	std::cout << "GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "GL_VENDOR: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GLU_VERSION: " << gluGetString(GLU_VERSION) << std::endl;			//std::cout << "GLU_EXTENSIONS: " << gluGetString(GLU_EXTENSIONS) << std::endl;
	std::cout << "GLUT_API_VERSION: " << GLUT_API_VERSION << std::endl;

	int res[16];

	/*
	#ifndef MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS
	#define MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS 0x8E81
	#endif 
	#ifndef MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS
	#define MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS 0x8E82
	#endif 

	glGetIntegerv(MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS,res);
	printf("MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS %d\n",res[0]);
	glGetIntegerv(MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS,res);
	printf("MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS %d\n",res[0]);
	*/

	wglSwapIntervalEXT(1);
	//glClampColorARB(GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE);CHECK_GL_ERROR();
	//glClampColorARB(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE);CHECK_GL_ERROR();
	//glClampColorARB(GL_CLAMP_READ_COLOR_ARB, GL_FALSE);CHECK_GL_ERROR();

	glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LEQUAL);				// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
	glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading
	glDisable(GL_CULL_FACE);
	glCullFace (GL_FRONT);
}
////////////////////////////////////////////////////////////////////////////////
int CheckGLError(char *file, int line)
{
	//return 0;
	GLenum glErr,glErr2;
	int retCode = 0;

	glErr = glErr2 = glGetError();
	while (glErr != GL_NO_ERROR) 
	{
	   char* str1 = (char*)gluErrorString(glErr);
	   if (str1)
			cout << "GL Error #" << glErr << "(" << str1 << ") " << " in File " << file << " at line: " << line << endl;
	   else
			cout << "GL Error #" << glErr << " in File " << file << " at line: " << line << endl;
		retCode = 1;
		glErr = glGetError();
	}
	if (glErr2 != GL_NO_ERROR) while(1)Sleep(100);;

	return 0;
}
////////////////////////////////////////////////////////////////////////////////
void CoreScreenshot(int counter,char* prefix)
{
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);

        int width=viewport[2];
        int height=viewport[3];

        char* data = (char*)malloc(width*height*3);

        glReadPixels(0, 0, width,height, GL_BGR_EXT , GL_UNSIGNED_BYTE, data);

        unsigned char bmp[58]=
                {	0x42,0x4D,0x36,0x30,0,0,0,0,0,0,0x36,0,0,0,0x28,0,0,0,
					0x40,0,0,0, // X-Size
					0x40,0,0,0, // Y-Size
					1,0,0x18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

        bmp[18]        =width;
        bmp[19]        =width>>8;
        bmp[22]        =height;
        bmp[23]        =height>>8;
        bmp[28]        =24; // bpp

		//static int counter=0; counter++;
		char filename[1000];
		if(prefix)
			sprintf(filename,"../output/%s%05d.bmp",prefix,counter);
		else
			sprintf(filename,"../output/%05d.bmp",counter);

        FILE* fn;
        if ((fn = fopen (filename,"wb")) != NULL)
        {
                fwrite(bmp ,1,54   ,fn);
                fwrite(data,1,width*height*3,fn);
                fclose(fn);
        }
}
////////////////////////////////////////////////////////////////////////////////
void CoreScreenshot()
{
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);

        int width=viewport[2];
        int height=viewport[3];

        char* data = (char*)malloc(width*height*3);

        glReadPixels(0, 0, width,height, GL_BGR_EXT , GL_UNSIGNED_BYTE, data);

        unsigned char bmp[58]=
                {	0x42,0x4D,0x36,0x30,0,0,0,0,0,0,0x36,0,0,0,0x28,0,0,0,
					0x40,0,0,0, // X-Size
					0x40,0,0,0, // Y-Size
					1,0,0x18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

        bmp[18]        =width;
        bmp[19]        =width>>8;
        bmp[22]        =height;
        bmp[23]        =height>>8;
        bmp[28]        =24; // bpp

		static int counter=0; counter++;
		char filename[1000];
		sprintf(filename,"../screenshots/%04d.bmp",counter);
		printf("Saved Screenshot %s\n",filename);

        FILE* fn;
        if ((fn = fopen (filename,"wb")) != NULL)
        {
                fwrite(bmp ,1,54   ,fn);
                fwrite(data,1,width*height*3,fn);
                fclose(fn);
        }
}
float fps=1;
////////////////////////////////////////////////////////////////////////////////
void CoreKeyMouse()
{
	static int time = timeGetTime (); int timeNew = timeGetTime ();
	static float deltaTime = 1.0; 
	deltaTime=deltaTime*0.9+0.1*double(int(timeNew-time)); time=timeNew;

	fps=1000.0/deltaTime;

	bool* key = keyboard.key;

	screen.screentime=deltaTime*0.01;
	
	vec3f movX = vec3f ( 1,0,0 );
	vec3f movY = vec3f ( 0,1,0 );
	vec3f movZ = vec3f ( 0,0,1 );
	movZ.rot_x (   screen.rot.y * 2*M_PI / 360 );
	movZ.rot_y (   screen.rot.x * 2*M_PI / 360 );
	movX.rot_x (   screen.rot.y * 2*M_PI / 360 );
	movX.rot_y (   screen.rot.x * 2*M_PI / 360 );
	if ( key[27] ) exit(0); // throw "MY_EXIT_MESSAGE"; //exit(0); // ESC Key
	if ( key[GLUT_KEY_UP   ] ) screen.pos=screen.pos+movZ * (0.07 * deltaTime);
	if ( key[GLUT_KEY_DOWN ] ) screen.pos=screen.pos-movZ * (0.07 * deltaTime);
	if ( key[GLUT_KEY_LEFT ] ) screen.pos=screen.pos+movX * (0.07 * deltaTime);
	if ( key[GLUT_KEY_RIGHT] ) screen.pos=screen.pos-movX * (0.07 * deltaTime);
	if ( keyboard.KeyPr('s') ) CoreScreenshot();
	if ( keyboard.KeyPr('m') ) CoreToggleFullscreen();

	screen.lookat=screen.pos+movZ;

	//printf("p %f %f %f\n", pos.x, pos.y, pos.z );

	//if ( key[GLUT_KEY_PAGE_UP   ] ) screen.rot.y-=0.1 * deltaTime;
	//if ( key[GLUT_KEY_PAGE_DOWN ] ) screen.rot.y+=0.1 * deltaTime;

	if ( mouse.button[0] )
	{
		screen.rot.x += mouse.mouseDX * 300;
		screen.rot.y += mouse.mouseDY * 300;
	}

	keyboard.update();
	mouse.update();
}
////////////////////////////////////////////////////////////////////////////////
GLuint CoreNewFloat16Tex(int width,int height,float* buffer,bool alpha)
{
	GLuint type = GL_TEXTURE_2D; // GL_TEXTURE_RECTANGLE_ARB;
	GLuint tex;
	glGenTextures (1, &tex);
	glBindTexture(type,tex);

	// set texture parameters
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP);//_TO_BORDER
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP);//GL_REPEAT

	GLuint format1 = GL_RGB_FLOAT16_ATI;
	GLuint format0 = GL_RGB16F_ARB;
	GLuint format2 = GL_FLOAT_RGB16_NV;
	GLuint format3 = GL_RGB;

	if (alpha)
	{
		format1 = GL_RGBA_FLOAT16_ATI;
		format0 = GL_RGBA16F_ARB;
		format2 = GL_FLOAT_RGBA16_NV;
		format3 = GL_RGBA;
	}
	glTexImage2D(type,0,format0,width,height,0,format3,GL_FLOAT,buffer);		
	if (glGetError() != GL_NO_ERROR) 
	{
		printf("CoreNewFloatTex - GL_RGBA32F_ARB failed\n");
		glTexImage2D(type,0,format1,width,height,0,format3,GL_FLOAT,buffer);
		if (glGetError() != GL_NO_ERROR) 
		{
			printf("CoreNewFloatTex - GL_RGB_FLOAT32_ATI failed\n");
			glTexImage2D(type,0,format2,width,height,0,format3,GL_FLOAT,buffer);
		}
	}
	glBindTexture(type,0);

	return tex;
}
////////////////////////////////////////////////////////////////////////////////
GLuint CoreNewFloat32Tex(int width,int height,float* buffer,bool alpha)
{
	GLuint type = GL_TEXTURE_2D; // GL_TEXTURE_RECTANGLE_ARB;
	GLuint tex;
	glGenTextures (1, &tex);
	glBindTexture(type,tex);

	// set texture parameters
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP);//_TO_BORDER
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP);//GL_REPEAT

	GLuint format1 = GL_RGB_FLOAT32_ATI;
	GLuint format0 = GL_RGB32F_ARB;
	GLuint format2 = GL_FLOAT_RGB32_NV;
	GLuint format3 = GL_RGB;

	if (alpha)
	{
		format1 = GL_RGBA_FLOAT32_ATI;
		format0 = GL_RGBA32F_ARB;
		format2 = GL_FLOAT_RGBA32_NV;
		format3 = GL_RGBA;
	}
	glTexImage2D(type,0,format0,width,height,0,format3,GL_FLOAT,buffer);		
	if (glGetError() != GL_NO_ERROR) 
	{
		printf("CoreNewFloatTex - GL_RGBA32F_ARB failed\n");
		glTexImage2D(type,0,format1,width,height,0,format3,GL_FLOAT,buffer);
		if (glGetError() != GL_NO_ERROR) 
		{
			printf("CoreNewFloatTex - GL_RGB_FLOAT32_ATI failed\n");
			glTexImage2D(type,0,format2,width,height,0,format3,GL_FLOAT,buffer);
		}
	}
	glBindTexture(type,0);

	return tex;
}
////////////////////////////////////////////////////////////////////////////////
GLuint CoreNewChar8Tex(int width,int height,uchar* buffer,bool alpha)
{
	GLuint gl_handle;
	glGenTextures(1,(GLuint*)(&gl_handle));

	glBindTexture  (GL_TEXTURE_2D, gl_handle);
	glPixelStorei  (GL_UNPACK_ALIGNMENT, 4);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf      (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,   GL_MODULATE);

//	if(alpha)	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_BGRA_EXT,width, height,/*GL_RGBA*/GL_BGRA_EXT, GL_UNSIGNED_BYTE, buffer );
//	else		gluBuild2DMipmaps(GL_TEXTURE_2D,GL_BGR_EXT, width, height,/*GL_RGB*/ GL_BGR_EXT , GL_UNSIGNED_BYTE, buffer );

	if(alpha)	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,width, height,/*GL_RGBA*/GL_BGRA_EXT, GL_UNSIGNED_BYTE, buffer );
	else		gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB, width, height,/*GL_RGB*/ GL_BGR_EXT , GL_UNSIGNED_BYTE, buffer );

  return gl_handle;
}
////////////////////////////////////////////////////////////////////////////////
GLuint CoreTextureFromBmp(Bmp &bmp)
{
	if(bmp.bpp==24) return CoreNewChar8Tex(bmp.width,bmp.height,bmp.data,0);
	if(bmp.bpp==32) return CoreNewChar8Tex(bmp.width,bmp.height,bmp.data,1);
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
void CoreDrawQuad()
{
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_QUADS);
   
	glTexCoord2f(0,1);	glVertex3f(-1, 1,0);
	glTexCoord2f(1,1);	glVertex3f(1 , 1,0);
	glTexCoord2f(1,0); 	glVertex3f(1 ,-1,0);	 
	glTexCoord2f(0,0); 	glVertex3f(-1,-1,0);	 

	glEnd();
	glEnable(GL_DEPTH_TEST);
}
////////////////////////////////////////////////////////////////////////////////
char*  CoreReadFile(char* name)
{
	FILE * fp = fopen (name, "rb");
	if(fp==0){printf("%s not found error!\n",name);while(1);;}
	fseek(fp,0,SEEK_END);
	int size=ftell(fp);
	fseek(fp,0,SEEK_SET);
	char* mem=(char*)malloc(size+1);
	memset(mem,0,size+1);
	fread(mem,1,size,fp);
	fclose(fp);
	return mem;
}
////////////////////////////////////////////////////////////////////////////////
double CoreCubicInterpolate(
   double y0,double y1,
   double y2,double y3,
   double mu)
{
   double a0,a1,a2,a3,mu2;

   mu2 = mu*mu;
   a0 = y3 - y2 - y0 + y1;
   a1 = y0 - y1 - a0;
   a2 = y2 - y0;
   a3 = y1;

   return(a0*mu*mu2+a1*mu2+a2*mu+a3);
}
////////////////////////////////////////////////////////////////////////////////

