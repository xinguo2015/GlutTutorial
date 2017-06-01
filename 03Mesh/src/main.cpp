#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>

#include "view/glutapp.h"
#include "view/utilities.h"

using namespace xglm;

char textBoxString[50] = "Here to set window title";
int  editBrightness = 1;
int  radiovalue     = 1;
int  liststart[2]   = {0,0};
int  listvalue[2]   = {0,0};
char*listitems[]    = {
	"#1 Apple", 
	"#2 Pear", 
	"#3 Grape", 
	"#4 Rice", 
	"#5 Water", 
	"#6 Salt", 
	"#7 Drink", 
	"#8 pig", 
	"#9 duke", 
	"#10 egg", 
	"#11 monkey", 
	"!!No MORE!!",
};

class MyView : public GLUTView
{
public:
	MyView() { setGUIFlag(0); }
	
	void draw2DObjects()
	{
		int w = 100, h = 36, hs = 45;
		int x = 50,  y = getHeight()-20;

		// 文本编辑框 
		_gui.textlabel(GenUIID(0), x, y-=hs, w, h, "Window Title");
		if( _gui.editbox(GenUIID(0), x+140, y, 500, h, textBoxString, sizeof(textBoxString)-1) ) {
			// text is changed, you can do something here ...
			glutSetWindowTitle(textBoxString);
		}
		// 静态标签
		_gui.textlabel(GenUIID(0), x, y-=hs, w, h, "Below are some buttons");
		// 按钮使用
		// put an array of buttons
		y -= hs;
		for( int k = 0; k<4; k++ ) {
			char label[256]; sprintf(label," button-%d", k);
			_gui.button(GenUIID(k), x+(w+10)*k, y, w, h, label);
		}
		// 滑动条使用
		// slider bars to tune the R/G/B background color
		for( int k = 0; k<3; k++ ) { 
			char *label[]={"Red","Green","Blue"};
			_gui.textlabel(GenUIID(0), x, y -= hs, w, h, label[k]);
			_gui.slider(GenUIID(k), x+65, y, 300, 30, 0.0f, 1.0f, 1.0f/255, &_bkcolor[k]);
		}
		//  a button to randomize the background color
		if ( _gui.button(GenUIID(0), x, y-= hs, 450, h, " click me to randomize the background color" ))
		{
			int m = getTicks();
			_bkcolor[0] = (m      &0xFF)/255.f;
			_bkcolor[1] = ((m>> 4)&0xFF)/255.f;
			_bkcolor[2] = ((m>> 8)&0xFF)/255.f;
			_bkcolor[3] = ((m>>12)&0xFF)/255.f;
		}
		// 复选框
		// a check box to toggle editing brightness
		char str[256];
		sprintf(str, "Editting brightness(%.2f)", (_bkcolor[0]+_bkcolor[1]+_bkcolor[2])/3.0f);
		_gui.checkbox( GenUIID(0), x, y-=hs, 300, 25, str, &editBrightness);
		if( editBrightness ) {
			// slider for editting brightness 
			double v0 = (_bkcolor[0]+_bkcolor[1]+_bkcolor[2])/3.0, v = v0;
			if ( _gui.slider(GenUIID(0), x, y-=hs, 365, 30, 0.0, 1.0, 1.0/255, &v) ) {
				v -= v0;
				for( int k = 0; k<3; k++ )
					_bkcolor[k] = CLAMP(_bkcolor[k]+v, 0, 1);
			}
		}
		// 单选框
		// draw a box frame
		_gui.radioFrame(x-5, y-hs*3-5,  370, hs * 3);
		// a group of radio buttons
		_gui.radio(GenUIID(0), x, y-=hs, 250, 25, "Good",             1, &radiovalue);
		_gui.radio(GenUIID(0), x, y-=hs, 250, 25, "Excellent",        2, &radiovalue);
		_gui.radio(GenUIID(0), x, y-=hs, 250, 25, "Great, wonderful", 3, &radiovalue);

		// 列表选择框
		x += 500;
		y -= 30;
		_gui.listbox(GenUIID(0),   x, y, 200, hs*4, listitems, 
					sizeof(listitems)/sizeof(listitems[0]), &liststart[0], &listvalue[0]);
		_gui.textlabel(GenUIID(0), x, y+=hs*4, w, h, listitems[listvalue[0]]);
		// another list box
		_gui.listbox(GenUIID(0), x, y+=35, 200, hs*6, listitems, 
					sizeof(listitems)/sizeof(listitems[0])/2, &liststart[1], &listvalue[1]);
		_gui.textlabel(GenUIID(0), x, y+=hs*6, w, h, listitems[listvalue[1]]);
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
