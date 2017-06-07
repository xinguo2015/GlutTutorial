
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "glutapp.h"
#include "imgui_keys.h"
#include "utilities.h"

#define UpsideDown(y) (getHeight()-1-(y))

namespace xglm {
	
	void GLUTView::draw2DObjects()
	{
		ImGUIState uis = _gui.getGUIState();
		unsigned int textcolor = 0xFF0000;
		char buf[256];

		int x = uis.mousex;
		int y = uis.mousey;
		// draw a square to follow the mouse
		fillRect(x-25, y-25, 50, 50, 0xFFFF);
		// print the keyboard info
		x = 50, y = getHeight()-60;
		sprintf(buf, "Keychar    = %c", uis.lastkeys[1]);
		drawText(buf, x, y-=40, textcolor, GLUT_BITMAP_TIMES_ROMAN_24);
		sprintf(buf, "Keypressed = %c", uis.lastkeys[0]);
		drawText(buf, x, y-=40, textcolor, GLUT_BITMAP_TIMES_ROMAN_24);
		// modifiers key
		drawText("Modifier keys are:", x, y-=40, textcolor, GLUT_BITMAP_TIMES_ROMAN_24);
		if( uis.lastkeys[2] & IMGUI_SHIFT )
			drawText("    Shift", x, y-=40, textcolor, GLUT_BITMAP_TIMES_ROMAN_24);
		if( uis.lastkeys[2] & IMGUI_CTRL )
			drawText("    Ctrl",  x, y-=40, textcolor, GLUT_BITMAP_TIMES_ROMAN_24);
		if( uis.lastkeys[2] & IMGUI_ALT )
			drawText("    Alt",   x, y-=40, textcolor, GLUT_BITMAP_TIMES_ROMAN_24);
	}

	void GLUTView::displayMessages()
	{
		char buf[128];
		sprintf(buf, "ESC -- toggle UI display, Ctrl+ESC -- quit, Running at %.2f FPS", _fps.getFPS());
		drawText(buf, 20, getHeight()-20, 0xFF7777, GLUT_BITMAP_TIMES_ROMAN_24 );
	}
	
	void GLUTView::cbDisplay(void)
	{
		// clears requested bits (color and depth) in glut window
		glClearColor(_bkcolor[0], _bkcolor[1], _bkcolor[2], _bkcolor[3]);
		glClearDepth(1.0f);// 0 is near, 1 is far
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// 3D drawing
		if( ! getGUIFlag() ) {
			draw3DObjects();
		}
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
		//glTranslatef(0, _viewport[3],0);
		//glScalef(1.f, -1.f, 1.f);
		glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);             //disable lighting for proper text color
		glDisable(GL_TEXTURE_2D);           //no texture
		displayMessages();
		// draw user's 2D objects
		if( getGUIFlag() ) {
			_gui.prepare();
			draw2DObjects();
			_gui.finish();
		}
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
	
	void GLUTView::applyProjectionAndModelview()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		_camera.applyProjection();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		_arcball.applyGLxform();
		//glMultMatrixd( _dragXform.get_glmatrix() );
		glMultMatrixd( _modelview.get_glmatrix() );
	}

	void GLUTView::draw3DObjects()
	{
		if( ! _shape ) return;
		// set up camera and model view
		applyProjectionAndModelview();
		// update arcball 
		_arcball.update(_gui.getGUIState());
		// draw the shape
		_shape->drawSolid();
	}

	
} //namespace xglm {