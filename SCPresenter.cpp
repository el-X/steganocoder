/* 
 * File:   SCPresenter.cpp
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/intl.h>
#include <wx/string.h>
#include "SCPresenter.h"

#include <iostream>

IMPLEMENT_APP(SCPresenter)

/**
 * Main of steganocoder. 
 */
bool SCPresenter::OnInit() {
    std::cout << "Hello" << std::endl;
    //Frame bla
    return true;
}

