// Standard Includes
#include <unordered_map>

// OpenSSL Includes
#include "openssl/sha.h"
#include "openssl/hmac.h"
#include "openssl/evp.h"

// Custom includes
#include "utils.h"

void sign(uint8_t* fptrAddr, uint8_t* fptrVal) const;
void auth(uint8_t* fptrAddr, uint8_t* fptrVal) const;
Data &getData();

struct Data
{
public:
    std::unordered_map<void *, uint8_t *> signatures;
    // Options and Config
    static const size_t PTR_LEN = 8;                   // in bytes for all
    static const size_t KEY_LEN = 32;
    static const size_t SIG_LEN = 32;
    const uint8_t KEY[KEY_LEN] = { };
};