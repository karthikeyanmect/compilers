//============================================================================
// Name        : enumeration.h
// Author      : Karthikeyan Muthalagu
// Description : Header, where all enumerations for TOKEN and its TYPES are defined
//============================================================================
using namespace std;
class enumeration
{
private:
	enum number
	{
		ZERO = '0',
		ONE = '1',
		TWO = '2',
		THREE = '3',
		FOUR = '4',
		FIVE = '5',
		SIX = '6',
		SEVEN = '7',
		EIGHT = '8',
		NINE = '9'
	};
	enum operators
	{
		PLUS = '+',
		MINUS = '-',
		MUL = '*',
		DIV = '/',
		EQUAL = '=',
		MOD = '%',
		AND = '&',
		OR = '|',
		GREATER = '>',
		LESSER = '<',
		NEGATE = '!',
		ASSIGN = ':',
		COMMA = ',',
		OPENB = '(',
		CLOSEB = ')',
		DOUBLE_PARAN = '"',
		SEMI = ';',
		OPEN_SB = '[',
		CLOSE_SB = ']',
		USCORE = '_'
	};
public:
	bool isNum(char c);
	bool isOperator(char c);
	bool isAlpha(char c);
	bool isSpace(char c);
	bool isStringSE(char c);
	bool isUscore(char c);
};
bool enumeration::isNum(char c)
{
	if (c == ZERO || c == ONE || c == TWO || c == THREE || c == FOUR || c
			== FIVE || c == SIX || c == SEVEN || c == EIGHT || c == NINE)
		return true;
	else
		return false;
}
bool enumeration::isOperator(char c)
{
	if (c == PLUS || c == MINUS || c == MUL || c == DIV || c == EQUAL || c
			== MOD || c == AND || c == OR || c == GREATER || c == LESSER || c
			== NEGATE || c == ASSIGN || c == COMMA || c == OPENB || c == CLOSEB
			|| c == SEMI || c == OPEN_SB || c == CLOSE_SB)
		return true;
	else
		return false;
}
bool enumeration::isAlpha(char c)
{
	if ((int(c) > 64 && int(c) < 91) || (int(c) > 96 && int(c) < 123))
		return true;
	else
		return false;
}
bool enumeration::isSpace(char c)
{
	if (int(c) == 32 || int(c) == 9 || int(c) == 10 || int(c) == 11 || int(c)
			== 12 || int(c) == 13) //Space || TAB || NewLine
		return true;
	else
		return false;
}
bool enumeration::isStringSE(char c)
{
	if (c == DOUBLE_PARAN)
		return true;
	else
		return false;
}
bool enumeration::isUscore(char c)
{
	if (c == USCORE)
		return true;
	else
		return false;
}
