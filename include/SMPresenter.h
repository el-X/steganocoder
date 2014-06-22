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
    void initGUIElements();
    void initImageHandlers();
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
    wxDECLARE_EVENT_TABLE();
    const wxString IMG_IN_FORMATS = _("Image (*.bmp;*.jpg;*.jpeg;*.png;*.tga;*.ico)|*.bmp;*.jpg;*.jpeg;*.png;*.tga;*.ico");
    const wxString IMG_OUT_FORMATS = _("Bitmap (*.bmp)|*.bmp");
    const wxString IMG_OUT_NAME = _("top_secret.bmp");
    const wxString MSG_WELCOME = _("Welcome to SteganoMonkey!");
    const wxString MSG_COR_IMG = _("Loaded image is corrupt!");
    const wxString MSG_RDY_ENC = _("Ready to encode...");
    const wxString MSG_RDY_DEC = _("Ready to decode...");
    const wxString MSG_ENC_SUCC = _("Encode successful!");
    const wxString MSG_DEC_SUCC = _("Decode successful!");
    const wxString MSG_NO_HDN_INF = _("Loaded picture contains no hidden message!");
    const wxString MSG_TOO_LONG = _("The provided message is too long for the given image!");
    const wxString MSG_WAIT_INPUT = _("Waiting for message input...");
    const wxString MSG_SAVE_PATH = _("Encoded image saved under: ");
    const wxString TXT_SAVE_IMG = _("Save Image");
    const wxString TXT_LOAD_IMG = _("Load Image");
};

DECLARE_APP(SMPresenter)

#endif	/* SMPRESENTER_H */

