/***********************************************************
* Author:					Leander Rodriguez
* Date Created:				04/30/14
* Last Modification Date:	05/02/14
* Lab Number:				CST 320 Lab 2
* Filename:					parser.cpp
*
************************************************************/
#include <iostream>

#include "parser.h"
#include "symbolTable.h"

vector<string> statements;
vector<string>::iterator stateIt;

extern vector<parserEntry> gTokens;
vector<parserEntry>::iterator currentToken;

extern symbolTable *sT;

inline bool Program();
inline bool Func_Key();
inline bool Idlist();
inline bool Idlist2();
inline bool Func_Block();
inline bool Func_Body();
inline bool Data_Def();
inline bool Data_Def2();
inline bool Statement();
inline bool Statement2();
inline bool Statement3();
inline bool In_Out();
inline bool In_Out2();
inline bool Expression();
inline bool Expression2();
inline bool Expression3();
inline bool Op();
inline bool Unary_exp();
inline bool Primary();
inline bool Exit();

void Parser()
{
	currentToken = gTokens.begin();

	bool passed = Program();

	/*if (passed == true)
	{
		cout << "Yes" << endl << endl;
	}
	else
	{
		cout << "No" << endl << endl;
	}*/

	cout << endl;

	for (stateIt = statements.begin(); stateIt != statements.end(); ++stateIt)
	{
		cout << *stateIt  << endl;
	}

	cout << endl;
	
}

/***********************************************
Program -> Func_Key id (IdList) Func_Block Exit
************************************************/
bool Program()
{
	vector<parserEntry>::iterator temp;
	temp = currentToken;

	if (currentToken == gTokens.end())
	{
		return true;
	}
	if (Func_Key())
	{
		if (currentToken->m_type == "Identifier")
		{
			sT->insert(currentToken->m_value, temp->m_value, "function name", "");
			++currentToken;
			if (currentToken->m_value == "(")
			{
				++currentToken;
				if (Idlist())
				{
					if (currentToken->m_value == ")")
					{
						++currentToken;
						if (Func_Block())
						{
							++currentToken;
							if (Exit())
							{
								//statements.push_back("Program -> Func_Key id (IdList) Func_Block Exit");
								return true;
							}
							else{ return false; }
						}
						else{ return false; }
					}
					else
					{ 
						cout << "Program failed. Expecting a )\n";
						return false; 
					}
				}
				else{ return false; }
			}
			else
			{ 
				cout << "Program failed. Expecting a (\n";
				return false; 
			}
		}
		else{ return false; }
	}
	else{ return false; }
}

/***********************
Func_Key -> void | int
************************/
bool Func_Key()
{
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Func_Key\n";
		return false;
	}
	if (currentToken->m_type == "Keyword")
	{
		if (currentToken->m_value == "void" )
		{
			statements.push_back("Function declaration");
			//statements.push_back("Func_Key -> void");
			++currentToken;
			return true;
		}
		else if (currentToken->m_value == "int")
		{
			statements.push_back("Function declaration");
			//statements.push_back("Func_Key -> int");
			++currentToken;
			return true;
		}
		else{ return false; }
	}
	else{ return false; }
}

/**********************************************
Idlist -> int id Idlist2 | lambda
***********************************************/
bool Idlist()
{
	vector<parserEntry>::iterator temp;
	temp = currentToken;

	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Idlist\n";
		return false;
	}
	if (currentToken->m_value == "int")
	{
		++currentToken;
		if (currentToken->m_type == "Identifier")
		{
			sT->insert(currentToken->m_value, temp->m_value, "variable name", "");
			++currentToken;
			if (Idlist2())
			{
				//statements.push_back("Idlist -> int id Idlist2");
				return true;
			}
			else{ return false; }
		}
		else{ return false; }
	}
	else
	{
		//statements.push_back("Idlist -> lambda");
		return true; 
	}
}

/****************************
Idlist2 -> , Idlist | lambda
*****************************/
bool Idlist2()
{
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Idlist2\n";
		return false;
	}
	if (currentToken->m_value == ",")
	{
		++currentToken;
		if (Idlist())
		{
			//statements.push_back("Idlist2 -> , Idlist");
			return true;
		}
		else{ return false; }
	}
	else
	{
		//statements.push_back("Idlist2 -> lambda");
		return true;
	}
}

/*****************************************
Func_Block -> { Func_Body }
******************************************/
bool Func_Block()
{
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Func_Block\n";
		return false;
	}
	if (currentToken->m_value == "{")
	{
		++currentToken;
		if (Func_Body())
		{
			++currentToken;
			if (currentToken->m_value == "}")
			{
				//statements.push_back("Func_Block -> { Func_Body }");
				return true;
			}
			else
			{
				cout << "Func_Block failed. Expecting a }\n";
				return false;
			}			
		}
		else{ return false; }
	}
	else
	{ 
		cout << "Func_Block failed. Expecting a {\n";
		return false; 
	}
}

/*****************************************
Func_Body -> Data_Def Statement Statement
******************************************/
bool Func_Body()
{
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Func_Body\n";
		return false;
	}
	if (Data_Def())
	{
		if (Statement())
		{
			if (Statement())
			{
				//statements.push_back("Func_Body -> Data_Def Statement Statement");
				return true;
			}
			else{ return false; }
		}
		else{ return false; }
	}
	else{ return false; }
}

/********************************************************
Data_Def -> int id Data_Def2 Data_Def | lambda
*********************************************************/
bool Data_Def()
{
	vector<parserEntry>::iterator temp;
	temp = currentToken;

	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Data_Def\n";
		return false;
	}
	if (currentToken->m_value == "int")
	{
		++currentToken;
		if (currentToken->m_type == "Identifier")
		{
			sT->insert(currentToken->m_value, temp->m_value, "variable name", "");
			++currentToken;
			if (Data_Def2())
			{
				++currentToken;
				if (Data_Def())
				{
					//statements.push_back("Data_Def -> int id Data_Def2 Data_Def");
					return true;
				}
				else{ return false; }
			}
			else{ return false; }
		}
		else{ return false; }
	}
	else
	{ 
		//statements.push_back("Data_Def -> lambda");
		return true; 
	}
}

/****************************
Data_Def2 -> ; | = Primary ;
*****************************/
bool Data_Def2()
{
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Data_Def2\n";
		return false;
	}
	if (currentToken->m_value == ";")
	{
		statements.push_back("Data declaration");
		//statements.push_back("Data_Def2 -> ;");
		return true;
	}
	else if (currentToken->m_value == "=")
	{
		++currentToken;
		if (Primary())
		{
			++currentToken;
			if (currentToken->m_value == ";")
			{
				statements.push_back("Assignment statement");
				//statements.push_back("Data_Def2 -> = num ;");
				return true;
			}
			else
			{ 
				cout << "Data definition failed. Expecting a ;\n";
				return false; 
			}
		}
		else{ return false; }
	}
	else{ return false; }
}

/*********************************
Statement -> if Statement2		| 
			 while Statement2	|
			 continue ;			|
			 return Expression ;|
			 In_Out				|
			 Expression ;		|
			 Data_Def			
**********************************/
bool Statement()
{
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Statement\n";
		return false;
	}
	if (currentToken->m_value == "if")
	{
		statements.push_back("If statement");
		//statements.push_back("Statement -> if Statement2");
		++currentToken;
		if (Statement2())
		{
			return true;
		}
		else{ return false; }
	}
	else if (currentToken->m_value == "while")
	{
		++currentToken;
		if (Statement2())
		{
			statements.push_back("While loop");
			//statements.push_back("Statement -> while Statement2");
			return true;
		}
		else{ return false; }
	}
	else if (currentToken->m_value == "continue")
	{
		++currentToken;
		if (currentToken->m_value == ";")
		{
			//statements.push_back("Statement -> continue ;");
			return true;
		}
		else
		{ 
			cout << "Statement failed. Expecting a ;\n"; 
			return false;
		}
	}
	else if (currentToken->m_value == "return")
	{
		++currentToken;
		if (Expression())
		{
			if (currentToken->m_value == ";")
			{
				//statements.push_back("Statement -> return Expression ;");
				return true;
			}
			else
			{ 
				cout << "Statement failed. Expecting a ;\n"; 
				return false;
			}
		}
		else{ return false; }
	
	}
	else if (In_Out())
	{
		//statements.push_back("Statement -> In_Out");
		return true;
	}
	else if (Expression())
	{
		if (currentToken->m_value == ";")
		{
			//statements.push_back("Statement -> Expression ;");
			return true;
		}
		else{ return false; }
	}
	else if (Data_Def())
	{
		//statements.push_back("Statement -> Data_Def");
		return true;
	}
	else{ return false; }
}

/**************************************************
Statement2 ->  (Expression) {Statement} Statement3
***************************************************/
bool Statement2()
{
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Statement2\n";
		return false;
	}
	if (currentToken->m_value == "(")
	{
		++currentToken;
		if (Expression())
		{
			if (currentToken->m_value == ")")
			{
				++currentToken;
				if (currentToken->m_value == "{")
				{
					++currentToken;
					if (Statement())
					{
						if (currentToken->m_value == "}")
						{
							++currentToken;
							if (Statement3())
							{
								//statements.push_back("Statement2 ->  (Expression) {Statement} Statement3");
								return true;
							}
							else{ return false; }
						}
						else
						{
							cout << "Statement failed. Expecting a }\n";
							return false;
						}
					}
					else{ return false; }
				}
				else
				{
					cout << "Statement failed. Expecting a {\n";
					return false;
				}
			}
			else
			{ 
				cout << "Statement failed. Expecting a )\n"; 
				return false;
			}
		}
		else{ return false; }
	}
	else
	{ 
		cout << "Statement failed. Expecting a (\n"; 
		return false;
	}
}

/****************************************
Statement3 ->  else {Statement} | lambda
*****************************************/
bool Statement3()
{
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Statement3\n";
		return false;
	}
	if (currentToken->m_value == "else")
	{
		++currentToken;
		if (currentToken->m_value == "{")
		{
			++currentToken;
			if (Statement())
			{
				++currentToken;
				if (currentToken->m_value == "}")
				{
					//statements.push_back("Statement3 ->  else {Statement}");
					return true;
				}
				else
				{
					cout << "Statement failed. Expecting a }\n";
					return false;
				}
			}
			else{ return false; }
		}
		else
		{
			cout << "Statement failed. Expecting a {\n";
			return false;
		}
		
	}
	else
	{
		//statements.push_back("Statement3 ->  lambda");
		return true; 
	}
}

/*************************************************
In_Out -> cin Op id In_Out2 | cout Op id In_Out2
**************************************************/
bool In_Out()
{
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in In_Out\n";
		return false;
	}
	if (currentToken->m_value == "cin")
	{
		++currentToken;
		if (Op())
		{
			++currentToken;
			if (currentToken->m_type == "Identifier")
			{
				if (sT->search(currentToken->m_value))
				{
					++currentToken;
					if (In_Out2())
					{
						//statements.push_back("In_Out -> cin Op id In_Out2");
						return true;
					}
					else{ return false; }
				}
				else
				{
					cout << "Error " 
						 << currentToken->m_value 
						 << " was never declared."  
						 <<  endl;
					return false;
				}
				
			}
			else{ return false; }
		}
		else
		{ 
			cout << "Input statement failed. Expecting a >>\n"; 
			return false;
		}
	}
	else if (currentToken->m_value == "cout")
	{
		++currentToken;
		if (Op())
		{
			++currentToken;
			if (currentToken->m_type == "Identifier")
			{
				if (sT->search(currentToken->m_value))
				{
					++currentToken;
					if (In_Out2())
					{
						//statements.push_back("In_Out -> cout Op id In_Out2");
						return true;
					}
					else{ return false; }
				}
				else
				{
					cout << "Error " 
						 << currentToken->m_value 
						 << " was never declared."  
						 <<  endl;
					return false;
				}
			}
			else{ return false; }
		}
		else
		{ 
			cout << "Output statement failed. Expecting a <<\n"; 
			return false;
		}
	}
	else
	{
		return false;
	}
}

/*****************************
In_Out2 -> Op id In_Out2 | ;
******************************/
bool In_Out2()
{
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in In_Out2\n";
		return false;
	}
	if (Op())
	{
		++currentToken;
		if (currentToken->m_type == "Identifier")
		{
			if (sT->search(currentToken->m_value))
			{
				++currentToken;
				if (In_Out2())
				{
					//statements.push_back("In_Out2 -> Op id In_Out2");
					return true;
				}
				else{ return false; }
			}
			else
			{
				cout << "Error "
					 << currentToken->m_value
					 << " was never declared."
					 << endl;
				return false;
			}
		}
		else{ return false; }
	}
	else if (currentToken->m_value == ";")
	{
		//statements.push_back("In_Out2 -> ;");
		++currentToken;
		return true;
	}
	else{ return false; }
}

/************************************
Expression -> id Expression2		| 
			  Unary_exp Expression3	| 
			  Primary Expression3
**************************************/
bool Expression()
{
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Expression\n";
		return false;
	}
	if (currentToken->m_type == "Identifier")
	{
		if (sT->search(currentToken->m_value))
		{
			++currentToken;
			if (Expression2())
			{
				//statements.push_back("Expression -> id Expression2");
				return true;
			}
			else{ return false; }
		}
		else
		{
			cout << "Error "
				 << currentToken->m_value
				 << " was never declared."
				 << endl;
			return false;
		}
	}
	else if (Unary_exp())
	{
		if (Expression3())
		{
			//statements.push_back("Expression -> Unary_exp Expression3");
			return true;
		}
		else{ return false; }
	}
	else if (Primary())
	{
		++currentToken;
		if (Expression3())
		{
			//statements.push_back("Expression -> Primary Expression3");
			return true;
		}
		else{ return false; }
	}
	else{ return false; }
}

/******************************************
Expression2 ->	= Expression Expression3| 
				(Primary) Expression3	| 
				Expression3
*******************************************/
bool Expression2()
{
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Expression2\n";
		return false;
	}
	if (currentToken->m_value == "=")
	{
		++currentToken;
		if (Expression())
		{
			if (Expression3())
			{
				statements.push_back("Assignment statement");
				//statements.push_back("Expression2 -> = Expression Expression3");
				return true;
			}
			else{ return false; }
		}
		else{ return false; }
	}
	else if (currentToken->m_type == "Symbol")
	{
		if (currentToken->m_value == "(")
		{
			++currentToken;
			if (Primary())
			{
				++currentToken;
				if (currentToken->m_value == ")")
				{
					++currentToken;
					if (Expression3())
					{
						//statements.push_back("Expression2 -> (Primary) Expression3");
						return true;
					}
					else{ return false; }
				}
				else
				{
					cout << "Expression failed. Expecting a )\n";
					return false;
				}
			}
			else{ return false; }
		}
		else
		{
			cout << "Expression failed. Expecting a (\n";
			return false;
		}
	}
	else if (Expression3())
	{
		//statements.push_back("Expression2 -> Expression3");
		return true;
	}
	else{ return false; }
}

/*******************************************
Expression3 ->  Op Expression Expression3 | 
				lambda
********************************************/
bool Expression3()
{
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Expression3\n";
		return false;
	}
	if (Op())
	{
		++currentToken;
		if (Expression())
		{
			//++currentToken;
			if (Expression3())
			{
				//statements.push_back("Expression3 -> Op Expression Expression3");
				return true;
			}
			else{ return false; }
		}
		else{ return false; }
	}
	else
	{
		//statements.push_back("Expression3 -> lambda");
		return true; 
	}
}

/************************************************************
Op -> + | - | * | / | % | < | > | <= | >= | = | == | >> | <<
*************************************************************/
bool Op()
{
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Op\n";
		return false;
	}
	if (currentToken->m_type == "Operator")
	{
		if (currentToken->m_value == "+"  ||
			currentToken->m_value == "-"  || 
			currentToken->m_value == "*"  || 
			currentToken->m_value == "/"  || 
			currentToken->m_value == "%"  )
		{
			//statements.push_back("Op -> Operator");
			return true;
		}
		else if ( currentToken->m_value == "<"  ||
				  currentToken->m_value == ">"  ||
				  currentToken->m_value == "<=" ||
				  currentToken->m_value == ">=" ||
				  currentToken->m_value == "=="  )
		{
			statements.push_back("Comparison expression");
			return true;
		}
		else if (currentToken->m_value == ">>")
		{
			statements.push_back("Input statement");
			return true;
		}
		else if (currentToken->m_value == "<<")
		{
			statements.push_back("Output statement");
			return true;
		}
		else{ return false; }
	}
	else{ return false; }
}

/*****************************************
Unary_exp -> ++ Primary | -- Primary
******************************************/
bool Unary_exp()
{
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Unary_exp\n";
		return false;
	}
	if (currentToken->m_value == "++")
	{
		++currentToken;
		if (Primary())
		{
			if (sT->search(currentToken->m_value))
			{
				++currentToken;
				if (currentToken->m_value == ";")
				{
					statements.push_back("Increment expression");
					//statements.push_back("Unary_exp -> ++ Primary");
					return true;
				}
				else
				{
					cout << "Increment statement failed. Expecting a ;\n";
					return false;
				}
			}
			else
			{
				cout << "Error "
					 << currentToken->m_value
					 << " was never declared."
					 << endl;
				return false;
			}
		}
		else{ return false; }
	}
	else if (currentToken->m_value == "--")
	{
		++currentToken;
		if (Primary())
		{
			if (sT->search(currentToken->m_value))
			{
				++currentToken;
				if (currentToken->m_value == ";")
				{
					statements.push_back("Decrement expression");
					//statements.push_back("-- Primary ");
					return true;
				}
				else
				{
					cout << "Decrement statement failed. Expecting a ;\n";
					return false;
				}
			}
			else
			{
				cout << "Error "
					<< currentToken->m_value
					<< " was never declared."
					<< endl;
				return false;
			}
		}
		else{ return false; }
	}
	else{ return false; }
}

/***********************
Primary -> id | num | , 
************************/
bool Primary()
{ 
	if (currentToken == gTokens.end())
	{
		cout << "Unexpected end of input in Primary\n";
		return false;
	}
	if (currentToken->m_type == "Identifier")
	{
		//statements.push_back("Primary -> id");
		return true;
	}
	else if (currentToken->m_value == ",")
	{
		//statements.push_back("Primary -> ,");
		return true;
	}
	else if (currentToken->m_type == "Integer" || currentToken->m_type == "Float" )
	{
		//statements.push_back("Primary -> num");
		return true;
	}
	else{ return false; }
}
/****************************
Exit ->  Program | Statement 
*****************************/
bool Exit()
{ 
	if (currentToken == gTokens.end())
	{
		return true;
	}
	if (Program())
	{
		//statements.push_back("Exit -> Program");
		return true;
	}
	else if (Statement())
	{
		//statements.push_back("Exit ->  Statement");
		return true;
	}
	else
	{return false;}
}
