/* 
 * File:   SCView.h
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#ifndef SCVIEW_H
#define	SCVIEW_H

#include <wx/frame.h>

class SCView {
public:
    SCView();
    virtual ~SCView();
    wxFrame* getFrame();
private:
    wxFrame* frame;
};

#endif	/* SCVIEW_H */

