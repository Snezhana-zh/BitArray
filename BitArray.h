#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <exception>

class BitArray {
using Type = unsigned long;
static const std::size_t align = sizeof(Type) * 8;
public:
    BitArray();
    ~BitArray();

    explicit BitArray(int num_bits, Type value = 0);

    BitArray(const BitArray& b);

    struct ConstBit {
        ConstBit(const BitArray& bitArray, int i);

        operator bool() const;

        long mask;
        Type part;
    };

    bool operator[](int i) const;

    struct Bit {
        Bit(const BitArray& bitArray, int i);

        operator bool();

        Bit operator=(bool value);

        long mask;
        Type* part;
        long inversionMask;
    };

    Bit operator[](int i);

    void swap(BitArray& b);

    template <typename T>
    void swap(T& a, T& b);

    BitArray& operator=(const BitArray& b);

    void resize(int num_bits, bool value = false);

    void clear();

    void push_back(bool bit);

    BitArray& operator&=(const BitArray& b);

    BitArray& operator|=(const BitArray& b);

    BitArray& operator^=(const BitArray& b);

    BitArray& operator<<=(int n);

    BitArray& operator>>=(int n);

    BitArray operator<<(int n) const;

    BitArray operator>>(int n) const;

    BitArray& set(int n, bool val = true);

    BitArray& set();

    BitArray& reset(int n);

    BitArray& reset();

    bool any() const;

    bool none() const;

    BitArray operator~() const;
   
    int count() const;

    int size() const;

    bool empty() const;

    std::string to_string() const;

private:
    std::size_t sizeInBits;
    std::size_t alignedSize;
    std::vector<Type>* data;
};

bool operator==(const BitArray& a, const BitArray& b);

bool operator!=(const BitArray& a, const BitArray& b);

BitArray operator&(const BitArray& b1, const BitArray& b2);

BitArray operator|(const BitArray& b1, const BitArray& b2);

BitArray operator^(const BitArray& b1, const BitArray& b2);

std::ostream& operator<<(std::ostream& out, const BitArray& b);

class Exception : public std::exception {
public:
    Exception(const std::string& message, const std::string& function)
        : errorMessage(message), functionName(function) {}

    const char* what() const noexcept override;

    const char* where() const noexcept;

private:
    std::string errorMessage;
    std::string functionName;
};