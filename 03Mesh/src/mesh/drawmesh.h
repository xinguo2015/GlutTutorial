#ifndef __draw_mesh_stuff_____________
#define __draw_mesh_stuff_____________

#include "xglm.h"
#include "pickpixel.h"

using namespace std;

namespace xglm {

class DrawMesh 
{
public:
	// draw smoothed group
	static void Face(
		const vector<Vec3i>& triangle,
		const vector<Vec3i>& normalIndex,
		const vector<Vec3f>& position, 
		const vector<Vec3f>& normal);
	static void Face(
		const vector<Vec3i>& triangle, 
		const vector<Vec3f>& position,
		const vector<Vec3f>& normal);
	static void FaceID(
		const vector<Vec3i>& triangle,
		const vector<Vec3f>& position );
	static void PointID(
		const vector<Vec3f>& position );
	static void Picked(PixelInfo pickedID,
		const vector<Vec3i>& triangle, 
		const vector<Vec3f>& position);
};
	
} //namespace xglm {

#endif //#ifndef __draw_mesh_stuff_____________
