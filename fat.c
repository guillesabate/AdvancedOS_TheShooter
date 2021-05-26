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

int findFatInDepth (int fd, char * searchfile, int root_directory, int start_address, fatData fat){
    int found = FILE_NOT_FOUND;
    char filename [FAT_DIRNAME_SIZE + 2], fileinfo [FAT_DIRNAME_SIZE + 1];
    unsigned int filesize;
    unsigned char filetype = FAT_UNKOWN_FILE;
    unsigned short cluster;

    // Loop through directory entries to look for the file
    for (int i = 0; i < fat.max_root_entries; i++){
        lseek(fd, start_address + i*FAT_DIRENTRY_SIZE, SEEK_SET);
        read(fd, &fileinfo, FAT_DIRNAME_SIZE);
        fileinfo[12] = '\0';
        if(fileinfo[0] == 0x00){
            break;
        }

        // Need to parse file in order to get rid of whitespace
        parseFat(fileinfo, filename, ' ');

        lseek (fd, root_directory +( i * FAT_DIRENTRY_SIZE), SEEK_SET);
        lseek (fd, FAT_DIRATTR_OFFSET, SEEK_CUR);
        read(fd, &filetype, FAT_DIRATTR_SIZE);

        switch(filetype){
            case FAT_ATTRDIRECTORY:
                if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0){
                    if (strcmp(filename, searchfile) == 0) printf(ERROR_DOT);
                } else {
                    lseek (fd, start_address +(i * FAT_DIRENTRY_SIZE) + FAT_DIRFSTCLUSLO_OFFSET, SEEK_SET);
                    read (fd, &cluster, FAT_DIRFSTCLUSLO_SIZE);

                    int next_start_address = (cluster - FAT_FIRSTCLUSTER) * fat.size * fat.sectors_cluster +
                                        fat.sectors_reserved * fat.size +
                                        fat.num_fat * fat.sectors_fat * fat.size +
                                        fat.max_root_entries * FAT_DIRENTRY_SIZE;

                    found = findFatInDepth(fd, searchfile, root_directory, next_start_address, fat);
                    return found;
                    break;
                }
            break;
            case FAT_ATTRARCHIVE:
                if (strcmp(filename, searchfile) == 0) {
                    lseek (fd, start_address +( i * FAT_DIRENTRY_SIZE) + FAT_DIRFILESIZE_OFFSET , SEEK_SET);
                    read (fd, &filesize, FAT_DIRFILESIZE_SIZE);
                    found = filesize;
                    break;
                }
            break;
            default:
                if (strcmp(filename, searchfile) == 0) {
                    printf(UNKOWNFILE_TXT);
                    break;
                }
            break;
        }
    }

    return found;
}

int findFatFile (int fd, char * searchfile){
    // Retrieve fat data
    fatData fat;
    fat = getFatInfo(fd);

    // Root directory addres is FatSize * ((Number of sectors per fat * Number of fat) + Reserved sectors)
    unsigned int root_directory = fat.size * (fat.sectors_reserved + fat.num_fat * fat.sectors_fat);

    return findFatInDepth(fd, searchfile, root_directory, root_directory, fat);
}

int deleteFatFile (int fd, char * searchfile){
    int found = FILE_NOT_FOUND;
    char filename [FAT_DIRNAME_SIZE + 2], fileinfo [FAT_DIRNAME_SIZE + 1];
    unsigned int filesize;
    unsigned char filetype = FAT_UNKOWN_FILE;
    unsigned short cluster, cluster_aux;

    // Retrieve fat data
    fatData fat;
    fat = getFatInfo(fd);

    // Root directory addres is FatSize * ((Number of sectors per fat * Number of fat) + Reserved sectors)
    unsigned int root_directory = fat.size * (fat.sectors_reserved + fat.num_fat * fat.sectors_fat);

    // Loop through directory entries to look for the file
    for (int i = 0; i < fat.max_root_entries; i++){
        lseek(fd, root_directory + i * FAT_DIRENTRY_SIZE, SEEK_SET);
        read(fd, &fileinfo, FAT_DIRNAME_SIZE);
        fileinfo[12] = '\0';
        if (fileinfo[0] == FAT_DIREND) break;

        // Need to parse file in order to get rid of whitespace
        parseFat(fileinfo, filename, ' ');

        // Get Filetype
        lseek (fd, root_directory + i * FAT_DIRENTRY_SIZE, SEEK_SET);
        lseek (fd, FAT_DIRATTR_OFFSET, SEEK_CUR);
        read(fd, &filetype, FAT_DIRATTR_SIZE);

        // Get First Cluster
        lseek (fd, root_directory + i * FAT_DIRENTRY_SIZE + FAT_DIRFILESIZE_OFFSET, SEEK_SET);
        read (fd, &cluster, sizeof(unsigned short));

        switch(filetype){
            // Check if file is a directory
            case FAT_ATTRDIRECTORY:
                if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0){
                    if (strcmp(filename, searchfile) == 0) printf(ERROR_DOT);
                } else if (strcmp(filename, searchfile) == 0) {
                    return FILE_IS_NOT_FILE;
                    break;
                }
            break;
            // Check if file is an archive
            case FAT_ATTRARCHIVE:
                if (strcmp(filename, searchfile) == 0) {

                    // Get filesize
                    lseek (fd, root_directory + i * FAT_DIRENTRY_SIZE + FAT_DIRFILESIZE_OFFSET , SEEK_SET);
                    read (fd, &filesize, FAT_DIRFILESIZE_SIZE);

                    // Get fileinfo
                    lseek(fd, root_directory + i*FAT_DIRENTRY_SIZE, SEEK_SET);
                    read(fd, &fileinfo, FAT_DIRNAME_SIZE);
                    // Rewrite to free entry
                    fileinfo[0] = FAT_DIRFREEENTRY;
                    write(fd, &fileinfo, sizeof(fileinfo)-1);


                    //Look for clusters
                    int off = fat.sectors_reserved * fat.size;
                    lseek (fd, off + cluster * 2, SEEK_SET);
                    read (fd, &cluster_aux, sizeof(unsigned short));

                    unsigned short buf = ZERO;

                    // Delete loop
                    while (cluster_aux < 65525) {
                        cluster = cluster_aux;
                        // Go to cluster and delete
                        lseek (fd, -1 * sizeof(unsigned short), SEEK_CUR);
                        write (fd, &buf, sizeof(unsigned short));
                        lseek (fd, off + cluster * 2, SEEK_SET);
                        read (fd, &cluster_aux, sizeof(unsigned short));
                    }
                    lseek (fd, -1 * sizeof(unsigned short), SEEK_CUR);
                    write (fd, &buf, sizeof(unsigned short));


                    found = filesize;
                    break;
                }
            break;
            default:
                if (strcmp(filename, searchfile) == 0) {
                    //printf(UNKOWNFILE_TXT);
                    break;
                }
            break;
        }
    }

    return found;
}
