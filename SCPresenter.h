/* 
 * File:   SCPresenter.h
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#ifndef SCPRESENTER_H
#define	SCPRESENTER_H

#include "SCView.h"
#include "SCModel.h"

class SCPresenter : public wxApp {
public:
    virtual bool OnInit();
private:
    SCView view;
    SCModel model;
};

DECLARE_APP(SCPresenter)

#endif	/* SCPRESENTER_H */

