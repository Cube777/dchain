#include "../include/dchain-bin.h"

#include <math.h>
#include <time.h>
#include <iostream>

using namespace dchain;

//Generate shifts for a given piece of data
//Returns pointer to heap memory
int* genShifts(unsigned char* bin, int size);

//Calculates how much random chars should be injected into bin data
int saltiness(int* shifts);

//Shift given data forward - in-place shifting
void shiftForward(unsigned char* bin, int size, int* shifts);

//Shift given data backwards - in-place shifting
void shiftBackward(unsigned char* bin, int size, int* shifts);

data dchain::binEncrypt(unsigned char* bin, unsigned int size, std::string keyword, bool salt /*= true*/)
{
	if (keyword.empty() || size == 0)
		return data();

	unsigned char* key = new unsigned char[keyword.size()];
	for (int i = 0; i < keyword.size(); i++)
		key[i] = keyword[i];
	int keySize = keyword.size();
	keyword.clear();

	int* keyShifts = genShifts(key, keySize);
	int crumbs;
	if (salt)
		crumbs = saltiness(keyShifts);
	else
		crumbs = 0;

	unsigned char* block = new unsigned char[keySize];
	unsigned char* crypt = new unsigned char[crumbs + size];
	int* dShifts = genShifts(key, keySize);
	int pos;

	static bool seed = true;
	if (seed) {
		seed = false;
		srand(time(NULL));
	}

	for (int i = 0; i < crumbs + size; i++) {
		pos = (i + keySize) % keySize;
		if (i < crumbs)
			block[pos] = rand() % 256;
		else
			block[pos] = bin[i - crumbs];

		if (pos + 1 == keySize) {
			shiftForward(block, keySize, dShifts);
			delete[] dShifts;
			for (int k = keySize - 1; k >= 0; k--)
				crypt[i - k] = block[keySize - k - 1];
			shiftForward(block, keySize, keyShifts);
			dShifts = genShifts(block, keySize);
		} else if (i + 1 == crumbs + size) {
			shiftForward(block, pos + 1, dShifts);
			for (int k = pos; k >= 0; k--)
				crypt[i - k] = block[pos - k];
		}
	}
	delete[] dShifts;
	delete[] block;

	delete[] key;
	delete[] keyShifts;
	data rtrn;
	rtrn.addr = crypt;
	rtrn.size = crumbs + size;
	return rtrn;
}

data dchain::binDecrypt(unsigned char* bin, unsigned int size, std::string keyword, bool salt /*= true*/)
{
	if (keyword.empty() || size == 0)
		return data();

	unsigned char* key = new unsigned char[keyword.size()];
	for (int i = 0; i < keyword.size(); i++)
		key[i] = keyword[i];
	int keySize = keyword.size();
	keyword.clear();

	int* keyShifts = genShifts(key, keySize);

	int crumbs;
	if (salt)
		crumbs = saltiness(keyShifts);
	else
		crumbs = 0;

	unsigned char* block = new unsigned char[keySize];
	unsigned char* plain = new unsigned char[size - crumbs];
	int* dShifts = genShifts(key, keySize);
	int pos;

	for (int i = 0; i < size; i++) {
		pos = (i + keySize) % keySize;
		block[pos] = bin[i];

		if (pos + 1 == keySize) {
			shiftBackward(block, keySize, dShifts);
			for (int k = keySize - 1; k >= 0; k--) {
				if ((i - k) >= crumbs)
					plain[i - crumbs - k] = block[keySize - k - 1];
			}
			shiftForward(block, keySize, dShifts);
			shiftForward(block, keySize, keyShifts);
			delete[] dShifts;
			dShifts = genShifts(block, keySize);
		} else if (i + 1 == size) {
			shiftBackward(block, pos + 1, dShifts);
			for (int k = pos; k >= 0; k--)
				plain[i - crumbs - k] = block[pos - k];
		}
	}
	delete[] dShifts;
	delete[] block;

	delete[] key;
	delete[] keyShifts;
	data rtrn;
	rtrn.addr = plain;
	rtrn.size = size - crumbs;
	return rtrn;
}

int* genShifts(unsigned char* bin, int size)
{
	int* shifts = new int[size + 1];
	shifts[size] = 0;

	int total = 0;
	for (int i = 0; i < size; i++) {
		if (i % 2 == 0)
			total += int(bin[i]);
		else
			total += int(bin[i]) * 2;
	}

	for (int i = 0; i < size; i++)
		shifts[i] = total % (int(bin[i]) + 1);

	return shifts;
}

int saltiness(int* shifts)
{
	int total = 0;
	for (int i = 0; shifts[i] != 0; i++)
		total += shifts[i];

	return total % 25;
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
