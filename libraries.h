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

#define FILE_NOT_FOUND -1
#define FILE_IS_NOT_FILE -2
#define UNKNOWN_FILESYSTEM -3
#define ZERO 0

//PRINT DEFINES
#define EOL "\n"
#define ERROR_VOLUME "\nError. Volume not found.\n"
#define ERROR_DIRECTORY "\nThe directory was not found\n"
#define CHECK_README "\nPlease check the readme.txt to see options available.\n"
#define OP_UNKNOWN "\nCould not identify the operation entered."
#define ERROR_ARGS "\nError. Incorrect number of parameters.\n\n"
#define FILESYSTEM "\n-------- Filesystem Information --------\n\nFilesystem: %s\n\n"
#define UNKNOWN_FILESYSTEM_TXT "File system is neither EXT2 nor FAT16\n\n"
#define FILE_NOT_FOUND_TXT "\nFile not found.\n\n"
#define FILE_FOUND_TXT "\nFile found. It occupies %d bytes.\n\n"
#define ERROR_DOT "\nError. Trying to access root or previous directory\n"
#define UNKOWNFILE_TXT "\nError. File Type unidentified\n"
#define DIRECTORY_FOUND "\nThe file you were looking for is a Directory.\n"
#define FILE_IS_NOT_FILE_TXT "\nError! The file you are trying to delete is a directory\n"
#define FILE_DELETED_TXT "\n\nThe %s file has been deleted.\n"
#endif
