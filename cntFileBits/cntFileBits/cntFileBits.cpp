#include "stdafx.h"
#include <time.h>
#include <iostream>
#include <Windows.h>
#include <thread>
#include <mutex>
#include <fstream>
#include <bitset>
#include <io.h>
#include <string>

#pragma warning(disable: 4996);

unsigned int zeroscounter = 0, onescounter = 0, r = 0, f = 0, t = 0, p = 0, v = 0, w = 0, s = 0, pc = 0, fc = 0, recursive = 0, maxThreads = 0, threadmarker = 0, settedthread = 0;
char* Paths[100];
char* Filter[100];
std::mutex countmutex;

//Get the FileSize
__int64 FileSize(const wchar_t* name)
{
	HANDLE hFile = CreateFile((LPCSTR)name, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return -1; // error condition, could call GetLastError to find out more

	LARGE_INTEGER size;
	if (!GetFileSizeEx(hFile, &size))
	{
		CloseHandle(hFile);
		return -1; // error condition, could call GetLastError to find out more
	}

	CloseHandle(hFile);
	return size.QuadPart;
}

//Read all Bits from given File
void ReadAllBytes(char const* filename)
{
		FILE *file = NULL;      // File pointer

		// Open the file in binary mode using the "rb" format string
		// This also checks if the file exists and/or can be opened for reading correctly
		if ((file = fopen(filename, "rb")) != NULL) {

			__int64 fileSize = FileSize((const wchar_t*)filename);

			if (fileSize > 0 && fileSize < 4294967296) {

				BYTE *fileBuf;          // Pointer to our buffered data
				fileBuf = new BYTE[fileSize];

				// Read the file in to the buffer
				fread(fileBuf, fileSize, 1, file);
				fclose(file);
				// Now that we have the entire file buffered, we can take a look at some binary infomation
				// Lets check for zeros and ones
				for (int j = 0; j < 2 * 32; j++) {
					if (((fileBuf[j / 8] >> (j % 8)) & 0x01) == 0)
					{
						zeroscounter++;
					}
					if (((fileBuf[j / 8] >> (j % 8)) & 0x01) == 1)
					{
						onescounter++;
					}
				}
				delete[]fileBuf;
			}
			
			else if (fileSize > 4294967296) {
								
				char c = NULL;
				do {
					c = fgetc(file);
					for (int j = 0; j < 2 * 32; j++) {
						if (((c / 8 >> (j % 8)) & 0x01) == 0)
						{
							zeroscounter++;
						}
						if (((c / 8 >> (j % 8)) & 0x01) == 1)
						{
							onescounter++;
						}
					}
				} while (c != EOF);
				fclose(file);
			}
			
		}
}

//Read all Data from given Destination Path
void ListDirectoryContents(const char *sDir)
{
	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;
	char sPath[2048];

	sprintf(sPath, "%s\\*.*", sDir);

		if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
		{
		std::cerr << "Path not found\n" << std::endl;
		return;
		}
		do
		{
			//Find first file will always return "."
			//and ".." as the first two directories.
			if (strcmp(fdFile.cFileName, ".") != 0
				&& strcmp(fdFile.cFileName, "..") != 0)
			{
				sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);

				//Is recursion active?
				if (r == 1) {
					//Is the entity a File or Folder?
					if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY && recursive > 0)
					{
						if (v == 1) {
							std::cout << "The following path will be read: " << sPath << std::endl;
						}
						recursive--;
						std::thread(ListDirectoryContents, sPath).join();
					}
					else {
						//Filter the files if filter is given
						if (f == 1) {
							if (fc == 1) {
								std::string stringpath = sPath;
								if (stringpath.find((std::string)Filter[0]) == std::string::npos) {
									if (t == 1) {
										if (maxThreads > 0 && threadmarker == 1) {
											maxThreads--;
											std::thread(ReadAllBytes, sPath).detach();
										}
										else if (maxThreads > 0 && threadmarker == 0) {
											threadmarker = 1;
											maxThreads--;
											std::thread(ReadAllBytes, sPath).detach();
										}
										else if (maxThreads == 0 && threadmarker == 1) {
											ReadAllBytes(sPath);
										}
									}
									else {
										//Optimierte Anzahl an Threads?!
										ReadAllBytes(sPath);
									}
								}
							}
							else if (fc == 2) {
								std::string stringpath = sPath;
								if ((stringpath.find((std::string)Filter[0]) == std::string::npos) && (stringpath.find((std::string)Filter[1]) == std::string::npos)) {
									if (t == 1) {
										if (maxThreads > 0 && threadmarker == 1) {
											maxThreads--;;
											std::thread(ReadAllBytes, sPath).detach();
										}
										else if (maxThreads > 0 && threadmarker == 0) {
											threadmarker = 1;
											maxThreads--;;
											std::thread(ReadAllBytes, sPath).detach();
										}
										else if (maxThreads == 0 && threadmarker == 1) {
											ReadAllBytes(sPath);
										}
									}
									else {
										//Optimierte Anzahl an Threads?!
										ReadAllBytes(sPath);
									}
								}
							}
							else if (fc == 3) {
								std::string stringpath = sPath;
								if ((stringpath.find((std::string)Filter[0]) == std::string::npos) && (stringpath.find((std::string)Filter[1]) == std::string::npos) && (stringpath.find((std::string)Filter[2]) == std::string::npos)) {
									if (t == 1) {
										if (maxThreads > 0 && threadmarker == 1) {
											maxThreads--;
											std::thread(ReadAllBytes, sPath).detach();
										}
										else if (maxThreads > 0 && threadmarker == 0) {
											threadmarker = 1;
											maxThreads--;
											std::thread(ReadAllBytes, sPath).detach();
										}
										else if (maxThreads == 0 && threadmarker == 1) {
											ReadAllBytes(sPath);
										}
									}
									else {
										//Optimierte Anzahl an Threads?!
										ReadAllBytes(sPath);
									}
								}
							}
						}
						else {
							//Create till maxThreads Threads
							if (t == 1) {
								if (maxThreads > 0 && threadmarker == 1) {
									maxThreads--;
									std::thread(ReadAllBytes, sPath).detach();
								}
								else if (maxThreads > 0 && threadmarker == 0) {
									threadmarker = 1;
									maxThreads--;
									std::thread(ReadAllBytes, sPath).detach();
								}
								else if (maxThreads == 0 && threadmarker == 1) {
									ReadAllBytes(sPath);
								}
							}
							else {
								//Optimierte Anzahl an Threads?!
								ReadAllBytes(sPath);
							}
						}
					}
				}
				else {
					//Is the entity a File or Folder?
					if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
					{
						if (v == 1) {
							std::cout << "Path: " << sPath << std::endl;
						}
						std::thread(ListDirectoryContents, sPath).join();
					}
					else {
						//Filter the files if filter is given
						if (f == 1) {
							if (fc == 1) {
								std::string stringpath = sPath;
								if (stringpath.find((std::string)Filter[0]) == std::string::npos) {
									if (t == 1) {
										if (maxThreads > 0 && threadmarker == 1) {
											maxThreads--;
											std::thread(ReadAllBytes, sPath).detach();
										}
										else if (maxThreads > 0 && threadmarker == 0) {
											threadmarker = 1;
											maxThreads--;
											std::thread(ReadAllBytes, sPath).detach();
										}
										else if (maxThreads == 0 && threadmarker == 1) {
											ReadAllBytes(sPath);
										}
									}
									else {
										//Optimierte Anzahl an Threads?!
										ReadAllBytes(sPath);
									}
								}
							}
							else if (fc == 2) {
								std::string stringpath = sPath;
								if ((stringpath.find((std::string)Filter[0]) == std::string::npos) && (stringpath.find((std::string)Filter[1]) == std::string::npos)) {
									if (t == 1) {
										if (maxThreads > 0 && threadmarker == 1) {
											maxThreads--;
											std::thread(ReadAllBytes, sPath).detach();
										}
										else if (maxThreads > 0 && threadmarker == 0) {
											threadmarker = 1;
											maxThreads--;
											std::thread(ReadAllBytes, sPath).detach();
										}
										else if (maxThreads == 0 && threadmarker == 1) {
											ReadAllBytes(sPath);
										}
									}
									else {
										//Optimierte Anzahl an Threads?!
										ReadAllBytes(sPath);
									}
								}
							}
							else if (fc == 3) {
								std::string stringpath = sPath;
								if ((stringpath.find((std::string)Filter[0]) == std::string::npos) && (stringpath.find((std::string)Filter[1]) == std::string::npos) && (stringpath.find((std::string)Filter[2]) == std::string::npos)) {
									if (t == 1) {
										if (maxThreads > 0 && threadmarker == 1) {
											maxThreads--;
											std::thread(ReadAllBytes, sPath).detach();
										}
										else if (maxThreads > 0 && threadmarker == 0) {
											threadmarker = 1;
											maxThreads--;
											std::thread(ReadAllBytes, sPath).detach();
										}
										else if (maxThreads == 0 && threadmarker == 1) {
											ReadAllBytes(sPath);
										}
									}
									else {
										//Optimierte Anzahl an Threads?!
										ReadAllBytes(sPath);
									}
								}
							}
						}
						else {
							//Create till max Threads
							if (t == 1) {
								if (maxThreads > 0 && threadmarker == 1) {
									maxThreads--;
									if (v == 1) {
										std::cout << "Path: " << sPath << std::endl;
									}
									std::thread (ReadAllBytes, sPath).join();
								}
								else if (maxThreads > 0 && threadmarker == 0) {
									threadmarker = 1;
									maxThreads--;
									if (v == 1) {
										std::cout << "Path: " << sPath << std::endl;
									}
									std::thread(ReadAllBytes, sPath).join();
								}
								else if (maxThreads == 0 && threadmarker == 1) {
									if (v == 1) {
										std::cout << "Path: " << sPath << std::endl;
									}
									ReadAllBytes(sPath);
								}
								else if (maxThreads == 0 && threadmarker == 0) {
									if (v == 1) {
										std::cout << "Path: " << sPath << std::endl;
									}
									//Optimierte Anzahl an Threads?!
									ReadAllBytes(sPath);
								}
							}
							else {								
								ReadAllBytes(sPath);
							}
						}
					}
				}
			}
		} while (FindNextFile(hFind, &fdFile));

		FindClose(hFind);	
}

/*Helpfunktion*/
void HelpFunction() {
	std::cout <<"Dieses Programm kann folgende zusaetzliche Operatoren:\n\ncntFileBits [-r [n]] [-f fileFilter] [-t maxThreads] [-h] [-v] [-w] -s startPath\n\n";
	std::cout << "startPath\t Gibt das Startverzeichnis an, ab dem die Dateien gelesen werden sollen.\n\t\t die Option -s kann auch mehrfach angegeben werden, z.B. wenn zwei Partitionen\n\t\t durchsucht werden sollen.\n\n";
	std::cout << "-r [n]\t\t Rekursives Lesen der Unterverzeichnisse. wenn n (bei n>=1) angegeben, dann\n\t\t bestimmt n die Tiefe der Rekursion; wird n nicht angegeben, dann werden\n\t\t rekursiv alle unter dem Startverzeichnis stehenden Verzeichnisse und deren\n\t\t Dateien gelese.\n\n";
	std::cout << "-f fileFilter\t fileFilter gibt an, welche Dateien gelesen werden sollen; z.B. .iso oder .jpg;\n\t\t wird diese Option nicht angegeben, so werden alle Dateien gelesen.\n\n";
	std::cout << "-t maxThreads\t maximale Anzahl an Threads; wird diese Option nicht angegeben, dann\n\t\t wird die Anzahl der Threads automatisch optimiert\n\n";
	std::cout << "-h\t\t Anzeige der Hilfe & Copyright Info: wird automatisch angezeigt, wenn beim\n\t\t Programmstart keine Option angegeben wurde.\n\n";
	std::cout << "-p\t\t Ausgabe der Prozessierungszeit auf stdout in Sekunden.Millisekunden\n\n";
	std::cout << "-v\t\t Erweiterte Ausgabe etwaiger Prozessierungsinformationen\n\n";
	std::cout << "-w\t\t Warten auf eine Taste unmittelbar bevor die Applikation terminiert\n\n\n";
	std::cout << "Copyright© by Christopher Wieland and Stephanie Kaschnitz\n";
	std::cout << "Creation on: 07.12.2017\n\n";
};

/*Will be used for storing all given Char* into Char[]*/
void StoreFilterInArray(char* Destination[], char* Source, unsigned short counter) {

	char * string = new char[strlen(Source)];
	memcpy(string, Source, strlen(Source) + 1);

	if (strchr(string, ';')) {

		char* pch = strtok(string, ";");

		while (pch != NULL)
		{
			Destination[counter] = pch;
			counter++;			
			pch = strtok(NULL, ";");
		}
	}
	else {
		Destination[counter] = string;
		counter++;
	}

	countmutex.lock();
	fc = counter;
	countmutex.unlock();
}
void StorePathInArray(char* Destination[], char* Source, unsigned short counter) {

	char * string = new char[strlen(Source)];
	memcpy(string, Source, strlen(Source) + 1);

	if (strchr(string, ';')) {

		char* pch = strtok(string, ";");

		while (pch != NULL)
		{
			Destination[counter] = pch;
			counter++;
			pch = strtok(NULL, ";");
		}
	}
	else {
		Destination[counter] = string;
		counter++;
	}

	countmutex.lock();
	pc = counter;
	countmutex.unlock();
}

/*Will be used to get the IntegerParameters for threads or the recursives */
void StoreRecursiveParameter(char* Source, unsigned short test, unsigned short global) {

	char * crekursiv = new char[strlen(Source)];
	memcpy(crekursiv, Source, strlen(Source) + 1);

	//Check, ob rekursiv eine Zahl ist.
	if (atoi(crekursiv) != 0) {
		test = atoi(crekursiv);
		if (test < 1)
			global = 0;
	}
	else {
		std::cerr << "Bitte geben Sie fuer den Parameter Zahl groesser 1 ein!\n\n";
		return;
	}
	countmutex.lock();
	recursive = test;
	countmutex.unlock();
}
void StoremaxThreadsParameter(char* Source, unsigned short test, unsigned short global) {

	char * crekursiv = new char[strlen(Source)];
	memcpy(crekursiv, Source, strlen(Source) + 1);

	//Check, ob rekursiv eine Zahl ist.
	if (atoi(crekursiv) != 0) {
		test = atoi(crekursiv);
		if (test < 1)
			global = 0;
	}
	else {
		std::cerr << "Bitte geben Sie fuer den Parameter Zahl groesser 1 ein!\n\n";
		return;
	}
	countmutex.lock();
	maxThreads = test;
	settedthread = test;
	countmutex.unlock();
}

/*Mainfunction*/
int main(int argc, char** argv) {

	clock_t startSingle = clock();

	//Zero parameters
	if (argc == 1)
	{
		HelpFunction();
		return 0;
	}

	//Looking for Parameters
	if (argc >= 1) {

		for (int i = 0; i < argc; ++i) {

			//If -h is given, show help
			if (strcmp(argv[i], "-h") == 0) {
				HelpFunction();
				return 0;
			}
			//If -v is given, set the value for "give more process information"
			else if (strcmp(argv[i], "-v") == 0) {
				v = 1;
			}
			//If -r is given, get the numer of recursion
			else if (strcmp(argv[i], "-r") == 0) {
				r = 1;
				std::thread(StoreRecursiveParameter,argv[i + 1], recursive, r).detach();
			}			
			//If -f is given, then it must be a filter ... store the filters!
			else if (strcmp(argv[i], "-f") == 0) {
				f = 1;
				std::thread(StoreFilterInArray,Filter, argv[i + 1], fc).detach();
			}
			//If -t is given, get the Count of maximum Threads
			else if (strcmp(argv[i], "-t") == 0) {
				t = 1;
				std::thread(StoremaxThreadsParameter,argv[i + 1], maxThreads, t).detach();
			}
			//If -p is given, set the value for "give process time"
			else if (strcmp(argv[i], "-p") == 0) {
				p = 1;
			}			
			//If -w is given, set the value for "push button before end" 
			else if (strcmp(argv[i], "-w") == 0) {
				w = 1;
			}
			//If : is in a given parameter, then it must be a path ... store the path!
			else if (strcmp(argv[i], "-s") == 0) {
				s = 1;								
				std::thread(StorePathInArray,Paths, argv[i+1], pc).detach();
			}
		}
	}

	if (s == 0) {
		std::cerr << "Please enter a start path.\n\n";
	}

	if (pc > 0) {
		for (int i = 0; i < pc; i++) {
			std::thread(ListDirectoryContents, Paths[i]).join();
		}
	}
	else {
		ListDirectoryContents(Paths[0]);
	}
	
	if (v == 1) {
		if (f == 1) {
			std::cout << "The choosen filters are: " << std::endl;
			for (int i = 0; i < fc; i++) {
				std::cout << Filter[i] << std::endl;
			}
		}
		if (t == 1) {
			std::cout << "\nAmount of threads is limited by: " << settedthread << std::endl;
			std::cout << "Used Threads: " << settedthread - maxThreads << std::endl;
		}
		if (r == 1) {
			std::cout << "\nThe choosen recursive depth is: " << recursive << std::endl;
		}
	}	
	
	std::cout << "\nThats the amount of ones: " << onescounter << std::endl;
	std::cout << "Thats the amount of zeros: " << zeroscounter << std::endl;

	//Plot the process time
	if (p == 1) {
		std::cout << "\nNeeded time: " << (clock() - startSingle) / double(CLOCKS_PER_SEC) * 1000 << " ms \n";
	}	
	
	//Let the user press a key to end the program.
	if (w == 1) {
		std::cin.ignore();
	}

	return 0;
}