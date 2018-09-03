/* bvhreader.c
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bvh.h"
#include "safe_malloc.h"


#define BUF_SIZE     64  //Taille du buffer
#define MAX_CHILDS   16  //Nombre maximum de sous-noeuds pour un noeud donné
#define MAX_CHANNELS 128 //Nb max de channels pour tout le fichier


/* * * * * * * * * * * * * * * * * * * * * * * * *
 *                                               *
 * FONCTIONS RELATIVES A LA LECTURE DE FICHIERS  *
 *                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * */

/* ReadNextToken:
 *    Lit le mot suivant dans le fichier
 */
void ReadNextToken(FILE *f, char *buf, int max_size)
{
  char c, *p;
  p = buf;
	
  //Passe les blancs
  while((fread(&c,1,1,f)==1)&&((c==' ')||(c=='\n')||(c=='\r')||(c=='\t')));
  *p = c;
  p++;
  //Lit le mot
  while(!((fread(&c,1,1,f)!=1)||(c==' ')||(c=='\n')||(c=='\r')||(c=='\t')||
          (p>buf+max_size-2))) {
    *p = c;
    p++;
  }
  *p = 0;
}


/* * * * * * * * * * * * * * * * * * * * * * *
 *                                           *
 * FONCTIONS RELATIVES A LA PARTIE HIERARCHY *
 *                                           *
 * * * * * * * * * * * * * * * * * * * * * * */

/* NameToType:
 *    Retourne le type de noeud en fonction de son nom
 */
int NameToType(char *name)
{
  if(!strcmp(name,"Head"))
    return HEAD;
  else if(!strcmp(name,"Neck"))
    return NECK;
  else if(!strcmp(name,"Chest"))
    return CHEST;
  else if(!strcmp(name,"Hips"))
    return HIPS;
	
  else if(!strcmp(name,"LeftCollar"))
    return LEFTCOLLAR;
  else if((!strcmp(name,"LeftShoulder"))||(!strcmp(name,"LeftUpArm")))
    return LEFTSHOULDER;
  else if((!strcmp(name,"LeftElbow"))||(!strcmp(name,"LeftLowArm")))
    return LEFTELBOW;
  else if((!strcmp(name,"LeftWrist"))||(!strcmp(name,"LeftHand")))
    return LEFTWRIST;
	
  else if((!strcmp(name,"LeftHip"))||(!strcmp(name,"LeftUpLeg")))
    return LEFTHIP;
  else if((!strcmp(name,"LeftKnee"))||(!strcmp(name,"LeftLowLeg")))
    return LEFTKNEE;
  else if((!strcmp(name,"LeftAnkle"))||(!strcmp(name,"LeftFoot")))
    return LEFTANKLE;
  else if(!strcmp(name,"LFoot"))
    return LEFTFOOT;

  else if(!strcmp(name,"RightCollar"))
    return RIGHTCOLLAR;
  else if((!strcmp(name,"RightShoulder"))||(!strcmp(name,"RightUpArm")))
    return RIGHTSHOULDER;
  else if((!strcmp(name,"RightElbow"))||(!strcmp(name,"RightLowArm")))
    return RIGHTELBOW;
  else if((!strcmp(name,"RightWrist"))||(!strcmp(name,"RightHand")))
    return RIGHTWRIST;
	
  else if((!strcmp(name,"RightHip"))||(!strcmp(name,"RightUpLeg")))
    return RIGHTHIP;
  else if((!strcmp(name,"RightKnee"))||(!strcmp(name,"RightLowLeg")))
    return RIGHTKNEE;
  else if((!strcmp(name,"RightAnkle"))||(!strcmp(name,"RightFoot")))
    return RIGHTANKLE;
  else if(!strcmp(name,"RFoot"))
    return RIGHTFOOT;
	
  else if((!strcmp(name,"neckDummy"))||(!strcmp(name,"Abdomen")))
    return UNINTERESSANT;

  else
    printf("DEBUG: %s is UNKNOWN\n",name);
  return UNKNOWN;
}

/* TypeToName:
 *    Le contraire
 */
char *TypeToName(int type)
{		
  switch(type) {
  case HEAD:
    return "Head";
  case NECK:
    return "Neck";
  case CHEST:
    return "Chest";
  case HIPS:
    return "Hips";
  case LEFTCOLLAR:
    return "LeftCollar";
  case LEFTSHOULDER:
    return "LeftShoulder";
  case LEFTELBOW:
    return "LeftElbow";
  case LEFTWRIST:
    return "LeftWrist";
  case LEFTHIP:
    return "LeftHip";
  case LEFTKNEE:
    return "LeftKnee";
  case LEFTANKLE:
    return "LeftAnkle";
  case LEFTFOOT:
    return "LeftFoot";
  case RIGHTCOLLAR:
    return "RightCollar";
  case RIGHTSHOULDER:
    return "RightShoulder";
  case RIGHTELBOW:
    return "RightElbow";
  case RIGHTWRIST:
    return "RightWrist";
  case RIGHTHIP:
    return "RightHip";
  case RIGHTKNEE:
    return "RightKnee";
  case RIGHTANKLE:
    return "RightAnkle";
  case RIGHTFOOT:
    return "RightFoot";
  case UNINTERESSANT:
    return "Uninteressant";
  }
  return "Unknown";
}

/* FreeNode:
 *    Libere la memoire du noeud et de ses sous noeuds
 */
void FreeNode(struct Node *node)
{
  int i;

  //printf("%s %d\n",node->name, node->nb_childs);
  for(i=0;i<node->nb_childs;i++) {
    FreeNode(&node->childs[i]);
  }
  if(node->nb_childs)
    free(node->childs);
  for(i=0;i<node->nb_channels;i++) {
    free(node->channels[i].data);
  }
  if(node->channels)
    free(node->channels);
}

/* ReadNode:
 *   Lit le noeud suivant ainsi que tous ses sous-noeuds
 */
int ReadNode(FILE *f, struct Node *node, int root, struct BvhFile *pbvh)
{
  char buf[BUF_SIZE];
  int i,r;
	
  node->nb_channels = 0;
  node->nb_childs = 0;

  //ROOT or JOINT
  ReadNextToken(f,buf,sizeof(buf));
  if(!( ((!strcmp(buf,"ROOT"))&&(root)) ||
        ((!strcmp(buf,"JOINT"))&&(!root)) )) {
    //End
    if(!strcmp(buf,"End")) {
      //Site
      ReadNextToken(f,buf,sizeof(buf));
      if(strcmp(buf,"Site")) {
        return -1;
      }
      //{
      ReadNextToken(f,buf,sizeof(buf));
      if(strcmp(buf,"{")) {
        return -2;
      }
      //} or OFFSET
      ReadNextToken(f,buf,sizeof(buf));
      if(!strcmp(buf,"OFFSET")) {
        // [x_float]
        ReadNextToken(f,buf,sizeof(buf));
        // [y_float]
        ReadNextToken(f,buf,sizeof(buf));
        // [z_float]
        ReadNextToken(f,buf,sizeof(buf));
        //}
        ReadNextToken(f,buf,sizeof(buf));
      }
      if(strcmp(buf,"}")) {
        return -3;
      }
      return 1;
    } else {
      return -4;
    }
  }
  // [Name]
  ReadNextToken(f,buf,sizeof(buf));
  //node->name = strdup(buf);
  node->type = NameToType(buf);
  //{
  ReadNextToken(f,buf,sizeof(buf));
  if(strcmp(buf,"{")) {
    return -3;
  }
  //OFFSET
  ReadNextToken(f,buf,sizeof(buf));
  if(strcmp(buf,"OFFSET")) {
    return -4;
  }
  // [x_float]
  ReadNextToken(f,buf,sizeof(buf));
  node->x = (float)atof(buf);
  // [y_float]
  ReadNextToken(f,buf,sizeof(buf));
  node->y = (float)atof(buf);
  // [z_float]
  ReadNextToken(f,buf,sizeof(buf));
  node->z = (float)atof(buf);
  // CHANNELS
  ReadNextToken(f,buf,sizeof(buf));
  if(strcmp(buf,"CHANNELS")) {
    return -5;
  }
  // [nb_channels]
  ReadNextToken(f,buf,sizeof(buf));
  node->nb_channels = atoi(buf);
  if(node->nb_channels<1) {
    return -6;
  }
  // [channels_list]
  node->channels = (struct Channel *)safe_malloc(sizeof(struct Channel) * 
                                                 node->nb_channels );
  for(i=0;i<node->nb_channels;i++) {
    pbvh->channelslist[pbvh->nb_channels] = &node->channels[i];
    pbvh->nb_channels++;
    ReadNextToken(f,buf,sizeof(buf));
    if(!strcmp(buf,"Xposition"))
      node->channels[i].type = XPOSITION;
    else if(!strcmp(buf,"Yposition"))
      node->channels[i].type = YPOSITION;
    else if(!strcmp(buf,"Zposition"))
      node->channels[i].type = ZPOSITION;
    else if(!strcmp(buf,"Zrotation"))
      node->channels[i].type = ZROTATION;
    else if(!strcmp(buf,"Xrotation"))
      node->channels[i].type = XROTATION;
    else if(!strcmp(buf,"Yrotation"))
      node->channels[i].type = YROTATION;
    else {
      free(node->channels);
      return -8;
    }
  }
  node->childs = (struct Node *)safe_malloc(sizeof(struct Node)*MAX_CHILDS);
  node->nb_childs = MAX_CHILDS;
  for(i=0;i<MAX_CHILDS;i++) {
    r = ReadNode(f,&node->childs[i],0,pbvh);
    if(r) break;
  }
  if(r==1) {
    node->nb_childs = 0;
    free(node->childs);
    node->childs = NULL;
    //}
    ReadNextToken(f,buf,sizeof(buf));
    if(strcmp(buf,"}")) {
      free(node->channels);
      node->channels = NULL;
      return -8;
    }
  } else {
    node->nb_childs = i;
    node->childs = (struct Node*)safe_realloc(node->childs,sizeof(struct Node)
                                              *node->nb_childs);
  }
  
  return 0;
}

/* ReadHierarchy:
 *    Lit la hierarchie du fichier
 */
int ReadHierarchy(FILE *f, struct BvhFile *pbvh)
{
  char buf[BUF_SIZE];
  
  ReadNextToken(f,buf,sizeof(buf));
  if(strcmp(buf,"HIERARCHY")) {
    return -1;
  }
  pbvh->rootnode = (struct Node*)safe_malloc(sizeof(struct Node));
  if(ReadNode(f,pbvh->rootnode,1,pbvh)) {
    FreeNode(pbvh->rootnode);
    return -2;
  }
  
  return 0;
}


/* * * * * * * * * * * * * * * * * * * * * *
 *                                         *
 * FONCTIONS RELATIVES A LA PARTIE MOTION  *
 *                                         *
 * * * * * * * * * * * * * * * * * * * * * */

/* ReadMotion:
 *    Lit la partie motion (la liste des mouvements)
 */
int ReadMotion(FILE *f, struct BvhFile *pbvh)
{
  char buf[BUF_SIZE];
  int i,j;
  float float_frame_time;
	
  //MOTION
  ReadNextToken(f,buf,sizeof(buf));
  if(strcmp(buf,"MOTION")) {
    return -1;
  }
  //Frames:
  ReadNextToken(f,buf,sizeof(buf));
  if(strcmp(buf,"Frames:")) {
    return -2;
  }
  //[nb_frames]
  ReadNextToken(f,buf,sizeof(buf));
  pbvh->nb_frames = atoi(buf);
  //Frame
  ReadNextToken(f,buf,sizeof(buf));
  if(strcmp(buf,"Frame")) {
    return -3;
  }
  //Time:
  ReadNextToken(f,buf,sizeof(buf));
  if(strcmp(buf,"Time:")) {
    return -4;
  }
  //[frame_time]
  ReadNextToken(f,buf,sizeof(buf));
  float_frame_time = (float)atof(buf);
  pbvh->frame_time = (int)(float_frame_time*1000);
  //[DATA]
  pbvh->channelslist = (struct Channel**)safe_realloc(pbvh->channelslist,
                                                      sizeof(struct Channel*)*
                                                      pbvh->nb_channels);
  for(j=0;j<pbvh->nb_channels;j++) {
    pbvh->channelslist[j]->data = (float*)safe_malloc(sizeof(float)*
                                                      pbvh->nb_frames);
  }
  for(i=0;i<pbvh->nb_frames;i++) {
    for(j=0;j<pbvh->nb_channels;j++) {
      ReadNextToken(f,buf,sizeof(buf));
      if((feof(f))&&(i<pbvh->nb_frames-1)&&(j<pbvh->nb_channels-1)) {
        return -5;
      }
      pbvh->channelslist[j]->data[i] = (float)atof(buf);
    }
  }
  return 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                           *
 * FONCTIONS RELATIVES A WriteHierarchy (pour le debuggage)  *
 *                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Ident:
 *   idente le fichier
 */
void Ident(FILE *f, int level)
{
  int i;
  for(i=0;i<level;i++) fprintf(f,"   ");
}

/* DisplayHierarchy:
 *    Ecrit le noeud et ses sous-noeuds dans un fichier
 */
void DisplayHierarchy(FILE *f, struct Node *node, int level)
{
  int i;
	
  Ident(f,level);
  fprintf(f,"%s (%f,%f,%f) {\n",
          TypeToName(node->type),node->x,node->y,node->z);
  //	fprintf(f,"%s (%f,%f,%f) {\n",node->name,node->x,node->y,node->z);
  for(i=0;i<node->nb_childs;i++) {
    DisplayHierarchy(f,&node->childs[i],level+1);
  }
  Ident(f,level);
  fprintf(f,"}\n");
}

/* WriteHierarchy:
 *    Ecrit la hierarchie du fichier dans un fichier de debuggage
 */
int WriteHierarchy(const char *filename, struct BvhFile *pbvh)
{
  FILE *f;
  f = fopen(filename,"w");
  if(!f)
    return -1;
  DisplayHierarchy(f,pbvh->rootnode,0);
  fclose(f);
  return 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                             *
 * FONCTIONS RELATIVES A LA REDUCTION PAR UN FACTEUR D'ECHELLE *
 *                                                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* NodeDiv:
 *    Divise le noeud et ses sous-noeuds par un facteur de div
 */
void NodeDiv(struct Node *node, float div)
{
  int i;

  node->x /= div;
  node->y /= div;
  node->z /= div;
  for(i=0;i<node->nb_childs;i++) {
    NodeDiv(&node->childs[i], div);
  }
}

/* DivByTen:
 *   Reduit la taille des vecteurs et de la position par 10
 */
void DivByTen(struct BvhFile *pbvh)
{
  int i,j;

  NodeDiv(pbvh->rootnode, (float)10);
  for(i=0;i<pbvh->nb_frames;i++) {
    for(j=0;j<pbvh->nb_channels;j++) {
      if((pbvh->channelslist[j]->type==XPOSITION)||
         (pbvh->channelslist[j]->type==YPOSITION)||
         (pbvh->channelslist[j]->type==ZPOSITION)) {
        pbvh->channelslist[j]->data[i] /= (float)10;
      }
    }
  }
}


/* * * * * * * * * * * *
 *                     *
 * FONCTIONS GENERALES *
 *                     *
 * * * * * * * * * * * */

/* ReadBVH:
 *    Lit la partie HIERARCHY et MOTION d'un fichier .BVH (BioVision)
 */
int ReadBVH(const char *filename, struct BvhFile *pbvh)
{
  FILE *f;
	
  f = fopen(filename,"r");
  if(!f) return -1;
	
  if(pbvh->channelslist) free(pbvh->channelslist);
  pbvh->channelslist = (struct Channel**)safe_malloc(sizeof(struct Channel*) *
                                                     MAX_CHANNELS);
  pbvh->nb_channels = 0;
	
  if(ReadHierarchy(f,pbvh)) {
    fclose(f);
    return -2;
  }
	
  if(ReadMotion(f,pbvh)) {
    fclose(f);
    return -3;
  }
  WriteHierarchy("hierarchy.txt",pbvh);
  fclose(f);
  return 0;
}

/* FreeBvh:
 *    Libere la structure BvhFile
 */
void FreeBvh(struct BvhFile *pbvh)
{
  if(!pbvh)
    return;
  if(pbvh->rootnode)
    FreeNode(pbvh->rootnode);
  free(pbvh->rootnode);
  if(pbvh->channelslist)
    free(pbvh->channelslist);
  free(pbvh);
}

/* CreateBvh:
 *    Cree une structure BvhFile
 */
struct BvhFile *CreateBvh(void)
{
  struct BvhFile *pbvh;
  pbvh = (struct BvhFile *)safe_malloc(sizeof(struct BvhFile));
  pbvh->nb_channels = 0;
  pbvh->nb_frames = 0;
  pbvh->channelslist = NULL;
  pbvh->rootnode = NULL;
  return pbvh;
}


/* END OF bvhreader.c */
