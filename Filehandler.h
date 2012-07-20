//============================================================================
// Name        : Filehandler.h
// Author      : Karthikeyan Muthalagu
// Description : Header, which handles file for reading the source language file
//============================================================================
#include <fstream>
using namespace ::std;
class fileHandle
{
private:
	ifstream fread;
	int position;
	int lineNo;
	char lastRead;
public:
	fileHandle(string fileName);
	~fileHandle();
	char nxtChar();
	void changePos(int skew);
	void updtLineNo();
	int getLineNo();
};
//Constructor
fileHandle::fileHandle(string fileName)
{
	fread.open("input.txt");
	position = 0;
	lineNo = 1;
}

fileHandle::~fileHandle()
{
	fread.close();
}

char fileHandle::nxtChar()
{
	fread.seekg(position);
	position++;
	char c = fread.get();
	lastRead = c;
	if (int(c) == 10)
		updtLineNo();
	return (c);
}

void fileHandle::changePos(int skew)
{
	if (int(this->lastRead) != 10)
		position += skew;
}
void fileHandle::updtLineNo()
{
	lineNo++;
}
int fileHandle::getLineNo()
{
	return lineNo;
}
