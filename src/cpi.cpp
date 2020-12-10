#include "cpi.h"
#include <string>
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <cassert>

using namespace std;


void ret_sign(uint8_t* retPtrVal) {
    logStream() << "Inside ret_sign" << std::endl;
    Data& data = getData();

    vector<uint8_t> buf(data.KEY_LEN + data.PTR_LEN, 0);

    memcpy(buf.data(), data.KEY.data(), data.KEY_LEN);
    memcpy(buf.data() + data.KEY_LEN, retPtrVal, data.PTR_LEN);

    SHA256(buf.data(), sizeof(buf), data.DIGEST.data());
    data.retSignatures.push_front(data.DIGEST);
}

void ret_auth(uint8_t* retPtrVal) {
    logStream() << "Inside ret_auth" << std::endl;
    Data& data = getData();

    vector<uint8_t> buf(data.KEY_LEN + data.PTR_LEN, 0);

    memcpy(buf.data(), data.KEY.data(), data.KEY_LEN);
    memcpy(buf.data() + data.KEY_LEN, retPtrVal, data.PTR_LEN);

    SHA256(buf.data(), sizeof(buf), data.DIGEST.data());

    if (data.DIGEST != data.retSignatures.front()) {
        fprintf(stderr, "return address overwritten");
        abort();
    }
    data.retSignatures.pop_front();
}

void sign(void(**fptrAddr)(), void(*fptrVal())) {
    Data& data = getData();

    if (data.signatures.find(fptrAddr) != data.signatures.end()) {
        auth(fptrAddr, fptrVal);
    }

    vector<uint8_t> buf(data.KEY_LEN + data.PTR_LEN, 0);

    memcpy(buf.data(), data.KEY.data(), data.KEY_LEN);
    memcpy(buf.data() + data.KEY_LEN, (uint8_t*)fptrVal, data.PTR_LEN);

    SHA256(buf.data(), sizeof(buf), data.DIGEST.data());

    data.signatures[fptrAddr] = data.DIGEST;

    // // HMAC 
    // uint8_t* digest2 = HMAC(EVP_sha256(), k, KEY_LEN, ptrval, PTR_LEN, nullptr, nullptr);
    // printBufAsHex(digest2, SIG_LEN, errs());
    logStream() << "Inside sign" << std::endl;
    (void)fptrAddr;
    (void)fptrVal;
}

void auth(void(**fptrAddr)(), void(*fptrVal())) {
    logStream() << "Inside auth" << std::endl;
    Data& data = getData();

    assert(data.signatures.find(fptrAddr) != data.signatures.end());

    vector<uint8_t> buf(data.KEY_LEN + data.PTR_LEN, 0);

    memcpy(buf.data(), data.KEY.data(), data.KEY_LEN);
    memcpy(buf.data() + data.KEY_LEN, (uint8_t*)fptrVal, data.PTR_LEN);

    SHA256(buf.data(), sizeof(buf), data.DIGEST.data());

    if (data.signatures[fptrAddr] != data.DIGEST) {
        fprintf(stderr, "stack smashing detected");
        abort();
    }
}

Data::Data() : DATA(vector<uint8_t>(PTR_LEN + KEY_LEN, 0)),
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
