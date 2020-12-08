// OpenSSL Includes
#include "openssl/sha.h"
#include "openssl/hmac.h"
#include "openssl/evp.h"

// Custom includes
#include "utils.h"

class CPI {
public:
    void sign() const;
    void auth() const;

private:
    // Options and Config
    static const size_t PTR_LEN = 8;                   // in bytes for all
    static const size_t KEY_LEN = 32;
    static const size_t SIG_LEN = 32;
    const uint8_t KEY[KEY_LEN] = { };

};