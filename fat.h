/**
 * Author: Guillermo Sabaté Castells (guillermo.sabate)
 *
 * Note: References to pages refer to the document "FAT: General Overview of On-Disk Format" Version 1.02 published in 1999;
 *       from which the information to carry out multiple of the following operations has been obtained.
 *
 **/

#ifndef _FAT_H_
#define _FAT_H_

#include "libraries.h"
#include "utils.h"


#define FAT16_TXT "FAT16"
#define FAT16_PRINT "System Name: %s\nSize: %d\nSectors per Cluster: %d\nReserved Sectors: %d\nNumber of FATs: %d\nMaxRootEntries: %d\nSectors per FAT: %d\nLabel: %s\n\n"

// Page 10
#define FAT_FILESYSTYPE_SIZE 8
#define FAT_FILESYSTYPE_OFFSET 54

// Page 8
#define FAT_OEMNAME_SIZE 8
#define FAT_OEMNAME_OFFSET 3
#define FAT_BYTSPERSEC_SIZE 2
#define FAT_BYTSPERSEC_OFFSET 11
#define FAT_SECPERCLUS_SIZE 1
#define FAT_SECPERCLUS_OFFSET 13
#define FAT_RSVDSECCNT_SIZE 2
#define FAT_RSVDSECCNT_OFFSET 14

// Page 9
#define FAT_NUMFATS_SIZE 1
#define FAT_NUMFATS_OFFSET 16
#define FAT_ROOTENTCNT_SIZE 2
#define FAT_ROOTENTCNT_OFFSET 17
#define FAT_FATZS16_SIZE 2
#define FAT_FATZS16_OFFSET 22

// Page 10
#define FAT_VOLLAB_SIZE 11
#define FAT_VOLLAB_OFFSET 43


typedef struct fatData {
    char sysname [9];                   // Page 8 (OEMNanem)
    unsigned short size;                // Page 8 (BytsPerSec)
    unsigned char sectors_cluster;      // Page 8 (SecPerClus)
    unsigned short sectors_reserved;    // Page 8 (RsvdSecCnt)
    unsigned char num_fat;              // Page 9 (NumFATs)
    unsigned short max_root_entries;    // Page 9 (RootEntCnt)
    unsigned short sectors_fat;         // Page 9 (FATSz16)
    char label [12];                    // Page 10 (VolLab)
} fatData;

fatData getFatInfo(int fd);

fatData readFatInfo(int fd);

void printFatInfo(fatData fat);

#endif
