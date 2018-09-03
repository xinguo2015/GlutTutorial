/* bvh.h 
 *
 *    Structures BVH
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

#ifndef __INC_BVH_H
#define __INC_BVH_H


// Un canal
struct Channel {
  //Le type du canal
  enum Type {
    XPOSITION,  //Position absolue par rapport à Ex
    YPOSITION,  // "        "      "    "      " Ey
    ZPOSITION,  // "        "      "    "      " Ez
    ZROTATION,  //Rotation autour de l'axe 0z
    XROTATION,  // idem pour Ox
    YROTATION   // idem pour Oy
  } type;
  //Les données du canal en fonction du temps
  float *data;
};


// Un noeud
struct Node {
  enum {
    HEAD,
    NECK,
    CHEST,
    
    HIPS,
    
    LEFTCOLLAR,
    LEFTSHOULDER,
    LEFTELBOW,
    LEFTWRIST,
    
    LEFTHIP,
    LEFTKNEE,
    LEFTANKLE,
    LEFTFOOT,
    
    RIGHTCOLLAR,
    RIGHTSHOULDER,
    RIGHTELBOW,
    RIGHTWRIST,
    
    RIGHTHIP,
    RIGHTKNEE,
    RIGHTANKLE,
    RIGHTFOOT,
    
    UNINTERESSANT,
    
    UNKNOWN
  } type;
  //char *name;         //Le nom du noeud (ie nom de l'articulation ou de l'os)
  float x, y, z;            //OFFSET (coordonnées du vecteur)
  int nb_channels;          //Le nombre de degrés de liberté
  struct Channel *channels; //Tableau des canaux
  int nb_childs;            //Nombre de sous-noeuds
  struct Node *childs;      //Tableau des sous-noeuds
};


// Un fichier BVH
struct BvhFile {
  //Le noeud racine
  struct Node *rootnode;
  //Liste de pointeurs vers les canaux 
  struct Channel **channelslist;
  //Nombre total de canaux (ie nombre total de degres de liberté,
  // tous membres confondus)
  int nb_channels;
  //Nombre de frames
  int nb_frames;
  //Durée d'une frame en secondes
  long int frame_time;
};


#endif //__INC_BVH_H

/* END OF bvh.h */
