/***********************************************************
* Author:					Leander Rodriguez
* Date Created:				04/04/14
* Last Modification Date:	04/12/14
* Lab Number:				CST 320 Lab 1A
* Filename:					symbolTable.h
*
************************************************************/

#ifndef  SYMBOLTABLE_H
#define  SYMBOLTABLE_H

#include <map>
#include <string>
#include <list>
#include <iterator>

using namespace std;

/***********************************************************
*
* a struct of type Node, holds each symbol's information
*
************************************************************/
struct Node
{
	string name;
	string type;
	string use;
	string value;
};

/***********************************************************
*
* symbol table class
*
************************************************************/
class symbolTable
{
	public:

		// ctor
		symbolTable() {}

		// methods
		bool insert( string symbol, string type, string use, string value );
		bool remove( string symbol );
		bool search( string symbol );
		
		list<Node> getSymbol(string symbol) { return m_symTbl[symbol]; }

		map<string, list<Node>>::iterator begin() { return m_symTbl.begin(); }
		map<string, list<Node>>::iterator end() { return m_symTbl.end(); }

	private:

		// member variable, container that holds all symbols
		map< string, list<Node>> m_symTbl;
};

#endif