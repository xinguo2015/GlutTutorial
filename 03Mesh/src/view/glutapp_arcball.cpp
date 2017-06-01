
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "Math3D/math.h"
#include "imgui.h"


namespace xglm {

	static Vec3d makeBallPoint(const Vec2d &p)
	{
		Vec3d b(p, 0);
		double plen2 = p.dot(p);
		if( plen2<1 ) b.z = sqrt(1-plen2);
		else          b  /= sqrt(plen2);
		// b.y = -b.y;
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

} //namespace xglm {