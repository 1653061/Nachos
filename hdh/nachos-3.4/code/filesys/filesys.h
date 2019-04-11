// filesys.h 
//	Data structures to represent the Nachos file system.
//
//	A file system is a set of files stored on disk, organized
//	into directories.  Operations on the file system have to
//	do with "naming" -- creating, opening, and deleting files,
//	given a textual file name.  Operations on an individual
//	"open" file (read, write, close) are to be found in the OpenFile
//	class (openfile.h).
//
//	We define two separate implementations of the file system. 
//	The "STUB" version just re-defines the Nachos file system 
//	operations as operations on the native UNIX file system on the machine
//	running the Nachos simulation.  This is provided in case the
//	multiprogramming and virtual memory assignments (which make use
//	of the file system) are done before the file system assignment.
//
//	The other version is a "real" file system, built on top of 
//	a disk simulator.  The disk is simulated using the native UNIX 
//	file system (in a file named "DISK"). 
//
//	In the "real" implementation, there are two key data structures used 
//	in the file system.  There is a single "root" directory, listing
//	all of the files in the file system; unlike UNIX, the baseline
//	system does not provide a hierarchical directory structure.  
//	In addition, there is a bitmap for allocating
//	disk sectors.  Both the root directory and the bitmap are themselves
//	stored as files in the Nachos file system -- this causes an interesting
//	bootstrap problem when the simulated disk is initialized. 
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef FS_H
#define FS_H

#include "copyright.h"
#include "openfile.h"
#include "syscall.h"

#ifdef FILESYS_STUB 		// Temporarily implement file system calls as 
				// calls to UNIX, until the real file system
				// implementation is available
class FileSystem {
  public:
	OpenFile** o; // mangr file
	int index;
    FileSystem(bool format) {
	o = new OpenFile*[10]; // 10 phan tu
	index = 0;
	for (int i=0; i<10; i++){
		o[i] = NULL;
	}
	this->Create("stdin",0); // stdin mode
	this->Create("stdout",0); // stdout mod
	o[index++] = Open("stdin"); // 0
	o[index] = Open("stdout"); // 1
    }	

    ~FileSystem()
     {
	for (int i = 0; i < 10; ++i)
	{
		if (o[i] != NULL) delete o[i];
	}
	delete[] o;
     }

    bool Create(char *name, int initialSize) { 
	int fileDescriptor = OpenForWrite(name);

	if (fileDescriptor == -1) return FALSE;
	Close(fileDescriptor); 
	return TRUE; 
	}

    OpenFile* Open(char *name) {
	  int fileDescriptor = OpenForReadWrite(name, FALSE);

	  if (fileDescriptor == -1) return NULL;
	  return new OpenFile(fileDescriptor);
      }

	
    OpenFileId Open(char *name, int type){ // Open file tra ve ID++ cua file
	  switch (type){
	     case 0:{ 	// cho viet va doc
	     int fileDescriptor = OpenForReadWrite(name, FALSE); // read and write

	     if (fileDescriptor == -1) return -1;
	     // TIm vi tri ma tai do chua co file nao duoc luu tru
	     for (int i=0; i<index+1;i++){
		if (o[i] == NULL){
			o[i] = new OpenFile(fileDescriptor,0);
			return i;
		}	
	     }
	     // index = 10 => full
	     if (index == 9)
		return -1;
	     
	     // index < 10, new vi tri ke tiep index++
	     index++;
	     o[index] = new OpenFile(fileDescriptor,0);
	     return index;
	     } break;
	     case 1:{ // cho chi doc
	     int fileDescriptor = OpenForRead(name,FALSE); // read-only

	     if (fileDescriptor == -1) return -1;
             // giong case 0
	     for (int i=0; i<index+1;i++){
                if (o[i] == NULL){
                        o[i] = new OpenFile(fileDescriptor,1);
                        return i++;
                } 
             }
             if (index == 9)
                return -1;
             index++;
             o[index] = new OpenFile(fileDescriptor,1);
             return index;
	     } break;

	     default:
		return -1;
         } 
	     
      }

    bool Remove(char *name) { return Unlink(name) == 0; }

};

#else // FILESYS
class FileSystem {
  public:
    OpenFile** o;
    int index;
    int *fileDes;
    FileSystem(bool format);		// Initialize the file system.
					// Must be called *after* "synchDisk" 
					// has been initialized.
    					// If "format", there is nothing on
									// the disk, so initialize the directory
    					// and the bitmap of free blocks.

    bool Create(char *name, int initialSize);  	
					// Create a file (UNIX creat)

    OpenFile* Open(char *name); 	// Open a file (UNIX open)
    OpenFileId Open(char *name, int type);

    bool Remove(char *name);  		// Delete a file (UNIX unlink)

    void List();			// List all the files in the file system

    void Print();			// List all the files and their contents

  private:
   OpenFile* freeMapFile;		// Bit map of free disk blocks,
					// represented as a file
   OpenFile* directoryFile;		// "Root" directory -- list of 
					// file names, represented as a file
};

#endif // FILESYS

#endif // FS_H
