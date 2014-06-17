/* 
 * File:   SCModel.h
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#ifndef SCMODEL_H
#define	SCMODEL_H

#include <string>

class SCModel {
public:

    SCModel();
    virtual ~SCModel();
    unsigned int getHeaderSize();
    std::string getModBitPattern();
    void encode(const std::string& msg);
    std::string decode();
    unsigned char* getModCarrierBytes() const;
    size_t getModCarrierBytesLength() const;
    void setModCarrierBytes(unsigned char* modBytes, size_t len);
    unsigned char* getUnmodCarrierBytes() const;
    size_t getUnmodCarrierBytesLength() const;
    void setUnmodCarrierBytes(unsigned char* unmodBytes, size_t len);
    bool checkForHeaderSignature() const;
private:
    std::string charToBits(const unsigned char& c) const;
    unsigned char bitsToChar(const std::string& bits) const;
    std::string createHeader(const std::string& msg);
    const std::string SGN = "SGN";
    unsigned char* modCarrierBytes;
    size_t modCarrierBytesLength = 0;
    unsigned char* unmodCarrierBytes;
    size_t unmodCarrierBytesLength = 0;
};

#endif	/* SCMODEL_H */

