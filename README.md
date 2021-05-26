# AdvancedOS_TheShooter
# Advanced Operating Systems Project
## Guillermo Sabaté Castells (guillermo.sabate)
## 26/05/2021 - La Salle BCN

### Instructions to run the code

This Project was performed in C language using Atom as IDE.
The compilation was carried through the makefile provided using the "make" command in the Project Directory.
All the files that are desired to be interacted with must be inside the /Files folder in either FAT16 or EXT2 format.
In order to run any command, make sure you type "shooter " and '/' before any of your required instructions.

### FileSystems

#### FAT16
	The FAT16 file system works with clusters.

#### EXT2
	The EXT2 file system works with superblocks, inodes and 

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

All of the data structures used in 

#### Tests performed
	In order to ensure the validity of my progress I used printfs, which have been removed.

#### Observed problems and resolution
	The main problem was working with the Documentation provided and the filesystems themselves, since they are not popularly used and very
	obtuse.

#### Temporal estimation
	Around the 100 hour mark.

### Explanation and assessment of the GIT.

	I honestly did not see the necessity of working with GIT in this specific project. Although the idea of using a git repo is typically an immediate
	yes for me, it most definitely makes more sense when working with other partners. Separating the project into branches is fine in order to see the
	sprint progress, but I consider it most certainly not necessary when working all alone. It has not been a setback, yet it didn't provide any 
	advantages in my opinion.

### General conclusions

	This project has been very challenging.

###### CHECK GIT FOR FINAL VERSION