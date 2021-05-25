/**
 * Author: Guillermo Sabaté Castells (guillermo.sabate)
 **/

#include "ext.h"

inodeData getInodeInfo (int fd){
    inodeData inode;

    lseek(fd, EXT_INODESIZE_OFFSET, SEEK_SET);
    read (fd, &inode.size, EXT_INODESIZE_SIZE);

    lseek(fd, EXT_INODESCOUNT_OFFSET, SEEK_SET);
    read (fd, &inode.num, EXT_INODESCOUNT_SIZE);

    lseek(fd, EXT_FIRSTINO_OFFSET, SEEK_SET);
    read (fd, &inode.first, EXT_FIRSTINO_SIZE);

    lseek(fd, EXT_INODESPERGROUP_OFFSET, SEEK_SET);
    read (fd, &inode.group, EXT_INODESPERGROUP_SIZE);

    lseek(fd, EXT_INODESCOUNT_OFFSET, SEEK_SET);
    read (fd, &inode.free, EXT_INODESCOUNT_SIZE);

    return inode;
}

inodeTable getInodeTable (int fd, extData ext, int inode){
    blockGroupDescriptor bgd;
    unsigned int inode_table_address;
    inodeTable inode_table;

    // Page 8 for Offset
    lseek(fd, EXT_SUPERBLOCK_OFFSET + ext.block.size, SEEK_SET);
    read(fd, &bgd, sizeof(blockGroupDescriptor));

    // Page
    int group = (inode - EXT_FIRST_INODE) / ext.inode.group;
    int offset = (inode - EXT_FIRST_INODE) % ext.inode.group;
    int padding = group * ext.block.groups * ext.block.size;

    //printf("Inode Group: %d, Block Groups: %d\n", ext.inode.group, ext.block.groups);

    inode_table_address = bgd.inode_table * ext.block.size + offset * ext.inode.size + padding;
    //printf("Table@: %d, InodeTable: %d, BlockSize: %d\n", inode_table_address, bgd.inode_table, ext.block.size);
    //printf("Group: %d, Offset: %d, Padding: %d\n", group, offset, padding);

    lseek(fd, inode_table_address, SEEK_SET);
    read(fd, &inode_table, sizeof(inodeTable));

    //printf("INODE:\nSize: %d\nBlocks: %d\n", inode_table.size, inode_table.blocks);
    return inode_table;
}

blockData getBlockInfo(int fd){
    blockData block;

    lseek(fd, EXT_LOGBLOCKSIZE_OFFSET, SEEK_SET);
    read (fd, &block.size, EXT_LOGBLOCKSIZE_SIZE);
    block.size = EXT_SUPERBLOCK_SIZE << block.size;

    lseek(fd, EXT_RBLOCKSCOUNT_OFFSET, SEEK_SET);
    read (fd, &block.reserved, EXT_RBLOCKSCOUNT_SIZE);

    lseek(fd, EXT_FREEBLOCKSCOUN_OFFSET, SEEK_SET);
    read (fd, &block.free, EXT_FREEBLOCKSCOUNT_SIZE);

    lseek(fd, EXT_BLOCKSCOUNT_OFFSET, SEEK_SET);
    read (fd, &block.total, EXT_BLOCKSCOUNT_SIZE);

    lseek(fd, EXT_FIRSTDATABLOCK_OFFSET, SEEK_SET);
    read (fd, &block.first, EXT_FIRSTDATABLOCK_SIZE);

    lseek(fd, EXT_BLOCKSPERGROUP_OFFSET, SEEK_SET);
    read (fd, &block.groups, EXT_BLOCKSPERGROUP_SIZE);

    lseek(fd, EXT_FRAGSPERGROUP_OFFSET, SEEK_SET);
    read (fd, &block.frags, EXT_FRAGSPERGROUP_SIZE);

    return block;
}

volumeData getVolumeInfo(int fd){
    volumeData volume;

    lseek(fd, EXT_VOLUMENAME_OFFSET, SEEK_SET);
    read (fd, &volume.volume_name, EXT_VOLUMENAME_SIZE);
    volume.volume_name[16] = '\0';

    lseek(fd, EXT_LASTCHECK_OFFSET, SEEK_SET);
    read (fd, &volume.last_check, EXT_LASTCHECK_SIZE);

    lseek(fd, EXT_MTIME_OFFSET, SEEK_SET);
    read (fd, &volume.last, EXT_MTIME_SIZE);

    lseek(fd, EXT_WTIME_OFFSET, SEEK_SET);
    read (fd, &volume.last_write, EXT_WTIME_SIZE);

    return volume;
}

extData getExtInfo (int fd){
    extData ext;
    ext.inode = getInodeInfo(fd);
    ext.block = getBlockInfo(fd);
    ext.volume = getVolumeInfo(fd);
    return ext;
}

extData readExtInfo (int fd){
    extData ext = getExtInfo (fd);
    printExtInfo(ext);
    return ext;
}

void printExtInfo(extData ext){
    char* tmp1 = convertDate(ext.volume.last_check);
    char* tmp2 = convertDate(ext.volume.last);
    char* tmp3 = convertDate(ext.volume.last_write);

    printf(FILESYSTEM, EXT2_TXT);
    printf(INODE_PRINT, ext.inode.size, ext.inode.num, ext.inode.first, ext.inode.group, ext.inode.free);
    printf(BLOCK_PRINT, ext.block.size, ext.block.reserved, ext.block.free, ext.block.total, ext.block.first, ext.block.groups, ext.block.frags);
    printf(VOLUME_PRINT, ext.volume.volume_name, tmp1, tmp2, tmp3);

    free(tmp1);
    free(tmp2);
    free(tmp3);

    return;
}

int findExtFile(int fd, char* searchfile){
    unsigned short dir_offset = 0;
    unsigned int entry;
    linkedDirectoryEntry dir_entry;

    // Retrieve ext data
    extData ext;
    ext = getExtInfo(fd);
    inodeTable inode, result;
    inode = getInodeTable(fd, ext, EXT_INITIAL_INODE);

    // Page 19 for max index of i_block array
    for (unsigned int i = 0; i < inode.blocks/(2<<ext.block.size) && dir_offset < inode.size; i++) {
        //printf("Checking iterator %d\n", i);
        if (inode.block[i] == 0) continue;
        entry = inode.block[i] * ext.block.size;

        for (int j = 0; dir_offset < inode.size; j++) {
            //printf("Checking block %d\n", j);
            lseek(fd, entry + dir_offset, SEEK_SET);
            read(fd, &dir_entry.inode, EXT_LLD_INODE_SIZE);
            read(fd, &dir_entry.rec_len, EXT_LLD_RECLEN_SIZE);
            read(fd, &dir_entry.name_len, EXT_LLD_NAMELEN_SIZE);

            dir_offset += dir_entry.rec_len;
            //printf("dir_offset = %d, inode size = %d\n", dir_offset, inode.size);

            if (dir_offset > inode.size) break;

            read(fd, &dir_entry.file_type, EXT_LLD_FILETYPE_SIZE);
            read(fd, dir_entry.name, dir_entry.name_len);
            dir_entry.name[(int)dir_entry.name_len] = '\0';
            //printf("Directory Entry: %s\n", dir_entry.name);

            if (strcmp(searchfile, dir_entry.name) == 0) {
                result = getInodeTable(fd, ext, dir_entry.inode);
                return result.size;
            }
        }
    }

    return FILE_NOT_FOUND;
}
