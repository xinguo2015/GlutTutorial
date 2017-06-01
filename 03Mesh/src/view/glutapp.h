#ifndef __glut_view_app_____
#define __glut_view_app_____

#include <string>
#include <map>
#include "Math3D/math.h"
#include "variables.h"
#include "myglut.h"
#include "imgui.h"
#include "timer.h"

namespace xglm {

using namespace std;

class MeshDoc;

class GLCamera
{
public:
	GLCamera();
	GLCamera(GLfloat fovy, GLfloat aspect, GLfloat znear,  GLfloat zfar);
public:
	void setProj() const;
public:
	GLfloat _fovy;   // field of view in Y direction
	GLfloat _aspect; // aspect ratio ( fovx / fovy )
	GLfloat _znear;  // near plane
	GLfloat _zfar;   // far plane
};

class GLUTView
{
public:
	GLUTView();
	
public:
	GLfloat    _bkcolor[4];  // background color
	GLint      _viewport[4]; // viewport 
	int        _guiFlag;     // show the GUI ?
	ImGUI      _gui;         // Graphic User Interface
	FPSCounter _fps;         // frame per second
	// 3D stuff
	MeshDoc *  _meshDoc;
	Mat4d      _
	GLCamera _cameraView;    // camera view
	Vec4d    _sceneCenter;   // c: center of the scene (in world)
	double   _sceneSize;     // size of the scene
	Vec4d    _sceneScaling;  // S: scaling imposed on the scene
	Mat4d    _projection;    // the projection matrix
	Mat4d    _modelview;     // M: model view matrix
	Mat4d    _userRotate;    // A: the rotation center is: c*M
public:
	virtual int  initialize();
	virtual void draw3DObjects();
	virtual void draw2DObjects();

	int  getHeight()          const { return _viewport[3]; }
	int  getWidth()           const { return _viewport[2]; }
	int  getGUIFlag()         const { return _guiFlag; }
	void setGUIFlag(int flag)       { _guiFlag = flag; }
	void displayMessages();
	
public:
	// glut callback functions
	virtual void cbReshape(int width, int height); 
	virtual void cbDisplay(void);
	virtual void cbOverlayDisplay(void);
	virtual void cbKeyboard(unsigned char key, int x, int y); 
	virtual void cbKeyboardUp(unsigned char key, int x, int y);
	virtual void cbSpecial(int key, int x, int y);
	virtual void cbSpecialUp(int key, int x, int y);         
	virtual void cbMouse(int button, int state, int x, int y);
	virtual void cbMotion(int x, int y);
	virtual void cbPassiveMotion(int x, int y );
	virtual void cbEntry(int state)                    {}
	virtual void cbVisibility(int state)               {}
	virtual void cbIdle(void)                          {}
	virtual void cbTimer(int value)                    {}  
	virtual void cbMenuStatus(int status, int x, int y){}
	virtual void cbMenuState(int state)                {}
	virtual void cbWindowStatus(int state)             {}
};

Quatd arcball(ImGUIState gs, Vec3d ballCenter, double ballSize);

template <class TView>
class GLUTApp
{
public:
	GLUTApp(const char title[] = "OpenGL Application", int winWidth = 800,  int winHeight = 600,
			int posX = 200,  int posY = 200 ) :_winID(-1),  _winWidth(winWidth),  
			_winHeight(winHeight), _winPosX(posX),  _winPosY(posY)
	{	
		_winTitle = string(title);
	}

	int createWindow(int argc, char *argv[])
	{
		glutInitWindowPosition( _winPosX, _winPosY );
		glutInitWindowSize( _winWidth, _winHeight );
		// GLUT_ALPHA must be specified for using alpha
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA | GLUT_ALPHA); 
		_winID = glutCreateWindow(_winTitle.c_str());
		if( _winID < 0 ) 
			return 0;
		return 1;
	}

	static TView * getView() 
	{
		if( !_glview  && ! (_glview = createView()) )  {
			printf("Cannot create view\n");
			exit(-1);
		}
		return _glview; 
	}

public:
	static void cbReshape(int width, int height)                { getView()->cbReshape(width, height); }
	static void cbDisplay(void)								    { getView()->cbDisplay(); }
	static void cbOverlayDisplay(void)							{ getView()->cbOverlayDisplay(); }
	static void cbKeyboard(unsigned char key, int x, int y)	    { getView()->cbKeyboard(key,x,y); }
	static void cbKeyboardUp(unsigned char key, int x, int y)	{ getView()->cbKeyboardUp(key,x,y); }
	static void cbSpecial(int key, int x, int y)              	{ getView()->cbSpecial(key,x,y); }
	static void cbSpecialUp(int key, int x, int y)           	{ getView()->cbSpecialUp(key,x,y); }
	static void cbMouse(int button, int state, int x, int y)	{ getView()->cbMouse(button,state,x,y); }
	static void cbMotion(int x, int y)							{ getView()->cbMotion(x,y); }
	static void cbPassiveMotion(int x, int y)                	{ getView()->cbPassiveMotion(x,y); }
	static void cbEntry(int state)                           	{ getView()->cbEntry(state); }
	static void cbVisibility(int state)                      	{ getView()->cbVisibility(state); }
	static void cbIdle(void)                                 	{ getView()->cbIdle(); }
	static void cbTimer(int value)                            	{ getView()->cbTimer(value); }
	static void cbMenuStatus(int status, int x, int y)      	{ getView()->cbMenuStatus(status,x,y); }
	static void cbMenuState(int state)                      	{ getView()->cbMenuState(state); }
	static void cbWindowStatus(int state)     					{ getView()->cbWindowStatus(state); }
	
protected:
	int    _winID;
	int    _winWidth;
	int    _winHeight;
	int    _winPosX;
	int    _winPosY;
	string _winTitle;
	
protected:
	static TView * _glview;
	static TView * createView()
	{
		TView * view = new TView;
		if( view  ) // initialize
			view->initialize();
		return view;
	}
};

template<typename TView> 
TView * GLUTApp<TView>::_glview;

} //namespace xglm {

#endif //#ifndef __glut_view_app_____
