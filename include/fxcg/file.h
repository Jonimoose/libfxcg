#ifndef _FXCG_FILE_H
#define _FXCG_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

int Bfile_CloseFile_OS( int HANDLE );
int Bfile_CreateEntry_OS( const unsigned short*filename, createmode_t mode, int*size );
int Bfile_DeleteEntry( const unsigned short *filename );
int Bfile_FindClose( int FindHandle );
int Bfile_FindFirst( const char *pathname, int *FindHandle, char *foundfile, void *fileinfo );
int Bfile_FindFirst_NON_SMEM( const char *pathname, int *FindHandle, char *foundfile, void *fileinfo );
int Bfile_FindNext( int FindHandle, char *foundfile, char *fileinfo );
int Bfile_FindNext_NON_SMEM( int FindHandle, char *foundfile, char *fileinfo );
int Bfile_GetFileSize_OS( int handle, int pos );
int Bfile_OpenFile_OS( const unsigned short*filename, open_mode_t mode );
int Bfile_ReadFile_OS( int HANDLE, void *buf, int size, int readpos );
int Bfile_SeekFile_OS( int handle, int pos );
int Bfile_TellFile_OS( int handle );
int Bfile_WriteFile_OS( int HANDLE, const void *buf, int size );
void Bfile_NameToStr_ncpy( unsigned char*source, const unsigned short*dest, int n );
void Bfile_StrToName_ncpy(unsigned short *dest, const unsigned char *source, int n);

typedef enum
{
  CREATEMODE_FILE = 1,
  CREATEMODE_FOLDER = 5
} createmode_t;

typedef enum
{
  READ = 0,
  READ_SHARE,
  WRITE,
  READWRITE,
  READWRITE_SHARE
} open_mode_t;

#ifdef __cplusplus
}
#endif

#endif
