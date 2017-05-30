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


GLUTApp<GLUTView> theAPP("GLUT Template", 800, 600);

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
