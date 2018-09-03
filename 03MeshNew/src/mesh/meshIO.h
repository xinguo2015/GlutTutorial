#ifndef _____mesh_io_h_____________
#define _____mesh_io_h_____________

#include <vector>
#include <string>
#include <iostream>

namespace xglm {
	
using namespace std;
	
template<typename TMesh>
class MeshIO 
{
public:
	static int readMesh(const string & filename, TMesh & mesh);
	static int readOBJ(istream & inputstream, TMesh & mesh);
};
	
} // namespace xglm {

#endif //#ifndef _____mesh_io_h_____________
