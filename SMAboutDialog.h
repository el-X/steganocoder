/* 
 * File:   SMAboutDialog.h
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#ifndef SMABOUTDIALOG_H
#define	SMABOUTDIALOG_H

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/string.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/dialog.h>
#include <wx/string.h>
#include <wx/hyperlink.h>

class SMAboutDialog : public wxDialog {
public:

    SMAboutDialog(wxWindow* parent, wxWindowID id = wxID_ANY,
            const wxString& title = wxEmptyString,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize);

    virtual ~SMAboutDialog() {
    };
protected:
    wxPanel* mainPanel;
    wxStaticBitmap* appImage;
    wxStaticLine* separator;
    wxStaticText* developerHeadingLabel;
    wxStaticText* developerLabel1;
    wxStaticText* developerLabel2;
    wxStaticBitmap* copyleftImage;
    wxStaticText* copyleftLabel;
    wxStaticText* iconLabel;
    wxHyperlinkCtrl* iconLink;
    wxStaticText* clipartLabel;
    wxHyperlinkCtrl* clipartLink;
    const wxFont& FONT_BOLD = wxFont(11, 74, 90, 92, false, _("Arial"));
    const std::string& TEXT_TITLE = "About SteganoMonkey";
    const std::string& TEXT_DEVELOPER_HEADING = "Developers:";
    const std::string& TEXT_DEVELOPER1 = "Thomas Matern,";
    const std::string& TEXT_DEVELOPER2 = "Robert Heimsoth, Alexander Keller";
    const std::string& TOOLTIP_COPYLEFT = "Copyleft";
    const std::string& TEXT_COPYLEFT = "2014 Hochschule Bremen, TI 6.2";
    const std::string& TEXT_CLIPART_BY = "Clipart by ";
    const std::string& TEXT_CLIPART = "Openclipart";
    const std::string& LINK_CLIPART = "https://openclipart.org/";
    const std::string& TEXT_ICON_BY = "Icons by ";
    const std::string& TEXT_ICON = "Tango";
    const std::string& LINK_ICON = "http://tango.freedesktop.org/Tango_Desktop_Project";
    void create();
    void doLayout();
};

#endif	/* SMABOUTDIALOG_H */

