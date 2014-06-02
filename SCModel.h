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
    char* getModCarrierBytes() const {
        return modCarrierBytes;
    };
    void setModCarrierBytes(char* modBytes) {
        modCarrierBytes = modBytes;
    };
    char* getUnmodCarrierBytes() const {
        return unmodCarrierBytes;
    };
    char* setUnmodCarrierBytes(char* unmodBytes) {
        unmodCarrierBytes = unmodBytes;
    };
    bool encode(const std::string& str);
    std::string decode();
private:
    std::string charToBits(const unsigned char& c) const;
    unsigned char bitsToChar(const std::string& bits) const;
    bool checkForHeaderSignature() const;
    std::string createHeader(const std::string& msg);
    std::string replaceASCII(std::string& str);
    const std::string SGN;
    char* modCarrierBytes;
    char* unmodCarrierBytes;
};

#endif	/* SCMODEL_H */

