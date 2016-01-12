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
char* shiftForward(char* bin, int size, int* shifts);

//Shift given data backwards - in-place shifting
char* shiftBackward(char* bin, int size, int* shifts);

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
	int crumbs = saltiness(keyShifts);

	char* block = new char[keySize];

	return data();
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
