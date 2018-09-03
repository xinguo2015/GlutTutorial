
#ifndef __xglm_h_________________________
#define __xglm_h_________________________

#include <stdio.h>
#include <vector>
#include <map>

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "marcos.h"


// print some log information

#ifdef _DEBUG
#define xglm_debug(format,...) printf("%s() in %s, line - %d)\n\t",__FUNCTION__, __FILE__, __LINE__), printf(format, ##__VA_ARGS__)
#define xglm_assert(a,format,...) if ( !(a) ) xglm_debug(format,##__VA_ARGS__)
#else
#define xglm_debug(format,...) 
#define xglm_assert(a,format,...)
#endif


namespace xglm {
	typedef glm::ivec2 Vec2i;
	typedef glm::ivec3 Vec3i;
	typedef glm::ivec4 Vec4i;

	typedef glm::uvec2 Vec2ui;
	typedef glm::uvec3 Vec3ui;
	typedef glm::uvec4 Vec4ui;

	typedef glm::vec2 Vec2f;
	typedef glm::vec3 Vec3f;
	typedef glm::vec4 Vec4f;
	typedef glm::mat3 Mat3f;
	typedef glm::mat4 Mat4f;

	typedef glm::dvec2 Vec2d;
	typedef glm::dvec3 Vec3d;
	typedef glm::dvec4 Vec4d;
	typedef glm::dmat3 Mat3d;
	typedef glm::dmat4 Mat4d;
	
	typedef glm::quat  Quatf;
	typedef glm::dquat Quatd;

	
	typedef std::vector<int>    IntArray;
	typedef std::vector<float>  FloatArray;
	typedef std::vector<double> DoubleArray;

	typedef std::vector<Vec3i>  V3iArray;
	typedef std::vector<Vec3f>  V3fArray;
	typedef std::vector<Vec3d>  V3dArray;

} //namespace xglm {

#endif //#ifndef __xglm_h_________________________