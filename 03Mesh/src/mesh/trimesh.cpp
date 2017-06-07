#include <math.h>
#include <cfloat>
#include "trimesh.h"
#include "xglm.h"

namespace xglm {
	
AABBox::AABBox()
:_lower(FLT_MAX, FLT_MAX, FLT_MAX), _upper(0,0,0)
{
}

bool AABBox::empty() const
{
	return (_lower.x==FLT_MAX);
}

int HEdgeList::edgeOf(int endVertex) const
{	
	vector<HalfEdge>::const_iterator f = std::find_if( begin(), end(), 
		[endVertex](const HalfEdge &a){ return a.endVtx()==endVertex; } );
	return ( f == end() ? (-1) : f-begin() );
}
int HEdgeList::nextEdgeOf(int endVertex) const
{
	vector<HalfEdge>::const_iterator f = std::find_if( begin(), end(), 
		[endVertex](const HalfEdge &a){ return a.endVtx()==endVertex; } );
	if( f == end() ) return (-1);
	return ( ++f == end() ? 0 : f-begin() );
}
int HEdgeList::prevEdgeOf(int endVertex) const
{
	vector<HalfEdge>::const_iterator f = std::find_if( begin(), end(), 
		[endVertex](const HalfEdge &a){ return a.endVtx()==endVertex; } );
	if( f == end() ) return (-1);
	return ( f == begin() ? size()-1 : f-begin()-1 );
}

int genVtxStars(
	const std::vector<Vec3i> & triangle,
	std::vector<VtxStar>     & vlink)
{
	const std::size_t FN = triangle.size();
	const std::size_t VN = vlink.size();
	std::for_each(vlink.begin(),vlink.end(),
		[](VtxStar& vL){ vL.resize(0); });
	// generate half edges of each triangle
	for( std::size_t k = 0; k<FN; k++ )
	{
		const Vec3i & f = triangle[k];
		for( int j=0; j<3; j++ )
		{
			xglm_assert(f[j]>=0 && f[j]<(int)VN, "vertex index out of range\n");
			vlink[f[j]].push_back(HalfEdge(f[(j+1)%3],k));
		}
	}
	return 1;
}

int genBoundaryEdges(
	const std::vector<Vec3i> & triangle,
	std::vector<VtxStar>     & vlink)
{
	int isManifold = 1;
	const std::size_t FN = triangle.size();
	const std::size_t VN = vlink.size();
	for( std::size_t k = 0; k<FN; k++ )
	{
		const Vec3i &f = triangle[k];
		for( int j=0; j<3; j++ )
		{
			int v1 = f[j];       // NOTE: inverse order
			int v0 = f[(j+1)%3]; // NOTE: inverse order
			xglm_assert(v0>=0&&v0<(int)VN&&v1>=0&&v1<(int)VN,"Index check error");
			// find half edge: v0->v1
			HEdgeList & es = vlink[v0];
			HEdgeList::iterator eFound = std::find_if(es.begin(),es.end(),
				[v1](const HalfEdge &h) { return h.endVtx()==v1; });
			// if not found, then add a boundary edge with leftFace = NullFace(-1)
			if( eFound==es.end() ) 
			{
				xglm_debug("Add a boundary edge(%d->%d | %d)\n", v0, v1, (int)k);
				vlink[v0].push_back(HalfEdge(v1,NullFace));
			}
			else if( eFound->leftFace()<0 )
			{
				// another face produce the same boundary
				xglm_debug("Multiple face generate the same boundary(%d->%d | %d)\n", v0, v1, (int)k);
				xglm_debug("Indicates non-manifold mesh");
				eFound->setLeftFace(eFound->leftFace()-1);
				isManifold = 0;
			}
		}
	}
	return isManifold;
}

void MeshData::Reset()
{
// 	position = NULL;
// 	texCoord = NULL;
// 	normal = NULL;
// 	tangent = NULL;
// 	triPosIndices = NULL;
// 	triTexIndices = NULL;
// 	triNormalIndices = NULL;
// 	polyPosIndices = NULL;
// 	polyTexIndices = NULL;
// 	polyNormalIndices = NULL;
	memset((void*)this,0,sizeof(MeshData));
}

void normalizeNormal(
	std::vector<Vec3f> & faceNormal,
	std::vector<float> & faceArea)
{
	const int FN = faceNormal.size();
	const int keepArea = (faceArea.size()==faceNormal.size());
	for( int k=0; k<FN; k++ ) 
	{
		float len = faceNormal[k].length();
		if( len>1.0e-6 )
			faceNormal[k] /= len;
		if( keepArea )
			faceArea[k] = len/2.f;
	}
}

int calcFaceNormals(
	const std::vector<Vec3f> & position,
	const std::vector<Vec3i> & triangle,
	std::vector<Vec3f> & faceNormal )
{
	const int FN = triangle.size();
	if( FN<1 ) return 0;
	faceNormal.resize( FN );
	for( int k=0; k<FN; k++ ) 
	{	
		const int *v = triangle[k].get_value();
		faceNormal[k] = (position[v[1]]-position[v[0]]).cross(
			(position[v[2]]-position[v[0]]));
	}
	return 1;
}

int TriMesh::needAABBox()
{
	if( mAABBox.empty() )
		mAABBox.set(&mPosition[0], mPosition.size());
	return 1;
}

int TriMesh::needEdges()
{
	mVtxStars.resize(mPosition.size());
	return genVtxStars(mTriangles,mVtxStars);
}

int TriMesh::needFaceNormals()
{
	calcFaceNormals( mPosition, mTriangles, mTriFaceNormal );
	mTriFaceArea.resize( mTriangles.size() );
	normalizeNormal( mTriFaceNormal, mTriFaceArea );
	return 1;
}

int TriMesh::needVtxNormals()
{
	const int FN = mTriangles.size();
	const int VN = mPosition.size();
	if( mTriFaceNormal.size()!=mTriangles.size() )
		needFaceNormals();
	mNormal.assign( VN, Vec3f(0,0,0) );
	for( int f=0; f<FN; f++ ) //visit all triangles
		for( int k=0; k<3; k++ ) // add face normal into vertex
		{
			xglm_assert(mTriangles[f][k]>=0 && mTriangles[f][k]<VN, "vertex index is out of range in TriMesh::needVtxNormals()\n");
			mNormal[mTriangles[f][k]] += mTriFaceNormal[f];
		}
	// normalize vertex normal
	for( int k=0; k<VN; k++ )
		mNormal[k].normalize();
	return 1;
}

int TriMesh::needVtxStars()
{
	const std::size_t VN = mPosition.size();
	if( mVtxStars.size()<VN )
	{
		needEdges();
		needBoundaryEdges();
	}
	return 1;
}

int TriMesh::needBoundaryEdges()
{
	return genBoundaryEdges(mTriangles, mVtxStars);
}

// manifold: (1) at most 1 simple boundary edge,
//           (2) no over lapped edge
int TriMesh::needManifold()
{
	const std::size_t VN = mPosition.size();
	if( mVtxStars.size()<VN )
		needBoundary();
	// identify boundary edges
	for( vector<VtxStar>::iterator k = mVtxStars.begin(); k<mVtxStars.end(); k++) {
		k->setManifoldFlag(0); // initialized as manifold
		if( k->boundaryFlag()>1 )
			k->setManifoldFlag(1); // not manifold
		else {
			const HEdgeList & eL = k->edges();
			for( HEdgeList::const_iterator e = eL.begin(); e<eL.end(); e++ )
			{
				int v1 = e->endVtx();
				// find overlapped edges
				HEdgeList::const_iterator f = std::find_if(++e,eL.end(),[v1](const HalfEdge &h) {
					return h.endVtx()==v1;
				});
				if( f<eL.end() ) { // found
					k->setManifoldFlag(1);  // not manifold
					break;
				}
			}
		}
	};
	
	return 1;
}

int TriMesh::needBoundary()
{
	const std::size_t VN = mPosition.size();
	if( mVtxStars.size()<VN )
		needVtxStars();
	// identify boundary edges
	for( vector<VtxStar>::iterator k = mVtxStars.begin(); k<mVtxStars.end(); k++) {
		HEdgeList & eL = k->edges();
		int bn = 0;
		std::for_each(eL.begin(), eL.end(), [& bn](const HalfEdge &e){ 
			if( e.leftFace()<0 ) 
				bn += e.leftFace(); 
		});
		k->setBoundaryFlag(-bn);
	};
	
	return 1;
}

// Normal of face f
Vec3f TriMesh::calcFaceNormal(int f) const
{	
	const int *v = mTriangles[f].get_value();
	return cross(
			mPosition[v[1]]-mPosition[v[0]],
			mPosition[v[2]]-mPosition[v[0]]);
}

HalfEdge* TriMesh::findHalfEdge(int vStart, int vEnd)
{
	xglm_assert(vStart>=0 && vStart<(int)mVtxStars.size(), "vStart is out of range");
	HEdgeList & eL = mVtxStars[vStart];
	vector<HalfEdge>::iterator e = std::find_if(eL.begin(), eL.end(), [vEnd](const HalfEdge& e) {
		return e.endVtx()==vEnd;
		});
	return (e==eL.end() ? NULL : &(*e));
}

} // namespace xglm {
