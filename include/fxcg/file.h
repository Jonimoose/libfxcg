#ifndef _FXCG_FILE_H
#define _FXCG_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

enum
{
  CREATEMODE_FILE = 1,
  CREATEMODE_FOLDER = 5
};

enum
{
  READ = 0,
  READ_SHARE = 1,
  WRITE = 2,
  READWRITE = 3,
  READWRITE_SHARE = 4
};

int Bfile_CloseFile_OS( int HANDLE );
int Bfile_CreateEntry_OS( const unsigned short*filename, int mode, int*size );
int Bfile_DeleteEntry( const unsigned short *filename );
int Bfile_FindClose( int FindHandle );
int Bfile_FindFirst( const char *pathname, int *FindHandle, char *foundfile, void *fileinfo );
int Bfile_FindFirst_NON_SMEM( const char *pathname, int *FindHandle, char *foundfile, void *fileinfo );
int Bfile_FindNext( int FindHandle, char *foundfile, char *fileinfo );
int Bfile_FindNext_NON_SMEM( int FindHandle, char *foundfile, char *fileinfo );
int Bfile_GetFileSize_OS( int handle, int pos );
int Bfile_OpenFile_OS( const unsigned short*filename, int mode );
int Bfile_ReadFile_OS( int HANDLE, void *buf, int size, int readpos );
int Bfile_SeekFile_OS( int handle, int pos );
int Bfile_TellFile_OS( int handle );
int Bfile_WriteFile_OS( int HANDLE, const void *buf, int size );
void Bfile_NameToStr_ncpy( unsigned char*source, const unsigned short*dest, int n );
void Bfile_StrToName_ncpy(unsigned short *dest, const unsigned char *source, int n);

#ifdef __cplusplus
}
#endif

#endif
