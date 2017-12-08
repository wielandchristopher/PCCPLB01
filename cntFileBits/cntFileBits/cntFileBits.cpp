#include "stdafx.h"
#include <time.h>
#include <iostream>

using namespace std;
#pragma warning(disable: 4996)

/*Helpfunktion*/
void hilfefunktion() {
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
unsigned short StoreStringInArray(char* Destination[], char* Source, unsigned short counter) {

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

	return counter;
}

/*Will be used to get the IntegerParameters for threads or the recursives */
unsigned short StoreIntParameters(char* Source, unsigned short test, unsigned short global) {

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
		return 0;
	}

}

/*Mainfunction*/
int main(int argc, char** argv) {

	clock_t startSingle = clock();
	unsigned short r = 0,f = 0, t = 0, p = 0, v = 0, w = 0, s = 0;

	//Zero parameters
	if (argc == 1)
	{
		hilfefunktion();
		return 0;
	}

	/*THREADFÄHIG*/
	//Looking for Parameters
	if (argc >= 1) {

		char* Paths[100];
		unsigned short pc = 0;
		char* Filter[100];
		unsigned short fc = 0;

		for (int i = 0; i < argc; ++i) {

			//If -h is given, show help
			if (strcmp(argv[i], "-h") == 0) {
				hilfefunktion();
				return 0;
			}
			//If -r is given, get the numer of recursion
			else if (strcmp(argv[i], "-r") == 0) {
				r = 1;
				unsigned short rekursiv = 0;
				rekursiv = StoreIntParameters(argv[i + 1], rekursiv, r);
			}			
			//If -f is given, then it must be a filter ... store the filters!
			else if (strcmp(argv[i], "-f") == 0) {
				f = 1;
				fc = StoreStringInArray(Filter, argv[i + 1], fc);
			}
			//If -t is given, get the Count of maximum Threads
			else if (strcmp(argv[i], "-t") == 0) {
				t = 1;
				unsigned short maxThreads = 0;
				maxThreads = StoreIntParameters(argv[i + 1], maxThreads, t);				
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
			else if (strcmp(argv[i], "-s") == 0) {
				s = 1;								
				pc = StoreStringInArray(Paths, argv[i+1], pc);				
			}
		}
	}

	if (s == 0) {
		cerr << "Es wurde kein Pfad zur Suche angegeben! Bitte geben Sie einen Pfad an.\n\n";
	}



	//DO MAGIC HERE and count the bits from the stores paths.
	// Im char* Array Paths befinden sich alle angegebenen Pfade, welche durchsucht werden müssen.
	// wenn f = 1 -> im char* Array Filter befinden sich alle Filter, welche mitgenommen werden müssen (sofern Filter angegeben sind).
	// mit pc und fc wissen wir, wie viele Filter bzw. Pfade angegeben wurden.
	// wenn r = 1 -> eine Zahl der tiefe der Rekursion wurde angegeben. Die Variable dazu lautet "rekursiv" (unsigned short)
	// wenn t = 1 -> eine Zahl der maximalen Threads wurde angegeben. Die Variable dazu lautet "maxThreads" (unsigned short)




	//Show detailed Process information - HIER MÜSSEN NOCH WIRKLICHE INFORMATIONEN REIN! 
	if (v == 1) {
		cout << "Etwaige Prozessierungsinformation :D\n";
	}
	//Plot the process time
	if (p == 1) {
		cout << "Zeit Single threaded: " << (clock() - startSingle) / double(CLOCKS_PER_SEC) * 1000 << " ms \n";
	}	
	//Let the user press a key to end the program.
	if (w == 1) {
		cin.ignore();
	}

	return 0;
}