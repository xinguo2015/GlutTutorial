#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "utilities.h"
#include "timer.h"

namespace xglm {

char gMediaPath[256] = "";

void SetMediaPath(const char path[])
{
	strcpy(gMediaPath, path);
}

char * MakeFullPath(const char dir[], const char filename[], char fullpath[])
{
	strcpy(fullpath, dir);
	return strcat(fullpath, filename);
}

const char * FullPath(const char filename[])
{
	static char path[256];
	return MakeFullPath(gMediaPath, filename, path);
}

void drawCircle(float cx, float cy, float r, int num_segments, unsigned int color) 
{ 
	float theta = 2 * M_PI / num_segments; 
	float c = cosf(theta);
	float s = sinf(theta);
	float x = r, y = 0; 
    
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP); 
	glColor4ubv( reinterpret_cast<GLubyte*>( & color ) );	
	for(int k = 0; k < num_segments; k++) 
	{ 
		float t = x;
		glVertex2f(x + cx, y + cy);
		//apply the rotation matrix
		x = c * x - s * y;
		y = s * t + c * y;
	} 
	glEnd(); 
}

void fillDiamond(int x, int y, int w, int h, unsigned int color)
{
	glBegin(GL_QUADS); {
		glColor4ubv( reinterpret_cast<GLubyte*>( & color ) );	
		glVertex2f(x+w/2,y);
		glVertex2f(x+w,y+h/2);
		glVertex2f(x+w/2,y+h);
		glVertex2f(x,y+h/2);
	}glEnd();
}

void fillRect(int x, int y, int w, int h, unsigned int color)
{
	glBegin(GL_QUADS); {
		glColor4ubv( reinterpret_cast<GLubyte*>( & color ) );	
		glVertex2f(x,y);
		glVertex2f(x+w,y);
		glVertex2f(x+w,y+h);
		glVertex2f(x,y+h);
	}glEnd();
}

void drawRect(int x, int y, int w, int h, unsigned int color)
{
	glLineWidth(1.0f);
	glBegin(GL_LINE_STRIP); {
		glColor4ubv( reinterpret_cast<GLubyte*>( & color ) );	
		glVertex2f(x,y);
		glVertex2f(x+w,y);
		glVertex2f(x+w,y+h);
		glVertex2f(x,y+h);
		glVertex2f(x,y);
	}glEnd();
}

void drawCrossX(int x, int y, int w, int h, unsigned int color)
{
	glLineWidth(3.0f);
	glBegin(GL_LINES); {
		glColor4ubv( reinterpret_cast<GLubyte*>( & color ) );
		glVertex2f(x+1, y+1);
		glVertex2f(x+w-1, y+h-1);
		glVertex2f(x+1, y+h-1);
		glVertex2f(x+w-1, y+1);
	} glEnd();
}

void drawText(const char text[], int x, int y, unsigned int color, void * bitmapfont)
{
	glColor4ubv( reinterpret_cast<GLubyte*>( & color ) );	
	glRasterPos2i(x, y);
	// loop all characters in the string
	while(*text)
	{
		glutBitmapCharacter(bitmapfont, *text);
		++text;
	}
}

void drawText(const char text[], unsigned int color, void *bitmapfont)
{
	glColor4ubv( reinterpret_cast<GLubyte*>( & color ) );	
	// loop all characters in the string
	while(*text)
	{
		//glutStrokeCharacter(bitmapfont, *text);
		glutBitmapCharacter(bitmapfont, *text);
		++text;
	}
}

}; //namespace xglm {

