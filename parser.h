/***********************************************************
* Author:					Leander Rodriguez
* Date Created:				04/30/14
* Last Modification Date:	05/02/14
* Lab Number:				CST 320 Lab 2
* Filename:					parser.h
*
************************************************************/
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <iterator>

using namespace std;

struct parserEntry
{
	parserEntry( string value, string type)
	{
		m_value = value;
		m_type = type;
	}

	std::string m_value;
	std::string m_type;
};

void Parser();

#endif