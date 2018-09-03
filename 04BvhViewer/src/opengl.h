/* opengl.h
 *
 *  Affichage de la motion capture en 3D grace à OpenGL (et grace aux matrices)
 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * BVHViewer !                              Copyright (C) 2002, AnTiKoNs *
 *-----------/                                                           *
 * This program is free software; you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation; either version 2 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * The author of this program may be contacted at antikons@ifrance.com   *
 *                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __INC_OPENGL_H
#define __INC_OPENGL_H

#include "bvh.h"

extern float g_glx, g_gly, g_glz, g_glp, g_glq, g_glr;


//Initialisation d'OpenGL
void GLInit(int argc, char *argv[]);
//Lancement de la boucle OpenGL
int GLLoop(void);
//Choix du fichier bvh
void GLSetBvhFile(struct BvhFile *pbvh);


#endif //__INC_OPENGL_H

/* END OF opengl.h */
