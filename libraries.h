/**
 * Author: Guillermo Sabaté Castells (guillermo.sabate)
 **/

#ifndef _LIBRARIES_H_
#define _LIBRARIES_H_

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>

#include "fat.h"
#include "ext.h"
#include "utils.h"


//PRINT DEFINES
#define EOL "\n"
#define ERROR "ERROR!\n"
#define TESTING "Testing...\n"
#define FILES_FOUND "%d files found\n"
#define NO_FILE "The file was not found\n"
#define NO_DIRECTORY "The directory was not found\n"

#define ERROR_CONFIG "Error opening configuration file.\n\n"
#define ERROR_ARGS "Error processing arguments.\n\n"
#define FILESYSTEM "------ Filesystem Information ------\n\nFilesystem: %s\n\n"
#define UNKNOWN_FILESYSTEM "File system is neither EXT2 nor FAT16\n\n"

#endif
