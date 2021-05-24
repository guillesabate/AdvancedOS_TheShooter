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
