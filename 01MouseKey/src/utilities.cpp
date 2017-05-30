#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
		glutBitmapCharacter(bitmapfont, *text);
		++text;
	}
}

double getTimeInMillSecond()
{
#if defined(_WIN32) || defined(_WIN64)
	LARGE_INTEGER startCount;
	LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startCount);
	return (startCount.QuadPart * (1000.0 / frequency.QuadPart));
#else
	timeval startCount;
	gettimeofday(&startCount, NULL);
    return (startCount.tv_sec * 1000.0 + startCount.tv_usec/1000.0);
#endif
}

double getTimeInMicroSecond()
{
#if defined(_WIN32) || defined(_WIN64)
	LARGE_INTEGER startCount;
	LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startCount);
	return (startCount.QuadPart * (1000000.0 / frequency.QuadPart));
#else
	timeval startCount;
	gettimeofday(&startCount, NULL);
    return (startCount.tv_sec * 1000000.0 + startCount.tv_usec);
#endif
}

int getTicks()
{
	double t = getTimeInMicroSecond();
	int b = 0x7FFFFFF;
	t -= b * (double)(int)(t/b);
	return (int)t;
}


}; //namespace xglm {

