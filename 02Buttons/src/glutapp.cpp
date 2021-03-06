
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "utilities.h"
#include "glutapp.h"

#define UpsideDown(y) (getHeight()-1-(y))

namespace xglm {
	
	GLUTView::GLUTView()
	{
		_bkcolor[0] = 0.9f; 
		_bkcolor[1] = 0.9f;
		_bkcolor[2] = 0.9f;
		_bkcolor[3] = 1.0f;
		_guiFlag = 1;
	}

	int GLUTView::initialize()
	{
		_fps.reset(30);
		_gui.initialize();
		return 1;
	}
	
	void GLUTView::draw3DObjects()
	{
	}

	void GLUTView::draw2DObjects()
	{
		ImGUIState uis = _gui.getGUIState();
		unsigned int textcolor = 0xFF0000;
		char buf[256];

		sprintf(buf, "Rendering speed = %.2f FPS", (float)_fps.getFPS());
		drawText(buf, getWidth()-350, getHeight()-30, textcolor, GLUT_BITMAP_TIMES_ROMAN_24);

		int x = uis.mousex;
		int y = uis.mousey;
		// draw a square to follow the mouse
		fillRect(x-25, y-25, 50, 50, 0xFFFF);
		// print the keyboard info
		x = 50, y = getHeight()-60;
		sprintf(buf, "Keychar    = %c", uis.lastkeychar);
		drawText(buf, x, y-=40, textcolor, GLUT_BITMAP_TIMES_ROMAN_24);
		sprintf(buf, "Keypressed = %c", uis.lastkey);
		drawText(buf, x, y-=40, textcolor, GLUT_BITMAP_TIMES_ROMAN_24);
		// modifiers key
		drawText("Modifier keys are:", x, y-=40, textcolor, GLUT_BITMAP_TIMES_ROMAN_24);
		if( uis.modifier & GLUT_ACTIVE_SHIFT )
			drawText("    Shift", x, y-=40, textcolor, GLUT_BITMAP_TIMES_ROMAN_24);
		if( uis.modifier & GLUT_ACTIVE_CTRL )
			drawText("    Ctrl",  x, y-=40, textcolor, GLUT_BITMAP_TIMES_ROMAN_24);
		if( uis.modifier & GLUT_ACTIVE_ALT )
			drawText("    Alt",   x, y-=40, textcolor, GLUT_BITMAP_TIMES_ROMAN_24);
	}

	void GLUTView::cbReshape (int width, int height)
	{
		glViewport(0, 0, width, height);
		glGetIntegerv(GL_VIEWPORT, _viewport);
	}

	void GLUTView::cbDisplay(void)
	{
		// clears requested bits (color and depth) in glut window
		glClearColor(_bkcolor[0], _bkcolor[1], _bkcolor[2], _bkcolor[3]);
		glClearDepth(1.0f);// 0 is near, 1 is far
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// setting up for 3D drawing
		draw3DObjects();
		_fps.update();
		
		if( _guiFlag ) {
			// compute the rendering speed (frames/second)
			// setting up for 2D drawing
			glMatrixMode(GL_PROJECTION);        // switch to projection matrix
			glPushMatrix();                     // save current projection matrix
			glLoadIdentity();                   // reset projection matrix
			gluOrtho2D(0, _viewport[2], 0, _viewport[3]); 
			glMatrixMode(GL_MODELVIEW);         // switch to modelview matrix
			glPushMatrix();                     // save current modelview matrix
			glLoadIdentity();                   // reset modelview matrix
			//glTranslatef(0, _viewport[3],0);
			//glScalef(1.f, -1.f, 1.f);
			glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
			glDisable(GL_LIGHTING);             //disable lighting for proper text color
			glDisable(GL_TEXTURE_2D);           //no texture
			// draw 2D objects
			_gui.prepare();
			draw2DObjects();
			_gui.finish();
			// pop up settings
			glPopAttrib();
			glPopMatrix();             
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
		}
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
		if( key==27 ) { // ESC key
			if( glutGetModifiers() & GLUT_ACTIVE_CTRL ) {
				//cleanup();
				exit(-1);
			}
			setGUIFlag(!getGUIFlag());
		}
		if( getGUIFlag() )
			_gui.onKeyboard(key, glutGetModifiers(), x, UpsideDown(y));
	}

	void GLUTView::cbKeyboardUp(unsigned char key, int x, int y)
	{
		//printf("cbKeyboardUp = %c %u\n", key, key);
		if( getGUIFlag() )
			_gui.onKeyboardUp(key, glutGetModifiers(), x, UpsideDown(y));
	}

	void GLUTView::cbSpecial(int key, int x, int y)
	{
		//printf("cbSpecial\n");
		if( getGUIFlag() )
			_gui.onSpecial(key, glutGetModifiers(), x, UpsideDown(y));
	}

	void GLUTView::cbSpecialUp(int key, int x, int y)
	{
		//printf("cbSpecialUp\n");
		if( getGUIFlag() )
			_gui.onSpecialUp(key, glutGetModifiers(), x, UpsideDown(y));
	}

	void GLUTView::cbMouse(int button, int state, int x, int y)
	{
		//printf("%d %d %d %d\n", button, state, x, y);
		if( getGUIFlag() )
			_gui.onMouse(button, state, x, UpsideDown(y));
	}

	void GLUTView::cbMotion(int x, int y)
	{
		if( getGUIFlag() )
			_gui.onMotion( x, UpsideDown(y) );
	}

	void GLUTView::cbPassiveMotion(int x, int y )
	{
		if( getGUIFlag() )
			_gui.onMotion( x, UpsideDown(y) );
	}

} //namespace xglm {