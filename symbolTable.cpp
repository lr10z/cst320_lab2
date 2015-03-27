/***********************************************************
* Author:					Leander Rodriguez
* Date Created:				04/04/14
* Last Modification Date:	04/06/14
* Lab Number:				CST 320 Lab 1A
* Filename:					symbolTable.cpp
*
************************************************************/

#include "symbolTable.h" 

/***********************************************************
* 
* returns true if symbol data passed-in gets inserted/stored
* into container
*
************************************************************/
bool symbolTable::insert(string symbol, string type, string use, string value)
{
	// prevents duplicate symbols
	if (search(symbol))
	{
		return false;
	}

	Node n;
	n.name = symbol;
	n.type = type;
	n.use = use;
	n.value = value;

	unsigned preInsert = m_symTbl.size();

	m_symTbl[symbol].push_back(n);

	if (m_symTbl.size() > preInsert)
	{
		return true;
	}
	
	return false;
}

/***********************************************************
*
* returns true if symbol passed-in is removed from container
*
************************************************************/
bool symbolTable::remove( string symbol )
{
	if (m_symTbl.size() == 0)
	{
		return false;
	}

	if (m_symTbl[symbol].empty())
	{
		return false;
	}
	
	m_symTbl.erase(symbol);

	return true;
}

/***********************************************************
*
* searches through container, returns true if symbol
* passed-in is found
*
************************************************************/
bool symbolTable::search( string symbol ) 
{
	if ( m_symTbl.find(symbol) == m_symTbl.end()  ||  m_symTbl.empty() )
	{
		return false;
	}
	else
	{
		return true;
	}
}