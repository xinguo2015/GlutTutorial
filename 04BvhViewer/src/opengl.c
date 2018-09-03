/* opengl.c
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

#include <GL/glut.h>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif //_WIN32
#include "bvh.h"


int g_glWindow;
struct BvhFile *g_glpbvh;
int g_cur_frame = 0;
long int g_gltime;
float g_glx = 0, g_gly = 0, g_glz = -30, g_glp = 0, g_glq = 0, g_glr = 0;


/* Retourne un temps en ms */

long int Time(void)
{
#ifdef _WIN32
  return GetTickCount();
#else
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv,&tz);
  return tv.tv_sec * 1000000 + tv.tv_usec;
#endif //_WIN32
}


/* Montre le squelette dans sa position initiale */

void ShowNode(struct Node *node)
{
  int i;
  
  //Sauvegarde la matrice
  glPushMatrix();
  //Trace le vecteur
  glBegin(GL_LINES);
  glVertex3i(0,0,0);
  glVertex3f(node->x,node->y,node->z);
  glEnd();
  //Translate tous les vecteurs suivants du vecteur du noeud
  glTranslatef(node->x,node->y,node->z);
  //Dessine les sous-noeuds
  for(i=0;i<node->nb_childs;i++) {
    ShowNode(&node->childs[i]);
  }
  //Remet la matrice sauvegardee
  glPopMatrix();
}


void GLShowBones(struct BvhFile *pbvh)
{
  ShowNode(pbvh->rootnode);
}

/* Montre le squelette en mouvement */

void ShowNodeMotion(struct Node *node, int frame)
{
  int i;
  
  //Sauvegarde la matrice
  glPushMatrix();	
  //Applique les transformations 
  // (dans l'ordre: impératif car il se peut que AB<>BA)
  for(i=0;i<node->nb_channels;i++) {
    switch(node->channels[i].type) {
    case XPOSITION:
      //Translation de x * Ex
      glTranslatef(node->channels[i].data[frame],0,0);
      break;
    case YPOSITION:
      //Translation de y * Ey
      glTranslatef(0,node->channels[i].data[frame],0);
      break;
    case ZPOSITION:
      //Translation de z * Ez
      glTranslatef(0,0,node->channels[i].data[frame]);
      break;
    case ZROTATION:
      //Rotation autour de l'axe 0z
      glRotatef(node->channels[i].data[frame],0,0,1);
      break;
    case YROTATION:
      //Rotation autour de l'axe 0y
      glRotatef(node->channels[i].data[frame],0,1,0);
      break;
    case XROTATION:
      //Rotation autour de l'axe 0x
      glRotatef(node->channels[i].data[frame],1,0,0);
    }
  }
  //Trace le vecteur
  glBegin(GL_LINES);
  glVertex3i(0,0,0);
  glVertex3f(node->x,node->y,node->z);
  glEnd();
  //Translate les vecteurs suivants
  glTranslatef(node->x,node->y,node->z);
  //Trace les sous-noeuds
  for(i=0;i<node->nb_childs;i++) {
    ShowNodeMotion(&node->childs[i],frame);
  }
  //Remet l'ancienne matrice
  glPopMatrix();
}


void GLShowMotion(struct BvhFile *pbvh)
{
  long int t;
  
  ShowNodeMotion(pbvh->rootnode,g_cur_frame);
  t = Time();
  if(t - g_gltime >= pbvh->frame_time) {
    g_cur_frame++;
    if(g_cur_frame>pbvh->nb_frames-1) {
      g_cur_frame = 0;
    }
    g_gltime = t;
  }
}


/* Fonctions OpenGL */

void GLReshape(int width, int height)  //Redimentionnement de la fenetre
{
  glViewport(0,0,width,height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45,(float)width/(float)height,1,100);
  glMatrixMode(GL_MODELVIEW);
}


void GLDraw(void)  //Dessin de la fenetre
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glRotatef(g_glp,1,0,0);
  glRotatef(g_glq,0,1,0);
  glRotatef(g_glr,0,0,1);
  glTranslatef(g_glx, g_gly, g_glz);
  //glTranslated(0,0,-30);
  if(g_glpbvh) {
    //GLShowBones(g_glpbvh);
    GLShowMotion(g_glpbvh);
  }
  glutSwapBuffers();
  glutPostRedisplay();
}


void GLInit(int argc, char *argv[])  //Initialisation
{
  g_glpbvh = NULL;
  
  glutInit(&argc,argv);
  glutInitWindowSize(640,480);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  
  g_glWindow = glutCreateWindow("BvhViewer");
  
  glutReshapeFunc(GLReshape);
  glutDisplayFunc(GLDraw);
}


int GLLoop(void)  //Loop
{
  glutMainLoop();
  return 0;
}


void GLSetBvhFile(struct BvhFile *pbvh)   //SetBvhFile
{
  g_glpbvh = pbvh;
  g_gltime = Time();
}

/* END OF opengl.c */
