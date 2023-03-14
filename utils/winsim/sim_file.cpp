
/*******************************************************************
  Windows sim of various Prizm SDK functions to make life easier.
 *******************************************************************/

#include <stdio.h>
#include "freeglut\include\GL\freeglut.h"
#include "prizmsim.h"
#include "prizmfont.h"
#include <ShlObj.h>

static char romPath[256] = { 0 };
static char ramPath[256] = { 0 };
static char devPath[256] = { 0 };

void InvalidateFullFile();

static void ResolvePaths() {
	if (romPath[0] == 0) {
		PWSTR path;
		if (SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path) == S_OK) {
			static char prizmPath[256] = { 0 };
			WideCharToMultiByte(CP_ACP, 0, (LPWSTR)path, -1, (LPSTR)prizmPath, 256, NULL, NULL);
			strcat(prizmPath, "\\Prizm");
			CreateDirectory(prizmPath, NULL);

			strcpy(romPath, prizmPath);
			strcpy(ramPath, prizmPath);

			// prizm rom files go in My Documents / Prizm / ROM
			strcat(romPath, "\\ROM");
			CreateDirectory(romPath, NULL);
			strcat(romPath, "\\");

			// prizm ram files go in My Documents / Prizm / RAM
			strcat(ramPath, "\\RAM");
			CreateDirectory(ramPath, NULL);
			strcat(ramPath, "\\");

			// should point to main project directory for most projects (working dir in VS debug set to .vcxproj directory)
			strcpy(devPath, "..\\");
		}
	}
}

static bool ResolveROMPath(const unsigned short* prizmPath, char* intoPath) {
	// construct path to ROM in Windows docs
	ResolvePaths();

	char filename[256];
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)prizmPath, -1, (LPSTR)filename, 256, NULL, NULL);

	// must begin with filesystem path
	if (!strncmp(filename, "\\\\fls0\\", 7)) {
		memmove(&filename[0], &filename[7], 256 - 7);

		strcpy(intoPath, romPath);
		strcat(intoPath, filename);

		return true;
	}

	// special dev0 path for winsim only stuff for files that point to home directory of project
	if (!strncmp(filename, "\\\\dev0\\", 7)) {
		memmove(&filename[0], &filename[7], 256 - 7);

		strcpy(intoPath, devPath);
		strcat(intoPath, filename);

		return true;
	}

	return false;
}

int Bfile_OpenFile_OS(const unsigned short *filenameW, int mode, int null) {
	InvalidateFullFile();

	char docFilename[512];
	if (!ResolveROMPath(filenameW, docFilename)) {
		return -5;
	}

	HFILE result;
	OFSTRUCT ofStruct;
	switch (mode) {
		case READ:
			result = OpenFile((LPCSTR)docFilename, &ofStruct, OF_READ);
			break;
		case WRITE:
			// no create here, have to call Bfile_CreateEntry_OS to create first
			result = OpenFile((LPCSTR)docFilename, &ofStruct, OF_WRITE);
			break;
		case READWRITE:
			result = OpenFile((LPCSTR)docFilename, &ofStruct, OF_READ | OF_WRITE);
			break;
		default:
			// remaining cases result in error
			result = HFILE_ERROR;
			break;
	}
	if (result == HFILE_ERROR)
		return -1;
	return (int) result;
}

int Bfile_CreateEntry_OS(const unsigned short*filename, int mode, size_t *size) {
	InvalidateFullFile();

	char docFilename[512];
	if (!ResolveROMPath(filename, docFilename)) {
		return -5;
	}

	if (mode == CREATEMODE_FOLDER) {
		if (CreateDirectory(docFilename, NULL)) {
			return 0;
		}
		return -1;
	}
	else if (mode == CREATEMODE_FILE) {
		OFSTRUCT ofStruct;
		HFILE file = OpenFile(docFilename, &ofStruct, OF_CREATE | OF_WRITE);
		if (file == HFILE_ERROR)
			return -1;

		int zero = 0;
		DWORD written;
		for (unsigned int i = 0; i < *size; i += 4) {
			WriteFile((HANDLE) file, &zero, 4, &written, NULL);
		}
		CloseHandle((HANDLE) file);
		return 0;
	}

	// unknown mode
	return -1;
}

int Bfile_SeekFile_OS(int handle, int pos) {
	SetFilePointer((HANDLE)(size_t)handle, pos, NULL, FILE_BEGIN);
	return pos;
}

int Bfile_ReadFile_OS(int handle, void *buf, int size, int readpos) {
	DWORD read = 0;
	if (readpos != -1) {
		SetFilePointer((HANDLE)(size_t)handle, readpos, NULL, FILE_BEGIN);
	}
	ReadFile((HANDLE)(size_t)handle, buf, size, &read, NULL);
	return read;
}

int Bfile_WriteFile_OS(int handle, const void* buf, int size) {
	InvalidateFullFile();

	DWORD written;
	WriteFile((HANDLE)handle, buf, size, &written, NULL);
	if (written == size) {
		return SetFilePointer((HANDLE)handle, 0, NULL, FILE_CURRENT);
	}
	return -1;
}

int Bfile_GetFileSize_OS(int handle) {
	return GetFileSize((HANDLE)(size_t)handle, NULL);
}

int Bfile_TellFile_OS(int handle) {
	return SetFilePointer((HANDLE)handle, 0, NULL, FILE_CURRENT);
}

int Bfile_CloseFile_OS(int handle) {
	InvalidateFullFile();

	CloseHandle((HANDLE)(size_t)handle);
	return 0;
}

int Bfile_DeleteEntry(const unsigned short *filename) {
	InvalidateFullFile();

	char docFilename[512];
	if (!ResolveROMPath(filename, docFilename)) {
		return -5;
	}

	if (DeleteFile(docFilename)) {
		return 0;
	}

	return -1;
}

struct simFindData {
	unsigned short id, type;
	unsigned long fsize, dsize;
	unsigned int property;
	unsigned long address;
	HANDLE winHandle;
	WIN32_FIND_DATA winData;

	void Fill() {
		fsize = winData.nFileSizeLow;
		dsize = winData.nFileSizeLow;
	}
};

int Bfile_FindFirst(const char *pathAsWide, int *FindHandle, char *foundfile, void *fileinfo) {
	InvalidateFullFile();

	char docPath[512];

	*FindHandle = 0;

	if (!ResolveROMPath((unsigned short*) pathAsWide, docPath)) {
		return -5;
	}

	simFindData* data = new simFindData;
	data->winHandle = FindFirstFile(docPath, &data->winData);

	if (data->winHandle == INVALID_HANDLE_VALUE) {
		delete data;
		return -1;
	}
	else {
		data->Fill();
		MultiByteToWideChar(CP_ACP, 0, (LPSTR)data->winData.cFileName, -1, (LPWSTR) foundfile, 256);
		if (fileinfo) memcpy(fileinfo, data, 20);
		*FindHandle = (int) data;
		return 0;
	}
}

int Bfile_FindNext(int FindHandle, char *foundfile, char *fileinfo) {
	InvalidateFullFile();

	simFindData* data = (simFindData*)FindHandle;
	if (data) {
		BOOL ret = FindNextFile(data->winHandle, &data->winData);
		if (ret) {
			data->Fill();
			MultiByteToWideChar(CP_ACP, 0, (LPSTR)data->winData.cFileName, -1, (LPWSTR)foundfile, 256);
			if (fileinfo) memcpy(fileinfo, data, 20);
			return 0;
		} else {
			return -16;
		}
	}
	return -1;
}

int Bfile_FindClose(int FindHandle) {
	InvalidateFullFile();

	simFindData* data = (simFindData*)FindHandle;
	if (data) {
		FindClose(data->winHandle);
		delete data;
	}
	return 0;
}

int MCS_CreateDirectory(unsigned char*dir) {
	if (!dir || dir[0] == 0)
		return 0xF0;

	ResolvePaths();

	char fullPath[512];
	strcpy(fullPath, ramPath);
	strcat(fullPath, (const char*) dir);
	if (CreateDirectory(fullPath, NULL)) {
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			return 0x42;
		} else {
			return 0x43;
		}
	}

	return 0;
}

static char curReadFile[512] = { 0 };

int MCSGetData1(int offset, int len_to_copy, void*buffer) {
	if (curReadFile[0] == 0) {
		return -1;
	}

	OFSTRUCT ofStruct;
	HFILE file = OpenFile(curReadFile, &ofStruct, OF_READ);
	if (file == HFILE_ERROR) {
		curReadFile[0] = 0;
		return -1;
	}

	SetFilePointer((HANDLE)file, offset, NULL, FILE_BEGIN);

	DWORD read = 0;
	ReadFile((HANDLE)file, buffer, len_to_copy, &read, NULL);
	CloseHandle((HANDLE)file);

	if (read != len_to_copy)
		return -1;

	return 0;
}

int MCSGetDlen2(unsigned char*dir, unsigned char*item, int*data_len) {
	ResolvePaths();
	strcpy(curReadFile, ramPath);
	strcat(curReadFile, (const char*)dir);
	strcat(curReadFile, "/");
	strcat(curReadFile, (const char*)item);

	OFSTRUCT ofStruct;
	HFILE file = OpenFile(curReadFile, &ofStruct, OF_READ);
	if (file == HFILE_ERROR) {
		curReadFile[0] = 0;
		return -1;
	}

	*data_len = GetFileSize((HANDLE) file, NULL);
	CloseHandle((HANDLE)file);

	return 0;
}

int MCS_WriteItem(unsigned char*dir, unsigned char*item, short itemtype, int data_length, int buffer) {
	ResolvePaths();

	if (data_length % 4 != 0)
		return 0x10;

	char fullPath[512];
	strcpy(fullPath, ramPath);
	strcat(fullPath, (const char*)dir);
	strcat(fullPath, "/");
	strcat(fullPath, (const char*) item);

	OFSTRUCT ofStruct;
	HFILE file = OpenFile(fullPath, &ofStruct, OF_CREATE | OF_WRITE);
	if (file == HFILE_ERROR)
		return 0x40;

	DWORD written = 0;
	WriteFile((HANDLE)file, (void*) buffer, data_length, &written, NULL);
	CloseHandle((HANDLE)file);

	if (written != data_length)
		return 0x11;

	return 0;
}

// Block handling: automatically invalidates any previous block with a file op from above

unsigned char* fullFile = nullptr;
unsigned int fullFileSize = 0;
int curBlockedFile = 0;

void ResolveFullFile(int handle) {
	curBlockedFile = handle;

	int tellPos = Bfile_TellFile_OS(handle);
	Bfile_SeekFile_OS(handle, 0);
	fullFileSize = Bfile_GetFileSize_OS(handle);
	fullFile = (unsigned char*) malloc(fullFileSize + 4095);
	Bfile_ReadFile_OS(handle, fullFile, fullFileSize, 0);
	Bfile_SeekFile_OS(handle, tellPos);
}

void InvalidateFullFile() {
	if (fullFile) {
		curBlockedFile = 0;
		memset(fullFile, 0, fullFileSize);
		free(fullFile);
		fullFileSize = 0;
		fullFile = nullptr;
	}
}

int Bfile_GetBlockAddress(int handle, int blockAddress, unsigned char** outPtr) {
	Assert(blockAddress % 4096 == 0);
	Assert(curBlockedFile == 0 || curBlockedFile == handle); // Winsim only supports one at a time

	if (!fullFile) {
		ResolveFullFile(handle);
	}

	Assert((unsigned int) blockAddress < fullFileSize);

	*outPtr = fullFile + blockAddress;

	return 0;
}
