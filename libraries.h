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
#include <unistd.h>
#include <time.h>

#define DIRECTORY "Files/"

#define FILE_FOUND 151
#define FILE_NOT_FOUND -1
#define FILE_DELETED 23

//PRINT DEFINES
#define EOL "\n"
#define ERROR_VOLUME "\nError. Volume not found.\n"
#define ERROR_DIRECTORY "\nThe directory was not found\n"
#define CHECK_README "\nPlease check the readme.txt to see options available.\n"
#define OP_UNKNOWN "\nCould not identify the operation entered."
#define ERROR_ARGS "\nError. Incorrect number of parameters.\n\n"
#define FILESYSTEM "\n-------- Filesystem Information --------\n\nFilesystem: %s\n\n"
#define UNKNOWN_FILESYSTEM "File system is neither EXT2 nor FAT16\n\n"
#define FILE_NOT_FOUND_TXT "\nFile not found.\n\n"
#define FILE_FOUND_TXT "\nFile found. It occupies %d bytes.\n\n"
#define ERROR_DOT "\nError. Trying to access root or previous directory\n"
#define UNKOWNFILE_TXT "\nError. File Type unidentified\n"
#define DIRECTORY_FOUND "\nThe file you were looking for is a Directory.\n"
#endif
