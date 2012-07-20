//============================================================================
// Name        : stack.h
// Author      : Karthikeyan Muthalagu
// Description : Header, where all stack data structures are defined
//============================================================================
#include <fstream>
#include <iostream>

using namespace ::std;

class tokenStack
{
public:
	tokenStack(int num)
	{
		top = 0;
		maxelem = num;
		s = new tokenType[maxelem];
	}
	void push(tokenType t)
	{
		if (top == maxelem)
			return;
		s[top++] = t;
	}
	tokenType pop()
	{
		if (top == 0)
			return ERROR;
		return s[--top];
	}
	void display()
	{
		if (top == 0)
		{
			cout << "(empty)\n";
			return;
		}
		for (int t = 0; t < top; t++)
			cout << s[t] << " ";
		cout << "\n";
	}
	int empty()
	{
		return top == 0;
	}
	int getSize()
	{
		return (top);
	}
private:
	tokenType *s;
	int top;
	int maxelem;
};

class StrStack
{
public:
	StrStack(int num)
	{
		top = 0;
		maxelem = num;
		s = new string[maxelem];
	}
	void push(string t)
	{
		if (top == maxelem)
			return;
		s[top++] = t;
	}
	string pop()
	{
		if (top == 0)
			return "-1";
		return s[--top];
	}
	string topp()
	{
		if (top == 0)
			return "-1";
		int t = top;
		return s[--t];
	}
	void display()
	{
		if (top == 0)
		{
			cout << "(empty)\n";
			return;
		}
		for (int t = 0; t < top; t++)
			cout << s[t] << " ";
		cout << "\n";
	}
	int empty()
	{
		return top == 0;
	}
	int getSize()
	{
		return (top);
	}
private:
	string *s;
	int top;
	int maxelem;
};

class IntStack
{
public:
	IntStack(int num)
	{
		top = 0;
		maxelem = num;
		s = new int[maxelem];
	}
	void push(int t)
	{
		if (top == maxelem)
			return;
		s[top++] = t;
	}
	int pop()
	{
		if (top == 0)
			return -1;
		return s[--top];
	}
	void display()
	{
		if (top == 0)
		{
			cout << "(empty)\n";
			return;
		}
		for (int t = 0; t < top; t++)
			cout << s[t] << " ";
		cout << "\n";
	}
	int topp()
	{
		if (top == 0)
			return -1;
		int t = top;
		return s[--t];
	}
	int empty()
	{
		return top == 0;
	}
	int getSize()
	{
		return (top);
	}
private:
	int *s;
	int top;
	int maxelem;
};
