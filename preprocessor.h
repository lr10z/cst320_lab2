/***********************************************************
* Author:					Leander Rodriguez
* Date Created:				04/12/14
* Last Modification Date:	04/17/14
* Lab Number:				CST 320 Lab 1B
* Filename:					preprocessor.h
*
************************************************************/

#ifndef  PREPROCESSOR_H
#define  PREPROCESSOR_H

#include <string>
#include <sstream>
#include <list>
#include <iterator>
#include <iostream>
#include <iomanip>

using namespace std;

/***********************************************************
*
* preprocessor class
*
************************************************************/
class preprocessor
{
	public:

		// ctor
		preprocessor(string line);

		// methods
		string get(){ return m_output; }

	private:

		// member variables
		string m_output;
		bool m_directive;
};

#endif