#include "cpi.h"
#include <string>
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <cassert>

using namespace std;

void ret_sign(uint8_t* retPtrVal) {
    // logStream() << "in ret_sign" << endl;
    Data& data = getData();

    // // data.DIGEST = vector<uint8_t>(digest2, digest2 + 32);
    std::vector<uint8_t> hash(EVP_MAX_MD_SIZE, 0);
    unsigned int hashLen;

    HMAC(
        EVP_sha256(),
        data.KEY.data(),
        static_cast<int>(data.KEY.size()),
        (const unsigned char*)&retPtrVal,
        data.PTR_LEN,
        hash.data(),
        &hashLen
    );



    data.retSignatures.push_front(hash);
}

void ret_auth(uint8_t* retPtrVal) {
    // logStream() << "in ret_auth" << endl;

    Data& data = getData();

    // // data.DIGEST = vector<uint8_t>(digest2, digest2 + 32);
    std::vector<uint8_t> hash(EVP_MAX_MD_SIZE, 0);
    unsigned int hashLen;

    HMAC(
        EVP_sha256(),
        data.KEY.data(),
        static_cast<int>(data.KEY.size()),
        (const unsigned char*)&retPtrVal,
        data.PTR_LEN,
        hash.data(),
        &hashLen
    );


    if (hash != data.retSignatures.front()) {
        fprintf(stderr, "return address overwritten");
        abort();
    }
    data.retSignatures.pop_front();
}

void sign(void(**fptrAddr)(), void(*fptrVal)()) {
    // logStream() << "in sign" << endl;

    Data& data = getData();

    if (data.signatures.find(fptrAddr) != data.signatures.end()) {
        auth(fptrAddr, *fptrAddr);
    }

    std::vector<uint8_t> hash(EVP_MAX_MD_SIZE, 0);
    unsigned int hashLen;

    HMAC(
        EVP_sha256(),
        data.KEY.data(),
        static_cast<int>(data.KEY.size()),
        (const unsigned char*)&fptrVal,
        data.PTR_LEN,
        hash.data(),
        &hashLen
    );

    // // // data.DIGEST = vector<uint8_t>(digest2, digest2 + 32);
    data.signatures[fptrAddr] = hash;
}

void auth(void(**fptrAddr)(), void(*fptrVal)()) {
    // logStream() << "in auth" << endl;

    Data& data = getData();

    std::vector<uint8_t> hash(EVP_MAX_MD_SIZE, 0);
    unsigned int hashLen;

    HMAC(
        EVP_sha256(),
        data.KEY.data(),
        static_cast<int>(data.KEY.size()),
        (const unsigned char*)&fptrVal,
        data.PTR_LEN,
        hash.data(),
        &hashLen
    );
    // // data.DIGEST = vector<uint8_t>(digest2, digest2 + 32);

    if (data.signatures[fptrAddr] != hash) {
        fprintf(stderr, "stack smashing detected");
        abort();
    }
}

Data::Data() : KEY(vector<uint8_t>(KEY_LEN, 0)) {
    srand(time(0));
    for (size_t i = 0; i < KEY_LEN; i += sizeof(int)) {
        KEY[i] = rand();
    };
}

Data& getData() {
    static Data data;
    return data;
}

std::ofstream& logStream() {
    static std::ofstream s;
    if (!s.is_open())
        s.open("cpi.log");
    return s;
}

std::string bufAsHex(uint8_t* data, unsigned len) {
    std::stringstream ss;
    for (unsigned i = 0; i < len; ++i)
        ss << std::hex << (int)data[i];
    return ss.str();
}
