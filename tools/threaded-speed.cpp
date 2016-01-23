//Tool to test the threaded module's speed
#include "../include/dchain-bin-threaded.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

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
	std::string key;
	for (int i = 0; i < PLENGTH; i++)
		key += char(rand() % 94 + 32);
	double etotal, dtotal, temp;
	etotal = 0;
	dtotal = 0;
	std::cout <<"All speeds are printed as MB/s...total (per thread)\n";
	std::chrono::high_resolution_clock::time_point t1, t2;
	std::chrono::duration<double> time;
	for (int i = 0; i < 100; i++) {
		t1 = std::chrono::high_resolution_clock::now();
		dchain::data enc = dchain::binThreadEncrypt(bin, size, key, THREADS);
		t2 = std::chrono::high_resolution_clock::now();
		temp = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
		etotal += size / 1000000 / temp;
		std::cout << "Encrypt: " << etotal / (i + 1)  << " (" << etotal / (i + 1) / THREADS << ")" << '\t';

		t1 = std::chrono::high_resolution_clock::now();
		dchain::data dec = dchain::binThreadDecrypt(enc.addr, enc.size, key, THREADS);
		t2 = std::chrono::high_resolution_clock::now();
		temp = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
		dtotal += size / 1000000 / temp;
		std::cout << "Decrypt: " << dtotal / (i + 1) << " (" << dtotal / (i + 1) / THREADS << ")"<< '\n';
		delete[] enc.addr;
		delete[] dec.addr;
	}
	delete[] bin;
	return 0;
}
