#ifndef SPT_VERSION
#define SPT_VERSION 1


/* The VERSION tag should be updated any time the
   SCN format changes in order to support backward
   compatibility (if implemented).  VERSION is 
   stored in the 2nd 4 bytes of the file */

#define VERSION_0001 1

#define VERSION VERSION_0001


/* The BYTE_SEX tag is used to check the endian
   of the  file being read in.  The IVE format
   is always written in the native endian of the
   machine to provide optimum reading of the file.
   BYTE_SEX is stored in the first 4 bytes of the 
   file */
#define ENDIAN_TYPE 0x01020304
#define OPPOSITE_ENDIAN_TYPE 0x04030201

#endif
