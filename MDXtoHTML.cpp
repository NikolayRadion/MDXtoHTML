
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
	
	// Файлы с которыми мы будем работать.
	ifstream input_file;
	ofstream output_file;

	/**
	* Собираем переданные нам параметры и открываем файлы для работы.
	* Если что-то не удалось сделать, то функция выведет ошибку, а мы закончим выполнение.
	*/
	if ( !readConsoleParams( argc, argv, input_file, output_file ) )
	{
		return 0;
	}

	// Данные для лексера пока тут. Нужно будет куда-нибудь засунуть для удобного изменения
	string res_words[] = { "reserve" };
	string structures[] = { "Def:", "Ex:", "Thm:", "Proof:", "Note:", "Comment:", "Task:", "Table:", "#", "##", "###", "####", "######", "+", "---", "*", "**" };
	string deviders[] = { " ", "\n", "{", "}", "\t"};
	// Запускаем лексический анализатор
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
