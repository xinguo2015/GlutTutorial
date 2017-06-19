
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
	
	void GLUTView::cbReshape (int width, int height)
	{
		glViewport(0, 0, width, height);
		glGetIntegerv(GL_VIEWPORT, _viewport);
		_camera.reshape(width, height);
		_arcball.setBallCenter(width/2,height/2);
		_arcball.setBallSize( width<height ? width : height );
		_pickbuf.resize(width,height);
		_pickbuf.markDirty(1);
	}
	
	void GLUTView::cbKeyboard(unsigned char key, int x, int y)
	{
		_gui.onKeyboard(key, glutGetModifiers(), x, UpsideDown(y));
	}

	void GLUTView::cbKeyboardUp(unsigned char key, int x, int y)
	{
		//printf("cbKeyboardUp = %c %u\n", key, key);
		_gui.onKeyboardUp(key, glutGetModifiers(), x, UpsideDown(y));
		
		if( key==27 ) { 
			//cleanup();
			exit(-1);
		}
	}

	void GLUTView::cbSpecial(int key, int x, int y)
	{
		//printf("cbSpecial\n");
		_gui.onSpecial(key, glutGetModifiers(), x, UpsideDown(y));
	}

	void GLUTView::cbSpecialUp(int key, int x, int y)
	{
		//printf("cbSpecialUp\n");
		_gui.onSpecialUp(key, glutGetModifiers(), x, UpsideDown(y));
		
		if( key==GLUT_KEY_F1 ) {
			setGUIFlag(!getGUIFlag());
		}
	}

	void GLUTView::cbMouse(int button, int state, int x, int y)
	{
		//printf("%d %d %d %d\n", button, state, x, y);
		_gui.onMouse(button, state, x, UpsideDown(y));
	}

	void GLUTView::cbMotion(int x, int y)
	{
		_gui.onMotion( x, UpsideDown(y) );
	}

	void GLUTView::cbPassiveMotion(int x, int y )
	{
		_gui.onMotion( x, UpsideDown(y) );
	}

} //namespace xglm {