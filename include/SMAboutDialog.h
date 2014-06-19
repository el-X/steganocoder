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
    const wxString TEXT_TITLE = _("About SteganoMonkey");
    const wxString TEXT_DEVELOPER_HEADING = _("Developers:");
    const wxString TEXT_DEVELOPER1 = _("Thomas Matern,");
    const wxString TEXT_DEVELOPER2 = _("Robert Heimsoth, Alexander Keller");
    const wxString TOOLTIP_COPYLEFT = _("Copyleft");
    const wxString TEXT_COPYLEFT = _("2014 Hochschule Bremen, TI 6.2");
    const wxString TEXT_CLIPART_BY = _("Clipart by ");
    const wxString TEXT_CLIPART = _("Openclipart");
    const wxString LINK_CLIPART = _("https://openclipart.org/");
    const wxString TEXT_ICON_BY = _("Icons by ");
    const wxString TEXT_ICON = _("Tango");
    const wxString LINK_ICON = _("http://tango.freedesktop.org/Tango_Desktop_Project");
    void create();
    void doLayout();
};

#endif	/* SMABOUTDIALOG_H */

