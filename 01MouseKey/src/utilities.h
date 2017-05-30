#ifndef ______utilities_h___________
#define ______utilities_h___________

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#if (defined _WIN32) || (defined _WIN64)
#pragma warning(disable:4996) 
#pragma warning(disable:4244) 
#pragma warning(disable:4800) 
#endif

#ifndef CLAMP
#define CLAMP(x,min,max) ((x)<(min) ? (min) : ((x)>(max)?(max):(x)) )
#endif

namespace xglm {

void SetMediaPath(const char path[]);
const char * FullPath(const char filename[]);
char * MakeFullPath(const char dir[], const char filename[], char fullpath[]);

void drawCrossX(int x, int y, int w, int h, unsigned int color);
void drawRect(int x, int y, int w, int h, unsigned int color);
void fillRect(int x, int y, int w, int h, unsigned int color);
void drawText(const char text[], int x, int y, unsigned int color, void * bitmapfont);
void drawText(const char text[], unsigned int color, void * bitmapfont);
double getTimeInMillSecond();
int getTicks();

//--------------------------------------------
} // namespace xglm {

//--------------------------------------------
#endif // #ifndef ______utilities_h___________