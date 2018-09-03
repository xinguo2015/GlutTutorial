
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "drawmesh.h"

namespace xglm {
	
	// draw smoothed group
	void DrawMesh::Face(
		const vector<Vec3i>& triangle,
		const vector<Vec3i>& normalIndex,
		const vector<Vec3f>& position,
		const vector<Vec3f>& normal)
	{
		glBegin(GL_TRIANGLES);
		xglm_assert(triangle.size()==normalIndex.size(),"wrong input");
		for( std::size_t k=0; k<triangle.size(); k++ )
		{
			const int * v = glm::value_ptr(triangle[k]);
			const int * n = glm::value_ptr(normalIndex[k]);
			glNormal3fv( glm::value_ptr(normal[n[0]])); glVertex3fv( glm::value_ptr(position[v[0]]));
			glNormal3fv( glm::value_ptr(normal[n[1]])); glVertex3fv( glm::value_ptr(position[v[1]]));
			glNormal3fv( glm::value_ptr(normal[n[2]])); glVertex3fv( glm::value_ptr(position[v[2]]));
		}
		glEnd();
	}
	
	// 1. facet normal  - normal.size()  = triangle.size()
	// 2. vertex normal - normal.size() >= vertex.size()
	void DrawMesh::Face(
		const vector<Vec3i>& triangle, 
		const vector<Vec3f>& position, 
		const vector<Vec3f>& normal) 
	{
		xglm_assert(triangle.size()==normal.size() || position.size()<=normal.size(),
					"wrong input");
		glBegin(GL_TRIANGLES);
		for( std::size_t k=0; k<triangle.size(); k++ )
		{
			const int * v = glm::value_ptr(triangle[k]);
			if( normal.size()==triangle.size() ) 
			{	// normal per face
				glNormal3fv(glm::value_ptr(  normal[ k  ]));
				glVertex3fv(glm::value_ptr(position[v[0]]));
				glVertex3fv(glm::value_ptr(position[v[1]]));
				glVertex3fv(glm::value_ptr(position[v[2]]));
			}
			else// if( normal.size()==position.size() )
			{	// normal per vertex
				glNormal3fv(glm::value_ptr(  normal[v[0]]));
				glVertex3fv(glm::value_ptr(position[v[0]]));
				glNormal3fv(glm::value_ptr(  normal[v[1]]));
				glVertex3fv(glm::value_ptr(position[v[1]]));
				glNormal3fv(glm::value_ptr(  normal[v[2]]));
				glVertex3fv(glm::value_ptr(position[v[2]]));
			}
		}
		glEnd();
	}
	
	void DrawMesh::FaceID(
		const vector<Vec3i>& triangle, 
		const vector<Vec3f>& position)
	{
		PixelInfo p;
		glBegin(GL_TRIANGLES);
		for( std::size_t k=0; k<triangle.size(); k++ )
		{
			const int * t = glm::value_ptr(triangle[k]);
			p.pack(2,k);
			glColor4ubv((unsigned char*)p);
			glVertex3fv(glm::value_ptr(position[t[0]]));
			glVertex3fv(glm::value_ptr(position[t[1]]));
			glVertex3fv(glm::value_ptr(position[t[2]]));
		}
		glEnd();
	}
	
	void DrawMesh::PointID(
		const vector<Vec3f>& points)
	{
		PixelInfo p;
		glBegin(GL_POINTS);
		for( std::size_t k=0; k<points.size(); k++ )
		{
			p.pack(1,k);
			glColor4ubv((unsigned char*)p);
			glVertex3fv(glm::value_ptr(points[k]));
		}
		glEnd();
	}
	
	void DrawMesh::Picked( PixelInfo pickedID,
		const vector<Vec3i>& triangles, 
		const vector<Vec3f>& points)
	{
		unsigned int typ = pickedID.getType();
		unsigned int idx = pickedID.getIndex();
		if( typ==1 && idx<points.size() ) { // vertex
			glBegin(GL_POINTS);
			glColor4ub(255,0,0,0);
			glVertex3fv(glm::value_ptr(points[idx]));
			glEnd();
			//printf("cursor points\n");
		}
		else if( typ==2 && idx<triangles.size() ) {  // face
			const int * t = glm::value_ptr(triangles[idx]);
			glPointSize(15);
			glBegin(GL_TRIANGLES);
			glColor4ub(255,0,0,0);
			for( int k = 0; k<3; k++ )
				glVertex3fv(glm::value_ptr(points[t[k]]));
			glEnd();
			//printf("cursor triangle\n");
		}
	}
////////////////////
} //namespace xglm {

