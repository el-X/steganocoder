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
#include <iostream>

using namespace std;

SCModel::SCModel() {
}

SCModel::~SCModel() {
}

void SCModel::encode(const string& msg) {
    assert(unmodCarrierBytes);
    
    cout << " Encoding... " << endl;
    string encoded_msg = createHeader(msg) + msg;
    cout << "EncMsg: " << encoded_msg << endl;
    unsigned int msg_size = msg.size();
    unsigned int encoded_msg_size = encoded_msg.size();
    char* binary = new char[encoded_msg_size * 8];
    cout << " MsgSize: " << msg_size << " EncMsgSize: " << encoded_msg_size << endl;
    unsigned int byte_counter = 0;

    for (unsigned int i = 0; i < encoded_msg_size; i++) {
        string byte = charToByte(encoded_msg[i]);
        //Bits setzen
        for (unsigned int j = 0; j < 8; j++) {
            binary[byte_counter + j] = byte[j];
        }
        byte_counter += 8;
    }
    cout << " Binary: \n " << binary << endl;
    modCarrierBytes = new unsigned char[encoded_msg_size * 8];
    for (unsigned int i = 0; i < encoded_msg_size * 8; i++) {
        modCarrierBytes[i] = unmodCarrierBytes[i] & 254;
        if (binary[i] == '1') {
            modCarrierBytes[i] = unmodCarrierBytes[i] + 1;
        }
    }
    cout << " Encoding finished " << endl;
}

unsigned int SCModel::getHeaderSize() {
    return (SGN.size() + 4);
}

string SCModel::decode() {
    assert(modCarrierBytes);
    
    string decoded_msg("");
    unsigned int sgn_size = SGN.size();
    string msg_binary_size("");

    for (unsigned int i = 0; i < 4; i++) {
        msg_binary_size += bitset<8>(modCarrierBytes[sgn_size + i]).to_string();
    }
    unsigned int msg_size = bitset<32>(msg_binary_size).to_ulong();
    char* binary = new char[msg_size * 8];
    unsigned int byte_counter = 0;

    for (unsigned int i = getHeaderSize()*8; i < (msg_size + getHeaderSize())*8; i++) {
        binary[byte_counter] = modCarrierBytes[i] & 1;
        byte_counter += 1;
    }
    for (unsigned int i = 0; i < sizeof (binary) / sizeof (binary[0]); i++) {
        decoded_msg += binary[i];
    }
    return decoded_msg;
}

string SCModel::replaceNonASCII(std::string& str) {
    return regex_replace(str, regex("[^\u0000-\u007F]"), string(""));
}

bool SCModel::checkForHeaderSignature() const {
    assert(modCarrierBytes);
    //Prüfe einzelne SGN Zeichen mit den ersten Zeichen der Bitmap
    for (size_t i = 0; i < SGN.length(); i++) {
        if (SGN[i] != modCarrierBytes[i])
            return false;
    }
    return true;
}

string SCModel::getModBitPattern() {
    assert(modCarrierBytes);
    string result("");
    for (size_t i = 0; i < sizeof (modCarrierBytes) / sizeof (modCarrierBytes[0]); i++) {
        result += charToByte(modCarrierBytes[i]) + " ";
    }
    return result;
}

string SCModel::createHeader(const std::string& msg) {
    cout << " Create Header..." << endl;
    string header(SGN); // Header startet mit Signatur.
    unsigned int msgSize = msg.size();
    bitset<32> bits(msgSize);
    string strBits = bits.to_string();
    cout << " MsgSize in Bits: " << strBits << endl;
    string byte("");
    // Länge der Nachricht in den Header reinschreiben (in Bitdarstellung)
    for (unsigned int i = 0; i < 4; i++) {
        byte = "";
        //        for(unsigned int j=1; j < 9; j++) {
        //            byte += bits[(i*j)-1];
        //        }
        //FIXME: Länge der Nachricht wird nicht richtig als String gebildet
        //       string enhält nur Leerzeichen??? oder ist nur so dargestellt?
        byte = strBits.substr(i * 8, 8);
        cout << " " << i << " byte: " << byte << endl;
        header += byteToChar(byte);
    }
    cout << " Header is: " << header << endl;
    return header;
}

/*
 * Returns an bit representation of an unsigned char as string.
 * Example: 'A' is "01000001"
 */
string SCModel::charToByte(const unsigned char& c) const {
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
unsigned char SCModel::byteToChar(const std::string& bits) const {
    assert(bits.size() == 8);
    std::bitset<8> bitsArray(bits);
    unsigned int asciiPos = bitsArray.to_ulong();
    return static_cast<unsigned char> (asciiPos);
};

unsigned char* SCModel::getModCarrierBytes() const {
    return modCarrierBytes;
};

void SCModel::setModCarrierBytes(unsigned char* modBytes) {
    modCarrierBytes = modBytes;
};

unsigned char* SCModel::getUnmodCarrierBytes() const {
    return unmodCarrierBytes;
};

void SCModel::setUnmodCarrierBytes(unsigned char* unmodBytes) {
    unmodCarrierBytes = unmodBytes;
};