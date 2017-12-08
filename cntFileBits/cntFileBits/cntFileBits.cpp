#include "stdafx.h"
#include <string>
#include <iostream>
#include <Vector>

using namespace std;
#pragma warning(disable: 4996)

/*Hilfefunktion*/
int hilfefunktion() {
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
	return 0;
};

/*Main Funktion*/
int main(int argc, char** argv) {

	short r = 0;
	short f = 0;
	short t = 0;
	short p = 0;
	short v = 0;
	short w = 0;
	short s = 0;

	//Abfrage ohne Angabe
	if (argc == 1)
	{
		hilfefunktion();
		return 0;
	}

	/*THREADFÄHIG??*/
	//Parameterabfrage
	if (argc >= 1) {

		char* Paths[100];
		int pc = 0;
		char* Filter[100];
		int fc = 0;

		for (int i = 0; i < argc; ++i) {

			//If -h is given, show help
			if (strcmp(argv[i], "-h") == 0) {
				hilfefunktion();
				return 0;
			}
			//If -r is given, get the numer of recursion
			else if (strcmp(argv[i], "-r") == 0) {
				r = 1;
				char * crekursiv = new char[strlen(argv[i + 1])];
				memcpy(crekursiv, argv[i + 1], strlen(argv[i + 1]) + 1);
				
				//Check, ob rekursiv eine Zahl ist.
				if (atoi(crekursiv) != 0) {
					int rekursiv = atoi(crekursiv);
					if (rekursiv < 1)
						r = 0;
				}
				else {
					cerr << "Bitte geben Sie fuer die Rekursion eien Zahl ein!\n\n";
					return 0;
				}
			}			
			//If -f is given, then it must be a filter ... store the filters!
			else if (strcmp(argv[i], "-f") == 0) {
				f = 1;
				char * fileFilter = new char[strlen(argv[i + 1])];
				memcpy(fileFilter, argv[i + 1], strlen(argv[i + 1]) + 1);

				if(strchr(fileFilter,';')){

					char* pch = strtok(fileFilter, ";");
					
					while (pch != NULL)
					{
						Filter[fc] = pch;
						fc++;	
						pch = strtok(NULL, ";");
					}
				}
				else {
					Filter[fc] = fileFilter;
					fc++;
				}
			}
			//If -t is given, get the Count of maximum Threads
			else if (strcmp(argv[i], "-t") == 0) {
				t = 1;
				char * cmaxThreads = new char[strlen(argv[i + 1])];
				memcpy(cmaxThreads, argv[i + 1], strlen(argv[i + 1]) + 1);
				
				//Check, ob maxThreads eine Zahl ist.
				if (atoi(cmaxThreads) != 0) {
					int maxThreads = atoi(cmaxThreads);
				}
				else {
					cerr << "Bitte geben Sie fuer die Threadanzahl eien Zahl ein!\n\n";
					return 0;
				}
			}
			//If -p is given, set the value for "give process time"
			else if (strcmp(argv[i], "-p") == 0) {
				p = 1;
			}
			//If -v is given, set the value for "give more process information"
			else if (strcmp(argv[i], "-v") == 0) {
				v = 1;
			}
			//If -w is given, set the value for "push button before end"
			else if (strcmp(argv[i], "-w") == 0) {
				w = 1;
			}
			//If : is in a given parameter, then it must be a path ... store the path!
			else if ((memchr(argv[i], ':', strlen(argv[i]) + 1) && i > 0)) {
				s = 1;				
				char * Path = new char[strlen(argv[i])];
				memcpy(Path, argv[i], strlen(argv[i]) + 1);

				Paths[pc] = Path;
				pc++;
			}
		}
	}

	if (s == 0) {
		cerr << "Es wurde kein Pfad zur Suche angegeben! Bitte geben Sie einen Pfad an.\n\n";
	}

	//DO MAGIC HERE and count the bits from the stores paths

	if (w == 1) {
		cin.ignore();
	}

	return 0;
}