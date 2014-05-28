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
    char* getModCarrierBytes() const;
    void setModCarrierBytes(char* modBytes);
    char* getUnmodCarrierBytes() const;
    char* setUnmodCarrierBytes(char* unmodBytes);
    bool encode();
    bool decode();
private:
    std::string charToBits(const unsigned char& c) const;
    unsigned char bitsToChar(const std::string& bits) const;
    const std::string SGN;
    char* modCarrierBytes;
    char* unmodCarrierBytes;
};

#endif	/* SCMODEL_H */

