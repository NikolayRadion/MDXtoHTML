#ifndef __MDXtoHTML_FORMATTER__
#define __MDXtoHTML_FORMATTER__

#include <string>

/**
*	����� ������������� ������ ������, ������� TeX ������������������ � �������� *text* � **text**
*/
class Formatter
{

	/**
	*	������ ���� *text* �� <b>text</b>
	*/
	string bold(string str) const
	{
		string ret;
		unsigned pos = 0;
		unsigned offset = 0;

		while ( pos != str.npos )
		{
			pos = str.find('*', offset);
			
			// ����� ����������� ���������
			if ( pos != str.npos )
			{
				ret += str.substr( offset, pos - offset );
				unsigned nextpos = str.find('*', pos + 1);

				// ����� ����������� ���������
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
public:

	string format(string str) const
	{
		string ret = bold(str);

		return ret;
	}
};

#endif