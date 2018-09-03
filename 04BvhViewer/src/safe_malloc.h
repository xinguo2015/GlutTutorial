/* safe_malloc.h
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

#ifndef __INC_SAFE_MALLOC_H
#define __INC_SAFE_MALLOC_H


void *safe_malloc(size_t size);
void *safe_realloc(void *memblock, size_t size);


#endif //__INC_SAFE_MALLOC_H

/* END OF safe_malloc.h */
