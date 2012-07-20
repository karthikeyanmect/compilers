//============================================================================
// Name        : queue.h
// Author      : Karthikeyan Muthalagu
// Description : Header, where all queue data structures are defined
//============================================================================
#include<string>
using namespace ::std;

class queueStr
{
private:
	int queueSize;
	int front;
	int rear;
	string *data;
public:
	// Constructing a queue
	queueStr(int size)
	{
		front = -1;
		rear = 0;
		queueSize = size;
		data = new string[size];
	}
	bool add(string value)
	{
		if (!isFull())
		{
			this->front++;
			this->data[this->front] = value;
			return true;
		}
		else
			return false;

	}
	bool isEmpty()
	{
		if (this->front < this->rear)
			return true;
		else
			return false;
	}
	string remove()
	{
		if (!isEmpty())
		{
			string ret;
			ret = this->data[this->rear];
			this->rear++;
			return ret;
		}
		else
			return "-1";

	}
	bool isFull()
	{
		if (this->front >= this->queueSize)
			return true;
		else
			return false;
	}
	int getSize()
	{
		return (front - rear + 1);
	}
};

class queueInt
{
private:
	int queueSize;
	int front;
	int rear;
	int *data;
public:
	// Constructing a queue
	queueInt(int size)
	{
		front = -1;
		rear = 0;
		queueSize = size;
		data = new int[size];
	}
	bool add(int value)
	{
		if (!isFull())
		{
			this->front++;
			this->data[this->front] = value;
			return true;
		}
		else
			return false;

	}
	bool isEmpty()
	{
		if (this->front < this->rear)
			return true;
		else
			return false;
	}
	int remove()
	{
		if (!isEmpty())
		{
			int ret;
			ret = this->data[this->rear];
			this->rear++;
			return ret;
		}
		else
			return -1;

	}
	bool isFull()
	{
		if (this->front >= this->queueSize)
			return true;
		else
			return false;
	}
	int getSize()
	{
		return (front - rear + 1);
	}
};
