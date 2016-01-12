#include "../include/dchain-bin.h"

#include <math.h>
#include <time.h>

using namespace dchain;

//Generate shifts for a given piece of data
//Returns pointer to heap memory
int* genShifts(char* bin, int size);

//Calculates how much random chars should be injected into bin data
int saltiness(int* shifts);

//Shift given data forward - in-place shifting
void shiftForward(char* bin, int size, int* shifts);

//Shift given data backwards - in-place shifting
void shiftBackward(char* bin, int size, int* shifts);

data binEncrypt(char* bin, unsigned int size, std::string keyword, bool salt /*= true*/)
{
	if (keyword.empty() || size == 0)
		return data();

	char* key = new char[keyword.size()];
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


	char* block = new char[keySize];
	char* crypt = new char[crumbs + size];
	int* dShifts = genShifts(key, keySize);
	int pos;

	for (int i = 0; i < crumbs + size; i++) {
		pos = (i + keySize + 1) % keySize;
		if (i < crumbs)
			block[pos] = random() % 256;
		else
			block[pos] = bin[i - crumbs];

		if (pos + 1 == keySize) {
			shiftForward(block, keySize, dShifts);
			delete dShifts;
			for (int k = keySize - 1; k >= 0; k--)
				crypt[i - k] = block[keySize - k - 1];
			shiftForward(block, keySize, keyShifts);
			dShifts = genShifts(block, keySize);
		} else if (i + 1 == crumbs + size) {
			shiftForward(block, pos + 1, dShifts);
			delete[] dShifts;
			for (int k = pos; k >= 0; k--)
				crypt[i - k] = block[pos - k];
			delete[] block;
		}
	}

	delete[] key;
	delete[] keyShifts;
	data rtrn;
	rtrn.addr = crypt;
	rtrn.size = crumbs + size;
	return rtrn;
}

int* genShifts(char* bin, int size)
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
		shifts[i] = (floor(total / int(bin[i]))) + (total % int(bin[i]));

	return shifts;
}

int saltiness(int* shifts)
{
	int total = 0;
	for (int i = 0; shifts[i] != 0; i++)
		total += shifts[i];

	return total % 25;
}

void shiftForward(char* bin, int size, int* shifts)
{
	int value;

	for (int i = 0; i < size; i++) {
		value = int(bin[i]) + shifts[i];
		while (value > 255)
			value -= 255;

		bin[i] = char(value);
	}
}

void shiftBackward(char* bin, int size, int* shifts)
{
	int value;

	for (int i = 0; i < size; i++) {
		value = int(bin[i]) - shifts[i];
		while (value < 0)
			value += 255;

		bin[i] = char(value);
	}
}
