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
		V3_ADD(_center, mesh->mAABBox._lower, mesh->mAABBox._upper);
		_center *= 0.5f;
		_radius = mesh->mAABBox.extend().length()/2;
	}
	// override method in Shape3D
	virtual Vec3f getCenter() const {
		return _center; 
	}
	// override method in Shape3D
	virtual float getRadius() const {
		return _radius; 
	}
	// override method in Shape3D
	virtual void  drawSolid() const {
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
	Vec3f    _center;
	float    _radius;
};

class MyView : public GLUTView
{
public:
	MyView() { 
		setGUIFlag(0); // hide gui
	}
	// override GLUTVIew::draw2DObjects()
	void draw2DObjects();
};
