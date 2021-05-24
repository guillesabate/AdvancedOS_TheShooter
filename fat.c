/**
 * Author: Guillermo Sabaté Castells (guillermo.sabate)
 **/

#include "fat.h"

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
}
