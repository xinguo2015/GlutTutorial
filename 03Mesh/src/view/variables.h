#ifndef __variables_h_________________________
#define __variables_h_________________________

#include <stdio.h>
#include <vector>
#include "Math3D/math.h"


#if (defined _WIN32) || (defined _WIN64)
#pragma warning(disable:4996) 
#pragma warning(disable:4244) 
#pragma warning(disable:4800) 
#endif

namespace xglm {

typedef std::vector<int>    IntArray;
typedef std::vector<float>  FloatArray;
typedef std::vector<double> DoubleArray;

typedef std::vector<Vec3i>  V3iArray;
typedef std::vector<Vec3f>  V3fArray;
typedef std::vector<Vec3d>  V3dArray;

class VarValue
{
	char v[sizeof(double)*16];
public:
	VarValue()          { memset(v,0,sizeof(v)); }
#define addmethod(Type) Type  get##Type() const { return *(const Type*)(const void*)v; } \
                        Type& get##Type()       { return *(Type*)(void*)v; } \
                        VarValue(Type value)    { get##Type()=value; }
	addmethod(char)
	addmethod(int)
	addmethod(unsigned)
	addmethod(float)
	addmethod(double)
	addmethod(Vec2ui)
	addmethod(Vec2i)
	addmethod(Vec2f)
	addmethod(Vec2d)
	addmethod(Vec3ui)
	addmethod(Vec3i)
	addmethod(Vec3f)
	addmethod(Vec3d)
	addmethod(Vec4ui)
	addmethod(Vec4i)
	addmethod(Vec4f)
	addmethod(Vec4d)
	addmethod(Quatf)
	addmethod(Quatd)
	addmethod(Mat4f)
	addmethod(Mat4d)
#undef addmethod
	const char* getstr() const  { return v; }
	const char* setstr(const char* s)
	{
		if( strlen(s)>sizeof(v)-1 ) {
			printf(__FILE__",line %d, The str is too long to store", __LINE__);
			return NULL;
		}
		strcpy(v,s);
		return v;
	}
};


class VarSet
{
public: 
	// access method
	VarValue & operator[](const std::string & key) { return _str_vs[key]; }
	VarValue & operator[]( int key               ) { return _int_vs[key]; }
	// access method with default value
	VarValue & get( int key, VarValue defaultValue ) {
		std::map<int,VarValue>::iterator p = _int_vs.find(key);
		if( p!=_int_vs.end() )
			return p->second;
		VarValue & v = _int_vs[key];
		v = defaultValue;
		return v; 
	}
	VarValue & get( const std::string & key, VarValue defaultValue ) {
		std::map<std::string,VarValue>::iterator p = _str_vs.find(key);
		if( p!=_str_vs.end() )
			return p->second;
		VarValue & v = _str_vs[key];
		v = defaultValue;
		return v; 
	}
	// check variable existence
	bool check(const std::string & key) const { return _str_vs.find(key)!=_str_vs.end(); }
	bool check(int                 key) const { return _int_vs.find(key)!=_int_vs.end(); }
protected:
	std::map<std::string,VarValue>   _str_vs;
	std::map<int,        VarValue>   _int_vs;
};

} //namespace xglm {

#endif //#ifndef __variables_h_________________________