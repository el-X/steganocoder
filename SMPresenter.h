/* 
 * File:   SMPresenter.h
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#ifndef SMPRESENTER_H
#define	SMPRESENTER_H

#include "SMView.h"
#include "SMModel.h"
#include "SMAboutDialog.h"

class SMPresenter : public wxApp {
public:
    virtual bool OnInit();
private:
    SMView* view;
    SMModel* model;
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

DECLARE_APP(SMPresenter)

#endif	/* SMPRESENTER_H */

