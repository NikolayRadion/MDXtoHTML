#ifndef __MDXtoHTML_NODES__
#define __MDXtoHTML_NODES__


#include <ostream>
#include "Compiler.h"
#include "MDXparser.h"
#include "Utils.h"

using namespace std;

struct node_block
{
	string type;
	TokensArray tokens;

	node_block( string &tp, TokensArray &tkns ):type(tp), tokens(tkns){}
};
inline ostream& operator<<( ostream& out, const node_block& block )
{
	if ( strcmp( block.type.c_str(), "string" ) == 0  )
	{
		out<<"<p>"<<block.tokens.front().text()<<"</p>"<<endl;
	}
	else if ( strcmp( block.type.c_str(), "header1" ) == 0 )
	{
		out<<"<h1>"<<block.tokens.back().text()<<"</h1>"<<endl;
	}
	else if ( strcmp( block.type.c_str(), "header2" ) == 0 )
	{
		out<<"<h2>"<<block.tokens.back().text()<<"</h2>"<<endl;
	}
	else if ( strcmp( block.type.c_str(), "header3" ) == 0 )
	{
		out<<"<h3>"<<block.tokens.back().text()<<"</h3>"<<endl;
	}
	else if ( strcmp( block.type.c_str(), "header4" ) == 0 )
	{
		out<<"<h4>"<<block.tokens.back().text()<<"</h4>"<<endl;
	}
	else if ( strcmp( block.type.c_str(), "Define" ) == 0 )
	{
		out<<"<dl><dt>"<<block.tokens[1].text()<<"</dt><dd>"<<block.tokens[2].text()<<"</dd></dl>"<<endl;
	}
	else if ( strcmp( block.type.c_str(), "Theorem" ) == 0 )
	{
		out<<"<dl class='theorem'><dt>"<<block.tokens[1].text()<<"</dt>\n";
		if ( block.tokens.size() > 2 )
			out<<"dd"<<block.tokens[2].text()<<"</dd>\n";
		out<<"</dl>\n"<<endl;
	}
	return out;
}

struct node_part
{
	node_block* block;
	node_part* nextpart;

	node_part( node_block* blck, node_part* nxtpart ):block(blck), nextpart(nxtpart){}
};
inline ostream& operator<<( ostream &out, const node_part& part)
{
	if ( part.block )
		out<<*part.block;
	if ( part.nextpart )
		out<<*part.nextpart;

	return out;
}

struct node_contains
{
	node_part* part;
	Token separator;
	node_contains* contains;

	node_contains( node_part* prt, Token sep, node_contains* cnts ): part(prt), separator(sep), contains(cnts){}
};
inline ostream& operator<<( ostream &out, const node_contains& contains )
{
	if ( contains.part )
		out<<*contains.part;
	if ( contains.separator.type() == ttLine )
	{
		out<<"<hr />"<<endl;
		out<<*contains.contains;
	}
	return out;
}

struct node_document
{
	node_contains* contains;

	node_document( node_contains* cnts ):contains(cnts){}
};
inline ostream& operator<<( ostream &out, const node_document& document )
{
	out<<"<html>\n<head>\n<meta charset='utf-8'>\n<title>Compiled MDX to HTML.</title>\n<style> dd {font-style: italic; } dt {margin-top: 1em; } dl.theorem dt {font-weight: bold;}</style></head>\n<body>"<<endl;

	out<<*document.contains;

	out<<"</body>\n</html>";

	return out;
}





#endif