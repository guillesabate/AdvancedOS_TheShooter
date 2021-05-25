/**
 * Author: Guillermo Sabaté Castells (guillermo.sabate)
 *
 * Note: References to pages refer to a PDF version of the .html website "https://www.nongnu.org/ext2-doc/ext2.html"
 *       on The Second Extended File System, Internal Layour by Dave Poirier;
 *       from which the information to carry out multiple of the following operations has been obtained.
 *
 **/

#ifndef _EXT_H_
#define _EXT_H_

#include "libraries.h"
#include "utils.h"

#define EXT2_TXT "EXT2"
#define INODE_PRINT "INODE INFO\nSize: %d\nNum Inodes: %d\nFirst Inode: %d\nInodes Group: %d\nInodes Free: %d\n\n"
#define BLOCK_PRINT "BLOCK INFO\nBlock Size: %d\nReserved Blocks: %d\nFree Blocks: %d\nTotal Blocks: %d\nFirst Block: %d\nGroup Blocks: %d\nGroup Frags: %d\n\n"
#define VOLUME_PRINT "VOLUME INFO\nVolume name: %s\nLast check: %s\nLast: %s\nLast write: %s\n\n"

// Page 8
#define EXT_FIRST_INODE 1
#define EXT_INITIAL_INODE 2
#define EXT_SUPERBLOCK_OFFSET 1024
// Page 11 (s_log_block_size)
#define EXT_SUPERBLOCK_SIZE 1024

// Page 9
#define EXT_FILESYSTYPE_OFFSET 56       // (s_magic)
#define EXT_FILESYSTYPE_VALUE 0xEF53    // (s_magic)

#define EXT_INODESIZE_SIZE 2
#define EXT_INODESIZE_OFFSET 88 + EXT_SUPERBLOCK_OFFSET
#define EXT_INODESCOUNT_SIZE 4
#define EXT_INODESCOUNT_OFFSET 0 + EXT_SUPERBLOCK_OFFSET
#define EXT_FIRSTINO_SIZE 4
#define EXT_FIRSTINO_OFFSET 84 + EXT_SUPERBLOCK_OFFSET
#define EXT_INODESPERGROUP_SIZE 4
#define EXT_INODESPERGROUP_OFFSET 40 + EXT_SUPERBLOCK_OFFSET
#define EXT_FREEINODESCOUNT_SIZE 4
#define EXT_FREEINODESCOUNT_OFFSET 16 + EXT_SUPERBLOCK_OFFSET

#define EXT_LOGBLOCKSIZE_SIZE 4
#define EXT_LOGBLOCKSIZE_OFFSET 24 + EXT_SUPERBLOCK_OFFSET
#define EXT_RBLOCKSCOUNT_SIZE 4
#define EXT_RBLOCKSCOUNT_OFFSET 8 + EXT_SUPERBLOCK_OFFSET
#define EXT_FREEBLOCKSCOUNT_SIZE 4
#define EXT_FREEBLOCKSCOUN_OFFSET 12 + EXT_SUPERBLOCK_OFFSET
#define EXT_BLOCKSCOUNT_SIZE 4
#define EXT_BLOCKSCOUNT_OFFSET 4 + EXT_SUPERBLOCK_OFFSET
#define EXT_FIRSTDATABLOCK_SIZE 4
#define EXT_FIRSTDATABLOCK_OFFSET 20 + EXT_SUPERBLOCK_OFFSET
#define EXT_BLOCKSPERGROUP_SIZE 4
#define EXT_BLOCKSPERGROUP_OFFSET 32 + EXT_SUPERBLOCK_OFFSET
#define EXT_FRAGSPERGROUP_SIZE 4
#define EXT_FRAGSPERGROUP_OFFSET 36 + EXT_SUPERBLOCK_OFFSET

#define EXT_VOLUMENAME_SIZE 16
#define EXT_VOLUMENAME_OFFSET 120 + EXT_SUPERBLOCK_OFFSET
#define EXT_LASTCHECK_SIZE 4
#define EXT_LASTCHECK_OFFSET 64 + EXT_SUPERBLOCK_OFFSET
#define EXT_MTIME_SIZE 4
#define EXT_MTIME_OFFSET 44 + EXT_SUPERBLOCK_OFFSET
#define EXT_WTIME_SIZE 4
#define EXT_WTIME_OFFSET 48 + EXT_SUPERBLOCK_OFFSET

// Page 23
#define EXT_LLD_INODE_SIZE 4
#define EXT_LLD_RECLEN_SIZE 2
#define EXT_LLD_NAMELEN_SIZE 1
#define EXT_LLD_FILETYPE_SIZE 1
#define EXT_LLD_NAME_SIZE 256


// Page 16
typedef struct blockGroupDescriptor{
    unsigned int block_bitmap;         // Page 16 (bg_block_bitmap)
	unsigned int inode_bitmap;         // Page 16 (bg_inode_bitmap)
	unsigned int inode_table;          // Page 16 (bg_inode_table)
	unsigned short free_blocks_count;  // Page 16 (bg_free_blocks_count)
	unsigned short free_inodes_count;  // Page 16 (bg_free_inodes_count)
	unsigned short used_dirs_count;    // Page 16 (bg_used_dirs_count)
	unsigned short pad;                // Page 16 (bg_pad)
} blockGroupDescriptor;

// Page 17
typedef struct inodeTable{
    unsigned short mode;
    unsigned short uid;
    unsigned int size;
    unsigned int atime;
    unsigned int ctime;
    unsigned int mtime;
    unsigned int dtime;
    unsigned short gid;
    unsigned short links_count;
    unsigned int blocks;
    unsigned int flags;
    unsigned int osdl;
    unsigned int block[15];
    unsigned int generation;
    unsigned int file_acl;
    unsigned int dir_acl;
    unsigned int faddr;
    char osd[12];
} inodeTable;

// Page 23
typedef struct linkedDirectoryEntry{
	unsigned int inode;
	unsigned short rec_len;
	char name_len;
	char file_type;
	char name[256];
}  linkedDirectoryEntry;

typedef struct inodeData {
    unsigned short size;            // Page 9 (s_inode_size)
    unsigned int num;               // Page 9 (s_inodes_count)
    unsigned int first;             // Page 9 (s_first_ino)
    unsigned int group;             // Page 9 (s_inodes_per_group)
    unsigned int free;              // Page 9 (s_free_inodes_count)
} inodeData;

typedef struct blockData{
    unsigned short size;            // Page 9 (s_log_block_size)
    unsigned int reserved;          // Page 9 (s_r_blocks_count)
    unsigned int free;              // Page 9 (s_free_blocks_count)
    unsigned int total;             // Page 9 (s_blocks_count)
    unsigned int first;             // Page 9 (s_first_data_block)
    unsigned int groups;            // Page 9 (s_blocks_per_group)
    unsigned int frags;             // Page 9 (s_frags_per_group)
} blockData;

typedef struct volumeData{
    char volume_name[17];           // Page 10 (s_volumename)
    unsigned int last_check;        // Page 9 (s_lastcheck)
    unsigned int last;              // Page 9 (s_mtime)
    unsigned int last_write;        // Page 9 (s_wtime)
} volumeData;

typedef struct extData{
    inodeData inode;
    blockData block;
    volumeData volume;
} extData;


inodeData getInodeInfo(int fd);

blockData getBlockInfo(int fd);

volumeData getVolumeInfo(int fd);

extData readExtInfo(int fd);

void printExtInfo(extData ext);

int findExtFile(int fd, char* searchfile);


#endif
