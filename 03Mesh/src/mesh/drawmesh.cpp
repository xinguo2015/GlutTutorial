
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
			const int * v =    triangle[k].get_value();
			const int * n = normalIndex[k].get_value();
			glNormal3fv(  normal[n[0]].get_value());
			glVertex3fv(position[v[0]].get_value());
			glNormal3fv(  normal[n[1]].get_value());
			glVertex3fv(position[v[1]].get_value());
			glNormal3fv(  normal[n[2]].get_value());
			glVertex3fv(position[v[2]].get_value());
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
			const int * v = triangle[k].get_value();
			if( normal.size()==triangle.size() ) 
			{	// normal per face
				glNormal3fv(  normal[ k  ].get_value());
				glVertex3fv(position[v[0]].get_value());
				glVertex3fv(position[v[1]].get_value());
				glVertex3fv(position[v[2]].get_value());
			}
			else// if( normal.size()==position.size() )
			{	// normal per vertex
				glNormal3fv(  normal[v[0]].get_value());
				glVertex3fv(position[v[0]].get_value());
				glNormal3fv(  normal[v[1]].get_value());
				glVertex3fv(position[v[1]].get_value());
				glNormal3fv(  normal[v[2]].get_value());
				glVertex3fv(position[v[2]].get_value());
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
			const int * t = triangle[k].get_value();
			p.pack(2,k);
			glColor4ubv((unsigned char*)p);
			glVertex3fv(position[t[0]].get_value());
			glVertex3fv(position[t[1]].get_value());
			glVertex3fv(position[t[2]].get_value());
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
			glVertex3fv(points[k].get_value());
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
			glVertex3fv(points[idx].get_value());
			glEnd();
			//printf("cursor points\n");
		}
		else if( typ==2 && idx<triangles.size() ) {  // face
			const int * t = triangles[idx].get_value();
			glPointSize(15);
			glBegin(GL_TRIANGLES);
			glColor4ub(255,0,0,0);
			for( int k = 0; k<3; k++ )
				glVertex3fv(points[t[k]].get_value());
			glEnd();
			//printf("cursor triangle\n");
		}
	}
////////////////////
} //namespace xglm {

