/* 
 * File:   SCView.h
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#ifndef SCVIEW_H
#define	SCVIEW_H

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
#include "SCStatusBar.h"
#include "SCAboutDialog.h"

class SCView : public wxFrame {
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
    SCView(const wxString& title = wxEmptyString,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxSize(800, 600))
    : wxFrame(NULL, wxID_ANY, title, pos, size) {};
    virtual ~SCView() {};
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
    SCAboutDialog* getAboutDialog() {
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
    SCStatusBar* statusBar;
    wxStaticBoxSizer* unmodImgSizer;
    wxStaticBoxSizer* secretMsgSizer;
    wxStaticBoxSizer* modImgSizer;
    wxStaticBoxSizer* bitPatternSizer;
    SCAboutDialog* aboutDialog;
    wxString oldStatusText;
    wxString lastHelpShown;
    const std::string TEXT_TITLE = "SteganoMonkey";
    const std::string TEXT_WELCOME = "Welcome to " + TEXT_TITLE + "!";
    const std::string MENU_FILE = "&File";
    const std::string MENU_EDIT = "&Edit";
    const std::string MENU_HELP = "&Help";
    const std::string MENUITEM_LOAD_UNMOD_IMG_TEXT = "&Load Unmodified Image...\tCtrl-U";
    const std::string MENUITEM_LOAD_UNMOD_IMG_HELP = "Load an image that you want to encode";
    const std::string MENUITEM_LOAD_MOD_IMG_TEXT = "&Load Modified Image...\tCtrl-M";
    const std::string MENUITEM_LOAD_MOD_IMG_HELP = "Load an image that contains a secret message";
    const std::string MENUITEM_SAVE_MOD_IMG_TEXT = "&Save Modified Image...\tCtrl-S";
    const std::string MENUITEM_SAVE_MOD_IMG_HELP = "Save an image that contains a secret message";
    const std::string MENUITEM_ENCODE_TEXT = "&Encode\tCtrl-E";
    const std::string MENUITEM_ENCODE_HELP = "Encode the unmodified image with the secret message";
    const std::string MENUITEM_DECODE_TEXT = "&Decode\tCtrl-D";
    const std::string MENUITEM_DECODE_HELP = "Decode the modified image containing a secret message";
    const std::string BTN_LOAD_IMG = "Load Image...";
    const std::string BTN_SAVE_IMG = "Save Image...";
    const std::string BTN_ENCODE = "Encode";
    const std::string BTN_DECODE = "Decode";
    const std::string TEXT_MAX_TXT_LENGTH = "Max Text Length:";
    const std::string TEXT_TXT_LENGTH = "Text Length:";
    const std::string TEXT_UNMOD_IMG = "Unmodified Image";
    const std::string TEXT_MOD_IMG = "Modified Image";
    const std::string TEXT_SECRET_MSG = "Secret Message";
    const std::string TEXT_BIT_PATERN = "Bit Pattern";
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

#endif	/* SCVIEW_H */

