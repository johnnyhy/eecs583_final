#include "cpi.h"

using namespace llvm;

void sign(uint8_t* fptrAddr, uint8_t* fptrVal) const {
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
    printf("sign'd yo bitch\n");
}

void auth(uint8_t* fptrAddr, uint8_t* fptrVal) const {
    // Data &data = getData();
    // data.
}

Data& getData() {
    static Data data;
    return data;
}