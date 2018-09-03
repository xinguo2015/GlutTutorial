/* safe_malloc.c
 *
 *    un malloc qui ne retourne jamais NULL
 *    a malloc which never returns NULL
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

#include <malloc.h>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif //_WIN32

void *safe_malloc(size_t size)
{
  void *p = NULL;
  do {
    p = malloc(size);
    if(p) return p;
    fprintf(stderr,"ERROR: out of memory !\n");
#ifdef _WIN32
    Sleep(1);
#else
    usleep(1000);
#endif
  } while(!p);
  return NULL;
}


void *safe_realloc(void *memblock, size_t size)
{
  void *p = NULL;
  do {
    p = realloc(memblock,size);
    if(p) return p;
    fprintf(stderr,"ERROR: out of memory !\n");
#ifdef _WIN32
    Sleep(1);
#else
    usleep(1000);
#endif
  } while(!p);
  return NULL;
}


/* END OF safe_malloc.c */
