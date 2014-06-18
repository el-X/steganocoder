/* 
 * File:   SMStatusBar.h
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#ifndef SMSTATUSBAR_H
#define	SMSTATUSBAR_H

#include <wx/string.h>
#include <wx/statusbr.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

class SMStatusBar: public wxStatusBar {
public:
    enum STATUS_STYLE {
        STATUS_NORMAL = 1,
        STATUS_ERROR = 2
    };
    SMStatusBar(wxWindow *parent, long style);
    virtual ~SMStatusBar() {};
    void OnSize(wxSizeEvent& event);
    void SetStatusText(const wxString& text, int number = 0);
    void setStatusStyle(long style);
    void setBackgroundColour(const wxColour& colour);
    void setForegroundColour(const wxColour& colour);
    void setAlignment(long alignment);
    void setBorder(long border);
    wxString GetStatusText(int number = 0) {
        return statusText->GetLabelText();
    };
    long getStatusStyle() {
        return this->style;
    };
    wxColour getBackgroundColour() {
        return this->backgroundColour;
    };
    wxColour getForegroundColour() {
        return this->foregroundColour;
    };
    long getAlignment() {
        return this->alignment;
    };
    long getBorder() {
        return this->border;
    };
    
protected:
    void repositionText();
    void applyStatusStyle();
    wxStaticText* statusText;
    long style = STATUS_NORMAL;
    wxColour backgroundColour;
    wxColour foregroundColour;
    long alignment;
    long border;
    
private:
    DECLARE_EVENT_TABLE()
};

#endif	/* SMSTATUSBAR_H */

