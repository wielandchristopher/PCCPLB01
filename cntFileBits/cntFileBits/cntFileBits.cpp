#include "stdafx.h"
#include <iostream>


/*Hilfefunktion*/
int hilfefunktion() {
	printf("Dieses Programm kann folgende zusaetzliche Operatoren:\ncntFileBits [-r [n]] [-f fileFilter] [-t maxThreads] [-h] [-v] [-w] startPath\n\n");
	printf("startPath\t Test\n");
	printf("-r [n]\t\t Test\n");
	printf("-f fileFilter\t Test\n");
	printf("-t maxThreads\t Test\n");
	printf("-h\t\t Test\n");
	printf("-v\t\t Test\n");
	printf("-w\t\t Test\n\n\n");
	printf("Copyright© by Christopher Wieland and Stephanie Kaschnitz\n");
	printf("Creation on: 07.12.2017\n\n");
	return 0;
};

/*Main Funktion*/
int main(int argc, char** argv) {

	if ((argc == 1) || (strcmp(argv[1], "-h") == 0))
	{
		hilfefunktion();
		return 0;
	}



	for (int i = 0; i < argc; ++i) {
		
		
		std::cout << argv[i] << std::endl;
	}

}
 
