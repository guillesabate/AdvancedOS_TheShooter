/**
 * Author: Guillermo Sabaté Castells (guillermo.sabate)
 **/

#ifndef _UTILS_H_
#define _UTILS_H_

#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "fat.h"
#include "ext.h"

#define T_FAT16 16
#define T_EXT2 2
#define T_WRONG 0
#define DIRECTORY "Files/"

int getExtension (int fylesistemfd);

char* convertDate (time_t time);

#endif
