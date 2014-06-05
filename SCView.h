/* 
 * File:   SCView.h
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#ifndef SCVIEW_H
#define	SCVIEW_H

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
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

class SCView : public wxFrame {
public:
    SCView(const wxString& title = _("SteganoCoder"),
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxSize(800, 600))
    : wxFrame(NULL, wxID_ANY, title, pos, size) {};
    virtual ~SCView();
    void create();
    void doLayout();
    wxStatusBar* getStatusBar() {
        return this->statusBar;
    }
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
    wxStatusBar* statusBar;
    wxStaticBoxSizer* unmodImgSizer;
    wxStaticBoxSizer* secretMsgSizer;
    wxStaticBoxSizer* modImgSizer;
    wxStaticBoxSizer* bitPatternSizer;
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
};

// Die IDs für die Events
enum EVENT_ID {
    ID_LOAD_UNMOD_IMG = 1,
    ID_LOAD_MOD_IMG = 2,
    ID_SAVE_MOD_IMG = 3,
    ID_ENCODE = 4,
    ID_DECODE = 5,
    ID_SECRET_MSG = 6
};

#endif	/* SCVIEW_H */

