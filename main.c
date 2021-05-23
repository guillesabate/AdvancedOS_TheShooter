/**
 * Author: Guillermo Sabaté Castells (guillermo.sabate)
 **/

//MODULES
#include "libraries.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[]){
    char* filesystem;
    char* operation;
    int fd;


    //Check correct numher of arguments
    if (argc <= 2 || argc > 3){
        printf(ERROR_ARGS);
        exit(EXIT_FAILURE);
    }

    //Copu arguments to operation and filesystem
    operation = (char*) malloc (sizeof(char) * (strlen(argv[1]) + 1));
    filesystem = (char*) malloc (sizeof(char) * (strlen(argv[2]) + 1 + strlen(DIRECTORY)));
    strcpy(operation, argv[1]);
    strcpy(filesystem, DIRECTORY);
    strcat(filesystem, argv[2]);

    //Open file
    if((fd = open(filesystem, O_RDWR))< 0){
        printf("%s\n", filesystem);
        printf("%d\n",errno);
        printf(NO_FILE);
    }

    switch(getExtension(fd)){
        case 'f':
            printf(FILESYSTEM, FAT16_TXT);
            //readFatInfo(fd);
            break;

        case 'e':
            printf(FILESYSTEM, EXT2_TXT);
            //readExtInfo(fd);
            break;

        default:
            printf(UNKNOWN_FILESYSTEM);
            break;
    }
    free (filesystem);
    free(operation);
}
