// Standard Includes
#include <unordered_map>

// OpenSSL Includes
#include "openssl/sha.h"
#include "openssl/hmac.h"
#include "openssl/evp.h"

void ret_sign(uint8_t* retPtrVal);
void sign(void(**fptrAddr)(), void(*fptrVal()));
void auth(void(**fptrAddr)(), void(*fptrVal()));

struct Data {
public:
    Data();
    std::unordered_map<void*, uint8_t*> signatures;
    // Options and Config
    static const size_t PTR_LEN = sizeof(void(*)());                   // in bytes for all
    static const size_t KEY_LEN = 64;
    static const size_t SIG_LEN = 256;
    uint8_t DATA[PTR_LEN] = { };
    uint8_t KEY[KEY_LEN] = { };
};

Data& getData();
