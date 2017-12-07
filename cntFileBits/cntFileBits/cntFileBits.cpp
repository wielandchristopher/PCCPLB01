#include "stdafx.h"
#include <iostream>


/*Hilfefunktion*/
int hilfefunktion() {
	printf("Dieses Programm kann folgende zusaetzliche Operatoren:\n\ncntFileBits [-r [n]] [-f fileFilter] [-t maxThreads] [-h] [-v] [-w] startPath\n\n");
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

	//Abfrage auf Hilfe oder keine Angabe
	if ((argc == 1) || (strcmp(argv[1], "-h") == 0))
	{
		hilfefunktion();
		return 0;
	}



	for (int i = 0; i < argc; ++i) {
		
		
		std::cout << argv[i] << std::endl;
	}

}
 
