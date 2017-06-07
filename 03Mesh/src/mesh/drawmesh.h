#ifndef ____draw_utilities_____________
#define ____draw_utilities_____________

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
	static void Picked(int pickedID,
		const vector<Vec3i>& triangle, 
		const vector<Vec3f>& position);
};
	
} //namespace xglm {

#endif //#ifndef ____draw_utilities_____________
