/* 
 * File:   SCStatusBar.cpp
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#include "SMStatusBar.h"
#include <iostream>

// Mapping für die StatusBar-Events vornehmen
BEGIN_EVENT_TABLE(SMStatusBar, wxStatusBar)
    EVT_SIZE(SMStatusBar::OnSize)
END_EVENT_TABLE()

/**
 * Konstruktor der eigens erstellten StatusBar.
 * @param parent der Parent, der diese StatusBar halten soll
 * @param style der Style der StatusBar
 */
SMStatusBar::SMStatusBar(wxWindow *parent, long style) 
: wxStatusBar(parent, wxID_ANY, style, _("")) {
    // anders als zur herkömmlichen StatusBar erhält diese hier explizit einen
    // wxStaticText; dies ermöglicht ein besseres Customizing 
    statusText = new wxStaticText(this, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, 0);
    this->setStatusStyle(STATUS_NORMAL);
    this->repositionText();
}

/**
 * Bei Änderung der Fenster- und somit auch der StatusBar-Größe muss der 
 * Text innerhalb der StatusBar repositioniert werden, was mit dieser Methode
 * erfolgt.
 */
void SMStatusBar::repositionText() {
    if (!statusText ) {
        return;
    }
    
    wxRect rect;    
	GetFieldRect(0, rect);  // der Rechteckbereich des 0-ten Feldes
    statusText->SetSize(rect.x + 3, rect.y + 2, rect.width, rect.height);
}

/**
 * Wird bei Änderung der StatusBar-Größe aufgerufen, um den Text neu 
 * auszurichten.
 * @param event ein wxSizeEvent
 */
void SMStatusBar::OnSize(wxSizeEvent& event) {
    this->repositionText();
    event.Skip();   // weitere Verarbeitung des wxSizeEvents für die StatusBar unterbinden
}

/**
 * Setzt einen globalen Style für die StatusBar. Momentan vorgesehen sind ein
 * normaler Style mit linksbündiger normaler Schrift und ein Error-Style mit
 * zentriertem weißem Text und einem roten Hintergrund. 
 * @param style der zu setzende Style
 */
void SMStatusBar::setStatusStyle(long style) {
    switch (style) {
        case STATUS_NORMAL:
            backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_MENU);
            foregroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_MENUTEXT);
            alignment = wxALIGN_LEFT;
            break;
        case STATUS_ERROR:
            backgroundColour = *wxRED;
            foregroundColour = *wxWHITE;
            alignment = wxALIGN_CENTER;
            break;
        default: 
            this->setStatusStyle(STATUS_NORMAL);
            break;
    }
    border = wxSTATIC_BORDER;
    this->style = style;
    this->applyStatusStyle();
}

/**
 * Wendet die gesetzten Style-Attribute an.
 */
void SMStatusBar::applyStatusStyle() {
    this->SetBackgroundColour(backgroundColour);
    statusText->SetForegroundColour(foregroundColour);
    this->SetWindowStyle(border);
    statusText->SetWindowStyle(alignment);
    this->Refresh();
}

/**
 * Dient zum Setzen des Labels auf der StatusBar.
 * @param text zu setzende Text
 * @param number das gewünschte Feld StatusBar-Feld (hier stets das 0-te)
 */
void SMStatusBar::SetStatusText(const wxString& text, int number) {
    statusText->SetLabel(text);
    this->repositionText();
}

/**
 * Zum Setzen der Hintergrundfarbe.
 * @param colour die gewünschte Hintergrundfarbe
 */
void SMStatusBar::setBackgroundColour(const wxColour& colour) {
    this->SetBackgroundColour(colour);
    this->backgroundColour = colour;
}

/**
 * Dient zum Setzen der Schriftfarbe.
 * @param colour die geünschte Schriftfarbe
 */
void SMStatusBar::setForegroundColour(const wxColour& colour) {
    statusText->SetForegroundColour(colour);
    this->foregroundColour = colour;
}

/**
 * Setzt die Ausrichtung des Textes.
 * @param alignment die gewünschte Textausrichtung
 */
void SMStatusBar::setAlignment(long alignment) {
    statusText->SetWindowStyle(alignment);
    this->alignment = alignment;
}

/**
 * Der Rahmen der StatusBar kann hier gesetzt werden.
 * @param border der gewünschte Rahmen
 */
void SMStatusBar::setBorder(long border) {
    this->SetWindowStyle(border);
    this->border = border;
}

