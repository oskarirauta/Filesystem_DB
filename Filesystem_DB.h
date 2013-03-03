#ifndef _Filesystem_DB_H
#define _Filesystem_DB_H

#if ARDUINO >= 100
 #include "Arduino.h"
#elif defined(__ARM3X8E__)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

/* Filesystem DB library v1.0

written by Oskari Rauta
*/

  
// Define here amount of characters in md5sum. Algorithm I am using, uses 16.
#ifndef MD5SUM_LENGTH
#define MD5SUM_LENGTH 16
#endif

// Separate databases for folders and files. And separate structs for 'em aswell..
typedef struct FSDB_folderNodeT {
  char *filename;
  FSDB_folderNodeT *next;
} FSDB_folderNode;

typedef struct FSDB_fileNodeT {
  char *filename;
  char md5sum[MD5SUM_LENGTH + 1];
  FSDB_fileNodeT *next;
} FSDB_fileNode;
  
class FS_Database {
 public:
  FSDB_folderNode *folderNode;
  FSDB_fileNode *fileNode;
  
  FS_Database(void);  // Initializer
  ~FS_Database() { this -> destroyDB(); } // Destructor: Release memory by releasing databases

  int fileCount(void); // Returns amount of files in database
  int folderCount(void); // Returns amount of folders in database
  
  /*
  Adding to database: when adding, function returns:
	0: if you tried to add a duplicate entry.
	1: Entry succesfully added
	2: Entry existed, md5sum was updated. [ only addFile ]
  */
  unsigned char addFolder(const char *filename); // Adds folder to database
  unsigned char addFile(const char *filename, const char *md5sum); // Adds file with md5sum to database
  
  /*
  Finding folders, files and database entries.
  To search a folder with path, use findFolder(), to search a file with location, use findFile()
  and to search a file with md5sum, use findFileMD5().

  To browse through all folders, use findFolderFirst() and to browse to next one,
  use findFolderNext(); equillavent functions for files are findFileFirst() and findFileNext()

  These always return:
	0:  previous entry was last, or if you received this with findFolderFirst() or findFileFirst()
		database does not contain entries at all. If you searched with criteria (filename, md5sum
		or folder path) - this means entry does not exist in database.
	1:  Entry was found.

  if Entry is found from database, it is always accessible in either folderNode or fileNode,
  whether it was a search in folder or file database. This is for criteria search and for
  browsing through the database with findFirst and findNext..

  Warning:  using findFileMD5 isn't adviced. It searches only for first file with given md5sum.
 			You cannot use findFileNext to search for next file with md5sum. Using this, for
			example; in duplicate file hunt, is useless. It's usable _only_ if all files are
			unique.

  Removing folders or files:
  When you have made a search, either with criteria, or browsed trough the database and found
  entry that you wish to remove from database, use removeFolder() or removeFile() to remove it.

  Warning: do not use other means to browse through database than provided ones. Provided ones
  also set prevFolder or prevFile pointers that are private and these are needed when removing
  entries are manipulating the database. If you fiddle around.. You will brake the database and
  possibly cause serious program errors and on some rare boards where memory is accessed directly;
  you might cause hardware damage as well.

  Also do not make changes to the entry - it is not, but you should consider it as read-only.
  Use addFile() to change md5sum if needed. To rename, you need to use removeFile() and then
  add it with new name to the database again, using addFile or addFolder.
  */
  
  unsigned char findFolder(const char *filename); 
  unsigned char findFolderFirst(void);
  unsigned char findFolderNext(void);
  unsigned char removeFolder(void);
  unsigned char findFile(const char *filename);
  unsigned char findFileMD5(const char *md5sum);
  unsigned char findFileFirst(void);
  unsigned char findFileNext(void);
  unsigned char removeFile(void);
  
  /*
  Destructor:
  When you are done with your database, use destroyDB() to release memory consumed.
  Also if there is a reason for just clearing the database, this does that also.
  */
  void destroyDB(void);

  /* Private section begins */
 private:
  FSDB_folderNode *folderDB;
  FSDB_fileNode *fileDB;
  FSDB_folderNode *prevFolder;
  FSDB_fileNode *prevFile;
  int _fileCount;
  int _folderCount;
  
  void destroyFolderDB(void);
  void destroyFileDB(void);

};
  
#endif
