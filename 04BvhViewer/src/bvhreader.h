/* bvhreader.h
 *
 *      Lecture des fichiers .BVH (Format BioVision)
 *
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

#ifndef __INC_BVHREADER_H
#define __INC_BVHREADER_H

#include "bvh.h"

//Lit un fichier BVH
int ReadBVH(const char *filename, struct BvhFile *pbvh);
//Applique un facteur d'échelle de 10
void DivByTen(struct BvhFile *pbvh);
//Ecrit la hierarchie du fichier dans un fichier de debuggage
int WriteHierarchy(const char *filename, struct BvhFile *pbvh);
//Cree une structure BvhFile
struct BvhFile *CreateBvh(void);
//Libere la structure BVH
void FreeBvh(struct BvhFile *pbvh);


#endif //__INC_BVHREADER_H

/* END OF bvhreader.h */
