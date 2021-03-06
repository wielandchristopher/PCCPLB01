#include "stdafx.h"
#include <time.h>
#include <iostream>
#include <Windows.h>
#include <thread>

#pragma warning(disable: 4996);

__int64 onescounter = 0, zeroscounter = 0;
bool r = false, f = false, t = false, p = false, v = false, v1 = false, w = false, s = false, threadmarker = false;
unsigned short pc = 0, fc = 0, recursive = 0, maxThreads = 0, settedthread = 0;
int localrec = 0;
char* Paths[100];
char* Filter[100];

LPVOID MapFileRead(LPCTSTR szFileName, size_t * lpcbSize)
{
	HANDLE hFile, hMapping;
	DWORD  dwFileSize;
	LPVOID lpView;
	MEMORY_BASIC_INFORMATION mbi;

	*lpcbSize = 0;

	hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return NULL;
	}

	dwFileSize = GetFileSize(hFile, NULL);
	if (INVALID_FILE_SIZE == dwFileSize)
	{
		CloseHandle(hFile);
		return NULL;
	}

	hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (NULL == hMapping)
	{
		CloseHandle(hFile);
		return NULL;
	}

	lpView = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);

	CloseHandle(hMapping);
	CloseHandle(hFile);

	if (NULL != lpView)
	{
		if (VirtualQuery(lpView, &mbi, sizeof(mbi)) >= sizeof(mbi))
		{
			*lpcbSize = min(dwFileSize, mbi.RegionSize);
		}
		else
		{
			*lpcbSize = dwFileSize;
		}
	}

	return lpView;
}

//Read all Bits from given File
void ReadAllBytes(char const* filename)
{		
	size_t cbSize, i;
	const char * file_view;

	file_view = (const char *)MapFileRead(TEXT(filename), &cbSize);

	if (file_view)
	{
		for (int j = 0; j < cbSize; j++) {
			for (int i = 0; i < 8; i++) {
				if (((file_view[j] >> i) & 0x01) == 0) {
					zeroscounter++;
				}
				else {
					onescounter++;
				}
			}
		}				
	}
}		

//Read all Data from given Destination Path
void ListDirectoryContents(const char *sDir, bool rec)
{
	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;
	char sPath[4096];

	if (rec) {
		localrec = recursive;
	}
	if (r) {	
		if (localrec > 0) {
			localrec--;
			sprintf(sPath, "%s\\*.*", sDir);
		}
		else if (localrec <= 0) {
			r = false;
		}
	}
	else {
		sprintf(sPath, "%s\\*.*", sDir);
	}

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

			//Is the entity a File or Folder?
			if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
			{
				if (v) {
					std::cout << "The following path will be read: " << sPath << std::endl;
				}
				
				if(!r || (r && localrec != 0))
				ListDirectoryContents(sPath,false);
			}
			else {
				//Filter the files if filter is given
				if (f) {
					bool filegood = true;
					if (fc >= 1) {
						for (int i = 0; i < fc; i++) {							
							if (std::string(sPath).length() > strlen(Filter[i]) && std::string(sPath).rfind(Filter[i]) == std::string(sPath).length() - strlen(Filter[i])) {
								filegood = false;
							}
						}
					}
					if (filegood) {
						if (t && maxThreads > 0) {

							threadmarker = true;
						}
						if (threadmarker) {
							if (maxThreads != 0) {
								maxThreads--;
								std::thread(ReadAllBytes, sPath).join();
							}
							else {
								ReadAllBytes(sPath);
							}
						}
						else {
							ReadAllBytes(sPath);
						}
					}
				}
				else {
					//Create till maxThreads Threads
					if (t && maxThreads > 0) {

						threadmarker = true;
					}					
					if(threadmarker){					
						if (maxThreads != 0) {
							maxThreads--;
							std::thread(ReadAllBytes, sPath).join();
						}
						else {						
							ReadAllBytes(sPath);						
						}
					}				
					else {
						ReadAllBytes(sPath);
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
	std::cout << "-v\t\t Erweiterte Ausgabe aller Prozessierungsinformationen\n\n";
	std::cout << "-v1\t\t Erweiterte Ausgabe weniger Prozessierungsinformationen\n\n";
	std::cout << "-w\t\t Warten auf eine Taste unmittelbar bevor die Applikation terminiert\n\n\n";
	std::cout << "Copyright© by Christopher Wieland and Stephanie Kaschnitz\n";
	std::cout << "Creation on: 07.12.2017\n\n";
};

/*Will be used for storing all given Char* into Char[]*/
void StoreCharInArray(char* Destination[], char* Source, unsigned short counter, bool type) {

	char * string = new char[strlen(Source)];
	memcpy(string, Source, strlen(Source) + 1);

	if (type) {

		Destination[counter] = string;
		counter++;
		pc = counter;
	}
	else {
		if (strchr(string, ';')) {

			char* pch = strtok(string, ";");

			while (pch != NULL)
			{
				Destination[counter] = pch;
				counter++;
				pch = strtok(NULL, ";");
			}
			fc = counter;
		}
		else {
			Destination[counter] = string;
			counter++;
			fc = counter;
		}
	}	
}

/*Will be used to get the IntegerParameters for threads or the recursives */
void StoreNumParameter(char* Source, unsigned short test, bool global, bool type) {

	char * crekursiv = new char[strlen(Source)];
	memcpy(crekursiv, Source, strlen(Source) + 1);

	//Check, ob rekursiv eine Zahl ist.
	if (atoi(crekursiv) != 0) {
		test = atoi(crekursiv);
		if (test < 1)
			global = false;
	}
	else {
		std::cerr << "Bitte geben Sie fuer den Parameter Zahl groesser 1 ein!\n\n";
		return;
	}

	if (type) {
		
		maxThreads = test;
		settedthread = test;
	}
	else {
		
		recursive = test;
	}
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
			//If -v is given, set the value for "give all process information"
			else if (strcmp(argv[i], "-v") == 0) {
				v = true;
			}
			//If -v1 is given, set the value for "give more process information"
			else if (strcmp(argv[i], "-v1") == 0) {
				v1 = true;
			}
			//If -r is given, get the numer of recursion
			else if (strcmp(argv[i], "-r") == 0) {
				r = true;
				std::thread(StoreNumParameter,argv[i + 1], recursive, r, false).detach();
			}			
			//If -f is given, then it must be a filter ... store the filters!
			else if (strcmp(argv[i], "-f") == 0) {
				f = true;
				std::thread(StoreCharInArray,Filter, argv[i + 1], fc, false).join();
			}
			//If -t is given, get the Count of maximum Threads
			else if (strcmp(argv[i], "-t") == 0) {
				t = true;
				std::thread(StoreNumParameter,argv[i + 1], maxThreads, t, true).detach();
			}
			//If -p is given, set the value for "give process time"
			else if (strcmp(argv[i], "-p") == 0) {
				p = true;
			}			
			//If -w is given, set the value for "push button before end" 
			else if (strcmp(argv[i], "-w") == 0) {
				w = true;
			}
			//If : is in a given parameter, then it must be a path ... store the path!
			else if (strcmp(argv[i], "-s") == 0) {
				s = true;
				std::thread(StoreCharInArray,Paths, argv[i+1], pc, true).join();
			}
		}
	}

	if (!s) {
		std::cerr << "Please enter a start path.\n\n";
	}

	if (pc > 0) {
		for (int i = 0; i < pc; i++) {
			std::thread(ListDirectoryContents,Paths[i], true).join();
		}
	}
	else {
		ListDirectoryContents(Paths[0], true);
	}
	
	if (v || v1) {
		if (f == 1) {
			std::cout << "\nThe choosen filters are: " << std::endl;
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
	
	std::cout << "\nThats the amount of ones:\t" << onescounter << std::endl;
	std::cout << "Thats the amount of zeros:\t" << zeroscounter << std::endl;

	//Plot the process time
	if (p) {
		std::cout << "\nNeeded time: " << (clock() - startSingle) / double(CLOCKS_PER_SEC) * 1000 << " ms \n";
	}	
	
	//Let the user press a key to end the program.
	if (w) {
		std::cin.ignore();
	}

	return 0;
}