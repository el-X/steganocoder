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
#include "SCAboutDialog.h"

class SCPresenter : public wxApp {
public:
    virtual bool OnInit();
private:
    SCView* view;
    SCModel* model;
    void init();
    void initImageHandlers();
    void onOpen(wxCommandEvent& event);
    void onClose(wxCommandEvent& event);
    void onLoad(wxCommandEvent& event);
    void onSave(wxCommandEvent& event);
    void onEncode(wxCommandEvent& event);
    void onDecode(wxCommandEvent& event);
    void onSecretMessageChange(wxCommandEvent& event);
    void onAbout(wxCommandEvent& event);
    void onExit(wxCommandEvent& event);
    int getMaxTextLength() const;
    void setEncodingAllowed(bool allowed);
    void setDecodingAllowed(bool allowed);
    void setSaveAllowed(bool allowed);
    bool isUnmodBmpSet();
    bool isModBmpSet();
    const wxString welcomeMsg = _("Welcome to SteganoMonkey!");
    unsigned char* emptyCharArray = new unsigned char[0];
    wxDECLARE_EVENT_TABLE();
};

DECLARE_APP(SCPresenter)

#endif	/* SCPRESENTER_H */

