/*
 *  ________  ____ __________________________________________________
 * |    ___ \|    |   \______   \_   _____|______  \______  \______  \
 * |    \  \/|    |   /|    |  _/|    __)_    /    /   /    /   /    /
 * |     \___|    |  / |    |   \|        \  /    /   /    /   /    /
 * \________/|______/  |________/_________/ /____/   /____/   /____/
 *
 * Developer: Kobus van Schoor alias Cube777
 * Email: pbscube@gmail.com
 *
 * You are free to use or modify any part of this code as long as you acknowledge
 * the original developer.
 */

/*
 * dchain is string encryption library that can encrypt any string with any length
 * keyword as long as the string only contains ASCII letters. Some of the strengths
 * of the algoritm are:
 *
 * - Very low complexity
 * - Using the same keyword and plaintext will never produce the same results
 * - No repeating pattern whatsoever (achieved by daisy chaining a keyword, hence
 *   the name of the algorithm)
 * - No limit to keyword length
 */


#pragma once
#include <string>

namespace dchain
{
/*
 * Encrypts a string. If salt is enabled (default), random chars will be injected into the string,
 * making the resulting cipherext unique. Will return an empty string if either the plaintext
 * or keyword is empty
 */
std::string strEncrypt(std::string plaintext, std::string keyword, bool salt = true);

/*
 * Decrypts a string. If salt was used at encryption it needs to be enabled again (default).
 * Should the wrong keyword be used it is possible that an empty string will be returned.
 * Will also return an empty string if the plaintext or keyword is empty.
 */
std::string strDecrypt(std::string ciphertext, std::string keyword, bool salt = true);
}
