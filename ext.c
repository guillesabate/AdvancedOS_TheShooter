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
    printf("\nname %s\n\n", volume.volume_name);

    lseek(fd, EXT_LASTCHECK_OFFSET, SEEK_SET);
    read (fd, &volume.last_check, EXT_LASTCHECK_SIZE);

    lseek(fd, EXT_MTIME_OFFSET, SEEK_SET);
    read (fd, &volume.last, EXT_MTIME_SIZE);

    lseek(fd, EXT_WTIME_OFFSET, SEEK_SET);
    read (fd, &volume.last_write, EXT_WTIME_SIZE);

    return volume;
}

extData readExtInfo (int fd){
    extData ext;

    ext.inode = getInodeInfo(fd);
    ext.block = getBlockInfo(fd);
    ext.volume = getVolumeInfo(fd);

    printExtInfo(ext);

    return ext;
}

void printExtInfo(extData ext){
    char* tmp1 = convertDate(ext.volume.last_check);
    char* tmp2 = convertDate(ext.volume.last);
    char* tmp3 = convertDate(ext.volume.last_write);

    printf(INODE_PRINT, ext.inode.size, ext.inode.num, ext.inode.first, ext.inode.group, ext.inode.free);
    printf(BLOCK_PRINT, ext.block.size, ext.block.reserved, ext.block.free, ext.block.total, ext.block.first, ext.block.groups, ext.block.frags);
    printf(VOLUME_PRINT, ext.volume.volume_name, tmp1, tmp2, tmp3);

    free(tmp1);
    free(tmp2);
    free(tmp3);

    return;
}
