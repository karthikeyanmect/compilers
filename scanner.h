//============================================================================
// Name        : scanner.h
// Author      : Karthikeyan Muthalagu
// Description : Header file for scanning the input file anf give one token at a time
//============================================================================


#include <iostream>
#include "Filehandler.h"
#include "enumeration.h"
#include "declaration.h"
using namespace std;

fileHandle file("input.txt");// Input File Name
enumeration validate;
char lexeme;

tokenType token;
string value;
void scaner();
char termin = ';';

void moveAhead()
{
	char c = file.nxtChar();
	while (!validate.isSpace(c) || validate.isOperator(c) )
	{
		c = file.nxtChar();
	}
	lexeme = c;
}
bool isKeyword()
{
	if (value == "program" || value == "begin" || value == "end" || value
			== "is" || value == "integer" || value == "boolean" || value
			== "and" || value == "or" || value == "not" || value == "if"
			|| value == "else" || value == "then" || value == "while" || value
			== "string" || value == "function")
		return true;
	else
		return false;
}
//Function that generates AlphaNumeric-String from input Buffer
void getIdenKey()
{
	value = "";
	while (validate.isAlpha(lexeme) || validate.isNum(lexeme) || validate.isUscore(lexeme))
	{
		value = value + lexeme;
		lexeme = file.nxtChar();
	}
	if (lexeme == termin || validate.isSpace(lexeme) || validate.isOperator(
			lexeme))
	{
		if (isKeyword())
			token = KEYWORD;
		else
			token = IDEN;
		file.changePos(-1);
	}
	else
	{
		token = ERROR;
		moveAhead();
	}

}
//Function that generates AlphaNumeric-String from input Buffer
void getString()
{
	lexeme = file.nxtChar();
	value = "\"";
	while (lexeme != '"')
	{
		value = value + lexeme;
		lexeme = file.nxtChar();
	}
	if (validate.isStringSE(lexeme))
	{
		value = value + '"';
		token = STRING;
	}
	else
	{
		token = ERROR;
		moveAhead();
	}

}

//Function that generates Number-String from input Buffer
void getNumber()
{
	value = "";
	while (validate.isNum(lexeme))
	{
		value = value + lexeme;
		lexeme = file.nxtChar();
	}
	if (lexeme == termin || validate.isSpace(lexeme) || validate.isOperator(
			lexeme))
	{
		token = NUMBER;
		file.changePos(-1);
	}
	else
	{
		token = ERROR;
		moveAhead();
	}
}

//Function that generates Operator-String from input Buffer
void getOperator()
{
	value = lexeme;
	switch (lexeme)
	{
	case '+':
		token = PLUS;
		break;
	case '(':
		token = OPENB;
		break;
	case ')':
		token = CLOSEB;
		break;
	case '[':
		token = OPEN_SB;
		break;
	case ']':
		token = CLOSE_SB;
		break;
	case ',':
		token = COMMA;
		break;
	case '-':
		token = MINUS;
		break;
	case '*':
		token = MUL;
		break;
	case '/':
		token = DIV;
		break;
	case '&':
		token = BAND;
		break;
	case '|':
		token = BOR;
		break;
	case '<':
		token = GE;
		break;
	case '>':
		token = LS;
		break;
	case ';':
		token = SEMI;
		break;
	case '=':
		lexeme = file.nxtChar();
		switch (lexeme)
		{
		case '=':
			token = EQUALITY;
			value = value + lexeme;
			break;
		default:
			token = ERROR;
			file.changePos(-1);
		}
		break;
	case ':':
		lexeme = file.nxtChar();
		switch (lexeme)
		{
		case '=':
			token = EQUAL;
			value = value + lexeme;
			break;
		default:
			token = ERROR;
			file.changePos(-1);
		}
		break;
	case '!':
		lexeme = file.nxtChar();
		switch (lexeme)
		{
		case '=':
			token = NOTEQUAL;
			value = value + lexeme;
			break;
		default:
			token = ERROR;
			file.changePos(-1);
		}
		break;
	default:
		token = ERROR;
	}
}
void isEF(char c)
{
	int val;
	val = c;
	if (val == EOF)
		token = END;
}
void ignoreSpace()
{
	char c = file.nxtChar();
	while (validate.isSpace(c))
	{
		c = file.nxtChar();
	}
	isEF(c);
	lexeme = c;
}
//Checking for the Comments
bool ignoreComment()
{
	// Code for Ignoring Comment
	if (lexeme == '/')
	{
		char c = file.nxtChar();
		if (c == '/')
		{
			while (int(c) != 10)
			{
				c = file.nxtChar();
			}
			scaner();
			return true;
		}
		else
			file.changePos(-1);
	}
	return false;
}
// Function that return one token at a time
void scaner()
{
	ignoreSpace();
	if (!ignoreComment())
	{
		if (validate.isAlpha(lexeme))
			getIdenKey();
		else if (validate.isNum(lexeme))
			getNumber();
		else if (validate.isStringSE(lexeme))
			getString();
		else if (validate.isOperator(lexeme))
			getOperator();
	}
}

