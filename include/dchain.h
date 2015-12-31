/*
 ________  ____ __________________________________________________
|    ___ \|    |   \______   \_   _____|______  \______  \______  \
|    \  \/|    |   /|    |  _/|    __)_    /    /   /    /   /    /
|     \___|    |  / |    |   \|        \  /    /   /    /   /    /
\________/|______/  |________/_________/ /____/   /____/   /____/

Developer: Kobus van Schoor alias Cube777
Email: pbscube@gmail.com

You are free to use or modify any part of this code as long as you acknowledge
the original developer.
*/

/*
This is a powerful encryption and decryption module. It is capable of encrypting any strings
as long as the string only contain characters defined in the ASCII table. The algorithm used
shifts each character by a specified value. This value is calculated using the keyword provided
and is unique for every letter. The algorithm dynamically calculates new values for each letter
during encryption from previously encrypted text, "daisy chaining", hence the name dchain.
*/

#pragma once
#include <string>

namespace dchain
{
//Encrypts a string, will return empty string if plaintext
//or keyword is empty. If salt is enabled, a calculated amount
//of extra random characters will be add to the final string
std::string strEncrypt(std::string plaintext, std::string keyword, bool salt = true);

//Decryptes a string, will return empty string if plaintext
//or keyword is empty. Will also return an empty string if an
//error occured while decrypting (can happen if keyword is incorrect).
//If salt is enabled the trailing chars added at encryption
//will be removed. If there is a mismatch between the salt value
//at encryption and decryption, the resulting string could possibly be empty.
std::string strDecrypt(std::string ciphertext, std::string keyword, bool salt = true);
}
