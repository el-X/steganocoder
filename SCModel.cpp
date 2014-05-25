/* 
 * File:   SCModel.cpp
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#include "SCModel.h"
#include <assert.h>
#include <string>
#include <bitset>
#include <cstdlib>

SCModel::SCModel() {
}

SCModel::~SCModel() {
}

/*
 * Returns an bit representation of an unsigned char as string.
 * Example: 'A' is "01000001"
 */
std::string SCModel::charToBits(const unsigned char& c) const {
    assert(c);
    std::string bits = "";
    for (int i = 7; i >= 0; i--) {
        bits.append((c & (1 << i)) ? "1" : "0");
    }
    return bits;
};

/*
 * Returns an unsigned char of given bit representation for an unsigned char.
 * Example: "01000001" is 'A'
 */
unsigned char SCModel::bitsToChar(const std::string& bits) const {
    assert(bits.size() == 8);
    unsigned char c = '$';
    std::bitset<8> bitsArray(bits);
    unsigned int asciiPos = bitsArray.to_ulong();
    if (asciiPos <= CHAR_MAX) {
        c = static_cast<unsigned char> (asciiPos);
    }
    return c;
};