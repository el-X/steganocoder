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
    SCView* view;
    SCModel* model;
    void init();
    bool onOpen(wxCommandEvent& event);
    bool onClose(wxCommandEvent& event);
    bool onExit(wxCommandEvent& event);
    bool onLoad(wxCommandEvent& event);
    bool onSave(wxCommandEvent& event);
    bool onEncode(wxCommandEvent& event);
    bool onDecode(wxCommandEvent& event);
    bool onSecretMessageChange(wxCommandEvent& event);
    std::string getWXMOTIF();
};

DECLARE_APP(SCPresenter)

#endif	/* SCPRESENTER_H */

