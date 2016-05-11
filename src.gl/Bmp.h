///////////////////////////////////////////
#pragma once
///////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "VecMath.h"
///////////////////////////////////////////
class Bmp
{
public:

	Bmp();
	Bmp(int x,int y,int bpp,unsigned char*data=0);
	Bmp(const char*filename);
	~Bmp();

	void load(const char *filename);
	bool save(const char*filename);

	vec3f getSxSyT(float x);
	int  sampleByte(int x,int y);
	bool set(int x,int y,int bpp,unsigned char*data);
	bool set3d(int x,int y,int z,int bpp,unsigned char*buffer);
	void crop(int x,int y);
	bool scale(int x,int y);
	bool blur(int count);
	bool hblur(int count);
	bool vblur(int count);
	bool addalpha(unsigned char r,unsigned char g,unsigned char b);
	bool normalize(void);
	bool normalMap(void);
	vec3f get_pixel(float x,float y);
	vec3f get_f_fdx_fdy(float x,float y);
	void make_border(int border,int r,int g,int b)
	{
		int s=width*height;
		for (int i=0;i<s;i++)
		{
			int cr=data[i*3+2];
			int cg=data[i*3+1];
			int cb=data[i*3+0];
			if((cr==r)&&(cg==g)&&(cb==b))
			{
				cr=0;
			}
			else
			{
				cr=1;
			}

			data[i*3+0]=cr;
			data[i*3+1]=cr;
			data[i*3+2]=cr;
		}

		//int level=1;
		for(int level=1;level<border;level++)
		{
			for (int j=1;j<height-1;j++)
			for (int i=1;i<width-1;i++)
			{
				int found=0;
				
				for (int ii=-1;ii<=1;ii++)
					if (data[(i+ii+j*width)*3]>=level)found++;
				for (int jj=-1;jj<=1;jj++)
					if (data[(i+(j+jj)*width)*3]>=level)found++;
					
				/*
				for (int ii=-1;ii<=1;ii++)
				for (int jj=-1;jj<=1;jj++)
					if (data[(i+ii+(j+jj)*width)*3]>=level)found++;
					*/

				if(found==6)data[(i+j*width)*3]=level+1;
			}
			//level++;
		}

		for (int i=0;i<s;i++)
		{
			int cb=data[i*3+0]-0;if(cb<0)cb=0;
			cb=cb*255/(border-0);
			data[i*3+0]=cb;
			data[i*3+1]=cb;
			data[i*3+2]=cb;
		}
	}

private:

	int  sampleMap(int i,int j);

public:

	unsigned char*data;
	int width;
	int height;
	int depth;
	int bpp;

private:

	unsigned char bmp[54];
};

