
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

//#include "Math3D/math.h"
#include "imgui.h"
#include "glutapp.h"


namespace xglm {

	static Vec3d makeBallPoint(const Vec2d &p)
	{
		Vec3d b(p, 0);
		double plen2 = glm::dot(p,p);
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
		_rotQuat = Quatd(1, 0,0,0);
		_dragQuat = Quatd(1, 0,0,0);
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
		_dragQuat = Quatd(1, 0,0,0);
	}
	
	
		static Vec3d orthoVector(const Vec3d &v)
		{
			double x = fabs(v.x), y = fabs(v.y), z = fabs(v.z);
			if( x<=y && x<=z ) return Vec3d(0, -v.z, v.y);
			if( y<=z ) return Vec3d(v.z, 0, -v.x);
			return Vec3d(-v.y, -v.x, 0);
		}
		
		// inputs are 2 unit vector
		Quatd QuatRot( const Vec3d & rotateFrom, const Vec3d & rotateTo )
		{
			double cc = glm::dot(rotateFrom, rotateTo);
			// parallel case
			if( cc>=1.0 ) 
				return Quatd(1, 0, 0, 0);
			else if( cc<=-1 ) // anti parallel
				return Quatd(0, glm::normalize(orthoVector(rotateFrom) ));

			return Quatd(sqrt((1+cc)/2), glm::normalize(glm::cross(rotateFrom, rotateTo) ) * sqrt((1-cc)/2) );
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
	
		static void calcAxisRadian(Quatd &q, Vec3d &axis, double &radians )
		{
			radians = acos(q.w) * 2;
			if ( radians == 0.0 )
				axis = Vec3d( 0.0, 0.0, 1.0 );
			else {
				axis = glm::normalize( Vec3d( q.x, q.y, q.z) );
			}
		}
	
	void Arcball::applyGLxform() const
	{
		Quatd q = _rotQuat;
		if( inDragging() )
			q = _dragQuat * _rotQuat;
		// [c][rot][-c]
		Vec3d axis = glm::axis(q);
		double rad = glm::angle(q);
		//calcAxisRadian(q, axis, rad);
		//printf("q=%f %f %f %f, rad=%f\n",q.x, q.y, q.z, q.w, rad);
		glTranslated( _rotCenter.x,  _rotCenter.y,  _rotCenter.z );
		glRotated( rad/M_PI*180, axis.x, axis.y, axis.z );
		glTranslated( -_rotCenter.x,  -_rotCenter.y,  -_rotCenter.z );
	}

	void Arcball::finishDrag()
	{
		_inDragging = false;
		//_rotQuat = _rotQuat * _dragQuat;
		_rotQuat = _dragQuat * _rotQuat;
		//_rotQuat.set_value(1, 0,0,0);
	}
	
	void Arcball::cancelDrag()
	{
		_inDragging = false;
	}
	
} //namespace xglm {