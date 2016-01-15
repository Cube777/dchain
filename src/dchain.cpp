#include "../include/dchain.h"

#include <string>
#include <time.h>
#include <vector>
#include <math.h>

//Generate shifts from a string, returns int* terminated with 0
//Heap memory, so delete when finished
int* genShifts(std::string str);

//Calculates how much random chars should be injected
//into string
int saltiness(int* shifts);

//Shifts a given string forward using previously
//calculated shifts
std::string ShiftForward(std::string str, int* shifts);

//Shifts a given string backward using previously
//calculated shifts
std::string ShiftBack(std::string str, int* shifts);

std::string dchain::strEncrypt(std::string plaintext, std::string keyword, bool salt /*=true*/)
{
	if (plaintext.empty() || keyword.empty())
		return std::string();

	int* keywordShift = genShifts(keyword);
	int* tempShift;
	std::vector<std::string> strblocks;
	std::string temp;

	//Inject random characters into the begining of the plaintext string,
	//which will combined with the daisy chaining generate a new, completely
	//different string with each encryption even though you use the same plaintext
	//and keyword
	if (salt) {
		int granules = saltiness(keywordShift);
		srand(clock());
		for (int i = 0; i < granules; i++)
			plaintext.insert(plaintext.begin(), char(rand() % 95 + 32));
	}

	//Break up plaintext into blocks of chars the size of the keyword
	//and copy into vector
	strblocks.reserve(ceil(plaintext.size() / keyword.size()));
	for (int i = 0; i < plaintext.size();) {
		temp.clear();

		for (; (i < plaintext.size()) && (temp.size() < keyword.size()); i++)
			temp += plaintext[i];

		strblocks.push_back(temp);
	}

	//Shift string block forward with dynamically generated keyword,
	//add that to the ciphertext vector and then shift the string
	//forward again to create the dynamic keyword for the next text block
	std::string currKeyword = keyword;
	std::vector<std::string> ciphertext;
	ciphertext.reserve(strblocks.size());
	for (int i = 0; i < strblocks.size(); i++) {
		tempShift = genShifts(currKeyword);
		temp = ShiftForward(strblocks[i], tempShift);
		delete[] tempShift;

		//Get next keyword
		ciphertext.push_back(temp);
		currKeyword = ShiftForward(temp, keywordShift);
	}

	temp.clear();
	for (int i = 0; i < ciphertext.size(); i++)
		temp += ciphertext[i];


	delete[] keywordShift;
	return temp;
}

std::string dchain::strDecrypt(std::string ciphertext, std::string keyword, bool salt /*=true*/)
{
	if (ciphertext.empty() || keyword.empty())
		return std::string();

	int* keywordShift = genShifts(keyword);

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

	//Shift string block backwards once with the dynamic keyword
	//to obtain plaintext. Then shift the ciphertext block forwards
	//once more to obtain the dynamic keyword for the next block
	std::string currKeyword = keyword;
	std::vector<std::string> plaintext;
	plaintext.reserve(strblocks.size());
	int* tempShift;

	for (int i = 0; i < strblocks.size(); i++) {
		//Get plaintext with dynamic keyword
		tempShift = genShifts(currKeyword);
		temp = ShiftBack(strblocks[i], tempShift);
		delete[] tempShift;
		plaintext.push_back(temp);

		//Get next keyword
		currKeyword = ShiftForward(strblocks[i], keywordShift);
	}

	temp.clear();
	for (int i = 0; i < plaintext.size(); i++)
		temp += plaintext[i];

	if (salt)
		temp.erase(0, saltiness(keywordShift));

	delete[] keywordShift;
	return temp;
}

int* genShifts(std::string str)
{
	int* shifts = new int[str.size() + 1];
	shifts[str.size()] = 0;

	//Calculates an integer, "total" by using the
	//sum ASCII values of the chars. If the char's
	//position in the string is an even number, multiply it's
	//value by two. This to to prevent strings containing the
	//same chars to generate the same shifts that are just in
	//a different order.
	int total = 0;
	for (int i = 0; i < str.size(); i++) {
		if (i % 2 == 0)
			total += int(str[i]);
		else
			total += int(str[i]) * 2;
	}

	//Calculate the shift for each char by dividing the total
	//with the ASCII value of the char and using the floor value of
	//the division. Then add the remainder of the division to the value
	//to prevent that two chars have the same value (because of rounding)
	for (int i = 0; i < str.size(); i++)
		shifts[i] = (floor(total / int(str[i]))) + (total % int(str[i]));

	return shifts;
}

int saltiness(int* shifts)
{
	int total = 0;
	for (int i = 0; shifts[i] != 0; i++)
		total += shifts[i];

	//This will limit the length of the salt to no more than 24
	//chars.
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
