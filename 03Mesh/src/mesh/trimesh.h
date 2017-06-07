#ifndef ___mesh_doc_h________________
#define ___mesh_doc_h________________

#include <vector>
#include <algorithm>
#include <math.h>
#include "Math3D/math.h"

#define NullFace   (-1)
#define NullVertex (-1)

using std::vector;

namespace xglm {
	
typedef vector<int> IntList;

class AABBox
{
public:
	Vec3d _lower;
	Vec3d _upper;
public: 
	AABBox();
	bool empty() const;
	void set(const Vec3d p[], int n);
	void set(const Vec3f p[], int n);
public:
	const Vec3d lower() const	{ return _lower; }
	const Vec3d upper() const	{ return _upper; }
	const Vec3d extend() const	{ return _upper-_lower;}
};

class HalfEdge
{
public:
	HalfEdge()                {}
	HalfEdge(int v, int f)    : mEndVtx(v),mLeftFace(f), mWeight(1) {}
public:
	int   endVtx() const      { return mEndVtx; }
	int   leftFace() const    { return mLeftFace; }
	void  setEndVtx(int v)    { mEndVtx = v; }
	void  setLeftFace(int f)  { mLeftFace = f; }
	void  setWeight( float w) { mWeight = w; }
	float weight() const      { return mWeight; }
public:
	int   mEndVtx;   // ending vertex 
	int   mLeftFace; // face at left side
	float mWeight;
};

class HEdgeList : public vector<HalfEdge> 
{
public:
	//  edge index by ending vertex
	int edgeOf(int endVertex) const;
	//  next edge index by ending vertex
	int nextEdgeOf(int endVertex) const;
	//  previous edge index by ending vertex
	int prevEdgeOf(int endVertex) const;
};

class VtxFlag
{
public:
	VtxFlag()                      :mFlags(0) {}
public:
	// get named flag value
	int manifoldFlag() const       { return mFlags & (0x01); }
	int boundaryFlag() const       { return (mFlags>>1) & 0x01; }
	int orientationFlag() const    { return (mFlags>>2) & 0x01; }
	int flag(int which) const      { if( which<0 || which>31 ) return 0; return (mFlags>>which) & 0x01; }
	// set named flag value
	void setManifoldFlag(int f)    { if( f ) mFlags |= 0x01;    else mFlags &= ~0x01; }
	void setBoundaryFlag(int f)    { if( f ) mFlags |= 0x01<<1; else mFlags &= ~(0x01<<1); }
	void setOrientationFlag(int f) { if( f ) mFlags |= 0x01<<2; else mFlags &= ~(0x01<<2); }
	void setFlag(int which, int f) { if( which<0 || which>31 ) return; if( f ) mFlags |= 0x01<<which; else mFlags &= ~(0x01<<which); }
protected:
	unsigned int mFlags;
};

class VtxStar : public VtxFlag, public HEdgeList
{
public:
	float weight() const            { return mWeight; }
	void  setWeight(float w)        { mWeight = w; }
	HEdgeList & edges()             { return (*this); }
	const HEdgeList & edges() const { return (*this); }
	
protected:
	float mWeight;
};

class VtxTopology
{
public:
	VtxTopology() : mManifold(0), mBoundary(0) {}
	short getManifold() const { return mManifold; }
	short getBoundary() const { return mBoundary; }
	void  setManifold( short m ) { mManifold = m; }
	void  setBoundary( short b ) { mBoundary = b; } 
protected:
	short mManifold;
	short mBoundary;
};

class TriMesh 
{
public:
	AABBox  mAABBox;      // bounding box
	Mat4d   mXform;       // transformation
public:
	// vertices
	vector<Vec3f>    mPosition;         // position
	vector<Vec3f>    mTexCoord;         // texture coordinate
	vector<Vec3f>    mNormal;           // normal vector
	vector<Vec3f>    mTangent;          // tagent vector
	// triangles
	vector<Vec3i>    mTriangles;        // triangle indices
	vector<Vec3i>    mTriTexIndices;    // triangle texture coord indices
	vector<Vec3i>    mTriNormalIndices; // triangle normal vector indices
	vector<Vec3f>    mTriFaceNormal;    // triangle face normal
	vector<float>    mTriFaceArea;      // triangle face normal
	// augmented vertices
	vector<VtxStar>  mVtxStars;
public:
	// if not presented, create them
	int needAABBox();
	int needEdges();
	int needVtxStars();
	int needVtxNormals();
	int needBoundaryEdges();
	int needBoundary();
	int needManifold();
	int needFaceNormals();
	int needFaceAreas();
	int needLaplacian(int type);
	// Delete everything
	void clear();
public:
	Vec3f calcFaceNormal(int f) const;
	HalfEdge* findHalfEdge(int vStart, int vEnd);
};

// for mesh io and processing algorithms;
class MeshData
{
public:
	MeshData() { Reset(); }
	void Reset();

public:
	vector<Vec3f>    *position;         // position
	vector<Vec3f>    *texCoord;         // texture coordinate
	vector<Vec3f>    *normal;           // normal vector
	vector<Vec3f>    *tangent;          // tagent vector
	vector<Vec3i>    *triangles;        // triangle vertex indices
	vector<Vec3i>    *triTexIndices;    // triangle texture coord indices
	vector<Vec3i>    *triNormalIndices; // triangle normal vector indices
	vector<IntList>  *polygons;         // polygon vertex indices
	vector<IntList>  *polyTexIndices;   // polygon texture coord indices
	vector<IntList>  *polyNormalIndices;// polygon normal vector indices
	vector<Vec3f>    *faceNormal;       // face normal vector
	vector<VtxStar>  *vtxStars;
};

} // namespace xglm {

#endif // #ifndef ___mesh_doc_h________________