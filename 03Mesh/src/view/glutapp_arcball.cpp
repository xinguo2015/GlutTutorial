
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "Math3D/math.h"
#include "imgui.h"
#include "glutapp.h"


namespace xglm {

	static Vec3d makeBallPoint(const Vec2d &p)
	{
		Vec3d b(p, 0);
		double plen2 = p.dot(p);
		if( plen2<1 ) b.z = sqrt(1-plen2);
		else          b  /= sqrt(plen2);
		//b.y = -b.y;
		return b;
	}
	
	Quatd arcball(ImGUIState gs, Vec2d ballCenter, double ballSize)
	{
		Vec2d pclick(gs.clickxy[0], gs.clickxy[1]);
		Vec2d pnow(gs.mousex, gs.mousey);
		Vec3d a = makeBallPoint( (pclick-ballCenter)/ballSize );
		Vec3d b = makeBallPoint( (pnow-ballCenter)/ballSize );
		return Quatd(a,b);
	}

	Arcball::Arcball()
	{
		_inDragging = 0;
		resetRotation();
	}

	void Arcball::resetRotation()
	{
		_rotQuat.set_value(0,0,0,1);
		_dragQuat.set_value(0,0,0,1);
	}

	void Arcball::setBallSize(double size)
	{
		_ballSize = size;
	}
	
	void Arcball::setBallCenter(double x, double y)
	{
		_ballCenter = Vec2d(x,y);
	}
	
	void Arcball::setRotCenter(double x, double y, double z)
	{
		_rotCenter = Vec3d(x,y,z);
	}
	
	int  Arcball::inDragging() const
	{
		return _inDragging;
	}
	
	void Arcball::startDrag()
	{
		_inDragging = true;
		_dragQuat.set_value(0,0,0,1);
	}
	
	int Arcball::update(ImGUIState gs)
	{
		int stage = 0;
		if( inDragging() ) {
			if( gs.buttondown ) { 
				// keep dragging
				Vec2d pclick(gs.clickxy[0], gs.clickxy[1]);
				Vec2d pnow(gs.mousex, gs.mousey);
				Vec3d a = makeBallPoint( (pclick-_ballCenter)/_ballSize );
				Vec3d b = makeBallPoint( (pnow-_ballCenter)/_ballSize );
				_dragQuat = Quatd(a,b);
				_dragQuat = _dragQuat * _dragQuat; // double speed;
				_dragQuat = _dragQuat * _dragQuat; // quadric speed;
				stage = 2;
			}
			else { // button up
				finishDrag();
				stage = 3;
			}
		} else if( gs.buttondown ) {
			startDrag();
			stage = 1;
		} 
		return stage;
	}
	
	void Arcball::applyGLxform() const
	{
		Vec3d axis; double angle; 
		// get the rotation in axis/angle
		if( inDragging() )
			//(_rotQuat*_dragQuat).get(axis, angle);
			(_dragQuat * _rotQuat).get(axis, angle);
		else
			_rotQuat.get(axis, angle);
		// [c][rot][-c]
		glTranslated( _rotCenter.x,  _rotCenter.y,  _rotCenter.z );
		glRotated( angle/M_PI*180, axis.x, axis.y, axis.z );
		glTranslated( -_rotCenter.x,  -_rotCenter.y,  -_rotCenter.z );
	}
	
	void Arcball::finishDrag()
	{
		_inDragging = false;
		//_rotQuat = _rotQuat * _dragQuat;
		_rotQuat = _dragQuat * _rotQuat;
		//_rotQuat.set_value(0,0,0,1);
	}
	
	void Arcball::cancelDrag()
	{
		_inDragging = false;
	}
	
} //namespace xglm {