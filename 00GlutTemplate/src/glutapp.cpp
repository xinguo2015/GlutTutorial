
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "utilities.h"
#include "glutapp.h"

namespace xglm {
	
	GLUTView::GLUTView()
	{
		_bkcolor[0] = 0.9f; 
		_bkcolor[1] = 0.9f;
		_bkcolor[2] = 0.9f;
		_bkcolor[3] = 1.0f;
	}

	int GLUTView::initialize()
	{
		glClearColor(_bkcolor[0], _bkcolor[1], _bkcolor[2], _bkcolor[3]);
		glClearDepth(1.0f);// 0 is near, 1 is far
		_fps.reset(30);
		return 1;
	}
	
	void GLUTView::draw3DObjects()
	{
	}

	void GLUTView::draw2DObjects()
	{
		char buf[256];
		unsigned int color = 0xFF00FF;
		unsigned int textcolor = 0xFF0000;
		sprintf(buf, "Rendering speed = %.2f FPS", (float)_fps.getFPS());
		fillRect(getViewWidth()/4, getViewHeight()/4, getViewWidth()/2, getViewHeight()/2, 0xFFFF);
		drawRect(getViewWidth()/4, getViewHeight()/4, getViewWidth()/2, getViewHeight()/2, 0xFF00FF);
		drawText("OpenGL Demo", getViewWidth()/4+100, getViewHeight()/2+50, textcolor);
		drawText(buf, getViewWidth()/4+50, getViewHeight()/2, textcolor);
	}

	void GLUTView::cbReshape (int width, int height)
	{
		glViewport(0, 0, width, height);
		glGetIntegerv(GL_VIEWPORT, _viewport);
	}

	void GLUTView::cbDisplay(void)
	{
		// clears requested bits (color and depth) in glut window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// setting up for 3D drawing
		draw3DObjects();
		// compute the rendering speed (frames/second)
		_fps.update();
		// setting up for 2D drawing
		glMatrixMode(GL_PROJECTION);        // switch to projection matrix
		glPushMatrix();                     // save current projection matrix
		glLoadIdentity();                   // reset projection matrix
		gluOrtho2D(0, _viewport[2], 0, _viewport[3]); 
		glMatrixMode(GL_MODELVIEW);         // switch to modelview matrix
		glPushMatrix();                     // save current modelview matrix
		glLoadIdentity();                   // reset modelview matrix
		glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
		glDisable(GL_LIGHTING);             //disable lighting for proper text color
		glDisable(GL_TEXTURE_2D);           //no texture
		// draw 2D objects
		draw2DObjects();
		// pop up settings
		glPopAttrib();
		glPopMatrix();             
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		// finish! and to present the drawings
		glFlush();
		glutSwapBuffers();
		glutPostRedisplay();
	}

	void GLUTView::cbOverlayDisplay(void)
	{
	}

	void GLUTView::cbKeyboard(unsigned char key, int x, int y)
	{
	}

	void GLUTView::cbKeyboardUp(unsigned char key, int x, int y)
	{
	}

	void GLUTView::cbSpecial(int key, int x, int y)
	{
	}

	void GLUTView::cbSpecialUp(int key, int x, int y)
	{
	}

	void GLUTView::cbMouse(int button, int state, int x, int y)
	{

	}

	void GLUTView::cbMotion(int x, int y)
	{
	}

	void GLUTView::cbPassiveMotion(int x, int y )
	{
	}


	

} //namespace xglm {