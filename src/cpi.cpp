#include "cpi.h"
#include <string>
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <cassert>

using namespace std;

void ret_sign(uint8_t* retPtrVal) {
    Data& data = getData();


    memcpy(data.BUF.data(), data.KEY.data(), data.KEY_LEN);
    memcpy(data.BUF.data() + data.KEY_LEN, &retPtrVal, data.PTR_LEN);

    logStream() << "Inside ret_sign: 0x" << bufAsHex(data.BUF.data() + data.KEY_LEN, data.BUF.size() - data.KEY_LEN) << std::endl;

    SHA256(data.BUF.data(), data.BUF.size(), data.DIGEST.data());

    data.retSignatures.push_front(data.DIGEST);
}

void ret_auth(uint8_t* retPtrVal) {
    Data& data = getData();


    memcpy(data.BUF.data(), data.KEY.data(), data.KEY_LEN);
    memcpy(data.BUF.data() + data.KEY_LEN, &retPtrVal, data.PTR_LEN);

    logStream() << "Inside ret_auth: 0x" << bufAsHex(data.BUF.data() + data.KEY_LEN, data.BUF.size() - data.KEY_LEN) << std::endl;

    SHA256(data.BUF.data(), data.BUF.size(), data.DIGEST.data());

    if (data.DIGEST != data.retSignatures.front()) {
        fprintf(stderr, "return address overwritten");
        abort();
    }
    data.retSignatures.pop_front();
}

void sign(void(**fptrAddr)(), void(*fptrVal)()) {

    Data& data = getData();

    if (data.signatures.find(fptrAddr) != data.signatures.end()) {
        auth(fptrAddr, *fptrAddr);
    }

    memcpy(data.BUF.data(), data.KEY.data(), data.KEY_LEN);
    memcpy(data.BUF.data() + data.KEY_LEN, &fptrVal, data.PTR_LEN);

    SHA256(data.BUF.data(), data.BUF.size(), data.DIGEST.data());

    logStream() << "Inside sign: 0x" << bufAsHex(data.BUF.data() + data.KEY_LEN, data.BUF.size() - data.KEY_LEN) << fptrVal << std::endl;

    data.signatures[fptrAddr] = data.DIGEST;

    // // HMAC 
    // uint8_t* digest2 = HMAC(EVP_sha256(), k, KEY_LEN, ptrval, PTR_LEN, nullptr, nullptr);
    // printBufAsHex(digest2, SIG_LEN, errs());
    (void)fptrAddr;
    (void)fptrVal;
}

void auth(void(**fptrAddr)(), void(*fptrVal)()) {
    Data& data = getData();

    assert(data.signatures.find(fptrAddr) != data.signatures.end());

    memcpy(data.BUF.data(), data.KEY.data(), data.KEY_LEN);
    memcpy(data.BUF.data() + data.KEY_LEN, &fptrVal, data.PTR_LEN);

    logStream() << "Inside auth: 0x" << bufAsHex(data.BUF.data() + data.KEY_LEN, data.BUF.size() - data.KEY_LEN) << std::endl;

    SHA256(data.BUF.data(), data.BUF.size(), data.DIGEST.data());

    if (data.signatures[fptrAddr] != data.DIGEST) {
        fprintf(stderr, "stack smashing detected");
        abort();
    }
}

Data::Data() : BUF(vector<uint8_t>(KEY_LEN + PTR_LEN, 0)),
KEY(vector<uint8_t>(KEY_LEN, 0)),
DIGEST(vector<uint8_t>(32, 0)) {
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
