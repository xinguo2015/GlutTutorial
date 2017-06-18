
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
	
	GLCamera::GLCamera()
	{
		_fovy   = 45;
		_aspect = 1;
		_znear  = 0.1f;
		_zfar   = 10;
	}
	
	GLCamera::GLCamera(GLfloat fovy, GLfloat aspect, GLfloat znear,  GLfloat zfar)
	{
		_fovy   = fovy;
		_aspect = aspect;
		_znear  = znear;
		_zfar   = zfar;
	}
	
	void GLCamera::applyProjection() const
	{
		gluPerspective(_fovy, _aspect, _znear, _zfar);
	}

	void GLCamera::reshape(int width, int height)
	{
		_aspect = width/float(height); 
	}

	GLUTView::GLUTView()
	{
		_bkcolor[0] = 0.9f; 
		_bkcolor[1] = 0.9f;
		_bkcolor[2] = 0.9f;
		_bkcolor[3] = 1.0f;
		setGUIFlag(1);
		_shape = NULL;
	}

	int GLUTView::init()
	{
		_fps.reset(100);
		_gui.initialize();
		return 1;
	}
	
	void GLUTView::setShape(Shape3D * shape)
	{
		if( ! shape ) return;
		_shape = shape;
		// store some scene parameters
		Vec3f cen = shape->getBdCenter();
		float rds = shape->getBdRadius();
		// model view
		Vec3f eye = cen + Vec3f(0, 0, 2*rds);
		_modelview.translation(-eye.x, -eye.y, -eye.z);
		// adjust near/far plane
		_camera._znear = rds/10;
		_camera._zfar = rds*10;
	}

	void GLUTView::initArcball()
	{
		Vec3f cen = _shape->getBdCenter();
		Vec4d rotCenter = _modelview * Vec4d(cen.x, cen.y, cen.z, 1);
		_arcball.resetRotation();
		_arcball.setRotCenter(rotCenter.x, rotCenter.y, rotCenter.z);
	}
	
	void GLUTView::setupLights()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLightfv(GL_LIGHT0, GL_POSITION, _values.get("GL_LIGHT0_POSITION", VarValue(Vec4f(0,0,0,1))).getVec4f().get_value());
		glLightfv(GL_LIGHT0, GL_DIFFUSE,  _values.get("GL_LIGHT0_DIFFUSE", VarValue(Vec4f(1,1,1,1))).getVec4f().get_value());
	}

	void GLUTView::setupGL() 
	{
		Vec3f cen = _shape->getBdCenter();
		float rds = _shape->getBdRadius();
		glClearColor(0, 0, 0, 0);
		glClearDepth(1.0f);// 0 is near, 1 is far
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_LIGHT1);
		glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment
		glLineWidth(2.0f);
		glPointSize(15.f);
		// init lights
		_values["GL_LIGHT0_POSITION"].getVec4f() = Vec4f( rds/4, 0.f, 0.f, 1.f );
		_values["GL_LIGHT0_DIFFUSE"].getVec4f() = Vec4f( 0.7f, 0.8f, 0.8f, 1.f );
		// setup light
		glEnable(GL_LIGHTING); 
		glEnable(GL_LIGHT0);
		setupLights();
	}

} //namespace xglm {