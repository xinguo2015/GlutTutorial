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
#include "common/utilities.h"
#include "mesh/trimesh.h"
#include "mesh/drawmesh.h"

using namespace xglm;

class MeshDoc: public Shape3D
{
public:
	MeshDoc(TriMesh * mesh = NULL)   {
		if( mesh ) setMesh(mesh); 
	}
public:
	void setMesh(TriMesh *mesh) {
		_mesh = mesh;
		_bdCenter = mesh->mAABBox._lower + mesh->mAABBox._upper;
		_bdCenter *= 0.5f;
		_bdRadius = glm::length(mesh->mAABBox.extend())/2;
		_rotCenter = _bdCenter;
		_rotation = Mat4f(1.f);
		_scaling = Vec3f(1.f, 1.f, 1.f);
	}
	// override method in Shape3D
	virtual void drawSolid() const {
		DrawMesh::Face(_mesh->mTriangles, _mesh->mPosition, _mesh->mNormal); 
	}
	// override method in Shape3D
	virtual void drawID()    const {
		DrawMesh::PointID(_mesh->mPosition); 
		DrawMesh::FaceID(_mesh->mTriangles, _mesh->mPosition); 
	}
	// override method in Shape3D
	virtual void drawPicked(int id) const {
		DrawMesh::Picked(id, _mesh->mTriangles, _mesh->mPosition); 
	}
protected:
	TriMesh* _mesh;
};

class MyView : public GLUTView
{
public:
	MyView() { 
		setGUIFlag(0); // hide gui
	}
	void drawUIOld();
	// override GLUTVIew::draw2DObjects()
	void drawUI();
};
