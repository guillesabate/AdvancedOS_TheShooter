/**
 * Author: Guillermo Sabaté Castells (guillermo.sabate)
 **/

#include "fat.h"

void parseFat(char * in, char* out, char c){
    int i;
    int j = 0;
    int charfound = 0;
    for (i = 0; i < FAT_DIRNAME_SIZE; i++){
        if (in[i] == c && !charfound){
            charfound = 1;
        } else if (in[i] != c && !charfound){
            out[j] = in[i];
            j++;
        } else if (in[i] == '\0'){
            out[j] = in[i];
            break;
        } else if (in[i] != c && charfound){
            out[j] = '.';
            j++;
            out[j] = in[i];
            j++;
        } else {
            // Ignore whitespace
        }
    }
    out[j] = '\0';
}



fatData getFatInfo(int fd){
    fatData fat;

    lseek(fd, FAT_OEMNAME_OFFSET, SEEK_SET);
    read (fd, &fat.sysname, FAT_OEMNAME_SIZE);
    fat.sysname[8] = '\0';

    lseek(fd, FAT_BYTSPERSEC_OFFSET, SEEK_SET);
    read (fd, &fat.size, FAT_BYTSPERSEC_OFFSET);

    lseek(fd, FAT_SECPERCLUS_OFFSET, SEEK_SET);
    read (fd, &fat.sectors_cluster, FAT_SECPERCLUS_SIZE);

    lseek(fd, FAT_RSVDSECCNT_OFFSET, SEEK_SET);
    read (fd, &fat.sectors_reserved, FAT_RSVDSECCNT_SIZE);

    lseek(fd, FAT_NUMFATS_OFFSET, SEEK_SET);
    read (fd, &fat.num_fat, FAT_NUMFATS_SIZE);

    lseek(fd, FAT_ROOTENTCNT_OFFSET, SEEK_SET);
    read (fd, &fat.max_root_entries, FAT_ROOTENTCNT_SIZE);

    lseek(fd, FAT_FATZS16_OFFSET, SEEK_SET);
    read (fd, &fat.sectors_fat, FAT_FATZS16_SIZE);

    lseek(fd, FAT_VOLLAB_OFFSET, SEEK_SET);
    read(fd, &fat.label, FAT_VOLLAB_SIZE);
    fat.label[11] = '\0';

    return fat;
}

fatData readFatInfo(int fd){
    fatData fat;
    fat = getFatInfo(fd);
    printFatInfo(fat);
    return fat;
}

void printFatInfo(fatData fat){
    printf(FILESYSTEM, FAT16_TXT);
    printf(FAT16_PRINT, fat.sysname, fat.size, fat.sectors_cluster, fat.sectors_reserved, fat.num_fat, fat.max_root_entries, fat.sectors_fat, fat.label);
    return;
}

int findFatFile (int fd, char * searchfile){
    int found = FILE_NOT_FOUND;
    char filename [FAT_DIRNAME_SIZE + 2], fileinfo [FAT_DIRNAME_SIZE + 1];
    unsigned int filesize;

    // Retrieve fat data
    fatData fat;
    fat = getFatInfo(fd);

    // Root directory addres is FatSize * ((Number of sectors per fat * Number of fat) + Reserved sectors)
    unsigned int root_directory = fat.size * (fat.sectors_reserved + fat.num_fat * fat.sectors_fat);
    //printf("\nROOT: %d\n", root_directory);

    // Loop through directory entries to look for the file
    for (int i = 0; i < fat.max_root_entries; i++){
        lseek(fd, root_directory + i*FAT_DIRENTRY_SIZE, SEEK_SET);
        read(fd, &fileinfo, FAT_DIRNAME_SIZE);
        fileinfo[12] = '\0';

        if(fileinfo[0] == 0x00){
            break;
        }

        // Need to parse file in order to get rid of whitespace
        parseFat(fileinfo, filename, ' ');
        //printf("FILE: %s\n", fileinfo);
        //printf("PARSED: %s\n\n", filename);

        // Check that the file check and the input are named the same
        if (strcmp(filename, searchfile) == 0) {
            lseek (fd, root_directory +( i * FAT_DIRENTRY_SIZE) + FAT_DIRFILESIZE_OFFSET , SEEK_SET);
            read (fd, &filesize, FAT_DIRFILESIZE_SIZE);

            found = filesize;
            break;
        }
    }

    return found;
}

int deleteFatFile (int fd, char * searchfile){
    printf("%s, %d\n", searchfile, fd);
    return FILE_NOT_FOUND;
}
