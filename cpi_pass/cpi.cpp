#include "cpi.h"

void ret_sign(uint8_t* retPtrVal) {
    logStream() << "Inside ret_sign" << std::endl;
    (void)retPtrVal;
}

void ret_auth(uint8_t* retPtrVal) {
    logStream() << "Inside ret_auth" << std::endl;
    (void)retPtrVal;
}

void sign(void(**fptrAddr)(), void(*fptrVal())) {
    // Data &data = getData();
    // uint8_t k[KEY_LEN] = {};
    // uint8_t ptrval[PTR_LEN] = { 0xff, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef };
    // uint8_t digest1[SIG_LEN] = {};

    // // plain 256 hash
    // SHA256(ptrval, PTR_LEN, digest1);
    // printBufAsHex(digest1, SIG_LEN, errs());

    // // HMAC 
    // uint8_t* digest2 = HMAC(EVP_sha256(), k, KEY_LEN, ptrval, PTR_LEN, nullptr, nullptr);
    // printBufAsHex(digest2, SIG_LEN, errs());
    logStream() << "Inside sign" << std::endl;
    (void)fptrAddr;
    (void)fptrVal;
}

void auth(void(**fptrAddr)(), void(*fptrVal())) {
    logStream() << "Inside auth" << std::endl;
    Data &data = getData();
    (void)data;
    (void)fptrAddr;
    (void)fptrVal;
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
