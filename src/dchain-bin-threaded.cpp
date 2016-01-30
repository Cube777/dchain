#include "../include/dchain-bin-threaded.h"

#include <math.h>
#include <time.h>
#include <iostream>
#include <thread>

using namespace dchain;

//Generate shifts for a given piece of data
//Returns pointer to heap memory
int* genShifts(unsigned char* bin, int size);

//Shift given data forward - in-place shifting
void shiftForward(unsigned char* bin, int size, int* shifts);

//Shift given data backwards - in-place shifting
void shiftBackward(unsigned char* bin, int size, int* shifts);

struct thrdData {
	int threadNum;
	int threads;
	int keySize;
	char unsigned* bin;
	int size;
	unsigned char* crypt;
	int* keyShifts;
};

void encryptThread(thrdData data);
void decryptThread(thrdData data);

data dchain::binThreadEncrypt(unsigned char* bin, unsigned int size, std::string keyword, int threads)
{
	if (keyword.empty() || size == 0 || threads == 0)
		return data();

	static bool seed = true;
	if (seed) {
		seed = false;
		srand(time(NULL));
	}

	unsigned char* key = new unsigned char[keyword.size()];
	for (int i = 0; i < keyword.size(); i++)
		key[i] = keyword[i];
	int keySize = keyword.size();
	keyword.clear();

	int* keyShifts = genShifts(key, keySize);
	std::thread* thrds[threads];
	unsigned char* crypt = new unsigned char[(threads * keySize) + size];

	for (int i = 0; i < threads; i++){
		thrdData temp;
		temp.threadNum = i;
		temp.threads = threads;
		temp.keySize = keySize;
		temp.bin = bin;
		temp.size = size;
		temp.crypt = crypt;
		temp.keyShifts = keyShifts;
		thrds[i] = new std::thread(encryptThread, temp);
	}

	for (int i = 0; i < threads; i++)
		thrds[i]->join();
	for (int i = 0; i < threads; i++)
		delete thrds[i];

	delete[] key;
	delete[] keyShifts;
	data rtrn;
	rtrn.addr = crypt;
	rtrn.size = size + (threads * keySize);
	return rtrn;
}

data dchain::binThreadDecrypt(unsigned char* bin, unsigned int size, std::string keyword, int threads)
{
	if (keyword.empty() || size == 0 || threads == 0)
		return data();

	unsigned char* key = new unsigned char[keyword.size()];
	for (int i = 0; i < keyword.size(); i++)
		key[i] = keyword[i];
	int keySize = keyword.size();
	keyword.clear();

	int* keyShifts = genShifts(key, keySize);
	std::thread* thrds[threads];
	unsigned char* plain = new unsigned char[size - (threads * keySize)];

	for (int i = 0; i < threads; i++){
		thrdData temp;
		temp.threadNum = i;
		temp.threads = threads;
		temp.keySize = keySize;
		temp.bin = bin;
		temp.size = size;
		temp.crypt = plain;
		temp.keyShifts = keyShifts;
		thrds[i] = new std::thread(decryptThread, temp);
	}

	for (int i = 0; i < threads; i++)
		thrds[i]->join();
	for (int i = 0; i < threads; i++)
		delete thrds[i];

	delete[] key;
	delete[] keyShifts;
	data rtrn;
	rtrn.addr = plain;
	rtrn.size = size - (threads * keySize);
	return rtrn;
}

int* genShifts(unsigned char* bin, int size)
{
	int* shifts = new int[size];

	int total = 0;
	for (int i = 0; i < size; i++) {
		if (i % 2 == 0)
			total += int(bin[i]);
		else
			total += int(bin[i]) * 2;
	}

	for (int i = 0; i < size; i++)
		shifts[i] = ( (total % (int(bin[i]) + 1)) );

	return shifts;
}

void shiftForward(unsigned char* bin, int size, int* shifts)
{
	int value;

	for (int i = 0; i < size; i++) {
		value = int(bin[i]) + shifts[i];
		while (value > 255)
			value -= 256;

		bin[i] = value;
	}
}

void shiftBackward(unsigned char* bin, int size, int* shifts)
{
	int value;

	for (int i = 0; i < size; i++) {
		value = int(bin[i]) - shifts[i];
		while (value < 0)
			value += 256;

		bin[i] = value;
	}
}

void encryptThread(thrdData data)
{
	unsigned char* block = new unsigned char[data.keySize];
	for (int i = 0; i < data.keySize; i++)
		block[i] = rand() % 256;

	shiftForward(block, data.keySize, data.keyShifts);
	for (int i = data.threadNum * data.keySize; i < (data.threadNum + 1) * data.keySize; i++)
		data.crypt[i] = block[i % data.keySize];
	shiftForward(block, data.keySize, data.keyShifts);
	int* dShifts = genShifts(block, data.keySize);

	for (int i = data.keySize * (data.threads + data.threadNum); i < data.size + (data.threads * data.keySize); i += (data.threads * data.keySize))
	{
		if (i + data.keySize > data.size + (data.threads * data.keySize))
		{
			int temp = i;
			for (; temp < data.size + (data.threads * data.keySize); temp++)
				block[temp % data.keySize] = data.bin[temp - (data.threads * data.keySize)];
			shiftForward(block, temp % data.keySize, dShifts);

			temp = i;
			for (; temp < data.size + (data.threads * data.keySize); temp++)
				data.crypt[temp] = block[temp % data.keySize];

			break;
		}

		for (int k = i; k - i < data.keySize; k++)
			block[k % data.keySize] = data.bin[k - (data.threads * data.keySize)];

		shiftForward(block, data.keySize, dShifts);
		for (int k = i; k - i < data.keySize; k++)
			data.crypt[k] = block[k - i];

		shiftForward(block, data.keySize, data.keyShifts);
		delete[] dShifts;
		dShifts = genShifts(block, data.keySize);
	}

	delete[] dShifts;
	delete[] block;
}

void decryptThread(thrdData data)
{
	unsigned char* block = new unsigned char[data.keySize];

	for (int i = data.threadNum * data.keySize; i < (data.threadNum + 1) * data.keySize; i++)
		block[i % data.keySize] = data.bin[i];

	shiftForward(block, data.keySize, data.keyShifts);
	int* dShifts = genShifts(block, data.keySize);

	for (int i = data.keySize * (data.threads + data.threadNum); i < data.size; i += (data.threads * data.keySize))
	{
		if (i + data.keySize > data.size)
		{
			int temp = i;
			for (; temp < data.size; temp++)
				block[temp % data.keySize] = data.bin[temp];
			shiftBackward(block, temp % data.keySize, dShifts);

			temp = i;
			for (; temp < data.size; temp++)
				data.crypt[temp - (data.threads * data.keySize)] = block[temp % data.keySize];

			break;
		}

		for (int k = i; (k - i) < data.keySize; k++)
			block[k - i] = data.bin[k];

		shiftBackward(block, data.keySize, dShifts);
		for (int k = i - (data.threads * data.keySize); (k + (data.threads * data.keySize) - i) < data.keySize; k++)
			data.crypt[k] = block[k % data.keySize];

		shiftForward(block, data.keySize, dShifts);
		shiftForward(block, data.keySize, data.keyShifts);
		delete[] dShifts;
		dShifts = genShifts(block, data.keySize);
	}

	delete[] dShifts;
	delete[] block;
}
