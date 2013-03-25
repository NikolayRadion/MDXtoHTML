
#include <iostream>
#include <fstream>
#include <string>

#define __DBGOUTPUT__

#include "service.h"
#include "MDXparser.h"
#include "Utils.h"
#include "Compiler.h"

using namespace std;



int main( int argc, char* argv[] )
{
	setlocale(0, ""); // setlocale(LC_CTYPE, "");
	
	// ����� � �������� �� ����� ��������.
	ifstream input_file;
	ofstream output_file;

	/**
	* �������� ���������� ��� ��������� � ��������� ����� ��� ������.
	* ���� ���-�� �� ������� �������, �� ������� ������� ������, � �� �������� ����������.
	*/
	if ( !readConsoleParams( argc, argv, input_file, output_file ) )
	{
		return 0;
	}

	// ������ ��� ������� ���� ���. ����� ����� ����-������ �������� ��� �������� ���������
	string res_words[] = { "reserve" };
	string structures[] = { "Def:", "Ex:", "Thm:", "Proof:", "Note:", "Comment:", "Task:", "Table:", "#", "##", "###", "####", "######", "+", "---", "*", "**" };
	string deviders[] = { " ", "\n", "{", "}", "\t"};
	// ��������� ����������� ����������
	Lexer lex;
	lex.Init( res_words, 1, structures, 17, deviders, 5 );

	string text;
	while ( !input_file.eof() )
	{
		string buf;
		getline(input_file, buf);
		text+= buf + "\n";
	}

#ifdef __DBGOUTPUT__
	MACRO_MESSAGE("\n---text:---\n" + text);
#endif

	lex.lex(text);
	lex.save_tokens();

	
	Compiler compiler;
	compiler.load_tokens( lex.get_tokens() );
	compiler.compile();

	output_file<<compiler;

	system("pause");
    return 0;
}
