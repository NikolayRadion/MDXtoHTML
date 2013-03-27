#ifndef __MDXtoHTML_FORMATTER__
#define __MDXtoHTML_FORMATTER__

#include <string>

/**
*	Класс форматирующий строку токена, заменяя TeX последовательности и оформляя *text* и **text**
*/
class Formatter
{

	/**
	*	Замена всех *text* на <b>text</b>
	*/
	string bold(string str) const
	{
		string ret;
		unsigned pos = 0;
		unsigned offset = 0;

		while ( pos != str.npos )
		{
			pos = str.find(" *", offset);
			
			// Нашли открывающую звездочку
			if ( pos != str.npos )
			{
				ret += str.substr( offset, pos - offset );
				unsigned nextpos = str.find("* ", pos + 1);

				// Нашли закрывающую звездочку
				if ( nextpos != str.npos )
				{
					ret += "<b>" + str.substr(pos+1, nextpos - pos - 1) + "</b>";
					offset = nextpos + 1;
				}
				else
					break;
			}
			else
				break;
		}

		ret += str.substr( offset );

		return ret;
	}

	string TeX(string str) const
	{
		string ret = str;
		unsigned s_count = 50 + 59;

		string s_tex[] = {"\\alpha", "\\Alpha", // Greek
							"\\beta", "\\Beta",
							"\\gamma", "\\Gamma",
							"\\delta", "\\Delta",
							"\\epsilon", "\\Epsilon",
							"\\zeta", "\\Zeta",
							"\\eta", "\\Eta",
							"\\theta", "\\Theta",
							"\\iota", "\\Iota",
							"\\kappa", "\\Kappa",
							"\\lambda", "\\Lambda",
							"\\mu", "\\Mu",
							"\\nu", "\\Nu",
							"\\xi", "\\Xi",
							"\\omicron", "\\Omicron",
							"\\pi", "\\Pi",
							"\\rho", "\\Rho",
							"\\sigma", "\\Sigma",
							"\\sigmaf", "C2 	",
							"\\tau", "\\Tau",
							"\\upsilon", "\\Upsilon",
							"\\phi", "\\Phi",
							"\\psi", "\\Psi",
							"\\chi", "\\Chi",
							"\\omega", "\\Omega",
							"\\frasl", "\\weierp", "\\image", "\\real \\trade", // Math
							"\\alefsym", "\\larr", "\\uarr", "\\rarr", "\\darr",
							"\\harr", "\\crarr", "\\lArr", "\\uArr", "\\rArr",
							"\\dArr", "\\hArr", "\\forall", "\\part", "\\exist",
							"\\empty", "\\nabla", "\\in", "\\notin", "\\ni",
							"\\prod", "\\sum", "\\minus", "\\lowast", "\\radic",
							"\\prop", "\\infin", "\\ang", "\\and", "\\or",
							"\\cap", "\\cup", "\\int", "\\there4", "\\sim",
							"\\cong", "\\asymp", "\\ne", "\\equiv", "\\le",
							"\\ge", "\\sub", "\\sup", "\\nsub", "\\sube",
							"\\supe", "\\oplus", "\\otimes", "\\perp", "\\sdot",
							"\\lceil", "\\rceil", "\\lfloor", "\\rfloor", "\\setminus"};

		string s_code[] = {	"&alpha;", "&Alpha;", // Greek
							"&beta;", "&Beta;",
							"&gamma;", "&Gamma;",
							"&delta;", "&Delta;",
							"&epsilon;", "&Epsilon;",
							"&zeta;", "&Zeta;",
							"&eta;", "&Eta;",
							"&theta;", "&Theta;",
							"&iota;", "&Iota;",
							"&kappa;", "&Kappa;",
							"&lambda;", "&Lambda;",
							"&mu;", "&Mu;",
							"&nu;", "&Nu;",
							"&xi;", "&Xi;",
							"&omicron;", "&Omicron;",
							"&pi;", "&Pi;",
							"&rho;", "&Rho;",
							"&sigma;", "&Sigma;",
							"&sigmaf;", "C2 	",
							"&tau;", "&Tau;",
							"&upsilon;", "&Upsilon;",
							"&phi;", "&Phi;",
							"&psi;", "&Psi;",
							"&chi;", "&Chi;",
							"&omega;", "&Omega;",
							"&frasl;", "&weierp;", "&image;", "&real; &trade;", // Math
							"&alefsym;", "&larr;", "&uarr;", "&rarr;", "&darr;",
							"&harr;", "&crarr;", "&lArr;", "&uArr;", "&rArr;",
							"&dArr;", "&hArr;", "&forall;", "&part;", "&exist;",
							"&empty;", "&nabla;", "&isin;", "&notin;", "&ni;",
							"&prod;", "&sum;", "&minus;", "&lowast;", "&radic;",
							"&prop;", "&infin;", "&ang;", "&and;", "&or;",
							"&cap;", "&cup;", "&int;", "&there4;", "&sim;",
							"&cong;", "&asymp;", "&ne;", "&equiv;", "&le;",
							"&ge;", "&sub;", "&sup;", "&nsub;", "&sube;",
							"&supe;", "&oplus;", "&otimes;", "&perp;", "&sdot;",
							"&lceil;", "&rceil;", "&lfloor;", "&rfloor;", "&#92;"};

		string::size_type pos;
		for ( unsigned i=0; i < s_count; ++i )
		{
			pos = ret.find(s_tex[i]);
			while ( pos != string::npos )
			{
				ret.replace( pos, s_tex[i].length() , s_code[i] );
				pos = ret.find(s_tex[i]);
			}
		}

		return ret;
	}
public:

	string format(string str) const
	{
		string ret = bold(str);
		ret = TeX(ret);

		return ret;
	}
};

#endif