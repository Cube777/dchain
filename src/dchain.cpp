#include "../include/dchain.h"

#include <time.h>
#include <stdlib.h>
#include <vector>

//Hash a string, returns int* terminated by a 0.
//Heap memory, so delete when finished
int* HashString(std::string str);

//Calculates how much random chars should be appended
//to string to obscure string length
int TrailingChars(int* shifts);

//Shifts a given string forward using previously
//calculated hash for HashString
std::string ShiftForward(std::string str, int* shifts);

//Shifts a given string backward using previously
//calculated hash from HashString
std::string ShiftBack(std::string str, int* shifts);

std::string dchain::strEncrypt(std::string plaintext, std::string keyword, bool salt /*=true*/)
{
	if (plaintext.empty() ||
			keyword.empty())
		return std::string();

	int* keywordShift = HashString(keyword);
	int* tempShift;
	std::vector<std::string> strblocks;
	std::string temp;

	//Add a calculated amount (calculated using the keyword)
	//of random chars to the cipherText to obscure text
	//length and change string completely with every encryption
	if (salt) {
		int trailChars = TrailingChars(keywordShift);
		srand(clock());
		for (int i = 0; i < trailChars; i++)
			plaintext.insert(temp.begin(), char(rand() % 95 + 32));
	}

	//Break up plaintext into blocks of chars the size of the keyword
	//and copy into vector
	for (int i = 0; i < plaintext.size();) {
		temp.clear();

		for (; (i < plaintext.size()) && (temp.size() < keyword.size()); i++)
			temp += plaintext[i];

		strblocks.push_back(temp);
	}

	//First shift plaintext forward with dynamic shift,
	//add that to cipherText vector, then shift it again
	//with keyword and use that as the keyword for the next string block
	std::string currKeyword = keyword;
	std::vector<std::string> ciphertext;
	ciphertext.reserve(strblocks.size());
	for (int i = 0; i < strblocks.size(); i++) {
		//Get next ciphertext block with dynamic keyword
		tempShift = HashString(currKeyword);
		temp = ShiftForward(strblocks[i], tempShift);
		delete tempShift;

		//Get next keyword
		ciphertext.push_back(temp);
		currKeyword = ShiftForward(temp, keywordShift);
	}

	temp.clear();
	for (int i = 0; i < ciphertext.size(); i++)
		temp += ciphertext[i];


	delete keywordShift;
	return temp;
}

std::string dchain::strDecrypt(std::string ciphertext, std::string keyword, bool salt /*=true*/)
{
	if (ciphertext.empty() ||
			keyword.empty())
		return std::string();

	int* keywordShift = HashString(keyword);

	//Break up the ciphertext into blocks the
	//size of the keyword
	std::vector<std::string> strblocks;
	std::string temp;
	for (int i = 0; i < ciphertext.size();) {
		temp.clear();

		for (; (i < ciphertext.size()) && (temp.size() < keyword.size()); i++)
			temp += ciphertext[i];

		strblocks.push_back(temp);
	}

	//Shift the strblock back once with the dynamic shifts
	//and add it to plaintext vector. Also shift strblock
	//forward to obtain next dynamixc shift
	std::string currKeyword = keyword;
	std::vector<std::string> plaintext;
	plaintext.reserve(strblocks.size());
	int* tempShift;

	for (int i = 0; i < strblocks.size(); i++) {
		//Get plaintext with dynamic keyword
		tempShift = HashString(currKeyword);
		temp = ShiftBack(strblocks[i], tempShift);
		delete tempShift;
		plaintext.push_back(temp);

		//Get next keyword
		currKeyword = ShiftForward(strblocks[i], keywordShift);
	}

	temp.clear();

	for (int i = 0; i < plaintext.size(); i++)
		temp += plaintext[i];


	if (salt) {
		int trailChars = TrailingChars(keywordShift);
		for (int i = 0; i < trailChars; i++) {
			if (ciphertext.empty())
				return std::string();

			ciphertext.erase(0, 1);
		}
	}


	return temp;
}

int* HashString(std::string str)
{
	int* shifts = new int[str.size() + 1];
	shifts[str.size()] = 0;

	//Calculates an integer, "total" by using the
	//ASCII value of the char if the char's position
	//is an even number, or the ASCII value * 2 if
	//it is an uneven value
	int total = 0;
	for (int i = 0; i < str.size(); i++) {
		if (i % 2 == 0)
			total += int(str[i]);
		else
			total += int(str[i]) * 2;
	}

	//Calculate the shift for each value
	//using (total / ASCII value) + remainder as
	//formula
	for (int i = 0; i < str.size(); i++)
		shifts[i] = (total / int(str[i])) + (total % int(str[i]));

	return shifts;
}

int TrailingChars(int* shifts)
{
	int total = 0;
	for (int i = 0; shifts[i] != 0; i++)
		total += shifts[i];

	return total % 25;
}

std::string ShiftForward(std::string str, int* shifts)
{
	int ascii;
	std::string temp;

	for (int i = 0; i < str.size(); i++) {
		//Increase ascii value with shift
		ascii = int(str[i]) + shifts[i];

		//Reduce ascci value until it is in the standard
		//ASCII table
		while (ascii > 126)
			ascii -= 95;

		temp += char(ascii);
	}

	return temp;
}

std::string ShiftBack(std::string str, int* shifts)
{
	int ascii;
	std::string temp;

	for (int i = 0; i < str.size(); i++) {
		//Decrease ascii with shift
		ascii = int(str[i]) - shifts[i];

		//Increase asscii until it is in
		//the standard ASCII table
		while (ascii < 32)
			ascii += 95;

		temp += char(ascii);
	}

	return temp;
}
