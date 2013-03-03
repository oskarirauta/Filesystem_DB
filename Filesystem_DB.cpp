/* Filesystem DB library

written by Oskari Rauta
*/

#include "Filesystem_DB.h"

FS_Database::FS_Database(void) {
  this -> _folderCount = 0;
  this -> _fileCount = 0;
  this -> folderDB = NULL;
  this -> fileDB = NULL;
  this -> folderNode = NULL;
  this -> fileNode = NULL;
  this -> prevFolder = NULL;
  this -> prevFile = NULL;
}

int FS_Database::fileCount(void) {
  return this -> _fileCount;
}

int FS_Database::folderCount(void) {
  return this -> _folderCount;
}

unsigned char FS_Database::addFolder(const char *filename) {
    
  this -> prevFolder = NULL;
  this -> folderNode = folderDB;
  
  if ( this -> folderNode == NULL ) { // Database has no entries
    this -> folderDB = new FSDB_folderNode;
    this -> folderNode = this -> folderDB;
    this -> folderNode -> next = NULL;
  } else { // Database has atleast 1 entry    

    if ( strcmp( this -> folderNode -> filename, filename ) == 0 ) {
      this -> prevFolder = NULL;
      this -> folderNode = NULL;
      return 0; // Don't allow head entry's duplication
    }
    
    while (( this -> folderNode -> next != NULL ) && ( strcmp(this -> folderNode -> next -> filename, filename) < 0 ))
      this -> folderNode = this -> folderNode -> next;

    if (( this -> folderNode -> next != NULL ) && ( strcmp( this -> folderNode -> next -> filename, filename ) == 0 )) {
      this -> prevFolder = NULL;
      this -> folderNode = NULL;
      return 0; // Don't allow duplicate entries in folder DB
    }
    
    this -> prevFolder = this -> folderNode;
    this -> folderNode = NULL;
    this -> folderNode = new FSDB_folderNode;
    this -> folderNode -> next = this -> prevFolder -> next;
    this -> prevFolder -> next = this -> folderNode;
  }
  
  this -> folderNode -> filename = new char[strlen(filename) + 1];
  strcpy(this -> folderNode -> filename, filename);
  this -> folderNode = NULL;
  this -> prevFolder = NULL;
  this -> _folderCount++;
  return 1;
}

unsigned char FS_Database::addFile(const char *filename, const char *md5sum) {

  this -> prevFile = NULL;
  this -> fileNode = fileDB;
  
  if ( this -> fileNode == NULL ) { // Database has no entries
    this -> fileDB = new FSDB_fileNode;
    this -> fileNode = this -> fileDB;
    this -> fileNode -> next = NULL;
  } else { // Database has atleast 1 entry    

    if ( strcmp( this -> fileNode -> filename, filename ) == 0 ) {
      if ( strcmp( this -> fileNode -> md5sum, md5sum ) != 0 ) {
        strcpy( this -> fileNode -> md5sum, md5sum);
        this -> prevFile = NULL;
        this -> fileNode = NULL;
        return 2; // Updated changed md5sum
      }
      this -> prevFile = NULL;
      this -> fileNode = NULL;
      return 0; // Don't allow head entry's duplication
    }
    
    while (( this -> fileNode -> next != NULL ) && ( strcmp(this -> fileNode -> next -> filename, filename) < 0 ))
      this -> fileNode = this -> fileNode -> next;

    if (( this -> fileNode -> next != NULL ) && ( strcmp( this -> fileNode -> next -> filename, filename ) == 0 )) {
      if ( strcmp( this -> fileNode -> next -> md5sum, md5sum ) != 0 ) {
        strcpy( this -> fileNode -> next -> md5sum, md5sum);
        this -> prevFile = NULL;
        this -> fileNode = NULL;
        return 2; // Updated changed md5sum
      }
      this -> prevFile = NULL;
      this -> fileNode = NULL;
      return 0; // Don't allow duplicate entries in folder DB
    }
    
    this -> prevFile = this -> fileNode;
    this -> fileNode = NULL;
    this -> fileNode = new FSDB_fileNode;
    this -> fileNode -> next = this -> prevFile -> next;
    this -> prevFile -> next = this -> fileNode;
  }
  
  this -> fileNode -> filename = new char[strlen(filename) + 1];
  strcpy(this -> fileNode -> filename, filename);
  strcpy(this -> fileNode -> md5sum, md5sum);
  this -> fileNode = NULL;
  this -> prevFile = NULL;
  this -> _fileCount++;
  return 1;
}

unsigned char FS_Database::findFolder(const char *filename) {
  
  if ( !this -> findFolderFirst())
    return 0;

  do {
    if ( strcmp(this -> folderNode -> filename, filename ) == 0 )
      return 1;
  } while ( this -> findFolderNext());
  
  return 0;
}

unsigned char FS_Database::findFolderFirst(void) {
  this -> prevFolder = NULL;
  this -> folderNode = this -> folderDB;
  
  if ( this -> folderNode == NULL ) return 0;
  return 1;
}

unsigned char FS_Database::findFolderNext(void) {
  
  if (( this -> folderNode == NULL ) || ( this -> folderNode -> next == NULL )) {
    this -> folderNode = NULL;
    this -> prevFolder = NULL;
    return 0;
  }
  
  this -> prevFolder = this -> folderNode;
  this -> folderNode = this -> folderNode -> next;
  return 1;
}

unsigned char FS_Database::removeFolder(void) {
  if ( this -> folderNode == NULL )
    return 0; // No entry selected
  
  if ( this -> prevFolder != NULL ) { // We are not removing the head entry..
    this -> prevFolder -> next = this -> folderNode -> next;
    this -> folderNode -> next = NULL;
    delete this -> folderNode -> filename;
    delete this -> folderNode;
    this -> folderNode = NULL;
    this -> prevFolder = NULL;
  } else { // We are removing the head entry..
    this -> prevFolder = this -> folderNode -> next;
    this -> folderDB = this -> prevFolder;
    this -> prevFolder = NULL;
    delete this -> folderNode -> filename;
    delete this -> folderNode;
    this -> folderNode = NULL;
  }
  
  this -> _folderCount--;
  
  return 1;
}

unsigned char FS_Database::findFile(const char *filename) {
  
  if ( !this -> findFileFirst())
    return 0;

  do {
    if ( strcmp(this -> fileNode -> filename, filename ) == 0 )
      return 1;
  } while ( this -> findFileNext());
  
  return 0;
}

unsigned char FS_Database::findFileMD5(const char *md5sum) {
  
  if ( !this -> findFileFirst())
    return 0;

  do {
    if ( strcmp(this -> fileNode -> md5sum, md5sum ) == 0 )
      return 1;
  } while ( this -> findFileNext());
  
  return 0;
}

unsigned char FS_Database::findFileFirst(void) {
  this -> prevFile = NULL;
  this -> fileNode = this -> fileDB;
  
  if ( this -> fileNode == NULL ) return 0;
  return 1;
}

unsigned char FS_Database::findFileNext(void) { 
  
  if (( this -> fileNode == NULL ) || ( this -> fileNode -> next == NULL )) {
    this -> fileNode = NULL;
    this -> prevFile = NULL;
    return 0;
  }
  
  this -> prevFile = this -> fileNode;
  this -> fileNode = this -> fileNode -> next;
  return 1;
}

unsigned char FS_Database::removeFile(void) {
  if ( this -> fileNode == NULL )
    return 0; // No entry selected
  
  if ( this -> prevFile != NULL ) { // We are not removing the head entry..
    this -> prevFile -> next = this -> fileNode -> next;
    this -> fileNode -> next = NULL;
    delete this -> fileNode -> filename;
    delete this -> fileNode;
    this -> fileNode = NULL;
    this -> prevFile = NULL;
  } else { // We are removing the head entry..
    this -> prevFile = this -> fileNode -> next;
    this -> fileDB = this -> prevFile;
    this -> prevFile = NULL;
    delete this -> fileNode -> filename;
    delete this -> fileNode;
    this -> fileNode = NULL;
  }
  
  this -> _fileCount--;
  
  return 1;
}

void FS_Database::destroyDB(void) {
  this -> destroyFolderDB();
  this -> destroyFileDB();
}

void FS_Database::destroyFolderDB(void) {
  
  this -> prevFolder = NULL;
  this -> folderNode = NULL;
  
  if ( this -> folderDB == NULL ) // Nothing to destroy
    return;

  while ( this -> folderDB -> next != NULL ) {
    this -> folderNode = this -> folderDB -> next;
    this -> folderDB -> next = this -> folderNode -> next;
    this -> folderNode -> next = NULL;
    delete this -> folderNode -> filename;
    delete this -> folderNode;
    this -> folderNode = NULL;    
  }
  
  delete this -> folderDB -> filename;
  delete this -> folderDB;
  
  this -> folderDB = NULL;
  this -> _folderCount = 0;
}

void FS_Database::destroyFileDB(void) {
  
  this -> prevFile = NULL;
  this -> fileNode = NULL;
  
  if ( this -> fileDB == NULL ) // Nothing to destroy
    return;

  while ( this -> fileDB -> next != NULL ) {
    this -> fileNode = this -> fileDB -> next;
    this -> fileDB -> next = this -> fileNode -> next;
    this -> fileNode -> next = NULL;
    delete this -> fileNode -> filename;
    delete this -> fileNode;
    this -> fileNode = NULL;    
  }
  
  delete this -> fileDB -> filename;
  delete this -> fileDB;
  
  this -> fileDB = NULL;
  this -> _fileCount = 0;  
}
