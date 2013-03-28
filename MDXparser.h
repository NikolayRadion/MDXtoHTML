#ifndef __MDXtoHTML_parser__
#define __MDXtoHTML_parser__

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "Utils.h"
#include "Formatter.h"

using namespace std;

enum TOKEN_TYPES
{
	ttHeader,
	ttDefine,
	ttTheorem,
	ttProof,
	ttNote,
	ttComment,
	ttExample,
	ttString,
	ttText,
	ttResWord,
	ttStructure,
	ttDevider,
	ttNewline,
	ttTab,
	ttLine,
	ttUnknown
};

class Token	
{
	friend class Lexer;
	TOKEN_TYPES _type;
	string _text;
	Formatter formatter;
public:
	TOKEN_TYPES type() const
	{
		return _type;
	}

	string text() const
	{
		return _text;
	}

	string format_text() const
	{
		return formatter.format(_text);
	}

	Token(): _type( ttUnknown ) {}
	Token( const Token &other ): _type( other.type() ), _text( other.text() ) {}
	Token( TOKEN_TYPES type, const string &text ): _type( type ), _text( text ), formatter() {}

	Token& operator=(const Token &other)
	{
		_type = other.type();
		_text = other.text();

		return *this;
	}

	bool operator==(const Token &other) const
	{
		return _type == other.type() && _text == other.text();
	}
};

typedef vector<Token> TokensArray;
typedef vector<string> StringsArray;

Token ErrorToken(ttUnknown, "ErrorToken");

class Lexer
{
	StringsArray _res_words, // Ключевые слова
				 _structures;   // Имена структур
	string		 _deviders;	 // Разделители

	TokensArray _tokensBuffer;

	unsigned _offset;

	TOKEN_TYPES identify_token_type( const string &text ) const
	{
		if ( _deviders.find( text ) != _deviders.npos )
		{
			// Фильтруем переводы строк
			if ( text[0] == '\n' )
				return ttNewline;
			if ( text[0] == '\t' )
				return ttTab;

			return ttDevider;
		}
		else if ( find(_res_words.begin(), _res_words.end(), text ) != _res_words.end() )
			return ttResWord;
		else if ( find(_structures.begin(), _structures.end(), text ) != _structures.end() )
		{
			// Фильтруем заголовки
			if ( text[0] == '#')
				return ttHeader;
			// Фильтруем разделительные линии
			if ( strcmp( text.c_str(), "---") == 0 )
				return ttLine;
			// Фильтруем определения
			if ( strcmp ( text.c_str(), "Def:") == 0 )
				return ttDefine;
			// Фильтруем теоремы
			if ( strcmp ( text.c_str(), "Thm:") == 0 )
				return ttTheorem;
			// Фильтруем доказательства
			if ( strcmp ( text.c_str(), "Proof:") == 0 )
				return ttProof;
			// Фильтруем замечания
			if ( strcmp ( text.c_str(), "Note:") == 0 )
				return ttNote;
			// Фильтруем примеры
			if ( strcmp ( text.c_str(), "Ex:") == 0 )
				return ttExample;

			return ttStructure;
		}
		else
		{
			return ttString;
		}
	}

	Token scan_token( const string &text)
	{
		Token ret;

		static const string delim = _deviders;

		unsigned delimpos = text.find_first_of(delim, _offset);

		// Выделение последней лексемы
		if ( delimpos == text.npos )
			ret._text = text.substr( _offset );
		// Иначе выделяем кусок из середины
		else
		{
			ret._text = text.substr( _offset, max( delimpos - _offset, unsigned(1) ) );;
			
			_offset = max( delimpos, _offset + 1 );
		}

		ret._type = identify_token_type( ret._text );
		return ret;
	}

public:
	const TokensArray& get_tokens() const
	{
		return _tokensBuffer;
	}

	void save_tokens() const
	{
		string types[] = {  "ttHeader",
							"ttDefine",	
							"ttTheorem",
							"ttProof",
							"ttNote",
							"ttComment",
							"ttExample",
							"ttString",
							"ttText",
							"ttResWord",
							"ttStructure",
							"ttDevider",
							"ttNewline",
							"ttTab",
							"ttLine",
							"ttUnknown"};

		for ( unsigned i = 0; i<_tokensBuffer.size(); ++i )
			MACRO_MESSAGE(types[ _tokensBuffer[i].type() ] + ": '" + _tokensBuffer[i].text() + "'");
	}

	bool lex(const string& text)
	{
		_offset = 0;
		_tokensBuffer.clear();

		if ( text.empty() )
			MACRO_ERROR_RET("Lexer::Lex trying to lex empty text", false);

		unsigned prev_offset = -1;
		Token token;

		do
		{
			if ( prev_offset == _offset )
				MACRO_ERROR_RET("Lexer::Lex error. Possibly end missed", false);

			prev_offset = _offset;

			token = scan_token(text);

			// Фильтруем плюсы, которые не являются маркерами списков
			if ( token.type() == ttStructure && token.text()[0] == '+' && _tokensBuffer[_tokensBuffer.size()-1].type() != ttTab )
				token._type = ttString;


			if ( token == ErrorToken )
				MACRO_ERROR_RET("Lexer::Lex error token", false);

			// Собираем подряд идущие токены ttString, если они на одной строке.
			if ( token.type() == ttString && _tokensBuffer.size() > 1 && _tokensBuffer[_tokensBuffer.size()-2].type() == ttString && _tokensBuffer[_tokensBuffer.size()-1].type() != ttNewline )
			{
				// Дописываем к тесту предыдущего ttString текст текущего и разделяем пробелом.
				_tokensBuffer[_tokensBuffer.size()-2]._text = _tokensBuffer[_tokensBuffer.size()-2].text() + " " + token.text();
				// Выкидываем ненужный теперь пробел.
				_tokensBuffer.pop_back();
			}
			// Собираем подряд идущие токены ttTab, если они на одной строке.
			else if ( token.type() == ttTab && _tokensBuffer.size() > 1 && _tokensBuffer[_tokensBuffer.size()-1].type() == ttTab)
			{
				_tokensBuffer[_tokensBuffer.size()-1]._text += token.text();
			}
			else
				_tokensBuffer.push_back( token );
		} while ( token.text() != "end" ); // ToDo: Придумать, чем ограничивать лексер.

		return true;
	}

	void Init(const string* res_words, unsigned rwsize,
			  const string* structures, unsigned stsize,
			  const string* deviders, unsigned desize)
	{
		_res_words.resize(rwsize);
		copy( res_words, res_words + rwsize, _res_words.begin() );

		_structures.resize(stsize);
		copy( structures, structures + stsize, _structures.begin() );

		for ( int i=0; i < desize; ++i )
			_deviders += deviders[i];
	}
};

#endif