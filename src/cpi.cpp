
#include "cpi.h"
#include <string>
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <cassert>

using namespace std;

void ret_sign(uint8_t* retPtrVal) {
    Data& data = getData();
 
    memcpy(data.BUF.data(), data.RET_KEY.data(), data.KEY_LEN);
    memcpy(data.BUF.data() + data.KEY_LEN, &retPtrVal, data.PTR_LEN);

    // logStream() << "Inside ret_sign: 0x" << bufAsHex(data.BUF.data() + data.KEY_LEN, data.BUF.size() - data.KEY_LEN) << std::endl;

    SHA256(data.BUF.data(), data.BUF.size(), data.DIGEST.data());

    data.retSignatures.push_front(data.DIGEST);
}

void ret_auth(uint8_t* retPtrVal) {
    Data& data = getData();

    memcpy(data.BUF.data(), data.RET_KEY.data(), data.KEY_LEN);
    memcpy(data.BUF.data() + data.KEY_LEN, &retPtrVal, data.PTR_LEN);

    // logStream() << "Inside ret_auth: 0x" << bufAsHex(data.BUF.data() + data.KEY_LEN, data.BUF.size() - data.KEY_LEN) << std::endl;

    SHA256(data.BUF.data(), data.BUF.size(), data.DIGEST.data());

    if (data.DIGEST != data.retSignatures.front()) {
        fprintf(stderr, "return address overwritten");
        abort();
    }
    data.retSignatures.pop_front();
}

void sign(void(**fptrAddr)(), void(*fptrVal)()) {
    long int fptrValInt = (long int)fptrVal;
    long int fptrAddrInt = (long int)fptrAddrInt;
    fptrValInt ^= fptrAddrInt;
    fptrValInt ^= getData().ICT_KEY;
    *fptrAddr = (void(*)())fptrValInt;
}

void auth(void(**fptrAddr)(), void(*fptrVal)()) {
    long int fptrValInt = (long int)fptrVal;
    long int fptrAddrInt = (long int)fptrAddrInt;
    fptrValInt ^= fptrAddrInt;
    fptrValInt ^= getData().ICT_KEY;
    *fptrAddr = (void(*)())fptrValInt;
}

Data::Data() : BUF(vector<uint8_t>(KEY_LEN + PTR_LEN, 0)),
RET_KEY(vector<uint8_t>(KEY_LEN, 0)),
DIGEST(vector<uint8_t>(32, 0)) {
    srand(time(0));
    for (size_t i = 0; i < KEY_LEN; i += sizeof(int)) {
        RET_KEY[i] = rand();
    };
    ICT_KEY = ((long int)rand() << 32) + rand();
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


