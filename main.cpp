#include "ExternalMergeSort.hpp"
#include "SortItem.hpp"
#include <ctime>

int main(int argc, char** argv)
{
	clock_t start = clock();

	const int bufferSize = 5 * 1000 * 1000;
	const int ramSize = 21 * 1000 * 1000;
	ExternalMergeSort < SortItem < 100,10,bufferSize > > sort(ramSize, 1000, std::vector < std::string > (1, "C:\\sort\\input"), "C:\\sort\\output");
	sort.sort();

	clock_t end = clock();
	printf("Run time = %.2f\n", (float)(end - start) / CLOCKS_PER_SEC);

	return 0;
}