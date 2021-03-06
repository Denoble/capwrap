/*-
 * Copyright (c) 2016 Stanley Uche Godfrey
 * All rights reserved.
 *This software was developed at Memorial University under the
 * NSERC Discovery program (RGPIN-2015-06048).
 *Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <dirent.h>

#include <errno.h>

#include <fcntl.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <sys/stat.h>

#include <sys/types.h>

#include <unistd.h>

#include <assert.h>

#include "libpreopen.h"



//extern char **environ;

int get_shared_memoryFD(){
    
    int k = atoi(getenv("SHARED_MEMORYFD"));
  
    return k ;
    
}
int access(const char *path, int mode)
{
	int fd = get_shared_memoryFD();
	struct po_map *map = po_unpack(fd);
        assert(map != NULL);
	struct po_relpath rel = po_find(map, path, NULL);
	return faccessat(rel.dirfd, rel.relative_path, mode,0);
}
int open(const char *path, int mode, ...)
{
        int fd = get_shared_memoryFD();
	struct po_map *map = po_unpack(fd);
        assert(map != NULL);
	struct po_relpath rel = po_find(map, path, NULL);
       	return openat(rel.dirfd, rel.relative_path, mode);
}

int stat(const char *path, struct stat *st)
{
        printf("stat path: '%s'\n", path);  
        int fd = get_shared_memoryFD();
	struct po_map *map = po_unpack(fd);
        assert(map != NULL);
	struct po_relpath rel = po_find(map, path, NULL);
     	return fstatat(rel.dirfd, rel.relative_path,st,AT_SYMLINK_NOFOLLOW);
}
