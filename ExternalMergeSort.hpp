#pragma once
#include "ExternalSort.hpp"
#include <algorithm>
#include <sstream>
#include <ctime>
#include <set>

template < typename T >
class ExternalMergeSort : public ExternalSort
{
public:
	ExternalMergeSort(int ramUsage, int outputSize, 
					  const std::vector < std::string >& inputFiles, 
					  const std::string& outputFile) : ExternalSort(ramUsage, outputSize, inputFiles, outputFile)
	{
	}

	virtual ~ExternalMergeSort() {}

	virtual void sort()
	{
		clock_t start = clock();
		int nChunks = _sortChunks();
		clock_t end = clock();
		printf("Chunks time = %.2f\n", (float)(end - start) / CLOCKS_PER_SEC);
		_kthMergeSort(nChunks);
	}

private:
	int _sortChunks()
	{		
		int chunkSize = _ramUsage / sizeof(T);
		std::vector < T > chunk;
		chunk.reserve(chunkSize);
		int inputIndex = 0;
		int nChunks = 0;
		while (true)
		{
			T tmp(_input[inputIndex]);
			if (!_input[inputIndex].eof())
			{
				chunk.push_back(tmp);
			}
			bool end = false;	
			if (_input[inputIndex].eof())
			{
				inputIndex++;
				if (inputIndex == _input.size())
				{
					end = true;
				}
			}
			if (chunk.size() == chunkSize || end)
			{
				std::sort(chunk.begin(), chunk.end());
				_writeChunk(chunk, nChunks++);
				chunk.clear();
			}	
			if (end)
			{
				break;
			}
		}
		return nChunks;
	}

	std::string _getChunkName(int index)
	{
		std::stringstream ss;
		ss << "C:\\sort\\chunk" << index;
		std::string res; ss >> res;
		return res;
	}

	void _writeChunk(const std::vector < T >& chunk, int chunkIndex)
	{
		std::ofstream out(_getChunkName(chunkIndex), std::ios::out | std::ios::binary);
		for (int i = 0; i < chunk.size(); i++)
		{
			chunk[i].write(out);
		}
		T::flush(out);
		out.close();
	}

	void _kthMergeSort(int nChunks)
	{
		int bufferSize = _ramUsage / sizeof(T) / (nChunks + 1);
		std::vector < std::vector < T > > buffers(nChunks + 1);
		std::vector < std::ifstream > streams(nChunks);
		std::vector < int > isEof(nChunks);
		std::set < std::pair < T,int > > q;
		for (int i = 0; i <= nChunks; i++)
		{
			buffers[i].reserve(bufferSize);
			if (i < nChunks)
			{
				streams[i] = std::ifstream(_getChunkName(i), std::ios::in | std::ios::binary);
				_readBuffer(streams[i], buffers[i], bufferSize, isEof[i]);
				if (!buffers[i].empty())
				{
					q.insert(std::make_pair(buffers[i].back(), i));
				}
			}
		}
		while (true)
		{
			int smallest = -1;
			if (!q.empty())
			{
				smallest = q.begin()->second;
				q.erase(q.begin());
				buffers[nChunks].push_back(buffers[smallest].back());
				buffers[smallest].pop_back();				
				if (buffers[smallest].empty() && !isEof[smallest])
				{
					_readBuffer(streams[smallest], buffers[smallest], bufferSize, isEof[smallest]);
				}
				if (!buffers[smallest].empty())
				{
					q.insert(std::make_pair(buffers[smallest].back(), smallest));
				}
			}
			if (smallest == -1 || buffers[nChunks].size() == bufferSize)
			{
				for (size_t i = 0; i < buffers[nChunks].size(); i++)
				{
					buffers[nChunks][i].write(_output);
				}
				buffers[nChunks].clear();
			}
			if (smallest == -1)
			{
				break;
			}
		}
		T::flush(_output);
	}

	void _readBuffer(std::ifstream& in, std::vector < T >& v, int cnt, int& isEof)
	{
		while (cnt > 0)
		{
			T tmp(in);
			if (in.eof())
			{
				isEof = true;
				break;
			}
			v.push_back(tmp);
			cnt--;
		}
		std::reverse(v.begin(), v.end());
	}
};