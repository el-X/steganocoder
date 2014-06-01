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
    void onOpen(wxCommandEvent& event);
    void onClose(wxCommandEvent& event);
    void onLoad(wxCommandEvent& event);
    void onSave(wxCommandEvent& event);
    void onEncode(wxCommandEvent& event);
    void onDecode(wxCommandEvent& event);
    void onSecretMessageChange(wxCommandEvent& event);
    void onAbout(wxCommandEvent& event);
    void onExit(wxCommandEvent& event);
    std::string getWXMOTIF();
    wxDECLARE_EVENT_TABLE();
};

DECLARE_APP(SCPresenter)

#endif	/* SCPRESENTER_H */

