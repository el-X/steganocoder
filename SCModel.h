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
    void setModCarrierBytes(unsigned char* modBytes);
    unsigned char* getUnmodCarrierBytes() const;
    void setUnmodCarrierBytes(unsigned char* unmodBytes);
private:
    std::string charToByte(const unsigned char& c) const;
    unsigned char byteToChar(const std::string& bits) const;
    bool checkForHeaderSignature() const;
    std::string createHeader(const std::string& msg);
    const std::string SGN = "SGN";
    unsigned char* modCarrierBytes;
    unsigned char* unmodCarrierBytes;
};

#endif	/* SCMODEL_H */

