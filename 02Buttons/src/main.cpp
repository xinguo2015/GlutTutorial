#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>

#include "glutapp.h"
#include "utilities.h"
#include "imgui.h"

using namespace xglm;

char editstring[50] = "Here to set window title";
int  checkboxValue  = 1;
int  radiovalue     = 1;
int  liststart      = 0;
int  listvalue      = 1;
char*listitems[]    = { "1 Apple", 
						"2 Pear", 
						"3 Grape", 
						"4 Rice", 
						"5 Water", 
						"6 Salt", 
						"7 Drink", 
						"No MORE",
};

class MyView : public GLUTView
{
public:
	
	void draw2DObjects()
	{
		int x = 50, y = getViewHeight()-20;
		int w = 100, h = 40, hs = 45;

		//_gui.button(GenUIID(0), x, y, 100, 45, " ButtonA");
		// put a text label 
		_gui.textlabel(GenUIID(0), x, y-=hs, "Below are some buttons");
		// put an array of buttons
		y -= hs;
		for( int k = 0; k<4; k++ ) {
			char label[256]; sprintf(label," button-%d", k);
			_gui.button(GenUIID(k), x+(w+10)*k, y, w, h, label);
		}
		// a button to randomize the background color
		if ( _gui.button(GenUIID(0), x, y-= hs, 400, h, "click me to change the background color" ))
		{
			int m = getTicks();
			_bkcolor[0] = (m      &0xFF)/255.f;
			_bkcolor[1] = ((m>> 4)&0xFF)/255.f;
			_bkcolor[2] = ((m>> 8)&0xFF)/255.f;
			_bkcolor[3] = ((m>>12)&0xFF)/255.f;
		}
	}
};

GLUTApp<MyView> theAPP("GLUT Template", 800, 600);

int main (int argc, char *argv[])
{
	// to see the ouput immediately for debugging
	setbuf(stdout, NULL);
	// initialize glut
	glutInit(&argc, argv);
	// create app window
	if ( !theAPP.createWindow(argc,argv) ) 
	{
		printf("Failed creating the window\n");
		return -1;
	}

	// binding the call back functions
	glutReshapeFunc( theAPP.cbReshape );
	glutDisplayFunc( theAPP.cbDisplay ); 
	// binding mouse functions
	glutMouseFunc(  theAPP.cbMouse ); 
	glutMotionFunc( theAPP.cbMotion ); 
	glutPassiveMotionFunc( theAPP.cbPassiveMotion ); 
	// binding keyboard functions
	glutKeyboardFunc(   theAPP.cbKeyboard );
	glutKeyboardUpFunc( theAPP.cbKeyboardUp );
	glutSpecialFunc(    theAPP.cbSpecial );
	glutSpecialUpFunc(  theAPP.cbSpecialUp );

	// running the application
	glutMainLoop();
    return 0;
}
