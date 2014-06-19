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
    wxDECLARE_EVENT_TABLE();
    const wxString IMG_IN_FORMATS = _T("Image (*.bmp;*.jpg;*.jpeg;*.png;*.gif;*.tiff;*.tga;*.ico)|*.bmp;*.jpg;*.jpeg;*.png;*.gif;*.tiff;*.tga;*.ico");
    const wxString IMG_OUT_FORMATS = _T("Bitmap (*.bmp)|*.bmp");
    const wxString IMG_OUT_NAME = _T("top_secret.bmp");
    const wxString MSG_WELCOME = _("Welcome to SteganoMonkey!");
    const wxString MSG_COR_IMG = _("Tried to load corrupted image!");
    const wxString MSG_RDY_ENC = _("Ready to encode!");
    const wxString MSG_RDY_DEC = _("Ready to decode!");
    const wxString MSG_NO_HDN_INF = _("Loaded picture has no hidden message!");
    const wxString MSG_TOO_LONG = _("Your message is too long for the given image!");
    const wxString MSG_WAIT_INPUT = _("Waiting for message.");
    const wxString MSG_SAVE_PATH = _("Image containing the secret message saved under: ");
    const wxString TXT_SAVE_IMG = _T("Save Image");
    const wxString TXT_LOAD_IMG = _T("Load Image");
};

DECLARE_APP(SMPresenter)

#endif	/* SMPRESENTER_H */

