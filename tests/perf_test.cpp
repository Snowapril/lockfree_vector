#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include "../src/LockFree/Vector.hpp" // TODO: replace relative path

int main()
{
	LockFree::Vector<int> vec;
	
    std::vector<std::thread> workers;

	return 0;
}