
#include "mesh/trimesh.h"
#include "mesh/meshIO.h"

#include "demo.h"

GLUTApp<MyView> theAPP("GLUT Template", 800, 600);

int main (int argc, char *argv[])
{
	// to see the ouput immediately for debugging
	setbuf(stdout, NULL);
	// initialize glut
	glutInit(&argc, argv);
	// create app window
	if ( !theAPP.createWindow(argc,argv) ) {
		printf("Failed creating the window\n");
		return -1;
	}
	// load mesh
	if( argc>1 ) {
		TriMesh mesh;
		MeshDoc mdoc;
		if( MeshIO<TriMesh>::readMesh(argv[1], mesh) ) {
			mesh.needAABBox();
			mesh.needFaceNormals();
			mesh.needVtxNormals();
			mdoc.setMesh( &mesh );
			theAPP.getView()->setShape( &mdoc );
			theAPP.getView()->setupGL();
		}
	}

	// binding the call back functions
	glutReshapeFunc( theAPP.cbReshape );
	glutDisplayFunc( theAPP.cbDisplay ); 
	// binding mouse functions
	glutMouseFunc(  theAPP.cbMouse ); 
	glutMotionFunc( theAPP.cbMotion ); 
	glutPassiveMotionFunc( theAPP.cbPassiveMotion ); 
	// binding keyboard functions
	glutKeyboardFunc(   theAPP.cbKeyboard );
	glutKeyboardUpFunc( theAPP.cbKeyboardUp );
	glutSpecialFunc(    theAPP.cbSpecial );
	glutSpecialUpFunc(  theAPP.cbSpecialUp );

	// running the application
	glutMainLoop();
    return 0;
}
