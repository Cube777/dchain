## dchain
### A symmetrical-key encryption library

dchain is a multi-platform encryption and decryption library capable of encrypting binary data as well as normal strings. There is no limit on the key length allowing very long keys in the full printable ASCII range. Some of the strong points of dchain:

* Every time you encrypt the same data with the same keyword it will produce a different result. This is thanks to the "daisy chaining" occuring in the algorithm (explained in length in dchain.h). This also means there will never be a repeating pattern even though the original data contains a repeating pattern.
* Low complexity and support for multi-threading. This allows for VERY fast encryption on multicore machines (From some rough testing I did this allows for up to 400MB/s on a quad-core i5 at 3.1GHz)
* Very simple implementation (The plain text version only requires the plaintext string and plaintext keyword)
* Original plaintext/data length is obscured in the process of encryption.

Please report any problems any bugs/issues and I will be glad to fix it.
