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
#include <cstring>

using namespace std;

SCModel::SCModel() {
}

SCModel::~SCModel() {
}

void SCModel::encode(const string& msg) {
    cout << " Encoding... " << endl;
    string encoded_msg(createHeader(msg) + msg);
    cout << "EncMsg: " << encoded_msg << endl;
//    unsigned int msg_size = msg.size();
    unsigned int encoded_msg_size = encoded_msg.size();
    char* binary = new char[encoded_msg_size * 8];
    cout << " EncMsgSize: " << encoded_msg_size << endl;
    size_t byte_counter = 0;
    for (size_t i = 0; i < encoded_msg_size; i++) {
        string byte = charToBits(encoded_msg.at(i));
        //Bits setzen
        for (size_t j = 0; j < 8; j++) {
            binary[byte_counter + j] = byte.at(j);
        }
        byte_counter += 8;
    }
    cout << " Binary was set" << endl;
    // Bereite die Bitmap zur Modifikation
    modCarrierBytes = new unsigned char[unmodCarrierBytesLength];
    modCarrierBytesLength = unmodCarrierBytesLength;
    // Kopiere das unmodifiziert Bild Bit für Bit in das modifizierte Bild
    cout << "Kopiere Byte fuer Byte aus Unmod in Mod" << endl;
    for (size_t i = 0; i < unmodCarrierBytesLength; i++) {
        modCarrierBytes[i] = unmodCarrierBytes[i];
    }
    cout << "Kopie fertig" << endl;
    // Schreibe Daten in die Bitmap, indem die Bits beim modifizierten Bild
    // verändert werden
    for (size_t i = 0; i < encoded_msg_size * 8; i++) {
        modCarrierBytes[i] = modCarrierBytes[i] & 254;
        if (binary[i] == '1') {
            modCarrierBytes[i] = modCarrierBytes[i] | 00000001;
            cout << "Kodiere Zeichen " << i << endl;
        }
    }
    cout << " Encoding finished " << endl;
}

unsigned int SCModel::getHeaderSize() {
    return (SGN.size() + 4);
}

string SCModel::decode() {
    cout << " Start decode " << endl;
    string decoded_msg("");
    unsigned int sgn_size = SGN.size();
    string msg_binary_size("");
    
    size_t msgSizeOffset = sgn_size * 8;
    cout << " msgSizeOffset: " << msgSizeOffset << endl;
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 8; j++) {
            //modCarrierBytes im ersten Fall von 24 enthält nichts!! -> crash
            msg_binary_size += charToBits(modCarrierBytes[msgSizeOffset + i*8 + j]).at(7);
        }
        cout << " Bits msgSize " << msg_binary_size << endl;
    }
    unsigned int msg_size = bitset < 32 > (msg_binary_size).to_ulong();
    cout << " Msg size: " << msg_size << endl;
    char* binary = new char[msg_size * 8];
    size_t byte_counter = 0;

    cout << "Binary Message with Header" << getHeaderSize() << endl;
    for (size_t i = getHeaderSize()*8; i < (msg_size + getHeaderSize())*8; i++) {
        binary[byte_counter] = charToBits(modCarrierBytes[i]).at(7);
        byte_counter += 1;
        cout << binary[byte_counter];
    }
    cout << binary << endl;
    string decoded_msg_buffer("");
    byte_counter = 0;
    for (size_t i = 0; i < msg_size * 8; i++) {
        //Baue einzelnes Zeichen aus 8 Bits zusammen...
        decoded_msg_buffer += binary[i];
        byte_counter++;
        
        //und füge es dann der MSG hinzu
        if(byte_counter == 8) {
            decoded_msg += bitsToChar(decoded_msg_buffer);
            decoded_msg_buffer = "";
            byte_counter = 0;
        }
        
    }
    return decoded_msg;
}

bool SCModel::checkForHeaderSignature() const {
    //Prüfe einzelne SGN Zeichen mit den ersten Zeichen der Bitmap
    cout << "Check Header" << endl;
    for (size_t i = 0; i < SGN.length(); i++) {
        string modCarrierBytesAsBit("");
        size_t offset = i*8;
        for (size_t j = 0; j < 8; j++) {
            modCarrierBytesAsBit += charToBits(modCarrierBytes[offset + j]).at(7);
        }
        cout << modCarrierBytesAsBit << endl;
        cout << "SGN:" << SGN[i] << " Mod:" << bitsToChar(modCarrierBytesAsBit) << endl;
        if (SGN[i] != bitsToChar(modCarrierBytesAsBit)) {
            return false;
        }
    }
    return true;
}
// FIXME String building takes too much time!!!

string SCModel::getModBitPattern() {
    string result("");
    for (size_t i = 0; i < 100 && i < modCarrierBytesLength; i++) {
        result += charToBits(modCarrierBytes[i]) + " ";
    }
    return result;
}

string SCModel::createHeader(const std::string& msg) {
    cout << " Create Header..." << endl;
    string header(SGN); // Header startet mit Signatur.
    unsigned int msgSize = msg.size();
    bitset < 32 > bits(msgSize);
    string strBits = bits.to_string();
    cout << " MsgSize in Bits: " << strBits << endl;
    string byte("");
    // Länge der Nachricht in den Header reinschreiben (in Bitdarstellung)
    for (size_t i = 0; i < 4; i++) {
        byte = "";
        byte = strBits.substr(i * 8, 8);
        cout << " " << i << " byte: " << byte << endl;
        header += bitsToChar(byte);
    }
    cout << " Header is: " << header << " " << header.size() << endl;
    return header;
}

/*
 * Returns an bit representation of an unsigned char as string.
 * Example: 'A' is "01000001"
 */
string SCModel::charToBits(const unsigned char& c) const {
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
    std::bitset < 8 > bitsArray(bits);
    unsigned int asciiPos = bitsArray.to_ulong();
    return static_cast<unsigned char> (asciiPos);
};

unsigned char* SCModel::getModCarrierBytes() const {
    return modCarrierBytes;
};

void SCModel::setModCarrierBytes(unsigned char* modBytes, size_t len) {
    //modCarrierBytes = modBytes;
    modCarrierBytes = new unsigned char[len];
    modCarrierBytesLength = len;
    cout << "SetModCarrier" << endl;
    for(size_t i=0; i<len; i++) {
        modCarrierBytes[i] = modBytes[i];
    }
    cout << "End SetModCarrier" << endl;
};

unsigned char* SCModel::getUnmodCarrierBytes() const {
    return unmodCarrierBytes;
};

void SCModel::setUnmodCarrierBytes(unsigned char* unmodBytes, size_t len) {
    //unmodCarrierBytes = unmodBytes;
    unmodCarrierBytes = new unsigned char[len];
    unmodCarrierBytesLength = len;
    cout << "SetUnModCarrier" << endl;
    for(size_t i=0; i<len; i++) {
        unmodCarrierBytes[i] = unmodBytes[i];
    }
    cout << "End SetUnModCarrier" << endl;
};

void SCModel::setModCarrierBytesLength(size_t size) {
    modCarrierBytesLength = size;
}

void SCModel::setUnmodCarrierBytesLength(size_t size) {
    unmodCarrierBytesLength = size;
}

