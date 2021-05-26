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

int findExtInDepth(int fd, char* searchfile, extData ext, int inode_address){
    int returning = FILE_NOT_FOUND;
    inodeTable inode, result;
    unsigned short dir_offset = 0;
    unsigned int entry;
    linkedDirectoryEntry dir_entry;

    inode = getInodeTable(fd, ext, inode_address);
    //printf("INODE SIZE: %d\n", inode.size);

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
            if (dir_entry.name_len == 0) continue;

            read(fd, &dir_entry.file_type, EXT_LLD_FILETYPE_SIZE);
            read(fd, dir_entry.name, dir_entry.name_len);
            dir_entry.name[(int)dir_entry.name_len] = '\0';
            //printf("Directory Entry: %s of name size %d\n", dir_entry.name, dir_entry.name_len);

            switch(dir_entry.file_type){
                case EXT_FT_DIR:
                    if (strcmp (dir_entry.name, "..") == 0 || strcmp (dir_entry.name, ".") == 0){
                        if (strcmp(searchfile, dir_entry.name) == 0) printf(ERROR_DOT);
                    } else {
                        if (strcmp(searchfile, dir_entry.name) == 0) {
                            printf(DIRECTORY_FOUND);
                            inode = getInodeTable(fd, ext, dir_entry.inode);
                            return inode.size;
                        }
                        else{
                            returning = findExtInDepth(fd, searchfile, ext, dir_entry.inode);
                            if (returning != FILE_NOT_FOUND) return returning;
                        }
                    }
                break;
                case EXT_FT_REG_FILE:
                    if (strcmp(searchfile, dir_entry.name) == 0) {
                        result = getInodeTable(fd, ext, dir_entry.inode);
                        return result.size;
                        break;
                    }
                break;
                case EXT_FT_UNKNOWN:
                    if (strcmp(searchfile, dir_entry.name) == 0) printf(UNKOWNFILE_TXT);
                break;
                default:
                    if (strcmp(searchfile, dir_entry.name) == 0) printf(UNKOWNFILE_TXT);
                break;
            }

        }
    }
    return FILE_NOT_FOUND;
}


int findExtFile (int fd, char* searchfile){
    // Retrieve ext data
    extData ext;
    ext = getExtInfo(fd);

    return findExtInDepth (fd, searchfile, ext, EXT_INITIAL_INODE);
}

int deleteExtFile(int fd, char* searchfile){
    inodeTable inode;
    unsigned short dir_offset = 0;
    unsigned int entry;
    linkedDirectoryEntry dir_entry;

    extData ext;
    ext = getExtInfo(fd);

    inode = getInodeTable(fd, ext, EXT_INITIAL_INODE);
    //printf("INODE SIZE: %d\n", inode.size);

    // Page 19 for max index of i_block array
    for (unsigned int i = 0; i < inode.blocks/(2<<ext.block.size) && dir_offset < inode.size; i++) {

        //printf("Checking iterator %d\n", i);
        if (inode.block[i] == 0) continue;
        entry = inode.block[i] * ext.block.size;

        for (int j = 0; dir_offset < inode.size; j++) {
            //printf("Checking block %d\n", j);
            int d_off = dir_offset;
            lseek(fd, entry + dir_offset, SEEK_SET);
            read(fd, &dir_entry.inode, EXT_LLD_INODE_SIZE);
            read(fd, &dir_entry.rec_len, EXT_LLD_RECLEN_SIZE);
            read(fd, &dir_entry.name_len, EXT_LLD_NAMELEN_SIZE);

            dir_offset += dir_entry.rec_len;
            //printf("dir_offset = %d, inode size = %d\n", dir_offset, inode.size);
            if (dir_offset > inode.size) break;
            if (dir_entry.name_len == 0) continue;

            read(fd, &dir_entry.file_type, EXT_LLD_FILETYPE_SIZE);
            read(fd, dir_entry.name, dir_entry.name_len);
            dir_entry.name[(int)dir_entry.name_len] = '\0';
            //printf("Directory Entry: %s of name size %d\n", dir_entry.name, dir_entry.name_len);

            switch(dir_entry.file_type){
                case EXT_FT_DIR:
                    if (strcmp (dir_entry.name, "..") == 0 || strcmp (dir_entry.name, ".") == 0){
                        if (strcmp(searchfile, dir_entry.name) == 0) printf(ERROR_DOT);
                    } else if (strcmp(searchfile, dir_entry.name) == 0) {
                        return FILE_IS_NOT_FILE;
                        break;
                    }
                break;
                case EXT_FT_REG_FILE:
                    if (strcmp(searchfile, dir_entry.name) == 0) {
                        inode = getInodeTable(fd, ext, dir_entry.inode);
                        blockGroupDescriptor bgd;
                        unsigned short length = 0;
                        char buff = 0;
                        int offset = 0;

                        // Get Block Group Descriptor
                        lseek(fd, EXT_SUPERBLOCK_OFFSET + ext.block.size, SEEK_SET);
                        read(fd, &bgd, sizeof(blockGroupDescriptor));

                        // Page 16 explanations on bitmap representations, 0 = free and 1 = available
                        const unsigned int bitmap_configurations [] = {0x00FE, 0X00FD, 0X00FB, 0X00F7, 0X00EF, 0X00DF, 0X00BF, 0X007F};

                        // Set cursor to block bitmap
                        offset = bgd.block_bitmap * ext.block.size;
                        lseek(fd, offset + inode.block[i] / EXT_BYTETOBITS, SEEK_SET);
                        read(fd, &buff, sizeof(char));
                        buff &= bitmap_configurations[bgd.block_bitmap % EXT_BYTETOBITS];
                        lseek(fd, offset + (inode.block[j] / EXT_BYTETOBITS), SEEK_SET);
                        write(fd, &buff, sizeof(char));

                        // Set cursor to inode bitmap
                        offset = bgd.inode_bitmap + ext.block.size;
                        lseek(fd, offset + (dir_entry.inode / EXT_BYTETOBITS), SEEK_SET);
                        buff = 0;
                        read(fd, &buff, sizeof(char));
                        buff &= bitmap_configurations[dir_entry.inode % EXT_BYTETOBITS];
                        lseek(fd, offset + (dir_entry.inode / EXT_BYTETOBITS), SEEK_SET);
                        write(fd, &buff, sizeof(char));

                        read(fd, &dir_entry.rec_len, EXT_LLD_RECLEN_SIZE);
                        read(fd, &dir_entry.name_len, EXT_LLD_NAMELEN_SIZE);

                        lseek(fd, entry + d_off, SEEK_SET);
                        read(fd, &dir_entry.inode, EXT_LLD_INODE_SIZE);
                        read(fd, &length, EXT_LLD_RECLEN_SIZE);
                        length += dir_entry.rec_len;
                        lseek(fd, -EXT_LLD_RECLEN_SIZE, SEEK_CUR);
                        write(fd, &length, EXT_LLD_RECLEN_SIZE);
                        buff = 0;
                        write(fd, &buff, EXT_LLD_NAMELEN_SIZE);
                        return inode.size;
                        break;
                    }
                break;
                case EXT_FT_UNKNOWN:
                    if (strcmp(searchfile, dir_entry.name) == 0) printf(UNKOWNFILE_TXT);
                break;
                default:
                    if (strcmp(searchfile, dir_entry.name) == 0) printf(UNKOWNFILE_TXT);
                break;
            }

        }
    }
    return FILE_NOT_FOUND;
}
