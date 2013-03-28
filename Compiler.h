#ifndef __MDXtoHTML_COMPILER__
#define __MDXtoHTML_COMPILER__

#include <sstream>
#include "MDXparser.h"
#include "Utils.h"
#include "nodes.h"

class Compiler
{
	TokensArray _tokens;
	unsigned ptr;
	unsigned current_string;
	string cur_str()
	{
		stringstream ret;
		ret<<current_string;
		return ret.str();
	}
	unsigned current_token;
	string cur_token()
	{
		stringstream ret;
		ret<<current_token;
		return ret.str();
	}
	/**
	*	�������� ��������� �������� ������ ������ �� ������� � ������� ������� �������� ������ � �����.
	*/
	void nextToken()
	{
		if ( _tokens[ptr].type() == ttNewline )
		{
			current_string++;
			current_token = 1;
		}
		else
		{
			current_token++;
		}
		++ptr;
	}

	

	node_document* root;

	/**
	* ����� ������� ����������� ���������� � ������� ������ �������
	*	document -> contains
	*	contains -> part '---' contains | part
	*	part	 -> block part | block
	*	block	 -> text
	*			  | header
	*			  | define
	*			  | theorem
	*			  | example
	*			  | list
	*/
	node_document* Cdocument()
	{
		return new node_document( Ccontains() );
	}

	node_contains* Ccontains()
	{
		node_part* npart = Cpart();
		Token line;
		node_contains* nextcontains = 0;
		// ���� ��� �� ��������� ������, �� ��������� ��������� ����
		if ( _tokens[ptr].type() == ttLine )
		{
			line = _tokens[ptr];
			nextToken(); nextToken();
			nextcontains = Ccontains();
		}
		return new node_contains(npart, line, nextcontains);
	}

	node_part* Cpart()
	{
		node_block* block = Cblock();
		node_part* nextpart = 0;
		// ���� �� ���� ������ ������ �����, �� ������� ������� ��� ���� �����
		if ( block )
		{
			nextpart = Cpart();
			return new node_part(block, nextpart);
		}
		else
			return 0;
	}

	node_block* Cblock()
	{
		TokensArray ta;
		string stype;
		if ( _tokens[ptr].type() == ttString && _tokens[ptr+1].type() == ttNewline )
		{
			ta.push_back( _tokens[ptr] );
			// ������ ���� ������� � ������� ������ ����� ��.
			nextToken(); nextToken();
			stype = "string";
		}
		else if ( _tokens[ptr].type() == ttHeader  )
		{
			// ���������, ��� ����� ���������� ��������� ���� ��� ������ � ������
			if ( _tokens.size() < ptr + 2 )
				MACRO_ERROR_RET("Compiler::After header announcement not found string. String: " + cur_str() + ", token: " + cur_token(), 0);

			// ������ �����
			ta.push_back(_tokens[ptr]);
			// �������� ����� � ������
			nextToken(); nextToken();
			// ������ ������ � ��������� ������.
			ta.push_back(_tokens[ptr]);
			nextToken(); nextToken();

			if ( ta.front().text().length() == 1 )
			{
				stype = "header1";	
			}
			else if ( ta.front().text().length() == 2 )
			{
				stype = "header2";
			}
			else if ( ta.front().text().length() == 3 )
			{
				stype = "header3";
			}
			else if ( ta.front().text().length() == 4 )
			{
				stype = "header4";
			}
			else
				MACRO_ERROR_RET("Compiler::Bad count of '#' in header block. String: " + cur_str() + ", token: " + cur_token(), 0);
		}
		else if ( _tokens[ptr].type() == ttDefine )
		{
			stype = "Define";
			ta.push_back( _tokens[ptr] );
			nextToken(); nextToken();

			unsigned sep = _tokens[ptr].text().find('-');

			if ( sep < 0 )
				MACRO_ERROR_RET("Compiler:: void of '-' in Def: clause. String: " + cur_str() + ", token: " + cur_token(), 0);
			
			string def = _tokens[ptr].text().substr(0, sep );
			string ine = _tokens[ptr].text().substr(sep + 1, _tokens[ptr].text().length() - sep );

			ta.push_back( Token(ttString, def) );
			ta.push_back( Token(ttString, ine) );

			nextToken(); nextToken();
		}
		else if ( _tokens[ptr].type() == ttTheorem )
		{
			stype = "Theorem";
			ta.push_back( _tokens[ptr] );
			nextToken(); nextToken();

			// ������ ����������� �������
			ta.push_back( _tokens[ptr] );
			nextToken(); nextToken();

			// ���� ���� �������������� (��� 4 ������: Proof:, ���� �������������� � ��� �����������), �� � ��� ����������
			// ToDo: ������� �������������� ������ � ��������.
			if ( _tokens[ptr].type() == ttProof && _tokens.size() > ptr + 2 )
			{
				ta.push_back( _tokens[ptr] );
				nextToken(); nextToken();

				ta.push_back( _tokens[ptr] );
				nextToken(); nextToken();
			}
		}
		else if ( _tokens[ptr].type() == ttNote )
		{
			stype = "Note";
			ta.push_back( _tokens[ptr] );
			nextToken(); nextToken();
			ta.push_back( _tokens[ptr] );
			nextToken(); nextToken();
		}
		else if ( _tokens[ptr].type() == ttComment )
		{
			stype = "Comment";
			ta.push_back( _tokens[ptr] );
			nextToken(); nextToken();
			ta.push_back( _tokens[ptr] );
			nextToken(); nextToken();
		}
		else if ( _tokens[ptr].type() == ttExample )
		{
			stype = "Example";
			ta.push_back( _tokens[ptr] );
			nextToken(); nextToken();
			ta.push_back( _tokens[ptr] );
			nextToken(); nextToken();
		}
		else
			MACRO_ERROR_RET("Compiler::Unknown type of node_block '" + _tokens[ptr].text() + "'. String: " + cur_str() + ", token: " + cur_token(), 0);

		return new node_block( stype, ta );
	}
public:
	
	void load_tokens( TokensArray tokens )
	{
		_tokens = tokens;
		ptr = 0;
		current_string = 1;
		current_token = 1;
	}

	void compile()
	{
		root = Cdocument();
	}

	void print()
	{
		cout<<*root;
	}

	friend inline ostream& operator<<( ostream &out, const Compiler& compiler );
};

inline ostream& operator<<( ostream &out, const Compiler& compiler )
{
	return out<<*compiler.root;
}

#endif