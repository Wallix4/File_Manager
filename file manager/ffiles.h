#pragma once
#include "Globals.h"
extern files flist, slist;
void addFiles(_finddata_t a, files ** b);
void saveClearStr();
void showStr(char * FileName, _fsize_t FileSize, unsigned int attrib, int y, bool newlist);
void show(files * first);
void show(files * first, int *CrntStr);
void showLastStr(files *fCrnt, int CrntStr);
void deleteElement(files ** fCrnt);
void deleteAll(files ** flast);
void sortAlph(files ** flast);
int sortAlph(files ** flast, files ** fCrnt, files ** addFile, char **crntName, char ** addName);
void sortAlph(files ** flast, int * CrntStr, files ** fCrnt, char ** prevPath);
void searchFiles(files ** flast);
int searchFiles(files ** flast, files ** fCrnt, files ** addFile, char **crntName, char ** addName);
void searchFiles(files ** flast, int * CrntStr, files ** fCrnt, char ** prevPath);
void FileCopy(FILE * source, FILE * dist);
void RefreshFiles(files ** flast, int * CrntStr, files ** fCrnt);
void RefreshFiles(files ** flast, int * CrntStr, files ** fCrnt, char ** prevPath);
void addLog(char * message, char * typemessage, char * extramessage = "" );
void getLogPath(const char * argv[]);
void getHelpPath(const char * argv[]);
void ExistFile(char(*str)[260]);
void newFolder(files **flast, files ** fCrnt, int *CrntStr);
void deleteFolder(char * path);
bool FolderCopy(char * path, char * fCopy);
int addElement(files ** flast, files **fCrnt, char * FileName, int *CrntStr, files ** newCurrent);
void afterRename(files ** flast, files **fCrnt, int *CrntStr);
void CountFileFolder(char * FolderPath, unsigned int * countFile, unsigned int * countFolder, unsigned int *sizeFolder);

void FolderInfo(files * fCrnt);

void FileInfo(files * fCrnt);

