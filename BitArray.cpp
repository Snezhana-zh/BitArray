#include "BitArray.h"

const char* Exception::what() const noexcept {
    return errorMessage.c_str();
}

const char* Exception::where() const noexcept {
    return functionName.c_str();
}

BitArray::BitArray() : sizeInBits(0), alignedSize(0), data(new std::vector<Type>(0)) {}
BitArray::~BitArray() {
    delete data;
}

BitArray::BitArray (int num_bits, Type value) : sizeInBits(num_bits),
    alignedSize(align + num_bits - 1 / align), data(new std::vector<Type>(alignedSize)) {
    if (num_bits < 0) {
        throw Exception("Negative size of arrray", __func__);
    }
    data->at(0) = value;
}

BitArray::BitArray(const BitArray& b) : sizeInBits(b.sizeInBits), alignedSize(b.alignedSize), data(new std::vector<Type>(b.alignedSize)) {
    for (size_t i = 0; i < b.alignedSize; ++i) {
        data->at(i) = b.data->at(i);
    }
}

BitArray::ConstBit::ConstBit(const BitArray& bitArray, int i) {
    mask = 1L << (i % align);
    part = (bitArray.data->at(i / align));
}

BitArray::ConstBit::operator bool() const {
    return part & mask;
}

bool BitArray::operator[](int i) const {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    if (sizeInBits <= i || i < 0) {
        throw Exception("Out of index", __func__);
    }
    ConstBit bit(*this, i);
    return bit;
}

BitArray::Bit::Bit(const BitArray& bitArray, int i) {
    mask = 1L << (i % align);
    part = &(bitArray.data->at(i / align));
    inversionMask = ~mask;
}

BitArray::Bit::operator bool() {
    return *part & mask;
}


BitArray::Bit BitArray::Bit::operator=(bool value) {
    *part = value ? *part | mask : *part & inversionMask;
    return *this;
}

BitArray::Bit BitArray::operator[](int i) {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    if (sizeInBits <= i || i < 0) {
        throw Exception("Out of index", __func__);
    }
    return { *this, i };
}

void BitArray::swap(BitArray& b) {
    if (b.alignedSize != alignedSize) {
        throw Exception("Not equal size of arrays", __func__);
    }
    swap<size_t>(b.alignedSize, alignedSize);
    swap<size_t>(sizeInBits, b.sizeInBits);
    for (size_t i = 0; i < b.alignedSize; i++) {
        swap<Type>(b.data->at(i), data->at(i));
    }
}

template <typename T>
void BitArray::swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

BitArray& BitArray::operator=(const BitArray& b) {
    alignedSize = b.alignedSize;
    sizeInBits = b.sizeInBits;
    delete data;
    data = new std::vector<Type>(b.alignedSize);
    for (size_t i = 0; i < b.alignedSize; ++i) {
        data->at(i) = b.data->at(i);
    }
    return (*this);
}

void BitArray::resize(int num_bits, bool value) {
    if (sizeInBits == num_bits) {
        return;
    }
    if (num_bits < 0) {
        throw Exception("Negative size of array", __func__);
    }
    alignedSize = align + num_bits - 1 / align;
    data->resize(alignedSize);
    size_t oldSize = sizeInBits;
    sizeInBits = num_bits;
    if (oldSize < num_bits) {
        for (size_t i = oldSize; i < num_bits; ++i) {
            (*this)[i] = value;
        }
    }
}

void BitArray::clear() {
    data->clear();
    sizeInBits = 0;
    alignedSize = 0;
}

void BitArray::push_back(bool bit) {
    if ((sizeInBits % align == 0) || sizeInBits == 0) {
        resize(sizeInBits + 1, 0);
        (*this)[sizeInBits - 1] = bit;
    }
    else {
        sizeInBits++;
        (*this)[sizeInBits - 1] = bit;
    }
}

BitArray& BitArray::operator&=(const BitArray& b) {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    if (alignedSize != b.alignedSize) {
        throw Exception("Not equal size of arrays", __func__);
    }
    for (size_t i = 0; i < sizeInBits; ++i) {
        (*this)[i] = (*this)[i] & b[i];
    }
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& b) {
    if (sizeInBits <= 0) {
        throw Exception("Empty array", __func__);
    }
    if (sizeInBits != b.sizeInBits) {
        throw Exception("Not equal size of arrays", __func__);
    }
    for (size_t i = 0; i < sizeInBits; ++i) {
        (*this)[i] = (*this)[i] | b[i];
    }
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b) {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    if (alignedSize != b.alignedSize) {
        throw Exception("Not equal size of arrays", __func__);
    }
    for (size_t i = 0; i < sizeInBits; ++i) {
        (*this)[i] = (*this)[i] ^ b[i];
    }
    return *this;
}

BitArray& BitArray::operator<<=(int n) {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    BitArray tmp = (*this);
    for (size_t i = n; i < sizeInBits; ++i) {
        bool el = tmp[i];
        (*this)[i - n] = el;
    }
    for (size_t i = sizeInBits - n; i < sizeInBits; ++i) {
        (*this)[i] = false;
    }
    return *this;
}

BitArray& BitArray::operator>>=(int n) {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    BitArray tmp = (*this);
    for (size_t i = n; i < sizeInBits; ++i) {
        bool el = tmp[i - n];
        (*this)[i] = el;
    }
    for (size_t i = 0; i < n; ++i) {
        (*this)[i] = false;
    }
    return *this;
}

BitArray BitArray::operator<<(int n) const {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    BitArray result = (*this);
    result <<= n;
    return result;
}

BitArray BitArray::operator>>(int n) const {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    BitArray result = (*this);
    result >>= n;
    return result;
}

BitArray& BitArray::set(int n, bool val) {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    (*this)[n] = val;
    return *this;
}

BitArray& BitArray::set() {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    for (size_t i = 0; i < alignedSize; ++i) {
        data->at(i) = ~0L;
    }
    return *this;
}

BitArray& BitArray::reset(int n) {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    (*this)[n] = false;
    return *this;
}


BitArray& BitArray::reset() {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    for (size_t i = 0; i < alignedSize; ++i) {
        data->at(i) = 0L;
    }
    return *this;
}


bool BitArray::any() const {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    for (size_t i = 0; i < alignedSize; i++) {
        if (data->at(i)) {
            return true;
        }
    }
    return false;
}

bool BitArray::none() const {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    for (size_t i = 0; i < alignedSize; i++) {
        if (data->at(i)) {
            return false;
        }
    }
    return true;
}

BitArray BitArray::operator~() const {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    BitArray result = (*this);
    for (size_t i = 0; i < sizeInBits; ++i) {
        result[i] = (*this)[i] ? false : true;
    }
    return result;
}

int BitArray::count() const {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    int count = 0;
    for (size_t i = 0; i < sizeInBits; ++i) {
        if ((*this)[i]) {
            count++;
        }
    }
    return count;
}

int BitArray::size() const {
    return sizeInBits;
}

bool BitArray::empty() const {
    return sizeInBits == 0;
}

std::string BitArray::to_string() const {
    if (alignedSize <= 0) {
        throw Exception("Empty array", __func__);
    }
    std::string strArray("");
    for (size_t i = 0; i < sizeInBits; ++i) {
        if ((*this)[i]) strArray += '1';
        else strArray += '0';
    }
    return strArray;
}

bool operator==(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) {
        return true;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] == b[i]) {
            return false;
        }
    }
    return true;
}

BitArray operator&(const BitArray& b1, const BitArray& b2) {
    if (b1.size() <= 0 || b2.size() <= 0) {
        throw Exception("Empty array", __func__);
    }
    BitArray result(b1);
    result &= b2;
    return result;
}

BitArray operator|(const BitArray& b1, const BitArray& b2) {
    if (b1.size() <= 0 || b2.size() <= 0) {
        throw Exception("Empty array", __func__);
    }
    BitArray result(b1);
    result |= b2;
    return result;
}

BitArray operator^(const BitArray& b1, const BitArray& b2) {
    if (b1.size() <= 0 || b2.size() <= 0) {
        throw Exception("Empty array", __func__);
    }
    BitArray result(b1);
    result ^= b2;
    return result;
}

std::ostream& operator<<(std::ostream& out, const BitArray& b) {
    if (b.size() <= 0) {
        throw Exception("Empty array", __func__);
    }
    for (size_t i = 0; i < b.size(); ++i) {
        out << b[i];
    }
    return out;
}