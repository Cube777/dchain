//This is an easy way to test the dchain string encryption library
//^C will exit out of the program
#include "../include/dchain.h"
#include <iostream>
#include <limits>

int main() {
	std::string plaintext, keyword, temp;
	int count;
	while (true) {
		std::cout << "Insert plaintext: ";
		std::getline(std::cin, plaintext);
		std::cout << "Insert keyword: ";
		std::getline(std::cin, keyword);
		std::cout << "Insert times encryption/decryption should occur: ";
		std::cin >> count;

		for (int i = 0; i < count; i++) {
			temp = dchain::strEncrypt(plaintext, keyword);
			std::cout << "String encrypted: " << temp << '\n';
			std::cout << "String decrypted: " << dchain::strDecrypt(temp, keyword) << '\n';
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}
