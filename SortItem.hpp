#pragma once
#include <fstream>

template < int Size,int KeySize,int BufferSize >
class SortItem
{
public:
	SortItem(std::ifstream& in)
	{
		read(in);
	}

	void read(std::ifstream& in)
	{
		in.read(data, Size);
	}

	void write(std::ofstream& out) const
	{
		memcpy(_outBuffer + _outBufferSize, data, Size);
		_outBufferSize += Size;
		if (_outBufferSize == BufferSize)
		{
			flush(out);
		}
	}

	bool operator <(const SortItem& other) const
	{
		int res = memcmp(data, other.data, KeySize);
		return res < 0;
	}

	static void flush(std::ofstream& out)
	{
		out.write(_outBuffer, _outBufferSize);
		_outBufferSize = 0;
	}

	char data[Size];

private:
	static int _outBufferSize;
	static char _outBuffer[BufferSize];
};

template < int Size,int KeySize,int BufferSize > int SortItem < Size,KeySize,BufferSize > :: _outBufferSize = 0;
template < int Size,int KeySize,int BufferSize > char SortItem < Size,KeySize,BufferSize > :: _outBuffer[BufferSize];