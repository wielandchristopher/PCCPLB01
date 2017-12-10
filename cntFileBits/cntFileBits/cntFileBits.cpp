#include "stdafx.h"
#include <time.h>
#include <iostream>
#include <Windows.h>
#include <thread>
#include <mutex>
#include <bitset>
#include <fstream>

using namespace std;
mutex countmutex;
#pragma warning(disable: 4996);

unsigned short zeroscounter = 0, onescounter = 0, r = 0, f = 0, t = 0, p = 0, v = 0, w = 0, s = 0, pc = 0, fc = 0, recursive = 0, maxThreads = 0, threadmarker = 0, settedthread = 0;

char* Paths[100];
char* Filter[100];

//Set zeros and ones
void setzeroscounter(unsigned short count) {
	countmutex.lock();
	zeroscounter += count;
	countmutex.unlock();
}
void setonescounter(unsigned short count) {
	countmutex.lock();
	onescounter += count;
	countmutex.unlock();
}
void decreasemaxThreads(unsigned short count) {
	countmutex.lock();
	maxThreads -= count;
	countmutex.unlock();
}

void setpcCount(unsigned short count) {
	countmutex.lock();
	pc = count;
	countmutex.unlock();
}
void setfcCount(unsigned short count) {
	countmutex.lock();
	fc = count;
	countmutex.unlock();
}

//Read all Bits from given File
void ReadAllBytes(char const* filename)
{
	long size;
	FILE *f;
	
	std::ifstream infile(filename);
	if (infile.good()) {

		f = fopen(filename, "rb");
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fclose(f);

		bitset<32> bits(size);
		string zerosandones = bits.to_string();

		if (v == 1) {
			cout << "This is the amount of Bytes: " << size << endl;
		}
		for (std::string::const_iterator it = zerosandones.begin(); it != zerosandones.end(); ++it)
			{
				if (*it == '0')
				{
					setzeroscounter(1);
				}
				if (*it == '1')
				{
					setonescounter(1);
				}
			}
	}		
}

//Read all Data from given Destination Path
void ListDirectoryContents(const char *sDir)
{
	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;
	char sPath[2048];
	unsigned short test = recursive;

	//Specify a file mask. *.* = We want everything!
	countmutex.lock();
	sprintf(sPath, "%s\\*.*", sDir);
	countmutex.unlock();

		if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
	{
		cerr << "Path not found\n" << endl;
		return;
	}
		do
		{
			//Find first file will always return "."
			//and ".." as the first two directories.
			if (strcmp(fdFile.cFileName, ".") != 0
				&& strcmp(fdFile.cFileName, "..") != 0)
			{
				countmutex.lock();
				sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);
				countmutex.unlock();

				//Is recursion active?
				if (r == 1) {
					//Is the entity a File or Folder?
					if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY && recursive > 0)
					{
						if (v == 1) {
							cout << "The following path will be read: " << sPath << endl;
						}
						recursive--;
						ListDirectoryContents(sPath);
					}
					else {
						//Filter the files if filter is given
						if (f == 1) {
							if (fc == 1) {
								string stringpath = sPath;
								if (stringpath.find((string)Filter[0]) == std::string::npos) {
									ReadAllBytes(sPath);
								}
							}
							else if (fc == 2) {
								string stringpath = sPath;
								if ((stringpath.find((string)Filter[0]) == std::string::npos) && (stringpath.find((string)Filter[1]) == std::string::npos)) {
									ReadAllBytes(sPath);
								}
							}
							else if (fc == 3) {
								string stringpath = sPath;
								if ((stringpath.find((string)Filter[0]) == std::string::npos) && (stringpath.find((string)Filter[1]) == std::string::npos) && (stringpath.find((string)Filter[2]) == std::string::npos)) {
									ReadAllBytes(sPath);
								}
							}
						}
						else {
							ReadAllBytes(sPath);
						}
					}
				}
				else {
					//Is the entity a File or Folder?
					if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
					{
						if (r == 1) {
							if (v == 1) {
								cout << "The following path will be read: " << sPath << endl;
							}
							recursive--;							
							ListDirectoryContents(sPath);
						}
						else {
							if (v == 1) {
								cout << "The following path will be read: " << sPath << endl;
							}
							ListDirectoryContents(sPath);
						}
					}
					else {
						//Filter the files if filter is given
						if (f == 1) {
							if (fc == 1) {
								string stringpath = sPath;
								if (stringpath.find((string)Filter[0]) == std::string::npos) {
									if (t == 1) {
										if (maxThreads > 0 && threadmarker == 1) {
											decreasemaxThreads(1);
											thread(ReadAllBytes, sPath).detach();
										}
										else if (maxThreads > 0 && threadmarker == 0) {
											threadmarker = 1;
											decreasemaxThreads(1);
											thread(ReadAllBytes, sPath).detach();
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
								string stringpath = sPath;
								if ((stringpath.find((string)Filter[0]) == std::string::npos) && (stringpath.find((string)Filter[1]) == std::string::npos)) {
									if (t == 1) {
										if (maxThreads > 0 && threadmarker == 1) {
											decreasemaxThreads(1);
											thread(ReadAllBytes, sPath).detach();
										}
										else if (maxThreads > 0 && threadmarker == 0) {
											threadmarker = 1;
											decreasemaxThreads(1);
											thread(ReadAllBytes, sPath).detach();
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
								string stringpath = sPath;
								if ((stringpath.find((string)Filter[0]) == std::string::npos) && (stringpath.find((string)Filter[1]) == std::string::npos) && (stringpath.find((string)Filter[2]) == std::string::npos)) {
									if (t == 1) {
										if (maxThreads > 0 && threadmarker == 1) {
											decreasemaxThreads(1);
											thread(ReadAllBytes, sPath).detach();
										}
										else if (maxThreads > 0 && threadmarker == 0) {
											threadmarker = 1;
											decreasemaxThreads(1);
											thread(ReadAllBytes, sPath).detach();
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
									decreasemaxThreads(1);
									thread (ReadAllBytes, sPath).join();
								}
								else if (maxThreads > 0 && threadmarker == 0) {
									threadmarker = 1;
									decreasemaxThreads(1);
									thread(ReadAllBytes, sPath).join();
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
			}
		} while (FindNextFile(hFind, &fdFile));

		FindClose(hFind);	
}

/*Helpfunktion*/
void HelpFunction() {
	cout <<"Dieses Programm kann folgende zusaetzliche Operatoren:\n\ncntFileBits [-r [n]] [-f fileFilter] [-t maxThreads] [-h] [-v] [-w] -s startPath\n\n";
	cout << "startPath\t Gibt das Startverzeichnis an, ab dem die Dateien gelesen werden sollen.\n\t\t die Option -s kann auch mehrfach angegeben werden, z.B. wenn zwei Partitionen\n\t\t durchsucht werden sollen.\n\n";
	cout << "-r [n]\t\t Rekursives Lesen der Unterverzeichnisse. wenn n (bei n>=1) angegeben, dann\n\t\t bestimmt n die Tiefe der Rekursion; wird n nicht angegeben, dann werden\n\t\t rekursiv alle unter dem Startverzeichnis stehenden Verzeichnisse und deren\n\t\t Dateien gelese.\n\n";
	cout << "-f fileFilter\t fileFilter gibt an, welche Dateien gelesen werden sollen; z.B. *.iso oder bild *.jpg;\n\t\t wird diese Option nicht angegeben, so werden alle Dateien gelesen.\n\n";
	cout << "-t maxThreads\t maximale Anzahl an Threads; wird diese Option nicht angegeben, dann\n\t\t wird die Anzahl der Threads automatisch optimiert\n\n";
	cout << "-h\t\t Anzeige der Hilfe & Copyright Info: wird automatisch angezeigt, wenn beim\n\t\t Programmstart keine Option angegeben wurde.\n\n";
	cout << "-p\t\t Ausgabe der Prozessierungszeit auf stdout in Sekunden.Millisekunden\n\n";
	cout << "-v\t\t Erweiterte Ausgabe etwaiger Prozessierungsinformationen\n\n";
	cout << "-w\t\t Warten auf eine Taste unmittelbar bevor die Applikation terminiert\n\n\n";
	cout << "Copyright© by Christopher Wieland and Stephanie Kaschnitz\n";
	cout << "Creation on: 07.12.2017\n\n";
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
		cerr << "Bitte geben Sie fuer den Parameter Zahl groesser 1 ein!\n\n";
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
		cerr << "Bitte geben Sie fuer den Parameter Zahl groesser 1 ein!\n\n";
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
				thread(StoreRecursiveParameter,argv[i + 1], recursive, r).detach();
			}			
			//If -f is given, then it must be a filter ... store the filters!
			else if (strcmp(argv[i], "-f") == 0) {
				f = 1;
				thread(StoreFilterInArray,Filter, argv[i + 1], fc).detach();
			}
			//If -t is given, get the Count of maximum Threads
			else if (strcmp(argv[i], "-t") == 0) {
				t = 1;
				thread(StoremaxThreadsParameter,argv[i + 1], maxThreads, t).detach();
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
				thread(StorePathInArray,Paths, argv[i+1], pc).detach();
			}
		}
	}

	if (s == 0) {
		cerr << "Es wurde kein Pfad zur Suche angegeben! Bitte geben Sie einen Pfad an.\n\n";
	}

	if (pc > 0) {
		for (int i = 0; i < pc; i++) {
			thread(ListDirectoryContents, Paths[i]).join();
		}
	}
	else {
		ListDirectoryContents(Paths[0]);
	}
	
	if (v == 1) {
		if (f == 1) {
			cout << "The choosen filters are: " << endl;
			for (int i = 0; i < fc; i++) {
				cout << Filter[i] << endl;
			}
		}
		if (t == 1) {
			cout << "\nAmount of threads is limited by: " << settedthread << endl;
			cout << "Used Threads: " << settedthread - maxThreads << endl;
		}
		if (r == 1) {
			cout << "\nThe choosen recursive depth is: " << recursive << endl;
		}
	}

	cout << "\nThats the amount of ones: " << onescounter << endl;
	cout << "Thats the amount of zeros: " << zeroscounter << endl;
	
	//Plot the process time
	if (p == 1) {
		cout << "Needed time: " << (clock() - startSingle) / double(CLOCKS_PER_SEC) * 1000 << " ms \n";
	}	
	//Let the user press a key to end the program.
	if (w == 1) {
		cin.ignore();
	}

	return 0;
}