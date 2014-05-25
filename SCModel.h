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
private:
    std::string charToBits(const unsigned char& c) const;
    unsigned char bitsToChar(const std::string& bits) const;
};

#endif	/* SCMODEL_H */

