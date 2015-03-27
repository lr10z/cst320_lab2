/***********************************************************
* Author:					Leander Rodriguez
* Date Created:				03/02/14
* Last Modification Date:	04/16/14
* Lab Number:				CST 320 Lab 1
* Filename:					analyzer.cpp
*
************************************************************/

#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <vector>

#include "symbolTable.h"
#include "preprocessor.h"
#include "parser.h"

using  namespace  std;

const regex KEYWORD("auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|typedef|union|unsigned|void|volatile|while|asm|bool|catch|class|const_cast|delete|dynamic_cast|explicit|false|friend|inline|mutable|namespace|new|operator|private|public|protected|reinterpret_cast|static_cast|template|this|throw|true|try|typeid|typename|using|virtual|wchar_t");
const regex IDENTIFIER("^[a-zA-Z_][a-zA-Z0-9_]*$");
const regex SYMBOL("\\{|\\}|\\(|\\)|,|\\'|\"|\\'|;|\\-\\-|\\+\\+|\\+\\=|\\-\\=|\\*\\=|\\/\\=|\\=\\=|\\!|\\-\\>");
const regex OPERATOR("\\<|\\>|\\<\\=|\\>\\=|\\=|\\+|\\-|\\*|\\/|\\%|\\&|\\&\\&|\\|\\||\\||\\+\\+|\\-\\-|\\+\\=|\\<\\<|\\>\\>|\\<\\<\\=|\\>\\>\\=|\\-\\=|\\=\\=|\\!|\\-\\>");
const regex STRING("\"([^\"\\\\]|\\\\.)*\"");
const regex INTEGER("(\\+|-)?[[:digit:]]+");
const regex FLOAT("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?");

vector<string> tokens;
vector<parserEntry> gTokens;

/*******************************************************************
* Purpose:  To display token and token type header
*
* Entry:	N/A
*
* Exit:		Upon display of token and token type header
*
*********************************************************************/
const void displayHeader()
{
	cout << endl;
	cout << setw(25) << left << "Token" << right << "Token type" << endl << endl;
	cout << setw(25) << left << "-----" << right << "----------" << endl << endl;
}

/*******************************************************************
* Purpose:  To display token and token type
*
* Entry:	Current token from file and its token type
*
* Exit:		Upon display of token and token type
*
*********************************************************************/
const void displayTokens(string str, string tokenType)
{
	//cout << setw(25) << left << str << right << tokenType << endl << endl;

	gTokens.push_back(parserEntry(str, tokenType));
}

/*******************************************************************
* Purpose:  To recognize and store individual tokens. If a symbol is
*			found within a string, white space is added around it,
*			the current string is parsed into tokens, and stored. If
*			there is no symbol within the string, string is stored
*			as token.
*
* Entry:	Current string read in from file
*
* Exit:		Once all tokens have been recognized and stored
*
*********************************************************************/
void Tokenize(const string& line)
{
	if (regex_search(line, SYMBOL))							// check for symbol
	{
		string temp = regex_replace(line, SYMBOL, " $& ");  // add white space around symbols
		stringstream iss(temp);								// used to create tokens from new white space parsing
		copy(istream_iterator<string>(iss),					// store newly created tokens
			istream_iterator<string>(),
			back_inserter<vector<string> >(tokens));
	}

	else tokens.push_back(line);
}

/**********************************************************************
* Purpose:  To iterate through vector of tokens, associate each token
*			with a type, and display results.
*
* Entry:	N/A
*
* Exit:		Upon successfully iterating through all container elements.
*
***********************************************************************/
const void tokenType()
{
	// iterate through container and assign token type to each element
	for (size_t i = 0; i < tokens.size(); ++i)
	{
		string result = "";

		if (regex_match(tokens[i], IDENTIFIER))
			result = "Identifier";
		else
			result = "ERROR: Illegal identifier or integer";

		if (regex_match(tokens[i], KEYWORD))
			result = "Keyword";

		if (regex_match(tokens[i], SYMBOL))
			result = "Symbol";

		if (regex_match(tokens[i], OPERATOR))
			result = "Operator";

		if (regex_match(tokens[i], INTEGER))
			result = "Integer";
		else if (regex_match(tokens[i], FLOAT))
			result = "Float";

		displayTokens(tokens[i], result);		// display tokens and their types
	}
}

/*******************************************************************
* Purpose:  To begin program, prompt user for a file name, open file,
*			read in data from file and analyze it
*
* Entry:	N/A
*
* Exit:		Upon program running successfully
*
*********************************************************************/
int main()
{
	string	fileName,					// used to hold fileName
			line;						// used to hold line read in from file

	cout << "Enter test file name: ";	// prompt for and display file name
	cin >> fileName;
	cout << endl;

	ifstream in(fileName);				// open file

	
	if (!in)							// check if file opened properly
	{
		cout << "*** Error opening file '"
			<< fileName
			<< "' ***"
			<< endl;

		exit(EXIT_FAILURE);
	}


	//
	// preprocessing
	//
	std::string content((std::istreambuf_iterator<char>(in)),
		(std::istreambuf_iterator<char>()));

	preprocessor *preP = new preprocessor(content);

	string newInput = preP->get();


	//
	// lexical analyzing
	//
	//displayHeader();					// display token and token type header

	istringstream iss(newInput);

	while (getline(iss, line, '\n'))	// loop until end of file
	{
		string str;						// used to hold individual strings
		stringstream ss(line);			// used to iterate through line stream

		while (ss >> str)				// loop through string iterations in line
		{
			Tokenize(str);				// parse strings into tokens
		}
	}

	in.close();							// close file

	tokenType();						// recognize token type and display it

	Parser();

	return  EXIT_SUCCESS;				// exit program
}