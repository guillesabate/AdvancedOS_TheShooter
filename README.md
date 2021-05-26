# AdvancedOS_TheShooter
# Advanced Operating Systems Project
## Guillermo Sabaté Castells (guillermo.sabate)
## 26/05/2021 - La Salle BCN

### Instructions to run the code

This Project was performed in C language using the Atom IDE.
The compilation has to be performed through the makefile provided using the "make" command once accessed the Project Directory. The code execution
was perfomed on the Montserrat server provided by La Salle BCN, therefore you should make sure you have all the libraries required to run the code.

All the files that could desired to be interacted with must be place inside the "/Files" folder in either FAT16 or EXT2 format. Other formats might
be discarded for any of the operations available.

In order to run any command, make sure you type "shooter " and '/' before any of your required instructions; followed by the volume to investigate
and optionally the file to look for (in case /find or /delete are performed).

### FileSystems
#### FAT16
	The File Allocation Table File System first appear around the late 1970s and early 1980s as part of the MS-DOS Operating System. It was
	conceived with the idea to store data up to 500K size, and currently has three versions: FAT12, FAT16 and FAT32. The FAT16 has 16-bit
	file entries and is characterized by a starting Boot Sector and BPB, which store all the information regarding the metadata of the
	filesystem. 

#### EXT2
	The Second Extended File System was first released in 1993 and it is part of the Linux Kernel. This filesystems works with data units in
	Blocks, which are stored under Block Groups. These Blocks and Block Groups are represented by Inodes, which contain pointers to the actual
	data stored in Blocks and the metadata with information relative to them. Lastly, the first block of information is a Superblock, which
	stores information regarding the metadata of the entire filesystem and how data is organized. Afterwards, information is stored in
	directory entries which have pointers to the first cluster of information to read, sized according to its 16-bit filesystem.

### Practice Explanation
#### Requirements

	/INFO
In this first requirement, we were asked to find out the general information regarding the composition of the filesystem to work on. In this first
part of the project, we also had to deal with the set up of basic libraries and modulation, which always requires an extra time to work on.

	/FIND
The "find" requirement performs, after Phase 3, an in-depth search for files both within root and other directories, and if found, retrieves the
size of the file in bytes through recursive calls if necessary.

	/DELETE
This last requirement allows the user to search for a file inside the root directory and delete it if found. 

#### Design

- main
	The main.c file acted as a UI and menu manager that not only checked for correct operational parameters and number of arguments, but also
	called the functions within utils to perform. It also opens the file descriptor if found and performs basic mallocs and frees for
	arguments.

- utils
	The utils module acts as a bridge between the menu and the filesystem-specific functions. The idea is that the menu does not know at any
	time what filesystem the rest of the program is working on and that calls can be performed independelty of that. This feature is achieved
	thorugh the getExtension function, which then calls for the EXT2 and FAT16 specific functions.
	
- fat
	The fat module works exclusively with FAT16 files and utilises many defined constants and data structures propietary to that filesystem.
	It performs all of the required functions for that specified filesystem.

- ext
	The ext module works exclusively with FAT16 files and utilises many defined constants and data structures propietary to that filesystem.
	It performs all of the required functions for that specified filesystem.

- libraries
	This module was used to include all the useful libraries that any of the modules might require as well as some defines for certain returns
	and specially printings that were relevant to the rest.

#### Data Structures Used

	Most of the data structures used in the project have been referenced to the provided documentation as comments above or in between them.

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
-> This data structure was inspired on the table 3.12 for Block Group Descriptor Structure in the EXT2 documentation, only in this instance
   the reserved bytes are not stored

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
-> This data structure mimics table 3.13 for Inode Structure in the EXT2 documentation

// Page 23
typedef struct linkedDirectoryEntry{
	unsigned int inode;
	unsigned short rec_len;
	char name_len;
	char file_type;
	char name[256];
}  linkedDirectoryEntry;
-> This data structure mimics table 4.1 for Linked Directory Entry Structure in the EXT2 documentation

typedef struct inodeData {
    unsigned short size;            // Page 9 (s_inode_size)
    unsigned int num;               // Page 9 (s_inodes_count)
    unsigned int first;             // Page 9 (s_first_ino)
    unsigned int group;             // Page 9 (s_inodes_per_group)
    unsigned int free;              // Page 9 (s_free_inodes_count)
} inodeData;
-> This data structure was inspired by the Project requirements for the Inode Data to be obtained

typedef struct blockData{
    unsigned short size;            // Page 9 (s_log_block_size)
    unsigned int reserved;          // Page 9 (s_r_blocks_count)
    unsigned int free;              // Page 9 (s_free_blocks_count)
    unsigned int total;             // Page 9 (s_blocks_count)
    unsigned int first;             // Page 9 (s_first_data_block)
    unsigned int groups;            // Page 9 (s_blocks_per_group)
    unsigned int frags;             // Page 9 (s_frags_per_group)
} blockData;
-> This data structure was inspired by the Project requirements for the Block Data to be obtained

typedef struct volumeData{
    char volume_name[17];           // Page 10 (s_volumename)
    unsigned int last_check;        // Page 9 (s_lastcheck)
    unsigned int last;              // Page 9 (s_mtime)
    unsigned int last_write;        // Page 9 (s_wtime)
} volumeData;
-> This data structure was inspired by the Project requirements for the Volume Data to be obtained

typedef struct extData{
    inodeData inode;
    blockData block;
    volumeData volume;
} extData;
-> This data structure is a superstructure that englobes an instance of each of the three previous data structures to form a global EXT2
   information structure.

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
-> This data structure was inspired by the Project requirements for the FAT16 Data to be obtained

#### Tests performed

	In order to ensure the validity of my progress I used printfs, which have been removed for the final delivery. One can observe that the
	use of dynamic memory is extremely scarce in order to avoid problems with memory allocation and frees, and in some cases the lack of
	modulation within each module's functions allowed for thorough printing and testing.

#### Observed problems and resolution

	The main problem was working with the Documentation provided and the filesystems themselves, since they are not popularly used and very
	obtuse.

#### Temporal estimation

	This was a project I found very hard to access at first due to the documentation format and lack of information regarding both filesystems
	and how to properly work with them. That said, I would break the project time spent into the following categories:
		- Documentation reading: 20 hours
		- Phase 1 (Development and Testing): 30 hours
		- Phase 2 (Development and Testing): 25 hours
		- Phase 3 (Development and Testing): 20 hours
		- Phase 4 (Development and Testing): 25 hours
		- README: 1 hour + 1 extra hour post-delivery.

### Explanation and assessment of the GIT.

I honestly did not see the necessity of working with GIT in this specific project. Although the idea of using a git repo is typically an immediate
yes for me, it most definitely makes more sense when working with other partners. Separating the project into branches is fine in order to see the
sprint progress, but I consider it most certainly not necessary when working all alone. It has not been a setback, yet it didn't provide any 
advantages in my opinion.

### General conclusions

This is one of the most challenging projects I have been put up against due to the density of the documentation and lack of information regarding
these filesystems on the internet. However, it has been challenging in a very motivating way, since it motivated my inner spirit to beat the
deadlinea and I trully invested myself in the project. I must admit, however, that if I had spent less time trying to understand the documentation
and more time programming at the due times, the project would have probably turned out totally different experience, given that I was very lazy at
the first stages of the project and had to make of it a "sprint" instead of a "marathon".

Regarding the learning outcomes, I am quite happy with the results since, although I personally had no personal interest in filesystem and data
that could be a bit more dated, I ended up enjoying working with these files and at every progress stage the satisfaction of finding out that the
phases you were implementing had real impact on the files' data and how one can interact at a very 'low-level' with them.

Overall, the experience was not the greatest I have ever had on a project yet, I would say that there is a lot of expertise and useful knowledge I
have extracted out of it.