#ifndef __MDXtoHTML_SERVICE__
#define __MDXtoHTML_SERVICE__

#include <string>
#include <iostream>
#include <strstream>
#include <fstream>
#include "Utils.h"

using namespace std;

/**
*
* Вытаскием параметры и открываем файлы для работы.
* Ключи:
*	-i -- входной файл. Обязательный параметр.
*	-o -- выходной файл. Необязательный параметр. По умолчанию компилируем в out.html
*/
bool readConsoleParams( int argc, char* argv[], ifstream &input_file, ofstream &output_file )
{
	string inputFile, outputFile;
	bool dInputFile = false;
	bool dOutputFile = false;

#ifdef __DBGOUTPUT__
	MACRO_MESSAGE("---Command line:---\n");
	//stringstream ss;
#endif
	for ( int i=1; i < argc; ++i )
	{

#ifdef __DBGOUTPUT__
		//ss<<i<<":"<<argv[i]<<endl;
#endif

		if ( strcmp( argv[i], "-i" ) == 0 && argc > i + 1 )
		{
			inputFile = argv[i+1];
			dInputFile = true;
		}
		
		if ( strcmp( argv[i], "-o" ) == 0 && argc > i + 1 )
		{
			outputFile = argv[i+1];
			dOutputFile = true;
		}
	}
#ifdef __DBG__
	//MACRO_MESSAGE(ss.string());
#endif

	if ( !dInputFile )
	{
		MACRO_ERROR_RET("Error: not defined input file.", false);
	}
	if ( !dOutputFile )
		outputFile = "out.html";

#ifdef __DBGOUTPUT__
	MACRO_MESSAGE("In: " + inputFile + "\n" );
	MACRO_MESSAGE("Out: "+ outputFile + "\n---------\n");
#endif

	input_file = ifstream(inputFile);
	output_file = ofstream(outputFile);


	return true;
}

#endif