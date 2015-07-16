#include "VecMath.h"

int vec3f::random_number = 3457734;

void vec3f::random_init()
{
	random_number = 3457734;
}

double vec3f::random_double(){
	static int t = random_number;
	t = (t * 2345633 + t*1245 + t*356 + t*35 + t/34 + t/325 - 8647445);
	random_number = t;
	
	return ( double (abs(t)%10000)/10000);
}




vec3f vec3f::random(){
	static vec3f rnd;
	rnd.x=random_double()*2-1;
	rnd.y=random_double()*2-1;
	rnd.z=random_double()*2-1;
	rnd.normalize();
	return rnd;
}

vec3f vec3f::normalize( vec3f a )
{ 
	double square = a.x*a.x + a.y*a.y + a.z*a.z;
	if (square <= 0.00001f ) 
	{
		a.x=1;a.y=0;a.z=0;
		return a; 
	}
	double len = 1.0f / (double)sqrt(square); 
	a.x*=len;a.y*=len;a.z*=len;
	return a; 
}


/*
bool lines_intersect ( vec3f line1[2] ,vec3f line2[2] , vec3f* intersection = NULL)
{
	// BBox

	if ( line1[0].x < line2[0].x )	if ( line1[0].x < line2[1].x )	if ( line1[1].x < line2[0].x )	if ( line1[1].x < line2[1].x ) return false;
	if ( line1[0].x > line2[0].x )	if ( line1[0].x > line2[1].x )	if ( line1[1].x > line2[0].x )	if ( line1[1].x > line2[1].x ) return false;
	if ( line1[0].y > line2[0].y )	if ( line1[0].y > line2[1].y )	if ( line1[1].y > line2[0].y )	if ( line1[1].y > line2[1].y ) return false;
	if ( line1[0].y < line2[0].y )	if ( line1[0].y < line2[1].y )	if ( line1[1].y < line2[0].y )	if ( line1[1].y < line2[1].y ) return false;

	// intersect

	
	//a x + b = c y + d
	//a x - c y = d - b

  //- a2 a1 x + a2 c1 y =-a2 d1 + a2 b1
//	a1 a2 x - a1 c2 y = a1 d2 - a1 b2

  //  y =  ( a1 d2 - a1 b2 - a2 d1 + a2 b1 ) / ( a2 c1 - a1 c2 )
	

	vec3f delta1 = line1[1] - line1[0];
	vec3f delta2 = line2[1] - line2[0];
	vec3f point1 = line1[0];
	vec3f point2 = line2[0];

	double div1 =  delta1.y*delta2.x - delta1.x*delta2.y;
	double div2 =  delta2.y*delta1.x - delta2.x*delta1.y;

	double a = 2;

	if ( div1 != 0)
	{
            a =  (	delta1.x*point2.y - 
					delta1.x*point1.y - 
					delta1.y*point2.x + 
					delta1.y*point1.x ) /  div1;
	}else
	if ( div2 != 0)
	{
            a =  (	delta2.x*point1.y - 
					delta2.x*point2.y - 
					delta2.y*point1.x + 
					delta2.y*point2.x ) /  div2;
	}					  
	if ( a <= 1 )
	if ( a >= 0 )
	{
		if (intersection)
		{
			if ( div1 != 0)
				*intersection = delta2 * a + point2;
			else
				*intersection = delta1 * a + point1;
		}
		return true;
	}

	return false;
};
*/
