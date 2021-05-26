/**
 * Author: Guillermo Sabaté Castells (guillermo.sabate)
 **/

#include "utils.h"


int getExtension(int filesystemfd){
    char * buff = malloc (FAT_FILESYSTYPE_SIZE);
    int int_buff = 0;

    // Looking for FAT16 sysfiletype
    lseek(filesystemfd, FAT_FILESYSTYPE_OFFSET, SEEK_SET);
    read (filesystemfd, buff, FAT_FILESYSTYPE_SIZE);
    buff[5] = '\0';
    if(strncmp(buff, FAT16_TXT, sizeof(FAT16_TXT)) == 0){
        free (buff);
        return T_FAT16;
    }

    // Looking for EXT2 sysfiletype
    lseek(filesystemfd, EXT_SUPERBLOCK_OFFSET, SEEK_SET);
    lseek(filesystemfd, EXT_FILESYSTYPE_OFFSET, SEEK_CUR);
    read(filesystemfd, &int_buff, 2);

    if (int_buff == EXT_FILESYSTYPE_VALUE){
        free (buff);
        return T_EXT2;
    }
    free (buff);
    return T_WRONG;
}

char* convertDate (time_t time){
    char* conversion = (char*) malloc (32 * sizeof(char));
    char* conv1 = (char*) malloc (32 * sizeof(char));
    char* conv2 = (char*) malloc (32 * sizeof(char));
    char* conv3 = (char*) malloc (32 * sizeof(char));

    struct tm lt;
    lt = *localtime(&time);
    if(strftime(conversion, sizeof(conversion), "%a %b", &lt) == 0){
        printf("Conversion ERRROR\n");
    }
    if(strftime(conv1, sizeof(conv1), " %d %H:", &lt) == 0){
        printf("Conversion ERRROR\n");
    }
    if(strftime(conv2, sizeof(conv2), "%M:%S", &lt) == 0){
        printf("Conversion ERRROR\n");
    }
    if(strftime(conv3, sizeof(conv2), " %Y", &lt) == 0){
        printf("Conversion ERRROR\n");
    }

    strcat(conversion,conv1);
    strcat(conversion,conv2);
    strcat(conversion,conv3);
    free(conv1);
    free(conv2);
    free(conv3);
    return conversion;
}

int readFileInfo(int fd){
    switch(getExtension(fd)){
        case T_FAT16:
            readFatInfo(fd);
            break;
        case T_EXT2:
            readExtInfo(fd);
            break;
        default:
            printf(UNKNOWN_FILESYSTEM_TXT);
            return UNKNOWN_FILESYSTEM;
            break;
    }
    return 0;
}

int findFile(int fd, char* searchfile){
    int result = FILE_NOT_FOUND;

    switch(getExtension(fd)){
        case T_FAT16:
            result = findFatFile(fd, searchfile);
        break;
        case T_EXT2:
            result = findExtFile(fd, searchfile);
        break;
        default:
            printf(UNKNOWN_FILESYSTEM_TXT);
            return UNKNOWN_FILESYSTEM;
        break;
    }
    if (result == FILE_NOT_FOUND){
        printf(FILE_NOT_FOUND_TXT);
    } else {
        printf(FILE_FOUND_TXT, result);
    }

    return result;
}

int deleteFile(int fd, char* searchfile){
    int result = FILE_NOT_FOUND;
    //printf("DELETING %s\n", searchfile);
    switch(getExtension(fd)){
        case T_FAT16:
            result= deleteFatFile(fd, searchfile);
        break;
        case T_EXT2:
            result= deleteExtFile(fd, searchfile);
        break;
        default:
            printf(UNKNOWN_FILESYSTEM_TXT);
            return UNKNOWN_FILESYSTEM;
        break;
    }
    switch (result) {
        case FILE_NOT_FOUND:
            printf(FILE_NOT_FOUND_TXT);
        break;
        case FILE_IS_NOT_FILE:
            printf(FILE_IS_NOT_FILE_TXT);
        break;
        default:
            printf(FILE_DELETED_TXT, searchfile);
        break;
    }
    return result;
}
