/**
 * Author: Guillermo Sabaté Castells (guillermo.sabate)
 **/

//MODULES
#include "libraries.h"
#include "utils.h"

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[]){
    char* filesystem;
    char* operation;
    char* searchfile;
    int fd;

    // Check correct numher of arguments
    if (argc <= 2 || argc > 4){
        printf(ERROR_ARGS);
        exit(EXIT_FAILURE);
    }

    // Copy arguments to operation and filesystem
    operation = (char*) malloc (sizeof(char) * (strlen(argv[1]) + 1));
    filesystem = (char*) malloc (sizeof(char) * (strlen(argv[2]) + 1 + strlen(DIRECTORY)));
    strcpy(operation, argv[1]);
    strcpy(filesystem, DIRECTORY);
    strcat(filesystem, argv[2]);

    // Check arguments are not exceeded in info operation
    if (strcmp(operation, "/info") == 0 && argc > 3){
        printf(ERROR_ARGS);
        exit(EXIT_FAILURE);
    } else if ((strcmp(operation, "/find") == 0 || strcmp(operation, "/delete") == 0) && argc < 4){
        printf(ERROR_ARGS);
        exit(EXIT_FAILURE);
    }

    // Open file
    if((fd = open(filesystem, O_RDWR))< 0){
        printf(ERROR_VOLUME);
        exit(EXIT_FAILURE);
    }

    // Check operation to perform depending on filesystem
    if (strcmp(operation, "/info") == 0){
        readFileInfo(fd);
    } else if (strcmp(operation, "/find") == 0){
        searchfile = (char*) malloc (sizeof(char) * (strlen(argv[3]) + 1));
        strcpy(searchfile, argv[3]);
        findFile(fd, searchfile);
        free (searchfile);
    } else if (strcmp(operation, "/delete") == 0){
        searchfile = (char*) malloc (sizeof(char) * (strlen(argv[3]) + 1));
        strcpy(searchfile, argv[3]);
        deleteFile(fd, searchfile);
        free (searchfile);
    } else {
        printf(OP_UNKNOWN);
        printf(CHECK_README);
    }
    close(fd);

    //Free dynamic memory
    free (filesystem);
    free (operation);
}
