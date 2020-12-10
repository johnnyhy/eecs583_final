// Standard Includes
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <vector>
#include <sstream>

// OpenSSL Includes
#include "openssl/sha.h"
#include "openssl/hmac.h"
#include "openssl/evp.h"

void ret_sign(uint8_t* retPtrVal);
void sign(void(**fptrAddr)(), void(*fptrVal()));
void auth(void(**fptrAddr)(), void(*fptrVal()));

class Data {
public:
    Data();
    std::unordered_map<void(**)(), std::vector<uint8_t>> signatures;
    std::deque<std::vector<uint8_t>> retSignatures;
    // Options and Config
    static const size_t PTR_LEN = sizeof(void(*)());                   // in bytes for all
    static const size_t KEY_LEN = 64;
    static const size_t SIG_LEN = 256;

    std::vector<uint8_t> DATA;
    std::vector<uint8_t> KEY;
    std::vector<uint8_t> DIGEST;
};

Data& getData();

std::ofstream& logStream();

std::string bufAsHex(uint8_t* data, unsigned len);
