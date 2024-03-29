#include <stdio.h> 
#include <stdlib.h>
#include <io.h>  
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <direct.h>
#include <chrono>
#include <ctime>
#include <thread>
#include <vector>
#include "huffman.h"
#include "fm.h"
#include "fwin.h"
#include "Globals.h"
#include "ffiles.h"
#include "fhex.h"
#pragma pack(1)
                               
using namespace std;
 char *logFile, *frameFile, *clearStr,*helpFile;
 COORD ConsoleSize = { 122,40 };

void savePath(char *FileName, char CrntPath[260], char **prevPath)
{
	if ((FileName[0] == '.') && (FileName[1] == '.') && (FileName[2] == '\0'))
	{
		int i = 3, j = 0;
		for (; CrntPath[i]; i++);
		for (j = i - 2; (CrntPath[j] != '\\'); j--);
		*prevPath = new char[i - j - 1];
		memcpy(*prevPath, CrntPath + j + 1, i - j - 1);
		(*prevPath)[i - j - 2] = '\0';
		for (i = 0; i < MAX_PATH; ++i) CrntPath[i] = '\0';
		_getcwd(CrntPath, MAX_PATH);
		if (CrntPath[3]) sprintf(CrntPath, "%s\\", CrntPath);
	}
	else sprintf(CrntPath, "%s%s\\", CrntPath, FileName);
}
void showPath(char CrntPath[260])
{
	if (CrntPath[ConsoleSize.X])
	{
		SetCursorPosition(0, ConsoleSize.Y - 2);
		SetColor(Black, White);
		for (int i = 0; i < ConsoleSize.X - 2; ++i) printf("%c", CrntPath[i]);
		printf("..");
	}
	else
	{
		SetCursorPosition(0, ConsoleSize.Y - 2);
		SetColor(Black, White);
		for (int i = 0; i < ConsoleSize.X; ++i) printf(" ");
		SetCursorPosition(0, ConsoleSize.Y - 2);
		printf("%s", CrntPath);
	}
}
bool exitFM(char * buffer, ConsoleColor background)
{
	short top = ConsoleSize.Y / 2 - 3;
	short bottom = ConsoleSize.Y / 2 + 2;
	short left = ConsoleSize.X / 2 - 31;
	short right = ConsoleSize.X / 2 + 31;
	CHAR_INFO *chiBuffer = new CHAR_INFO[7 * 62];
	showWindow(&chiBuffer, top, left, bottom, right, background);
	SetCursorPosition(left + 18, top + 1);
	printf("%s",buffer);
	SetCursorPosition(left + 8, top + 3);
	SetColor(White, background);
	printf("  ��  ");
	SetCursorPosition(left + 48, top + 3);
	SetColor(background, White);
	printf("  ���  ");
	bool yes = TRUE;
	unsigned char key;
	do
	{
		key = _getch();
		if (key == 224) 
		{
			key = _getch();
			switch (key)
			{
			case 75: yes = TRUE;
				break;
			case 77: yes = FALSE;
				break;
			default:
				break;
			}
		}
		if (key == 27)
		{
			yes = false;
			break;
		}
		if (yes)
		{
			SetCursorPosition(left + 8, top + 3);
			SetColor(White, background);
			printf("  ��  ");
			SetCursorPosition(left + 48, top + 3);
			SetColor(background, White);
			printf("  ���  ");
		}
		else
		{
			SetCursorPosition(left + 8, top + 3);
			SetColor(background, White);
			printf("  ��  ");
			SetCursorPosition(left + 48, top + 3);
			SetColor(White, background);
			printf("  ���  ");
		}
	} while (key != 13);
	hideWindow(chiBuffer, top, left, bottom, right);
	delete[] chiBuffer;
	return yes;

}
void preOptions(const char *argv[])
{
	getLogPath(argv);
	getHelpPath(argv);
	getFramePath(argv);
	SetBufferSize();
	saveClearStr();
	if (!fopen(frameFile, "r"))
	{
		ConsoleFrame();
		saveConsoleToFile(frameFile);
	}
	SetConsoleTitle(L"File Manager by Farid and Sergey");
	addLog("��������� ��������", "INFO");
	EnableCursor(false);
	_chdir("C:\\");
	setlocale(LC_ALL, "rus");
}

									//������� ������ ������� :)
									//printf("%.*s", dlina,str);   :)


int main(int argc, const char * argv[]) 
	{
		preOptions(argv);                             
		bool Disk[26], error;
		char buffer[260], fCopy[260], pathCopy[MAX_PATH], CrntPath[MAX_PATH] = "C:\\", *prevPath = NULL;
		pathCopy[0] = '\0';
		int CrntStr = 0, key;
		FILE *source = NULL, *dist;
		files *flast = flist.next;
		searchFiles(&flast);
		files *fCrnt = flast;
		show(flast);
		showPath(CrntPath);
		do
		{
			key = _getch();
			switch (key)
			{
			case 224:
				key = _getch();
				if ((key == 80) && (fCrnt->next))
				{
					fCrnt = fCrnt->next;
					if (CrntStr + 1 == (ConsoleSize.Y - 5))
					{
						readBlockUp(Blue);
						SetColor(Cyan, White);
						showStr(fCrnt->file.name, fCrnt->file.size, fCrnt->file.attrib, ConsoleSize.Y - 4, FALSE);
					}
					else
					{
						readStringFromConsole(CrntStr, Blue, White);
						CrntStr++;
						readStringFromConsole(CrntStr, Cyan, White);
					}
				}
				if ((key == 72) && (fCrnt->prev)) {
					fCrnt = fCrnt->prev;
					if (!CrntStr)
					{
						readBlockDown(Blue);
						SetColor(Cyan, White);
						showStr(fCrnt->file.name, fCrnt->file.size, fCrnt->file.attrib, 2, FALSE);
					}
					else
					{
						readStringFromConsole(CrntStr, Blue, White);
						CrntStr--;
						readStringFromConsole(CrntStr, Cyan, White);
					}
				}
				break;
			case 13:
				if (fCrnt->file.attrib & _A_SUBDIR)
				{
					if (!(fCrnt->file.attrib & _A_SYSTEM))
					{
						_chdir(fCrnt->file.name);
						savePath(fCrnt->file.name, CrntPath, &prevPath);
						addLog("������� � �����", "INFO", fCrnt->file.name);
						if (prevPath) RefreshFiles(&flast, &CrntStr, &fCrnt, &prevPath);
						else RefreshFiles(&flast, &CrntStr, &fCrnt);
						showPath(CrntPath);
					}
					else showError("� ��� ��� ������� � ������ �����", "");
				}
				else
				{
					runHEX(fCrnt->file.name, &(fCrnt->file.size));
					SetColor(Cyan, White);
					showStr(fCrnt->file.name, fCrnt->file.size, fCrnt->file.attrib, CrntStr + 2, true);
					addLog(fCrnt->file.name, "INFO", "������ � HEX ���������");
				}
				break;
			case 8:
				if (CrntPath[3])
				{
					_chdir("..");
					savePath("..", CrntPath, &prevPath);
					RefreshFiles(&flast, &CrntStr, &fCrnt, &prevPath);
					showPath(CrntPath);
				}
				break;
			case 32:
				if (fCrnt->file.attrib & _A_SUBDIR)
					if (fCrnt->file.attrib & _A_SYSTEM) showError("� ��� ��� ������� � ������ �����", "");
					else FolderInfo(fCrnt);

				else
					FileInfo(fCrnt);
					break;
			case 0:
				key = _getch();
				switch (key)
				{
				case 59: //F1
					HelpWindow(helpFile);
					break;
				case 60:             //F2
					if (renameWindow(fCrnt->file.name))
					{
						afterRename(&flast, &fCrnt, &CrntStr);
						readStringFromConsole(CrntStr, Cyan, White);
					}
					break;
				case 61:
					if (source != NULL) {
						fclose(source);
						source = NULL;
					}
					pathCopy[0] = '\0';
					memcpy(fCopy, fCrnt->file.name, 260);
					if (fCrnt->file.attrib & _A_SUBDIR)
					{
						if (!(fCrnt->file.attrib & _A_SYSTEM))
						{
							_chdir(fCopy);
							_getcwd(pathCopy, MAX_PATH);
							_chdir("..");
						}
						else showError("� ��� ��� ������� � ����������� ������ �����", "");
					}
					else
					{
						source = fopen(fCopy, "r+b");
						if (source != NULL)  addLog(fCopy, "COPY", "�������� � �����");
						else
						{
							addLog("������ �������� �����", "ERROR");
							showError("���� �� ����� ���� ����������", "");
						}
					}
					break;
				case 62:               //F4
					if (source != NULL)
					{
						dist = fopen(fCopy, "rb");
						memcpy(buffer, fCopy, 260);
						if (dist != NULL)
						{
							fclose(dist);
							ExistFile(&fCopy);
						}
						dist = fopen(fCopy, "wb");
						FileCopy(source, dist);
						fclose(dist);
						addElement(&flast, &fCrnt, fCopy, &CrntStr, NULL);
						memcpy(fCopy, buffer, 260);
						addLog(fCopy, "COPY", "������� ����������");
					}
					if (pathCopy[0])
					{
						error = TRUE;
						for (int i = 0; pathCopy[i]; i++)
						{
							if (pathCopy[i] != CrntPath[i])
							{
								if (FolderCopy(pathCopy, fCopy)) addElement(&flast, &fCrnt, fCopy, &CrntStr, NULL);
								addLog(fCopy, "COPY", "������� ����������");
								error = FALSE;
								break;
							}
						}
						if (error) showError("������ ���������� ����� � ��������", "");
					}
					break;
				case 63:      //F5
					newFolder(&flast,&fCrnt,&CrntStr);
					addLog("�������� ����� �����", "INFO");
					break;
				case 64:
					listDisk(Disk);
					if (chooseDisk(Disk))
					{
						_getcwd(CrntPath, MAX_PATH);
						RefreshFiles(&flast, &CrntStr, &fCrnt);
						showPath(CrntPath);
					}
					break;
				case 65:    //F7
					huffman(fCrnt->file.name,"e");
					RefreshFiles(&flast, &CrntStr, &fCrnt);
					break;
				case 66: 
					huffman(fCrnt->file.name,"d");
					RefreshFiles(&flast, &CrntStr, &fCrnt);
					break;
				case 67:    //F9
					if ((fCrnt->file.name[0] == '.') && (fCrnt->file.name[1] == '.') && (fCrnt->file.name[2] == '\0')) showError("���� ����� ���������� �������", "");
					else
					{
						if (exitFM("�� ����� ������ �������?", Red))
						{
							if (fCrnt->file.attrib & _A_SUBDIR)
							{
								deleteFolder(fCrnt->file.name);
								addLog(fCrnt->file.name, "INFO", "�������");
							}
							else
								if (remove(fCrnt->file.name) == -1) showError("������ ���� �� ����� ���� ������", "");
								else addLog(fCrnt->file.name, "INFO", "������");
								if (deleteBlockUp(CrntStr))
								{
									deleteElement(&fCrnt);
									CrntStr--;
									readStringFromConsole(CrntStr, Cyan, White);
									showLastStr(fCrnt, CrntStr);
								}
								else
								{
									deleteElement(&fCrnt);
									SetColor(Cyan, White);
									showStr(fCrnt->file.name, fCrnt->file.size, fCrnt->file.attrib, 2, FALSE);
								}
						}
						break;
					}
				case 68:
					RefreshFiles(&flast, &CrntStr, &fCrnt);
					break;
				default:
					break;
				}
				break;
			case 't':
				//txtRead(fCrnt->file.name, fCrnt->file.size);
				break;
			case 49:
			    ConsoleSize = { 80,25 };
				getFramePath(argv);
				SetBufferSize();
				SetBufferSize();
				saveClearStr();
				RefreshFiles(&flast, &CrntStr, &fCrnt);
				break;
			case 50:
				ConsoleSize = { 122,40 };
				getFramePath(argv);
				SetBufferSize();
				saveClearStr();
				RefreshFiles(&flast, &CrntStr, &fCrnt);
				break;
			//case 51:
			//	ConsoleSize = { 206,70 };
			//	getFramePath(argv);
			//	SetBufferSize();
			//	saveClearStr();
			//	RefreshFiles(&flast, &CrntStr, &fCrnt);
			//	break;
			//case 52:
			//	ConsoleSize = { 248,70 };
			//	getFramePath(argv);
			//	SetBufferSize();
			//	saveClearStr();
			//	RefreshFiles(&flast, &CrntStr, &fCrnt);
			//	break;
			case 27:
				if (!exitFM("�� ����� ������ �����?", Green)) key = 1;
			default:
				break;
			}
		} while (key != 27);
		if (source != NULL) fclose(source);
		addLog("��������� ���������","INFO");
		delete[] logFile;
		delete[] clearStr;
		delete[] frameFile;
		return 0;
	}
