//============================================================================
// Name        : memory.h
// Author      : Karthikeyan Muthalagu
// Description : Header for managing MEMORY and REGISTERS in code generation
//============================================================================

class memory
{

private:
	int memIndex;
	int heapIndex;
public:
	memory()
	{
		memIndex = -1;
		heapIndex = 1100;
	}
	int getNewMem()
	{
		memIndex++;
		return (memIndex);
	}
	int currentMem()
	{
		return memIndex;
	}
	int allocateHeap(int size)
	{
		int temp = heapIndex + 1;
		heapIndex = heapIndex + size;
		return temp;
	}

};
class registers
{

private:
	int regIndex;
public:
	registers()
	{
		regIndex = -1;
	}
	int getNewReg()
	{
		regIndex++;
		return (regIndex);
	}
	int currentReg()
	{
		return regIndex;
	}
	void freeRegister()
	{
		if (regIndex != 0)
			regIndex--;
	}
	void setRegNum(int Num)
	{
		regIndex = Num;
	}
};

