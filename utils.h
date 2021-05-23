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

#define DIRECTORY "Files/"

char getExtension (int fylesistemfd);

#endif
