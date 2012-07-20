//============================================================================
// Name        : misc.h
// Author      : Karthikeyan Muthalagu
// Description : Header for all miscellaneous functions
//============================================================================
#include<sstream>
#include<string>

using namespace std;
int StringToInt(string str)
{
	istringstream buffer(str);
	int some_int;
	buffer >> some_int;
	return some_int;
}
/*
 * Function to Convert Integer to String
 */
string intToString(int number)
{
	if (number == 0)
		return "0";
	string temp = "";
	string returnvalue = "";
	while (number > 0)
	{
		temp += number % 10 + 48;
		number /= 10;
	}
	for (int i = 0; i < temp.length(); i++)
		returnvalue += temp[temp.length() - i - 1];
	return returnvalue;
}
