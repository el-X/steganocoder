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
#include <limits.h>
#include <regex>
#include <cstring>

using namespace std;

SCModel::SCModel() {
}

SCModel::~SCModel() {
}

string SCModel::replaceASCII(std::string& str) {
    return regex_replace(str, regex("[^\u0000-\u007F]"), string(""));
}

bool SCModel::checkForHeaderSignature() const {

    //Prüfe einzelne SGN Zeichen mit den ersten Zeichen der Bitmap
    for(int i=0; i < SGN.length(); i++) {
        if(SGN[i] != modCarrierBytes[i]) return false;
    }
    
    return true;
}

string SCModel::createHeader(const std::string& msg) {
    string strMsgLength = "";
    if(strlen(msg.c_str()) < 4) {
        for (int i = 0; i < 4 - strlen(msg.c_str()); i++) {
            strMsgLength += "0";
        }
    }
    return SGN + strMsgLength;
}

/*
 * Returns an bit representation of an unsigned char as string.
 * Example: 'A' is "01000001"
 */
string SCModel::charToBits(const unsigned char& c) const {
    assert(c);
    string bits = "";
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