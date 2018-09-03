/* main.c
 *
 *    Fonction 'main'
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

#ifdef _WIN32

#include <conio.h> //Les fonction d'E/S toutes connes
#include <windows.h>
#pragma warning (disable:4244)	/* Disable bogus conversion warnings. */
#pragma warning (disable:4305)  /* VC++ 5.0 version of above warning. */

#else

#include <pthread.h>

#endif //_WIN32

#include <stdio.h>
#include "bvh.h"
#include "opengl.h"
#include "bvhreader.h"



struct BvhFile *g_pbvh;


void PrintWelcome(void)
{
  printf(
         "Welcome to the BvhViewer vers 0.01\n"
         "----------------------------------\n"
         "\n"
         "Press \'h\' for help\n"
         "\n"
         );
}


void PrintHelp(void)
{
  printf(
         "Help:\n"
         "-----\n"
         "\n"
         "\'c\' : Display copyright information\n"
         "\'l\' : Load a BioVision file\n"
         "\'e\' : Exit program\n"
         "\n"
         "\'i\' : Reset position\n"
         "\n"
         "\'x\' : Translate of +ex\n"
         "\'X\' : Translate of -ex\n"
         "\'y\' : Translate of +ey\n"
         "\'Y\' : Translate of -ey\n"
         "\'z\' : Translate of +ez\n"
         "\'Z\' : Translate of -ez\n"
         "\'p\' : Rotate around +ex\n"
         "\'P\' : Rotate around -ex\n"
         "\'q\' : Rotate around +ey\n"
         "\'Q\' : Rotate around -ey\n"
         "\'r\' : Rotate around +ez\n"
         "\'R\' : Rotate around -ez\n"
         "\n"
         );
}


void PrintCopyright(void)
{
  printf(
  "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n"
  "* BvhViewer v0.01 (c) 2002, Frederic Parrel <antikons@ifrance.com>      *\n"
  "*                                                                       *\n"
  "*                   FREE SOFWARE - LOGICIEL LIBRE                       *\n"
  "*                                                                       *\n"
  "* This program is free software; you can redistribute it and/or modify  *\n"
  "* it under the terms of the GNU General Public License as published by  *\n"
  "* the Free Software Foundation; either version 2 of the License, or     *\n"
  "* (at your option) any later version.                                   *\n"
  "*                                                                       *\n"
  "* This program is distributed in the hope that it will be useful,       *\n"
  "* but WITHOUT ANY WARRANTY; without even the implied warranty of        *\n"
  "* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *\n"
  "* GNU General Public License for more details.                          *\n"
  "*                                                                       *\n"
  "* Bvh files belongs to different companies                              *\n"
  "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n"
  "\n"
  );
}


void LoadFile(void)
{
#ifdef _WIN32

  OPENFILENAME ofn;
  char buf[MAX_PATH];
  buf[0] = 0;
  
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = NULL;
  ofn.hInstance = GetModuleHandle(NULL);
  ofn.lpstrFilter = "Bvh files\0*.bvh\0All files\0*.*\0\0";
  ofn.lpstrCustomFilter = NULL;
  ofn.nMaxCustFilter = 0;
  ofn.nFilterIndex = 0;
  ofn.lpstrFile = buf;
  ofn.nMaxFile = sizeof(buf);
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = ".\\bvh";
  ofn.lpstrTitle = NULL;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
  ofn.nFileOffset = 0;
  ofn.nFileExtension = 0;
  ofn.lpstrDefExt = "bvh";
  ofn.lCustData = 0;
  ofn.lpfnHook = NULL;
  ofn.lpTemplateName = NULL;
  if(!GetOpenFileName(&ofn)) {
    return;
  }
  printf("Loading file...\n");
  GLSetBvhFile(NULL);
  FreeBvh(g_pbvh);
  g_pbvh = NULL;
  g_pbvh = CreateBvh();
  if(ReadBVH(buf,g_pbvh)) {
    printf("ERROR: Cannot load %s\n\n",buf);
    return;
  }
  DivByTen(g_pbvh);
  GLSetBvhFile(g_pbvh);
  printf("File loaded.\n\n");
#else
  
  char buf[256];
  
  printf("Enter a file name:");
  fgets(buf,256,stdin);
  buf[strlen(buf)-1]=0;
  printf("Loading \"%s\" ...\n",buf);
  GLSetBvhFile(NULL);
  FreeBvh(g_pbvh);
  g_pbvh = CreateBvh();
  if(ReadBVH(buf,g_pbvh)) {
    printf("ERROR: Cannot load %s\n\n",buf);
    return;
  }
  DivByTen(g_pbvh);
  GLSetBvhFile(g_pbvh);
  printf("File loaded.\n\n");
  
#endif //_WIN32
}


#ifdef _WIN32

DWORD WINAPI CommandThread(LPVOID null)
{

#else

int _getch(void) { char buf[4]; fgets(buf,4,stdin); return buf[0]; }

void *CommandThread(void *arg)
{

#endif //_WIN32
  
  for(;;) {
    switch(_getch()) {
    case 'h':
      PrintHelp();
      break;
    case 'c':
      PrintCopyright();
      break;
    case 'l':
      LoadFile();
      break;
    case 'e':
      GLSetBvhFile(NULL);
      FreeBvh(g_pbvh);
      exit(0);
    case 'x':
      g_glx += 0.5;
      break;
    case 'X':
      g_glx -= 0.5;
      break;
    case 'y':
      g_gly += 0.5;
      break;
    case 'Y':
      g_gly -= 0.5;
      break;
    case 'z':
      g_glz += 0.5;
      break;
    case 'Z':
      g_glz -= 0.5;
      break;
    case 'p':
      g_glp += 5;
      break;
    case 'P':
      g_glp -= 5;
      break;
    case 'q':
      g_glq += 5;
      break;
    case 'Q':
      g_glq -= 5;
      break;
    case 'r':
      g_glr += 5;
      break;
    case 'R':
      g_glr -= 5;
      break;
    case 'i':
      g_glx = g_gly = g_glp = g_glq = g_glr = 0;
      g_glz = -30;
    }
  }

#ifdef _WIN32
  
  return 0;

#else

  pthread_exit(0);

#endif //_WIN32

}

void StartCommandLoop(void)
{
#ifdef _WIN32

  DWORD dw;
  if(CreateThread(NULL,0,CommandThread,NULL,0,&dw)==NULL) {
    printf("Cannot create thread\n");
    FreeBvh(g_pbvh);
    g_pbvh = NULL;
    exit(-1);
  }

#else

  pthread_t th;
  if(pthread_create(&th, NULL, CommandThread, NULL) < 0) {
    printf("Cannot create thread\n");
    FreeBvh(g_pbvh);
    g_pbvh = NULL;
    exit(-1);
  }
  pthread_detach(th);
  
#endif //_WIN32
}


int main(int argc, char *argv[])
{
  g_pbvh = NULL;
  PrintWelcome();
  GLInit(argc,argv);
  StartCommandLoop();
  return GLLoop();
}


/* END OF main.c */
