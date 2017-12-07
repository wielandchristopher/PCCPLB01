#include "stdafx.h"
#include <iostream>

/*Hilfefunktion*/
int hilfefunktion() {
	printf("Dieses Programm kann folgende zusaetzliche Operatoren:\n\ncntFileBits [-r [n]] [-f fileFilter] [-t maxThreads] [-h] [-v] [-w] -s startPath\n\n");
	printf("startPath\t Gibt das Startverzeichnis an, ab dem die Dateien gelesen werden sollen.\n\t\t die Option -s kann auch mehrfach angegeben werden, z.B. wenn zwei Partitionen\n\t\t durchsucht werden sollen.\n\n");
	printf("-r [n]\t\t Rekursives Lesen der Unterverzeichnisse. wenn n (bei n>=1) angegeben, dann\n\t\t bestimmt n die Tiefe der Rekursion; wird n nicht angegeben, dann werden\n\t\t rekursiv alle unter dem Startverzeichnis stehenden Verzeichnisse und deren\n\t\t Dateien gelese.\n\n");
	printf("-f fileFilter\t fileFilter gibt an, welche Dateien gelesen werden sollen; z.B. *.iso oder bild *.jpg;\n\t\t wird diese Option nicht angegeben, so werden alle Dateien gelesen.\n\n");
	printf("-t maxThreads\t maximale Anzahl an Threads; wird diese Option nicht angegeben, dann\n\t\t wird die Anzahl der Threads automatisch optimiert\n\n");
	printf("-h\t\t Anzeige der Hilfe & Copyright Info: wird automatisch angezeigt, wenn beim\n\t\t Programmstart keine Option angegeben wurde.\n\n");
	printf("-p\t\t Ausgabe der Prozessierungszeit auf stdout in Sekunden.Millisekunden\n\n");
	printf("-v\t\t Erweiterte Ausgabe etwaiger Prozessierungsinformationen\n\n");
	printf("-w\t\t Warten auf eine Taste unmittelbar bevor die Applikation terminiert\n\n\n");
	printf("Copyright© by Christopher Wieland and Stephanie Kaschnitz\n");
	printf("Creation on: 07.12.2017\n\n");
	return 0;
};

/*Main Funktion*/
int main(int argc, char** argv) {

	int r = 0;
	int f = 0;
	int t = 0;
	int p = 0;
	int v = 0;
	int w = 0;
	int s = 0;
	char startpath[3000];
	char fileFilter[3000];
	char cmaxThreads[3000];
	char crekursiv[3000];
	int rekursiv = 0;
	int maxThreads = 0;

	//Abfrage auf Hilfe oder keine Angabe
	if ((argc == 1) || (strcmp(argv[1], "-h") == 0))
	{
		hilfefunktion();
		return 0;
	}

	/*THREADFÄHIG??*/
	//Parameterabfrage
	if (argc > 1) {
		for (int i = 0; i < argc; ++i) {

			if (strcmp(argv[i], "-r") == 0) {
				r = 1;
				strcpy_s(crekursiv, argv[i + 1]);
				
				//Check, ob rekursiv eine Zahl ist.
				if (atoi(crekursiv) != 0) {
					rekursiv = atoi(crekursiv);
				}
				else {
					printf("Bitte geben Sie fuer die Rekursion eien Zahl ein!\n\n");
					return 0;
				}
			}
			else if (strcmp(argv[i], "-f") == 0) {
				f = 1;
				strcpy_s(fileFilter, argv[i + 1]);				
			}
			else if (strcmp(argv[i], "-t") == 0) {
				t = 1;
				strcpy_s(cmaxThreads, argv[i + 1]);
				
				//Check, ob maxThreads eine Zahl ist.
				if (atoi(cmaxThreads) != 0) {
					maxThreads = atoi(cmaxThreads);
				}
				else {
					printf("Bitte geben Sie fuer die Threadanzahl eien Zahl ein!\n\n");
					return 0;
				}
			}
			else if (strcmp(argv[i], "-p") == 0) {
				p = 1;
			}
			else if (strcmp(argv[i], "-v") == 0) {
				v = 1;
			}
			else if (strcmp(argv[i], "-w") == 0) {
				w = 1;
			}
			else if (strcmp(argv[i], "-s") == 0) {
				s = 1;
				strcpy_s(startpath, argv[i+1]);
			}
		}
	}

	if (s == 0) {
		printf("Es wurde kein Pfad zur Suche angegeben! Bitte geben Sie einen Pfad an.\n\n");
	}




	return 0;
}
 
