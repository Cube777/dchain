//Tool to test the threaded module's speed
#include "../include/dchain-bin-threaded.h"
#include <iostream>
#include <fstream>
#include <thread>

#define THREADS 4
#define PLENGTH 1000
#define TESTFILE "test"

int main()
{
	std::cout << "Trying to open file \"" << TESTFILE << "\"...\n";
	std::ifstream ind(TESTFILE, std::ios::binary|std::ios::ate);
	if (ind.is_open())
		std::cout << "File \"" << TESTFILE << "\" opened...\n";
	else {
		std::cout << "Could not open file \"" << TESTFILE << "\". Aborting...\n";
		return 1;
	}
	int size = ind.tellg();
	ind.seekg(0, std::ios::beg);
	unsigned char* bin = new unsigned char[size];
	ind.read((char *)bin, size);
	ind.close();
	std::string temp;
	for (int i = 0; i < PLENGTH; i++)
	temp += char(rand() % 94 + 32);
	double etotal, dtotal;
	etotal = 0;
	dtotal = 0;
	int active_threads;
	if (THREADS < std::thread::hardware_concurrency())
		active_threads = THREADS;
	else
		active_threads = std::thread::hardware_concurrency();
	std::cout <<"All speeds are printed as MB/s...total (per thread)\n";
	dchain::data dec;
	for (int i = 0; i < 1; i++) {
		double start = clock();
		dchain::data enc = dchain::binThreadEncrypt(bin, size, temp, THREADS);
		double end = clock();
		etotal += size / 1000000 / ((end - start) / CLOCKS_PER_SEC / active_threads);
		std::cout << "Encrypt: " << etotal / (i + 1)  << " (" << etotal / (i + 1) / THREADS << ")" << '\t';

		start = clock();
		dec = dchain::binThreadDecrypt(enc.addr, enc.size, temp, THREADS);
		end = clock();
		dtotal += size / 1000000 / ((end - start) / CLOCKS_PER_SEC / active_threads);
		std::cout << "Decrypt: " << dtotal / (i + 1) << " (" << dtotal / (i + 1) / THREADS << ")"<< '\n';
		delete[] enc.addr;
	}
	std::ofstream asd("testout", std::ios::binary);
	asd.write((char *)dec.addr, dec.size);
	asd.close();
	delete[] bin;
	return 0;
}
