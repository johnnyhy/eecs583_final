#include "cpi.h"
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace std;


void ret_sign(uint8_t* retPtrVal) {
    printf("sign'd yo return bitch\n");
    (void)retPtrVal;
}

void ret_auth(uint8_t* retPtrVal) {
    printf("auth'd you return ass\n");
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
    printf("sign'd yo bitch\n");
    (void)fptrAddr;
    (void)fptrVal;
}

void auth(void(**fptrAddr)(), void(*fptrVal())) {
    // Data &data = getData();
    // data.
    (void)fptrAddr;
    (void)fptrVal;
}

Data::Data() {
    srand(time(0));
    for (int i = 0; i < KEY_LEN; i += sizeof(int)) {
        *((int*)(KEY + 1)) = rand();
    };

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


    return tmp_s;
}

Data& getData() {
    static Data data;
    return data;
}