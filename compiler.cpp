//============================================================================
// Name        : compiler.cpp
// Author      : Karthikeyan Muthalagu
// Description : Main file for handling Recursive Parsing and code Generation
//============================================================================


#include <iostream>
#include <stdlib.h>
#include "scanner.h"
#include "queue.h"
#include "symbol.h"
#include "stack.h"
#include "memory.h"
#include "misc.h"
#include "write.h"

void EXPRESSION();
void TERM();
void STATEMENT();
void DECLARATION();
symbol getSym(string val);
symbol* getSymPtr(string val);
string dataType;
string currentTypeLeft = "";
string expressionSemantics = "";
string current;
int scope;
string currentOperator;
string functionName;
string arrayName;
string functionReturnType;
symbolMgmt st[100];
symbolMgmt dummy;
queueStr *pList;
StrStack dataList(100);
StrStack exprStk(255);

/*
 * code Generation Variable
 */

int variableIndex = 10;
int variableIndex_bkp = 10;
IntStack varIndStk(100);
registers cReg;
memory cMem;
int ifLabelCount = 0;
int whileLabelCount = 0;
IntStack whileStack(100);
int callLabelCount = 0;
int stringLabelCount = 0;
StrStack fNameStk(100);
StrStack functionReturnLabelStack(100);
int funcOfFuncCall = 0;
bool consumeBool = 0;
string lastString;
writeFile w;
bool isOprConsumed;
int errorCount;

void emit(string print)
{
	w.emitCode(print);
	//cout << "       " << print << endl;
}
void emitLbl(string print)
{
	w.emitLabel(print);
	//cout << print << endl;
}
void emitLog(string print)
{
	w.emitLog(print);
	w.emitLog("\n");
	errorCount++;
	//cout << print << endl;
}
void prePopulate()
{

	emitLbl("#include <stdlib.h>");
	emitLbl("int main(void)");
	emitLbl("{");
	emitLbl("//Memory Declaration");
	emitLbl("int M[500000];");
	emitLbl("//Register Declaration");
	emitLbl("int R[100];");
	emitLbl("//Frame and Stack Pointer Declaration");
	emitLbl("int frmptr = 2;");
	emitLbl("int stkptr = 2;");
	emitLbl("//String Pointer and RETURN register Declaration");
	emitLbl("int retValue;");
	emitLbl("int strptr;");
	emitLbl("void *labelPtr;");
	emitLbl("goto _main;");
	emitLbl("\n");
	emitLbl("//Predefined Procedures");

	emitLbl("/********integer putInt(integer)********/");
	emitLbl("_putInt:");
	emit("printf(\"%d\\n\", M[frmptr + 1]);");
	emit("M[frmptr - 4] = M[frmptr + 1];");
	emit("goto *M[frmptr-3];");
	emitLbl("\n");
	emitLbl("/********integer getInt()********/");
	emitLbl("_getInt:");
	emit("scanf(\"%d\",&M[frmptr - 4 ]);");
	emit("goto *M[frmptr-3];");

	emitLbl("\n");
	emitLbl("/********integer putBool(boolean)********/");
	emitLbl("_putBool:");
	emit("printf(\"%d\\n\", M[frmptr + 1]);");
	emit("M[frmptr - 4] = M[frmptr + 1];");
	emit("goto *M[frmptr-3];");
	emitLbl("\n");
	emitLbl("/********boolean getBool()********/");
	emitLbl("_getBool:");
	emit("scanf(\"%d\",&R[0]);");
	emit("M[frmptr - 4]=R[0];");
	emit("if(R[0]!=0)");
	emit("M[frmptr - 4]=1;");
	emit("goto *M[frmptr-3];");

	emitLbl("\n");
	emitLbl("/********boolean int2bool(integer)********/");
	emitLbl("_int2bool:");
	emit("R[0] = M[frmptr + 1];");
	emit("M[frmptr - 4] = R[0];");
	emit("if (R[0] != 0)");
	emit("M[frmptr - 4] = 1;");
	emit("goto *M[frmptr-3];");
	emitLbl("\n");
	emitLbl("/********integer bool2int(boolean)********/");
	emitLbl("_bool2int:");
	emit("R[0] = M[frmptr + 1];");
	emit("M[frmptr - 4] = R[0];");
	emit("if (R[0] != 0)");
	emit("M[frmptr - 4] = 1;");
	emit("goto *M[frmptr-3];");
	emitLbl("\n");
	emitLbl("/********integer sqrt(integer)********/");
	emitLbl("_sqrt:");
	emit("R[0] = 0;");
	emit("M[frmptr + 2] = R[0];");
	emit("R[1] = 1073741824;");
	emit("M[frmptr + 3] = R[1];");
	emitLbl("_whileSqRoot1:");
	emit("R[1] =  M[frmptr + 3];");
	emit("R[2] =  M[frmptr + 1];");
	emit("R[1] = (R[1] > R[2])?1:0;");
	emit("if(R[1] == 0)");
	emit("goto _endwhileSqRoot1;");
	emit("R[2] =  M[frmptr + 3];");
	emit("R[3] = 4;");
	emit("R[2] = R[2] / R[3];");
	emit("M[frmptr + 3] = R[2];");
	emit("goto _whileSqRoot1;");
	emitLbl("_endwhileSqRoot1:");
	emit("R[2] =  M[frmptr + 2];");
	emit("R[3] =  M[frmptr + 3];");
	emit("R[2] = R[2] + R[3];");
	emit("R[3] = 1;");
	emit("R[2] = R[2] - R[3];");
	emit("M[frmptr + 4] = R[2];");
	emitLbl("_whileSqRoot2:");
	emit("R[2] =  M[frmptr + 3];");
	emit("R[3] = 0;");
	emit("R[2] = (R[2] > R[3])?1:0;");
	emit("if(R[2] == 0)");
	emit("goto _endwhileSqRoot2;");
	emit("R[3] =  M[frmptr + 1];");
	emit("R[4] =  M[frmptr + 4];");
	emit("R[3] = (R[3] > R[4])?1:0;");
	emit("if(R[3] == 0)");
	emit("goto _elseSqRoot1;");
	emit("R[4] =  M[frmptr + 1];");
	emit("R[5] =  M[frmptr + 2];");
	emit("R[4] = R[4] - R[5];");
	emit("R[5] =  M[frmptr + 3];");
	emit("R[4] = R[4] - R[5];");
	emit("M[frmptr + 1] = R[4];");
	emit("R[4] =  M[frmptr + 2];");
	emit("R[5] =  M[frmptr + 3];");
	emit("R[6] = 2;");
	emit("R[5] = R[5] * R[6];");
	emit("R[4] = R[4] + R[5];");
	emit("M[frmptr + 2] = R[4];");
	emit("goto _endifSqRoot1;");
	emitLbl("_elseSqRoot1:");
	emitLbl("_endifSqRoot1:");
	emit("R[4] = M[frmptr + 2];");
	emit("R[5] = 2;");
	emit("R[4] = R[4] / R[5];");
	emit("M[frmptr + 2] = R[4];");
	emit("R[4] = M[frmptr + 3];");
	emit("R[5] = 4;");
	emit("R[4] = R[4] / R[5];");
	emit("M[frmptr + 3] = R[4];");
	emit("R[4] = M[frmptr + 2];");
	emit("R[5] = M[frmptr + 3];");
	emit("R[4] = R[4] + R[5];");
	emit("R[5] = 1;");
	emit("R[4] = R[4] - R[5];");
	emit("M[frmptr + 4] = R[4];");
	emit("goto _whileSqRoot2;");
	emitLbl("_endwhileSqRoot2:");
	emit("R[4] = M[frmptr + 2];");
	emit("M[frmptr - 4 ] = R[4];");
	emit("goto *M[frmptr-3];");
	emitLbl("\n");
	emitLbl("/********string getString()********/");
	emitLbl("_getString:");
	emit("strptr=1000;");
	emit("R[1]=0;");
	emit("rotateGet: R[0]=getchar();");
	emit("M[strptr]=R[0];");
	emit("strptr++;");
	emit("R[1]++;");
	emit("if(R[0]!=10 && R[1]<1100)");
	emit("goto rotateGet;");
	emit("M[strptr]=10;");
	emit("M[frmptr - 4 ] = 1000;");
	emit("goto *M[frmptr-3];");

	emitLbl("\n");
	emitLbl("/********integer putString(string)********/");
	emitLbl("_putString:");
	emit("strptr=M[frmptr+1];");
	emit("R[1]=0;");
	emit("rotatePut:R[0]=M[strptr];");
	emit("printf(\"%c\%\",R[0]);");
	emit("strptr++;");
	emit("R[1]++;");
	emit("if(R[0]!=10 && R[1]<1100)");
	emit("goto rotatePut;");
	emit("M[frmptr - 4 ] = 1;");
	emit("goto *M[frmptr-3];");
	emitLbl("\n");
	emitLbl("/********CODE GENERATION STARTS HERE********/");
}
int getMemoryOffset(string val)
{
	symbol s = getSym(val);
	return (s.getMemNumber());
}
int getArrayHeap(string val)
{
	symbol s = getSym(val);
	return (s.getHeapNum());
}
void saveVariableIndex()
{
	variableIndex_bkp = variableIndex;// for the function
	varIndStk.push(variableIndex);
	variableIndex = 10;
}
void restoreVariableIndex()
{
	if (varIndStk.getSize() != 0)
	{
		variableIndex = varIndStk.pop();
		variableIndex++;
	}
}
void saveFname(string name)
{
	fNameStk.push(name);
}
void restoreFname()
{
	if (fNameStk.getSize() != 0)
		string temp = fNameStk.pop();
}
void calleeCleanup(string fName)
{
	emit("goto *M[frmptr-3];");
}

void pushReturnName(string nam)
{
	functionReturnLabelStack.push(nam);
}
string popReturnName()
{
	if (functionReturnLabelStack.getSize() != 0)
		return functionReturnLabelStack.pop();
}
int pushRegisters()
{
	int count = cReg.currentReg();
	for (int i = 0; i <= count; i++)
	{
		emit("M[stkptr]= R[" + intToString(i) + "];");
		emit("stkptr++;");
	}
	return count;
}
void popRegisters(int count)
{
	int i;
	for (int i = count; i >= 0; i--)
	{
		emit("stkptr--;");
		emit("R[" + intToString(i) + "] = M[stkptr];");
	}
	cReg.setRegNum(count);
}

bool typeChecking()
{
	if (currentOperator == "*" || currentOperator == "/" || currentOperator
			== "-" || currentOperator == ">" || currentOperator == "<"
			|| currentOperator == "&" || currentOperator == "|")
	{
		if (expressionSemantics != current || expressionSemantics != "integer")
			emitLog("Semantics Error @ Line No "
					+ intToString(file.getLineNo()) + " ::: "
					+ "Data Type Mismatch");
	}
	else if (currentOperator == "+")
	{
		if (expressionSemantics != current || (expressionSemantics != "integer"
				&& expressionSemantics != "string"))
			emitLog("Semantics Error @ Line No "
					+ intToString(file.getLineNo()) + " ::: "
					+ "Data Type Mismatch");
	}
	else if (currentOperator == "==" || currentOperator == "!=")
	{
		if (expressionSemantics != current || (expressionSemantics != "integer"
				&& expressionSemantics != "string" && expressionSemantics
				!= "boolean"))
			emitLog("Semantics Error @ Line No "
					+ intToString(file.getLineNo()) + " ::: "
					+ "Data Type Mismatch");
	}
	else if (currentOperator == "and" || currentOperator == "or")
	{
		if (expressionSemantics != current || (expressionSemantics != "boolean"
				&& expressionSemantics != "integer"))
			emitLog("Semantics Error @ Line No "
					+ intToString(file.getLineNo()) + " ::: "
					+ "Data Type Mismatch");
	}

}
void populateArraySize(string size)
{
	symbol *s;
	s = getSymPtr(arrayName);
	int siz;
	if (s->getDataType() == "string")
	{
		siz = StringToInt(size) * 100;
	}
	else
	{
		siz = StringToInt(size);
	}
	int heap = cMem.allocateHeap(siz);
	s->setArraySize(siz);
	s->setHeapNum(heap);
	s->setIsHeader(false);
}
void populateSymbol()
{
	symbol s;
	s.setSymbolType(token);
	s.setDataType(dataType);
	s.setSymbolValue(value);
	s.setIsFunction(0);
	// for String memory Allocation
	if (dataType == "string")
	{
		int heap = cMem.allocateHeap(100);
		s.setHeapNum(heap);
	}

	s.setArraySize(0);
	s.setMemNumber(variableIndex);
	s.setIsHeader(false);
	variableIndex++;
	if (st[scope].searchSymbol(s))
		emitLog("Error @ Line No " + intToString(file.getLineNo()) + " ::: \""
				+ value + "\" ::: " + "Symbol Already Defined");
	else
	{
		st[scope].insertSymbol(s);
	}
}
void populateLibrary(string libName, string libReturnType, queueStr *paraList,
		int scp, int varIndex)
{
	symbol s;
	s.setSymbolType(IDEN);
	s.setDataType(libReturnType);
	s.setSymbolValue(libName);
	s.setParaList(paraList);
	s.setIsFunction(1);
	s.setArraySize(0);
	s.setIsHeader(true);
	s.setMemNumber(varIndex);
	if (st[scp].searchSymbol(s))
		emitLog("Error @ Line No " + intToString(file.getLineNo()) + " ::: \""
				+ libName + "\" ::: " + "Symbol Already Defined");
	else
		st[scp].insertSymbol(s);
}
void populateLibraryScope(int scop)
{
	queueStr *libParaList;
	libParaList = new queueStr(1);
	libParaList->add("integer");
	populateLibrary("putInt", "integer", libParaList, scop, 1);
	libParaList = new queueStr(1);
	populateLibrary("getInt", "integer", libParaList, scop, 2);
	libParaList = new queueStr(1);
	libParaList->add("integer");
	populateLibrary("sqrt", "integer", libParaList, scop, 3);
	libParaList = new queueStr(1);
	populateLibrary("getString", "string", libParaList, scop, 4);
	libParaList = new queueStr(1);
	libParaList->add("string");
	populateLibrary("putString", "integer", libParaList, scop, 5);

	libParaList = new queueStr(1);
	populateLibrary("getBool", "boolean", libParaList, scop, 6);

	libParaList = new queueStr(1);
	libParaList->add("boolean");
	populateLibrary("putBool", "integer", libParaList, scop, 7);

	libParaList = new queueStr(1);
	libParaList->add("integer");
	populateLibrary("int2bool", "boolean", libParaList, scop, 8);

	libParaList = new queueStr(1);
	libParaList->add("boolean");
	populateLibrary("bool2int", "integer", libParaList, scop, 9);
}
void populateFunction()
{
	symbol s;
	s.setSymbolType(IDEN);
	s.setDataType(functionReturnType);
	s.setSymbolValue(functionName);
	s.setParaList(pList);
	s.setIsFunction(1);
	s.setIsHeader(false);
	s.setArraySize(0);
	s.setMemNumber(variableIndex_bkp);
	if (st[scope - 1].searchSymbol(s))
		emitLog("Error @ Line No " + intToString(file.getLineNo()) + " ::: \""
				+ functionName + "\" ::: " + "Symbol Already Defined");
	else
		st[scope - 1].insertSymbol(s);
}
int getScope(string val)
{
	symbol s(val);
	int count = scope;
	bool found = false;
	while (count >= 1 && found == false)
	{
		if (st[count].searchSymbol(s) == false)
		{
			count--;
			found = false;
		}
		else
		{
			found = true;
		}
	}
	if (found)
		return count;
	else
		return -1;
}
string getDataType()// getting data type of current symbol from ST
{
	int count = getScope(value);
	return ((st[count].getSymbol(value, scope)).getDataType());
}
symbol getSym(string val)
{
	int count = getScope(val);
	return st[count].getSymbol(val, scope);
}
symbol* getSymPtr(string val)
{
	int count = getScope(val);
	return st[count].getSymbolPtr(val, scope);
}
void validateSemantics()
{
	symbol s;
	s.setSymbolValue(value);
	int count = scope;
	bool found = false;
	while (count >= 1 && found == false)
	{
		if (st[count].searchSymbol(s) == false)
		{
			count--;
			found = false;
		}
		else
		{
			found = true;
		}
	}
	if (!found)
	{
		emitLog("Error @ Line No " + intToString(file.getLineNo()) + " ::: \""
				+ value + "\" ::: " + "Symbol Not Defined");
		exit(1);
	}
}
void accepted(string terminal)
{
	// A Template Function for ACK an accept
}
void failed(string terminal)
{
	emitLog("Error @ Line No " + intToString(file.getLineNo()) + " ::: "
			+ value + " ::: " + "Grammatical/Parser Error");
	exit(1);
}
bool match(string terminal)
{
	scaner();
	if (value == terminal)
	{
		accepted(value);
		return true;
	}
	else
	{
		failed(value);
		return false;
	}
}
bool matchStr(string terminal)
{
	if (value == terminal)
	{
		accepted(value);
		return true;
	}
	else
	{
		failed(value);
		return false;
	}
}
void IDENTIFIER()
{
	scaner();
	if (token == IDEN)
	{
		accepted(value);
	}
	else
		failed(value);
	//Fill  Symbol table
}
bool IDENTIFIERStr()
{
	if (token == IDEN)
	{
		accepted(value);
		return true;
	}
	else
	{
		failed(value);
		return false;
	}
}
void TYPE_MARK_FUNCTION()
{
	if (value == "integer" || value == "boolean" || value == "string")
	{
		pList->add(value);
		dataType = value;
		accepted(value);
		return;
	}
	else
		failed(value);
}
void TYPE_MARK()
{
	scaner();
	if (value == "integer" || value == "boolean" || value == "string")
	{
		dataType = value;
		accepted(value);
		return;
	}
	else
		failed(value);
}
void VARIABLE_DECLARATION()
{
	TYPE_MARK_FUNCTION();
	IDENTIFIER();
	populateSymbol();
}
void PARAMETER()
{
	VARIABLE_DECLARATION();
}
void PARA_LIST()
{
	PARAMETER();
	scaner();
	if (value == ",")
	{
		scaner();
		PARA_LIST();
	}
	else if (value == ")")
	{
		accepted(value);
		return;
	}
	else
		failed(value);

}
string FUNCTION_HEADER()
{
	scaner();
	IDENTIFIERStr();
	functionName = value;
	match("(");
	pList = new queueStr(100);
	scaner();
	if (value != ")")
		PARA_LIST();
	populateFunction(); //call function to populate Function in ST
	return functionName;
}
void FUNCTION_BODY(string fName)
{
	DECLARATION();
	matchStr("begin");
	emitLbl("_" + fName + ":");// function call name
	emit("stkptr = stkptr + " + intToString(variableIndex - 10) + ";");
	scaner();
	STATEMENT();
	matchStr("end");
	calleeCleanup(fName);
	match("function");
}
void FUNCTION_DECLARATION()
{
	scope++;
	populateLibraryScope(scope);
	saveVariableIndex();
	string temp = FUNCTION_HEADER();
	saveFname(temp);
	FUNCTION_BODY(temp);
	restoreFname();
	st[scope] = dummy; // deleting or making the ST as null for closed SCOPE
	scope--;
	restoreVariableIndex();
	scaner();
}
void VARIABLE_DEC()
{
	IDENTIFIERStr();
	arrayName = value;
	populateSymbol();
	scaner();
	if (value == "[")
	{
		scaner();
		if (token == NUMBER)
		{
			populateArraySize(value);
			accepted("number");
			scaner();
			if (value == "]")
			{
				accepted(value);
				scaner();
			}
			else
				failed(value);
		}
		else
			failed("number");
	}

}
void DECLARATION()
{

	scaner();
	if (value == "integer" || value == "boolean" || value == "string")
	{
		dataType = value;
		functionReturnType = value;
		accepted(value);
		scaner();
		if (value == "function")
		{
			accepted(value);
			FUNCTION_DECLARATION();
		}
		else
			VARIABLE_DEC();
		//scaner();
		if (token == SEMI)
		{
			DECLARATION();
		}
	}

}
void ARG_LIST(queueStr paraList);
void ARG_LIST_D(queueStr paraList)
{
	if (value == ",")
	{
		accepted(",");
		ARG_LIST(paraList);
	}
	else
	{
		if (paraList.getSize() != 0)
			emitLog("Semantics Error  @ Line No " + intToString(
					file.getLineNo()) + " ::: "
					+ "Less than expected number of argument passed");
		return;
	}
}
void ARG_LIST(queueStr paraList)
{
	EXPRESSION();

	int curReg = cReg.currentReg();
	emit("M[stkptr] = R[" + intToString(curReg) + "];");
	emit("stkptr++;");

	string parameter = paraList.remove();
	if (expressionSemantics != parameter)
		emitLog("Semantics Error in Argument passing @ Line No " + intToString(
				file.getLineNo()) + " ::: " + expressionSemantics + "<------>"
				+ parameter);
	expressionSemantics = "";
	ARG_LIST_D(paraList);
}
void FUNCTION_CALL()
{
	queueStr paraList(100);
	symbol s = getSym(functionName);
	paraList = *(s.getParaList());
	if (paraList.getSize() != 0)
		ARG_LIST(paraList);
	else
		scaner();
}
void FACTOR()
{
	int curReg;
	bool isArray;
	bool isString = 0;
	if (value == "(")
	{
		accepted(value);
		EXPRESSION();
		matchStr(")");

		if (exprStk.getSize() != 0)
			current = exprStk.pop();

		scaner();
	}
	else if (token == IDEN)
	{
		accepted(value);
		functionName = value;
		string ff = value;
		validateSemantics();
		current = getDataType();
		if (current == "string")
		{
			isString = 1;
			lastString = value;
		}

		if (expressionSemantics == "")
			expressionSemantics = current;

		scaner();
		if (value == "(")
		{
			accepted("(");
			string fName = functionName;

			int rCount = pushRegisters();
			callLabelCount++;
			emit("// **********Calling Setup*********");
			emit("M[stkptr] = stkptr;"); //pushing Stkptr
			emit("stkptr++;");
			emit("stkptr++;"); // for return Value
			emit("labelPtr = && _ret" + fName + intToString(callLabelCount)
					+ ";");
			pushReturnName("_ret" + fName + intToString(callLabelCount));
			emit("M[stkptr]= labelPtr;"); //pushing Return Address
			emit("stkptr++;");
			emit("stkptr++;"); // for Func of Func frame pointer
			emit("stkptr++;"); // for next frame pointer

			emit("M[stkptr]=frmptr;"); //pushing Frame Pointer

			if (funcOfFuncCall >= 1)
			{
				int rNum = cReg.getNewReg();
				emit("R[" + intToString(rNum) + "] = M[frmptr - 1];");
				for (int count = 1; count < funcOfFuncCall; count++)
				{
					emit("R[" + intToString(rNum) + "] = M[R[" + intToString(
							rNum) + "]];");
				}
				emit("M[R[" + intToString(rNum) + "] - 2] = stkptr;");
				if (getSym(fName).getIsHeader() == 0)
					emit("stkptr=stkptr+10;");
				else
					emit("stkptr++;");
			}
			else
			{
				emit("M[frmptr-1]=stkptr;");
				if (getSym(fName).getIsHeader() == 0)
					emit("stkptr=stkptr+10;");
				else
					emit("stkptr++;");
			}
			emit("// **********Calling Setup Ends*********");
			funcOfFuncCall++;
			FUNCTION_CALL();

			funcOfFuncCall--;
			if (funcOfFuncCall >= 1)
			{
				int rNum = cReg.getNewReg();
				emit("R[" + intToString(rNum) + "] = M[frmptr - 1];");
				for (int count = 1; count < funcOfFuncCall; count++)
				{
					emit("R[" + intToString(rNum) + "] = M[R[" + intToString(
							rNum) + "]];");
				}
				emit("frmptr = M[R[" + intToString(rNum) + "] - 2];");
			}
			else
			{
				emit("frmptr = M[frmptr - 1];");
			}

			matchStr(")");
			emit("goto _" + fName + ";");
			emitLbl(popReturnName() + ":");

			emit("stkptr = M[frmptr - 5];");
			emit("retValue = M[frmptr - 4];"); // pop the return value
			emit("frmptr = M[frmptr];");// pop old frmptr

			popRegisters(rCount);
			int mNum = getMemoryOffset(fName);
			emit("M[frmptr + " + intToString(mNum) + "] = retValue;");

			if (exprStk.getSize() != 0)
				expressionSemantics = exprStk.pop();

			scaner();
		}
		else if (value == "[")
		{
			consumeBool = 0;
			//code for Array Handling
			accepted(value);
			exprStk.push(expressionSemantics);
			exprStk.push(current);
			EXPRESSION();
			// Checking the array index Datatype
			if (expressionSemantics != "integer")
			{
				emitLog("Error @ Line No " + intToString(file.getLineNo())
						+ " ::: " + "Array Index is not a integer");

			}
			current = exprStk.pop();
			expressionSemantics = exprStk.pop();
			curReg = cReg.currentReg();
			isArray = 1;
			matchStr("]");
			consumeBool = 1;
			scaner();
		}

		int rNum = cReg.getNewReg();
		if (!isArray)
		{
			int mNum = getMemoryOffset(ff);
			emit("R[" + intToString(rNum) + "] =  M[frmptr + " + intToString(
					mNum) + "];");
		}
		else if (isArray)
		{
			if (!isString)
			{
				int mNum = getArrayHeap(ff);
				emit("R[" + intToString(rNum) + "] =  M[" + intToString(mNum)
						+ " + R[" + intToString(curReg) + "]];");

				emit("R[" + intToString(curReg) + "] = R[" + intToString(rNum)
						+ "];");
				cReg.freeRegister();
			}
			else
			{
				int mNum = getMemoryOffset(ff);
				emit("R[" + intToString(rNum) + "] =  M[frmptr + "
						+ intToString(mNum) + "];");
				emit("R[" + intToString(rNum) + "] = R[" + intToString(rNum)
						+ "] +  R[" + intToString(curReg) + "] * 100;");
			}

		}
		return;
	}
	else if (token == STRING || token == NUMBER)
	{
		accepted(value);
		if (token == NUMBER)
		{
			// Handling Boolean
			if ((expressionSemantics == "boolean" or currentTypeLeft
					== "boolean") and (value == "1" or value == "0"))
				current = "boolean";
			else if ((value != "1" and value != "0") and currentTypeLeft
					== "boolean" and consumeBool == 1)
				emitLog("Error @ Line No " + intToString(file.getLineNo())
						+ " ::: " + "Integer Assigned for Boolean");
			else
				current = "integer";
		}
		else if (token == STRING)
		{
			current = "string";
		}

		if (current == "integer")
		{
			int rNum = cReg.getNewReg();
			emit("R[" + intToString(rNum) + "] = " + value + ";");
		}
		else if (current == "string")
		{
			emit("//code for Double quoted String");
			if (value.length() > 102)
			{
				emitLog("String Size Exceeded 100 Characters");
				emitLog("Size Error @ Line No " + intToString(file.getLineNo())
						+ " ::: " + "String Size Exceeded 100 Characters");
			}
			else
			{
				int sNum = cMem.allocateHeap(100);
				emit("strptr = " + intToString(sNum) + ";");
				for (int count = 1; count < value.length() - 1; count++)
				{
					string temp = "M[strptr]= '";
					temp += value[count];
					emit(temp + "';");
					emit("strptr++;");
				}
				int rNum = cReg.getNewReg();
				emit("M[strptr] = 10;"); // putting EOF at the end of String
				emit("R[" + intToString(rNum) + "] =" + intToString(sNum) + ";");
			}
		}
		if (expressionSemantics == "")
			expressionSemantics = current;
		scaner();
	}
	else
		failed(value);
}
void TERM_D()
{
	if (value == "*" || value == "/")
	{
		currentOperator = value;
		string oper = value;
		scaner();
		TERM();

		int curReg = cReg.currentReg();
		emit("R[" + intToString(curReg - 1) + "] = R["
				+ intToString(curReg - 1) + "] " + oper + " R[" + intToString(
				curReg) + "];");
		cReg.freeRegister();
	}
	else
		return;
}
void TERM()
{
	FACTOR();
	/*
	 * Procedure to do Type checking in an expression
	 */
	typeChecking();
	TERM_D();

}
void RELATION_D()
{
	if (value == "<" || value == ">" || value == "==" || value == "!=")
	{
		isOprConsumed = true;
		currentOperator = value;
		string oper = value;
		scaner();
		TERM();

		if (current != "string")
		{
			int curReg = cReg.currentReg();
			emit("R[" + intToString(curReg - 1) + "] = (R[" + intToString(
					curReg - 1) + "] " + oper + " R[" + intToString(curReg)
					+ "])?1:0;");
			cReg.freeRegister();
		}
		else if (current == "string")
		{
			int reg1 = cReg.currentReg();
			int reg2 = reg1 - 1;
			int data1 = cReg.getNewReg();
			int data2 = cReg.getNewReg();
			int result = cReg.getNewReg();
			int strNum = getArrayHeap(lastString);
			emit("//comparison code for Strings" );
			stringLabelCount++;
			emitLbl("_strLabel" + intToString(stringLabelCount) + ":");
			emit("R[" + intToString(data1) + "] = M[R[" + intToString(reg1)
					+ "]];");
			emit("R[" + intToString(data2) + "] = M[R[" + intToString(reg2)
					+ "]];");
			emit("R[" + intToString(reg1) + "] = R[" + intToString(reg1)
					+ "] + 1;");
			emit("R[" + intToString(reg2) + "] = R[" + intToString(reg2)
					+ "] + 1;");
			emit("R[" + intToString(result) + "] = ( R[" + intToString(data1)
					+ "] == R[" + intToString(data2) + "])?1:0;");
			emit("if ( R[" + intToString(result) + "] == 0)");
			emit("goto _strCmp" + intToString(stringLabelCount) + ";");
			emit("if ( R[" + intToString(data1) + "] != 10)");
			emit("goto _strLabel" + intToString(stringLabelCount) + ";");
			emitLbl("_strCmp" + intToString(stringLabelCount) + ":");
			emit("R[" + intToString(reg2) + "] = R[" + intToString(result)
					+ "];");
			if (oper == "!=")
				emit("R[" + intToString(reg2) + "] = ! R[" + intToString(reg2)
						+ "];");
			cReg.freeRegister();
			cReg.freeRegister();
			cReg.freeRegister();
			cReg.freeRegister();
		}

	}
	else
		return;
}
void RELATION()
{
	TERM();
	RELATION_D();
}
void ARITH_OP_D()
{
	if (value == "+" || value == "-" || value == "&" || value == "|")
	{
		currentOperator = value;
		string oper = value;
		scaner();
		RELATION();
		int curReg = cReg.currentReg();
		emit("R[" + intToString(curReg - 1) + "] = R["
				+ intToString(curReg - 1) + "] " + oper + " R[" + intToString(
				curReg) + "];");
		cReg.freeRegister();
	}
	else
		return;

}
void ARITH_OP()
{
	if (value == "not")
	{
		EXPRESSION();
	}
	else
	{
		RELATION();
	}

}
void EXPRESSION_D()
{
	if (value == "and" || value == "or")
	{
		currentOperator = value;

		string oper;
		if (value == "and")
			oper = "&&";
		else
			oper = "||";

		scaner();
		ARITH_OP();
		int curReg = cReg.currentReg();
		emit("R[" + intToString(curReg - 1) + "] = R["
				+ intToString(curReg - 1) + "] " + oper + " R[" + intToString(
				curReg) + "];");
		cReg.freeRegister();
	}
	else
		return;

}
void EXPRESSION_DD()
{
	string value1 = value;
	ARITH_OP_D();
	EXPRESSION_D();
	if (value != "+" && value != "-" && value != "&" && value != "|" && value
			!= "and" && value != "or")
		return;
	else
		EXPRESSION_DD();

}
void EXPRESSION()
{
	scaner();
	if (value == "not")
	{
		scaner();
	}
	if (expressionSemantics != "")
	{
		exprStk.push(expressionSemantics);
		expressionSemantics = "";
	}
	RELATION();
	EXPRESSION_D();
	EXPRESSION_DD();

}
void IF_STMT()
{
	ifLabelCount++;
	EXPRESSION();
	if (!isOprConsumed && expressionSemantics != "boolean")
		emitLog("Error @ Line No  :" + intToString(file.getLineNo()) + "  "
				+ ":::  Operator not used in IF Expression");

	int currReg = cReg.currentReg();
	emit("if(R[" + intToString(currReg) + "] == 0)");
	emit("goto _else" + intToString(ifLabelCount) + ";");
	matchStr("then");
	isOprConsumed = false;
	scaner();
	STATEMENT();
	emit("goto _endif" + intToString(ifLabelCount) + ";");
	emitLbl("_else" + intToString(ifLabelCount) + ":");

	if (value == "else")
	{
		accepted(value);
		scaner();
		STATEMENT();
	}
}
void WHILE_STMT()
{
	whileLabelCount++;
	whileStack.push(whileLabelCount);
	emitLbl("_while" + intToString(whileLabelCount) + ":");
	EXPRESSION();
	int currReg = cReg.currentReg();
	emit("if(R[" + intToString(currReg) + "] == 0)");
	emit("goto _endwhile" + intToString(whileStack.topp()) + ";");
	STATEMENT();
	emit("goto _while" + intToString(whileStack.topp()) + ";");
}
void ASSIGN_STMT()
{
	matchStr(":=");
	EXPRESSION();
}
void STATEMENT()
{
	currentOperator = "";
	if (value != "end" && value != "else")
	{
		if (value == "if")
			IF_STMT();
		else if (value == "while")
			WHILE_STMT();
		else if (token != SEMI && IDENTIFIERStr())
		{
			bool isArray;
			bool isString = 0;
			int arrReg;
			validateSemantics();

			int mNum;
			if (value == fNameStk.topp())
				mNum = -100;
			else
				mNum = getMemoryOffset(value);

			int arrNum = getArrayHeap(value);

			currentTypeLeft = getDataType();
			if (currentTypeLeft == "string")
				isString = 1;
			exprStk.empty();
			expressionSemantics = "";
			/*
			 * for array on LHS of a statement
			 */
			scaner();
			if (value == "[")
			{
				isArray = 1;
				accepted(value);
				EXPRESSION();
				// Checking the array index Datatype
				if (expressionSemantics != "integer")
				{
					emitLog("Size Error @ Line No  :" + intToString(
							file.getLineNo()) + "  "
							+ "Array Index is not a integer");

				}
				arrReg = cReg.currentReg();
				matchStr("]");
				scaner();
			}
			/*
			 * end of array handling
			 */

			ASSIGN_STMT();
			if (currentTypeLeft != expressionSemantics)
				// might need this
				//				if ((currentTypeLeft == "boolean" && expressionSemantics
				//						!= "integer") && (expressionSemantics == "boolean"
				//						&& currentTypeLeft != "boolean"))
				emitLog("Semantics Error @ Line No :" + intToString(
						file.getLineNo()) + " ::: "
						+ "Assignment Datatype Mismatch");
			exprStk.empty();// Clearing stack before and after calling the ASSIGN_STMT()
			expressionSemantics = "";
			if (!isArray)
			{
				if (!isString)
				{
					if (mNum != -100)
					{
						int curReg = cReg.currentReg();
						emit("M[frmptr + " + intToString(mNum) + "] = R["
								+ intToString(curReg) + "];");
						cReg.freeRegister();
					}
					else if (scope != 1)
					{
						int curReg = cReg.currentReg();
						emit("M[frmptr - 4 ] = R[" + intToString(curReg) + "];");
						cReg.freeRegister();
					}
				}
				else
				{

					emit ( "//Code for LHS of String" );
					int curReg = cReg.currentReg();
					int newRegNum = cReg.getNewReg();
					int dataReg = cReg.getNewReg();
					int countReg = cReg.getNewReg();
					stringLabelCount++;
					if (mNum != -100)
					{
						emit("R[" + intToString(newRegNum) + "] = "
								+ intToString(arrNum) + ";");
					}
					else if (scope != 1)
					{
						emit("R[" + intToString(newRegNum) + "] = "
								+ intToString(1000) + ";");
					}
					emit("R[" + intToString(countReg) + "]= 0;");
					emitLbl("_strLabel" + intToString(stringLabelCount) + ":");
					emit("R[" + intToString(dataReg) + "] = M[R["
							+ intToString(curReg) + "]];");
					emit("M[R[" + intToString(newRegNum) + "]] = R["
							+ intToString(dataReg) + "];");
					emit("R[" + intToString(newRegNum) + "]=R[" + intToString(
							newRegNum) + "] + 1;");
					emit("R[" + intToString(countReg) + "]=R[" + intToString(
							countReg) + "] + 1;");
					emit("R[" + intToString(curReg) + "]=R[" + intToString(
							curReg) + "] + 1;");
					emit("if(R[" + intToString(dataReg) + "]!=10)");
					emit("if(R[" + intToString(countReg) + "]!=100)");
					emit("goto _strLabel" + intToString(stringLabelCount) + ";");
					cReg.freeRegister();
					cReg.freeRegister();
					cReg.freeRegister();
					cReg.freeRegister();
					if (mNum != -100)
					{
						int curReg = cReg.currentReg();
						emit("M[frmptr + " + intToString(mNum) + "] = "
								+ intToString(arrNum) + ";");
						cReg.freeRegister();
					}
					else if (scope != 1)
					{
						int curReg = cReg.currentReg();
						emit("M[frmptr - 4 ] = " + intToString(1000) + ";");
						cReg.freeRegister();
					}
				}
			}
			else if (isArray)
			{
				if (!isString)
				{
					int curReg = cReg.currentReg();
					emit("M[" + intToString(arrNum) + " + R[" + intToString(
							arrReg) + "]] = R[" + intToString(curReg) + "];");
					cReg.freeRegister();
					cReg.freeRegister();
				}
				else
				{
					emit("//Code for LHS of String array");

					int curReg = cReg.currentReg();
					int newRegNum = cReg.getNewReg();
					int dataReg = cReg.getNewReg();
					int countReg = cReg.getNewReg();
					stringLabelCount++;
					emit("R[" + intToString(newRegNum) + "] = " + intToString(
							arrNum) + " + R[" + intToString(arrReg) + "]*100;");
					emit("R[" + intToString(countReg) + "]= 0;");
					emitLbl("_strLabel" + intToString(stringLabelCount) + ":");
					emit("R[" + intToString(dataReg) + "] = M[R["
							+ intToString(curReg) + "]];");
					emit("M[R[" + intToString(newRegNum) + "]] = R["
							+ intToString(dataReg) + "];");
					emit("R[" + intToString(newRegNum) + "]=R[" + intToString(
							newRegNum) + "] + 1;");
					emit("R[" + intToString(countReg) + "]=R[" + intToString(
							countReg) + "] + 1;");
					emit("R[" + intToString(curReg) + "]=R[" + intToString(
							curReg) + "] + 1;");
					emit("if(R[" + intToString(dataReg) + "]!=10)");
					emit("if(R[" + intToString(countReg) + "]!=100)");
					emit("goto _strLabel" + intToString(stringLabelCount) + ";");
					cReg.freeRegister();
					cReg.freeRegister();
					cReg.freeRegister();
					cReg.freeRegister();
					if (mNum != -100)
					{
						int curReg = cReg.currentReg();
						emit("M[frmptr + " + intToString(mNum) + "] = "
								+ intToString(arrNum) + ";");
						cReg.freeRegister();
					}
					else if (scope != 1)
					{
						int curReg = cReg.currentReg();
						emit("M[frmptr - 4 ] = " + intToString(curReg) + ";");
						cReg.freeRegister();
					}

				}
			}

		}

		//Checking the END of statement after every Statement Call
		if (token == SEMI)
		{
			scaner();
			STATEMENT();
		}
		else if (value == "end")
		{
			scaner();
			if (value == "if" || value == "while")
			{
				if (value == "if")
				{
					emitLbl("_endif" + intToString(ifLabelCount) + ":");
					ifLabelCount--;
				}
				if (value == "while")
				{
					emitLbl("_endwhile" + intToString(whileStack.pop()) + ":");
				}
				accepted(value);
				scaner();
				STATEMENT();
			}
			else
				failed(value);
		}
		else
			failed(value);
	}
	else
		return;
}

void PROGRAM_BODY()
{
	DECLARATION();
	matchStr("begin");
	emitLbl("_main:");
	emit("stkptr = " + intToString(variableIndex + 2) + ";");
	scaner();
	STATEMENT();
	matchStr("end");
	match("program");
}
void PROGRAM_HEADER()
{
	/*
	 * Populating Library functions in ST
	 */
	populateLibraryScope(1);

	match("program");
	IDENTIFIER();
	match("is");
}
void PROG()
{
	PROGRAM_HEADER();
	scope = 1;
	PROGRAM_BODY();
}
int main()
{
	prePopulate();
	PROG();
	emit("return EXIT_SUCCESS;");
	emitLbl("}");
	cout<<"||||---------------Check the ouput/log.txt for Compiler Result---------------||||"<<endl;
	if(errorCount==0)
		emitLog("Successfully Compiled !! !!");
	else
		emitLog("Compilation Failed with above ERRORS");
}
