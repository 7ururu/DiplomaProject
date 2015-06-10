#pragma once
#include <fstream>
#include <vector>
#include <string>

class ExternalSort
{
public:
	ExternalSort(int ramUsage, int outputSize, 
				const std::vector < std::string >& inputFiles, 
				const std::string& outputFile) : _ramUsage(ramUsage), _outputSize(outputSize)
	{		
		for (size_t i = 0; i < inputFiles.size(); i++)
		{
			_input.push_back(std::ifstream(inputFiles[i], std::ios::in | std::ios::binary));
		}
		_output = std::ofstream(outputFile, std::ios::out | std::ios::binary);
	}

	virtual ~ExternalSort() 
	{
		for (size_t i = 0; i < _input.size(); i++)
		{
			_input[i].close();
		}
		_output.close();
	}

	virtual void sort() = 0;

protected:
	std::vector < std::ifstream > _input;
	std::ofstream _output;

	int _ramUsage;
	int _outputSize;
};