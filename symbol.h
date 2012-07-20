//============================================================================
// Name        : scanner.cpp
// Author      : Karthikeyan Muthalagu
// Description : File for Symbol Definition and Symbol Table Management
//============================================================================
#include <iostream>
#define BUCKET_COUNT 12
using namespace std;

class symbol
{
private:
	tokenType symbolType;
	string symbolValue;
	string dataType;
	int scope;

	/**
	 * Variables for Function handling
	 */
	bool isFunction;
	queueStr *paraList;
	int memNumber;
	/**
	 * variables for array handling
	 */
	int arraySize;
	int heapNum;
	bool isHeader;
public:
	int getScope()
	{
		return scope;
	}

	tokenType getSymbolType()
	{
		return symbolType;
	}

	string getSymbolValue()
	{
		return symbolValue;
	}

	void setScope(int scope)
	{
		this->scope = scope;
	}

	void setSymbolType(tokenType symbolType)
	{
		this->symbolType = symbolType;
	}

	void setSymbolValue(string symbolValue)
	{
		this->symbolValue = symbolValue;
	}
	//Types of Constructor
	symbol(tokenType symbolType, string symbolValue, int scope)
	{
		setSymbolType(symbolType);
		setSymbolValue(symbolValue);
		setScope(scope);
	}
	symbol(tokenType symbolType, int scope)
	{
		setSymbolType(symbolType);
		setScope(scope);
	}
	symbol(string value, int scope)
	{
		setSymbolValue(value);
		setScope(scope);

	}
	symbol(string value)
	{
		setSymbolValue(value);
	}
	symbol()
	{

	}

	string getDataType() const
	{
		return dataType;
	}

	void setDataType(string dataType)
	{
		this->dataType = dataType;
	}

	bool getIsFunction() const
	{
		return isFunction;
	}

	void setIsFunction(bool isFunction)
	{
		this->isFunction = isFunction;
	}

	queueStr *getParaList() const
	{
		return paraList;
	}

	void setParaList(queueStr *paraList)
	{
		this->paraList = paraList;
	}

	int getMemNumber()
	{
		return memNumber;
	}

	void setMemNumber(int memNumber)
	{
		this->memNumber = memNumber;
	}

	int getArraySize()
	{
		return arraySize;
	}

	void setArraySize(int arraySize)
	{
		this->arraySize = arraySize;
	}

	int getHeapNum()
	{
		return heapNum;
	}

	void setHeapNum(int heapNum)
	{
		this->heapNum = heapNum;
	}

	bool getIsHeader()
	{
		return isHeader;
	}

	void setIsHeader(bool isHeader)
	{
		this->isHeader = isHeader;
	}

};
// Node Definition
struct node
{
	symbol s;
	node *next;
};

class linkedlist
{
private:
	node *lastNode;
	node *firstNode;
	int size;
public:
	// Constructor Creating New Node
	linkedlist()
	{
		size = 0;
	}
	void addNode(symbol sym)
	{
		node *temp;
		temp = new node;
		temp->s = sym;
		if (this->size == 0)
		{
			this->lastNode = temp;
			this->firstNode = temp;
		}
		else
		{
			(this->lastNode)->next = temp;
			this->lastNode = temp;
		}
		size++;
	}
	symbol topNode()
	{
		return ((this->firstNode)->s);
	}
	symbol endNode()
	{
		return ((this->lastNode)->s);
	}

	int getSize() const
	{
		return size;
	}

	void setSize(int size)
	{
		this->size = size;
	}

	bool findSymbol(symbol sym)
	{
		node *ptr;
		ptr = firstNode;
		string temp = sym.getSymbolValue();
		for (int i = 0; i < size; i++)
		{

			if (ptr->s.getSymbolValue() == temp)
			{
				return true;
			}
			ptr = ptr->next;

		}

		return false;
	}
	symbol getSymbol(string value)
	{
		node *ptr;
		ptr = firstNode;
		for (int i = 0; i < size; i++)
		{
			if (ptr->s.getSymbolValue() == value)
				return ptr->s;
			ptr = ptr->next;
		}
	}
	symbol* getSymbolPtr(string value)
	{
		node *ptr;
		ptr = firstNode;
		for (int i = 0; i < size; i++)
		{
			if (ptr->s.getSymbolValue() == value)
				return &(ptr->s);
			ptr = ptr->next;
		}
	}
	void display()
	{
		node *ptr;
		ptr = firstNode;
		for (int i = 0; i < size; i++)
		{
			cout << ptr->s.getDataType() << "-----" << ptr->s.getSymbolValue()
					<< "-----" << ptr->s.getHeapNum() << endl;
			ptr = ptr->next;
		}
	}
};

class symbolMgmt
{
public:
	symbolMgmt()
	{
		for (int i = 0; i < BUCKET_COUNT; i++)
		{
			linkedlist *ll;
			ll = new linkedlist();
			this->bukets[i] = ll;
		}
	}
	int getBucketNo(string str)
	{
		int hash = 0;
		unsigned int i = 0;
		while (i < str.length())
		{
			hash = hash + int(str[i]);
			i++;
		}
		return hash % BUCKET_COUNT;
	}
	void insertSymbol(symbol s)
	{
		string value = s.getSymbolValue();
		int index = getBucketNo(value);
		(this->bukets[index])->addNode(s);
	}
	bool searchSymbol(symbol sym)
	{
		int index = getBucketNo(sym.getSymbolValue());
		return ((this->bukets[index])->findSymbol(sym));
	}
	symbol getSymbol(string value, int scope)
	{
		symbol *sym;
		sym = new symbol(value);
		int index = getBucketNo(value);
		return ((this->bukets[index])->getSymbol(value));

	}
	symbol* getSymbolPtr(string value, int scope)
	{
		symbol *sym;
		sym = new symbol(value);
		int index = getBucketNo(value);
		return ((this->bukets[index])->getSymbolPtr(value));

	}
	void displayTable()
	{
		for (int i = 0; i < BUCKET_COUNT; i++)
		{
			cout << "BUCKET " << i << endl;
			this->bukets[i]->display();
		}
	}
private:
	symbol s;
	int hash;
	linkedlist *bukets[BUCKET_COUNT];
};

