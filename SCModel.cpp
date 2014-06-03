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

void SCModel::encode(const string& msg) {
    string encoded_msg = createHeader(msg) + msg;
    unsigned int msg_size = msg.size();
    unsigned int encoded_msg_size = encoded_msg.size();
    char* binary = new char[encoded_msg_size * 8];
    
    unsigned int byte_counter = 0;
    
    for(unsigned int i = 0; i < encoded_msg_size; i++) {
        string byte = charToBits(encoded_msg[i]);
        //Bits setzen
        for(unsigned int j = 0; j < 8; j++) {
            binary[byte_counter+j] = byte[j];
        }
        byte_counter += 8;
    }
    modCarrierBytes = new char[encoded_msg_size * 8];
    for(unsigned int i=0; i < encoded_msg_size * 8; i++) {
        modCarrierBytes[i] = unmodCarrierBytes[i] & 254;
        if(binary[i] == '1') {
            modCarrierBytes[i] = unmodCarrierBytes[i] + 1;
        }
    }
}

unsigned int SCModel::getHeaderSize() {
    return (SGN.size() + 4);
}

string SCModel::decode() {
    string decoded_msg("");
    
    unsigned int sgn_size = SGN.size();
    string msg_binary_size("");
    for(unsigned int i=0; i < 4; i++) {
        msg_binary_size += bitset<8>(modCarrierBytes[sgn_size+i]).to_string();
    }
    unsigned int msg_size = bitset<32>(msg_binary_size).to_ulong();
    
    char* binary = new char[msg_size * 8];
    
    unsigned int byte_counter = 0;
    
    for(unsigned int i=getHeaderSize()*8; i < (msg_size+getHeaderSize())*8; i++) {
        binary[byte_counter] = modCarrierBytes[i] & 1;
        byte_counter += 1;
    }
    
    for(unsigned int i=0; i < strlen(binary); i++) {
        decoded_msg += binary[i];
    }
    
    return decoded_msg;
}

string SCModel::replaceNonASCII(std::string& str) {
    return regex_replace(str, regex("[^\u0000-\u007F]"), string(""));
}

bool SCModel::checkForHeaderSignature() const {

    //Prüfe einzelne SGN Zeichen mit den ersten Zeichen der Bitmap
    for(int i=0; i < SGN.length(); i++) {
        if(SGN[i] != modCarrierBytes[i]) 
            return false;
    }
    
    return true;
}

string SCModel::getModBitPattern() {
    string result("");
    for(unsigned int i=0; i < strlen(modCarrierBytes); i++) {
        result += charToBits(modCarrierBytes[i]);
    }
    return result;
}

string SCModel::createHeader(const std::string& msg) {
    string strMsgLength = "";
    unsigned int msgSize = msg.size();
    bitset<32> bits(msgSize);
    string byte("");
    for(unsigned int i=1; i < 5; i++) {
        byte = "";
        for(unsigned int j=1; j < 9; j++) {
            byte += bits[(i*j)-1];
        }
        strMsgLength += bitsToChar(byte);
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
    std::bitset<8> bitsArray(bits);
    unsigned int asciiPos = bitsArray.to_ulong();
    return static_cast<unsigned char> (asciiPos);
};