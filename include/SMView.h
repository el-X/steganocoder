/* 
 * File:   SMView.h
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#ifndef SMVIEW_H
#define	SMVIEW_H

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/colour.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/menu.h>

#include "splashscreen.h"
#include "SMStatusBar.h"
#include "SMAboutDialog.h"

class SMView : public wxFrame {
public:
    // Die IDs für die Events
    enum EVENT_ID {
        ID_LOAD_UNMOD_IMG = 1,
        ID_LOAD_MOD_IMG = 2,
        ID_SAVE_MOD_IMG = 3,
        ID_ENCODE = 4,
        ID_DECODE = 5,
        ID_SECRET_MSG = 6
    };
    SMView(const wxString& title = wxEmptyString,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxSize(800, 600))
    : wxFrame(NULL, wxID_ANY, title, pos, size) {};
    virtual ~SMView() {};
    void create();
    void doLayout();
    void showSplashScreen();
    void setStatusBarText(const wxString& text); 
    void setStatusBarErrorText(const wxString& text); 
    wxStaticBitmap* getUnmodStaticBitmap() {
        return this->unmodStaticBitmap;
    }
    wxStaticBitmap* getModStaticBitmap() {
        return this->modStaticBitmap;
    }
    wxButton* getLoadUnmodImgBtn() {
        return this->loadUnmodImgBtn;
    }
    wxButton* getLoadModImgBtn() {
        return this->loadModImgBtn;
    }
    wxButton* getSaveModImgBtn() {
        return this->saveModImgBtn;
    }
    wxButton* getEncodeBtn() {
        return this->encodeBtn;
    }
    wxButton* getDecodeBtn() {
        return this->decodeBtn;
    }
    wxTextCtrl* getMaxTxtLengthOutput() {
        return this->maxTxtLengthOutput;
    }
    wxTextCtrl* getTxtLengthOutput() {
        return this->txtLengthOutput;
    } 
    wxTextCtrl* getSecretMsgInput() {
        return this->secretMsgInput;
    }
    wxTextCtrl* getBitpatternOutput() {
        return this->bitPatternOutput;
    }
    wxMenuItem* getSaveModImgMenuItem() {
        return this->saveModImgMenuItem;
    }
    wxMenuItem* getEncodeMenuItem() {
        return this->encodeMenuItem;
    }
    wxMenuItem* getDecodeMenuItem() {
        return this->decodeMenuItem;
    }
    SMAboutDialog* getAboutDialog() {
        return this->aboutDialog;
    }
    
protected:
    wxMenuItem *saveModImgMenuItem;
    wxMenuItem *encodeMenuItem;
    wxMenuItem *decodeMenuItem;
    wxPanel* mainPanel;
    wxScrolledWindow* unmodImgScrolledWindow;
    wxStaticBitmap* unmodStaticBitmap;
    wxButton* loadUnmodImgBtn;
    wxStaticLine* unmodImgSeparator;
    wxStaticText* maxTxtLengthLabel;
    wxTextCtrl* maxTxtLengthOutput;
    wxTextCtrl* secretMsgInput;
    wxStaticText* txtLengthLabel;
    wxTextCtrl* txtLengthOutput;
    wxButton* encodeBtn;
    wxButton* decodeBtn;
    wxScrolledWindow* modImgScrolledWindow;
    wxStaticBitmap* modStaticBitmap;
    wxButton* loadModImgBtn;
    wxStaticLine* modImgSeparator;
    wxButton* saveModImgBtn;
    wxTextCtrl* bitPatternOutput;
    SMStatusBar* statusBar;
    wxStaticBoxSizer* unmodImgSizer;
    wxStaticBoxSizer* secretMsgSizer;
    wxStaticBoxSizer* modImgSizer;
    wxStaticBoxSizer* bitPatternSizer;
    SMAboutDialog* aboutDialog;
    wxString oldStatusText;
    wxString lastHelpShown;
    const wxString TEXT_TITLE = _("SteganoMonkey");
    const wxString TEXT_WELCOME = _("Welcome to " + TEXT_TITLE + "!");
    const wxString MENU_FILE = _("&File");
    const wxString MENU_EDIT = _("&Edit");
    const wxString MENU_HELP = _("&Help");
    const wxString MENUITEM_LOAD_UNMOD_IMG_TEXT = _("&Load Unmodified Image...\tCtrl-U");
    const wxString MENUITEM_LOAD_UNMOD_IMG_HELP = _("Load an image that you want to encode");
    const wxString MENUITEM_LOAD_MOD_IMG_TEXT = _("&Load Modified Image...\tCtrl-M");
    const wxString MENUITEM_LOAD_MOD_IMG_HELP = _("Load an image that contains a secret message");
    const wxString MENUITEM_SAVE_MOD_IMG_TEXT = _("&Save Modified Image...\tCtrl-S");
    const wxString MENUITEM_SAVE_MOD_IMG_HELP = _("Save an image that contains a secret message");
    const wxString MENUITEM_ENCODE_TEXT = _("&Encode\tCtrl-E");
    const wxString MENUITEM_ENCODE_HELP = _("Encode the unmodified image with the secret message");
    const wxString MENUITEM_DECODE_TEXT = _("&Decode\tCtrl-D");
    const wxString MENUITEM_DECODE_HELP = _("Decode the modified image containing a secret message");
    const wxString BTN_LOAD_IMG = _("Load Image...");
    const wxString BTN_SAVE_IMG = _("Save Image...");
    const wxString BTN_ENCODE = _("Encode");
    const wxString BTN_DECODE = _("Decode");
    const wxString TEXT_MAX_TXT_LENGTH = _("Max Text Length:");
    const wxString TEXT_TXT_LENGTH = _("Text Length:");
    const wxString TEXT_UNMOD_IMG = _("Unmodified Image");
    const wxString TEXT_MOD_IMG = _("Modified Image");
    const wxString TEXT_SECRET_MSG = _("Secret Message");
    const wxString TEXT_BIT_PATERN = _("Bit Pattern");
    void createMenuBar();
    void createUpperLeftBox();
    void createLowerLeftBox();
    void createMiddleBox();
    void createUpperRightBox();
    void createLowerRightBox();
    void layoutUpperLeftBox();
    void layoutLowerLeftBox();
    void layoutUpperRightBox();
    void layoutLowerRightBox();
    virtual void DoGiveHelp(const wxString& help, bool show);
};

#endif	/* SMVIEW_H */

