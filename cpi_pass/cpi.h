// Standard Includes
#include <unordered_map>

// OpenSSL Includes
#include "openssl/sha.h"
#include "openssl/hmac.h"
#include "openssl/evp.h"

void ret_sign(uint8_t* retPtrVal);
void sign(void(**fptrAddr)(), void(*fptrVal()));
void auth(void(**fptrAddr)(), void(*fptrVal()));

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

Data &getData();

