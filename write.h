//============================================================================
// Name        : write.h
// Author      : Karthikeyan Muthalagu
// Description : Header, which handles file handling for CODE Generation and Error Log Generation
//============================================================================
#include<string>
using namespace ::std;

class writeFile
{
private:
	ofstream output;
	ofstream status;
public:
	writeFile()
	{
		output.open("output/Launch.c");
		status.open("output/log.txt");
	}

	void emitCode(string code)
	{
		output << "             " + code;
		output << "\n";
	}
	void emitLabel(string label)
	{
		output << label;
		output << "\n";
	}
	void emitLog(string log)
	{
		status << log;
	}
};
