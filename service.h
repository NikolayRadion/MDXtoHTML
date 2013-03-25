#ifndef __MDXtoHTML_SERVICE__
#define __MDXtoHTML_SERVICE__

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

/**
*
* ��������� ��������� � ��������� ����� ��� ������.
* �����:
*	-if -- ������� ����. ������������ ��������.
*	-of -- �������� ����. �������������� ��������. �� ��������� ����������� � out.html
*/
bool readConsoleParams( int argc, char* argv[], ifstream &input_file, ofstream &output_file )
{
	string inputFile, outputFile;
	bool dInputFile = false;
	bool dOutputFile = false;

#ifdef __DBGOUTPUT__
	cout<<"---readConsoleParams:---"<<endl;
#endif

	for ( int i=1; i < argc; ++i )
	{

#ifdef __DBGOUTPUT__
		cout<<i<<":"<<argv[i]<<endl;
#endif

		if ( strcmp( argv[i], "-if" ) == 0 && argc > i + 1 )
		{
			inputFile = argv[i+1];
			dInputFile = true;
		}
		
		if ( strcmp( argv[i], "-of" ) == 0 && argc > i + 1 )
		{
			outputFile = argv[i+1];
			dOutputFile = true;
		}
	}

	if ( !dInputFile )
	{
		cout<<"Error: not defined input file."<<endl;
		return false;
	}
	if ( !dOutputFile )
		outputFile = "out.html";

#ifdef __DBGOUTPUT__
	cout<<"In: "<<inputFile<<endl;
	cout<<"Out: "<<outputFile<<endl<<"---------"<<endl;
#endif

	input_file = ifstream(inputFile);
	output_file = ofstream(outputFile);


	return true;
}

#endif