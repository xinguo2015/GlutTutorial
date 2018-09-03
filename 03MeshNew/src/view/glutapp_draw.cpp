
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "glutapp.h"
#include "imgui_keys.h"
#include "utilities.h"
#include "mesh/drawmesh.h"

#define UpsideDown(y) (getHeight()-1-(y))

namespace xglm {
	
	void GLUTView::drawUI()
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

	void GLUTView::showHelp()
	{
		char msg[128];
		int w, x = 20, y = 8;
		sprintf(msg, "F1 -- Options UI,   ESC -- Quit");
		drawText(msg, x, y, 0xFF7777, GLUT_BITMAP_TIMES_ROMAN_24 );
		sprintf(msg, "Running at %.2f FPS", _fps.getFPS());
		w = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)msg);
		w = (w+29)/30*30;
		drawText(msg, getWidth()-w, y, 0xFF7777, GLUT_BITMAP_TIMES_ROMAN_24 );
	}

	void GLUTView::drawScene(void)
	{
		if( ! _shape ) return;
		// set up camera and model view
		applyProjectionAndModelview();
		// update pick buf
		if( needUpdatePickBuf() )
			genPickBuf();
		glClearColor(_bkcolor[0], _bkcolor[1], _bkcolor[2], _bkcolor[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3f(1.f, 1.f, 1.f);
		// draw the shape
		_shape->drawSolid();
		if( _values.get("DrawPicked", VarValue(1)).getint() )
			drawPicked();
	}

	void GLUTView::cbDisplay(void)
	{
		// arcball
		if( ! getGUIFlag() ) {
			// update arcball 
			int ds = _arcball.update(_gui.getGUIState());
			if( ds==2 ) _pickbuf.markDirty(1);
		}
		// draw 3d stuff
		drawScene();
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
		showHelp();
		// draw user's 2D objects
		if( getGUIFlag() ) {
			_gui.prepare();
			drawUI();
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
		glMultMatrixd( glm::value_ptr( _modelview ) );
	}
	
	bool GLUTView::needUpdatePickBuf()
	{
		return ( _values.get("PickingFlag", VarValue(0)).getint() 
			&&   _pickbuf.isDirty() 
			&&   _pickbuf.mBuffer.size()>0 );
	}
	void GLUTView::genPickBuf()
	{
		glPushMatrix();
		glPushAttrib(GL_COLOR_BUFFER_BIT|GL_LIGHTING_BIT|GL_POLYGON_BIT|GL_POINT_BIT); 
		{
			glDisable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glClearColor(0.f, 0.f, 0.f, 0.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			_shape->drawID();
			glFlush();
			// read out the color buffer 
			glReadBuffer(GL_BACK);
			glReadPixels(0,0,_viewport[2], _viewport[3], GL_RGBA, 
						 GL_UNSIGNED_BYTE, _pickbuf.getBuf());
		}
		glPopAttrib();
		glPopMatrix();
		_pickbuf.markDirty(0);
	}
	
	void GLUTView::drawPicked()
	{
		const ImGUIState & gs = _gui.getGUIState();
		int x = gs.mousex;
		int y = gs.mousey;
		if( ! _pickbuf.contain(x,y) )
			return;
		PixelInfo picked = _pickbuf.getAt(x,y);
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		_shape->drawPicked( picked );
		glPopAttrib();
	}

} //namespace xglm {