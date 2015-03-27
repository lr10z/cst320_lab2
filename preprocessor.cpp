/***********************************************************
* Author:					Leander Rodriguez
* Date Created:				04/12/14
* Last Modification Date:	04/18/14
* Lab Number:				CST 320 Lab 1B
* Filename:					preprocessor.cpp
*
************************************************************/
#include <regex>

#include "preprocessor.h" 
#include "symbolTable.h"

symbolTable *sT = new symbolTable();

const regex IDENTIFIER("^[a-zA-Z_][a-zA-Z0-9_]*$");

/*******************************************************************
* Purpose:  To modify the input file by replacing symbols with
*			constant values, and removing comments
*
* Entry:	Input file string
*
* Exit:		Modified input file
*
*********************************************************************/
preprocessor::preprocessor( string line )
{
	// local data
	string str, output;
	//symbolTable *sT = new symbolTable();

	// string buffer created
	istringstream iss(line);

	// parses string into new lines
	while (getline(iss, line, '\n'))
	{
		// remove all comments found in a line
		size_t pos = line.rfind("//");

		if (pos != string::npos)
		{
			line.replace(pos, string::npos, "");
		}

		// checks if line is not empty
		if (!line.empty())
		{
			// checks if line begins with a constant
			if (line.at(0) == '#')
			{
				// line buffer created
				stringstream ss(line);

				// parse lines into individual strings/tokens
				while (ss >> str)
				{
					if (m_directive == false)
					{
						if ( str == "#else" ||  str == "#endif")
						{
							m_directive = true;
						}
						else if (str == "#elif")
						{
							string str2 = "", str3;
							int op, s3;

							ss >> str >> str2 >> str3;

							if (sT->search(str))
							{
								// searches symbol table and analyzes symbol
								bool check;
								string op1 = sT->getSymbol(str).begin()->value;

								if (str2 == "==")
								{
									check = (op1 == str3);
								}
								else if (str2 == "<")
								{
									op = stoi(op1);
									s3 = stoi(str3);
									check = (op < s3);
								}
								else if (str2 == ">")
								{
									op = stoi(op1);
									s3 = stoi(str3);
									check = (op > s3);
								}
								else if (str2 == "!=")
								{
									check = (op1 != str3);
								}

								// decides whether or not to include the next line
								if (check == true)
								{
									m_directive = true;
								}
								else
								{
									m_directive = false;
								}
							}
						}

						continue;
					}

					if (str == "#endif")
					{
						m_directive = true;
						continue;
					}

					// if #define constant found, insert into symbol table
					else if (str == "#define")
					{
						string str2;

						ss >> str >> str2;

						if (regex_match(str, IDENTIFIER))
						{
							sT->insert(str, "", "", str2);
						}
						else
						{
							cout << "Error: expected an identifier" << endl;
						}
					}
					else if (str == "#ifdef" ||  str == "ifndef" )
					{
						ss >> str;

						// checks if symbol is in symbol table and sets
						// whether the next line should display or not
						if (sT->search(str))
						{
							m_directive = true;
						}
						else
						{
							m_directive = false;
						}
					}
					else if(str == "#if" || str == "#elif")
					{
						string str2 = "", str3;

						ss >> str >> str2 >> str3;

						// searches symbol table and analyzes symbol
						if (sT->search(str))
						{
							bool check;
							int op, s3;
							string op1 = sT->getSymbol(str).begin()->value;

							if (str2 == "==")
							{
								check = (op1 == str3);
							}
							else if (str2 == "<")
							{
								check = (op1 < str3);
							}
							else if (str2 == ">")
							{
								op = stoi(op1);
								s3 = stoi(str3);
								check = (op1 > str3);
							}
							else if (str2 == "!=")
							{
								op = stoi(op1);
								s3 = stoi(str3);
								check = (op1 != str3);
							}

							// decides whether or not to include the next line
							if ( check == true )
							{
								m_directive = true;
							}
							else
							{
								m_directive = false;
							}
						}
						else
						{
							m_directive = false;
						}
					}
					else if (str == "#else" && m_directive == true)
					{
						m_directive = false;
					}
					else
					{
						cout << "Error: unsupported preprocessor directive" << endl;
					}
				}
			}
			else
			{
				if (m_directive == false)
				{
					continue;
				}
				
				// symbol table iterator is created
				map<string, list<Node>>::const_iterator it = sT->begin();

				// iterate through symbol table and replace symbols with constant values
				for (it; it != sT->end(); ++it)
				{
					string first = it->first;
					string second = (--it->second.end())->value;

					// search line for symbols
					size_t pos = line.find(first);

					// for all symbols found in line, replace with constant values
					while (pos != string::npos)
					{
						line.replace(pos, first.length(), second);
						pos = line.find(first);
					}
				}

				// input file gets modified, now with constants replaced and comments removed
				output += line + '\n';
			}
		}
	}

	// modified input file is returned
	m_output = output;
}
