/* Filesystem DB library v1.0

written by Oskari Rauta
*/

#include <Filesystem_DB.h>

// Initialize our database
FS_Database fsdb;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("Filesystem database example.\n\nInitially database is empty. So entry counters should report 0. Let's see.");
  Serial.print("Amount of folders in db: "); Serial.print(fsdb.folderCount());
  Serial.print(" - Amount of files in db: "); Serial.println(fsdb.fileCount());
  
  Serial.println("\nLet's add some folders:");
  Serial.println("/tmp"); fsdb.addFolder("/tmp");
  Serial.println("/bin"); fsdb.addFolder("/bin");
  Serial.println("/var"); fsdb.addFolder("/var");
  Serial.println("/dev"); fsdb.addFolder("/dev");
  Serial.println("/usr"); fsdb.addFolder("/usr");
  Serial.println("/usr/bin"); fsdb.addFolder("/usr/bin");
  Serial.println("/root"); fsdb.addFolder("/root");
  Serial.println("/var/tmp\n\n8 Folders. Add some files too:"); fsdb.addFolder("/var/tmp");
  
  Serial.println("/tmp/swap.mem"); fsdb.addFile("/tmp/swap.mem", "1234567812345678");
  Serial.println("/root/mail.db"); fsdb.addFile("/root/mail.db", "1234567812345678");
  Serial.println("/bin/busybox"); fsdb.addFile("/bin/busybox", "1234567812345678");
  Serial.println("/bin/bash"); fsdb.addFile("/bin/bash", "1234567812345678");
  Serial.println("/usr/bin/X"); fsdb.addFile("/usr/bin/X", "1234567812345678");
  Serial.println("/bin/env"); fsdb.addFile("/bin/env", "1234567812345678");
  Serial.println("/root/read.me\n\nLooks promising? 7 Files. Let's add some duplicates:"); fsdb.addFile("/root/read.me", "1234567812345678");
  Serial.println("/tmp/swap.mem"); fsdb.addFile("/root/mail.db", "1234567812345678");
  Serial.println("/root/mail.db"); fsdb.addFile("/root/mail.db", "1234567812345678");
  Serial.println("/bin/busybox\n\nThere. 3 duplicates added. Add /tmp/swap.mem with another md5sum:"); fsdb.addFile("/bin/busybox", "1234567812345678");
  Serial.println("/tmp/swap.mem\n\nNow search for /bin/busybox in files and if found - remove it.."); fsdb.addFile("/tmp/swap.mem", "a1b1c1d19981");
 
  if ( fsdb.findFile("/bin/busybox")) { 
    Serial.println("Found it. Removing.");
    if ( fsdb.removeFile()) Serial.println("Removed. Amount of files should be now 6 and /bin/busybox should not be in the database anymore.");
    else Serial.println("Remove failed.");
  } else Serial.println("This wasn't supposed to happen. It wasn't found in database..");

  Serial.println("Now let's see some stats:");
  Serial.print("Amount of folders in db: "); Serial.print(fsdb.folderCount());
  Serial.print(" - Amount of files in db: "); Serial.println(fsdb.fileCount());
  Serial.println("\nList of folders:");
  if ( fsdb.findFolderFirst() ) {
    Serial.println(fsdb.folderNode -> filename);
    while ( fsdb.findFolderNext() )
      Serial.println(fsdb.folderNode -> filename);
  }
  
  Serial.println("\nList of files:");
  if ( fsdb.findFileFirst() ) {
    Serial.print(fsdb.fileNode -> filename); Serial.print(" - file hash: "); Serial.println(fsdb.fileNode -> md5sum);
    while ( fsdb.findFileNext() ) {
      Serial.print(fsdb.fileNode -> filename); Serial.print(" - file hash: "); Serial.println(fsdb.fileNode -> md5sum);
    }
  }

  Serial.println("Now - Erase the library to initial/free memory used by it. And then, show stats once again..");
  fsdb.destroyDB();
  Serial.print("Amount of folders in db: "); Serial.print(fsdb.folderCount());
  Serial.print(" - Amount of files in db: "); Serial.println(fsdb.fileCount());

  Serial.println("\nThank you for using Filesystem Database library. Noticed how it orders entries of database in alphabetical order?");
  Serial.println("If this database needs to be compared with another database ( remote / local system ) - this feature might come handy..");

}

void loop() {
  // put your main code here, to run repeatedly: 
  
}