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
 * This is the same algorithm as the dchain string encryption lib, but with support for binary data.
 * Note that this algorithm and the string encryption algoritm are NOT compatible!
 * See include/dchain.h for notes and comments on algorithm
 *
 */

#pragma once
#include <string>

namespace dchain
{
	struct data {
		char* addr;
		unsigned int size;
	};
/*
 * Encrypts a piece of binary data. If salt is enabled (default), random chars will be injected into the data
 * making the resulting encrypted data unique. Return data will be empty if either the keyword or data size is null.
 * Returned data is heap allocted.
 */
	data binEncrypt(char* bin, unsigned int size, std::string keyword, bool salt = true);

/*
 * Decrypts a piece of binary data. If salt was used at encryption it needs to be enabled again (default).
 * Should the wrong keyword be used it is possible that the returned data size will be null.
 * Data will also be empty if keyword is empty or size is null.
 * Returned data is heap allocted.
 */
	data binDecrypt(char* bin, unsigned int size, std::string keyword, bool salt = true);
}
