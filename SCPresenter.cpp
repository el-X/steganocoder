/* 
 * File:   SCPresenter.cpp
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/intl.h>
#include <wx/string.h>
#include <wx/regex.h>
#include "SCPresenter.h"
#include <iostream>
#include <string>

using namespace std;

IMPLEMENT_APP(SCPresenter)

// Mapping für die Events vornehmen
wxBEGIN_EVENT_TABLE(SCPresenter, wxApp)
    EVT_MENU(SCView::ID_LOAD_UNMOD_IMG, SCPresenter::onLoad)
    EVT_BUTTON(SCView::ID_LOAD_UNMOD_IMG, SCPresenter::onLoad)
    EVT_MENU(SCView::ID_LOAD_MOD_IMG, SCPresenter::onLoad)
    EVT_BUTTON(SCView::ID_LOAD_MOD_IMG, SCPresenter::onLoad)
    EVT_MENU(SCView::ID_SAVE_MOD_IMG, SCPresenter::onSave)
    EVT_BUTTON(SCView::ID_SAVE_MOD_IMG, SCPresenter::onSave)
    EVT_MENU(SCView::ID_ENCODE, SCPresenter::onEncode)
    EVT_BUTTON(SCView::ID_ENCODE, SCPresenter::onEncode)
    EVT_MENU(SCView::ID_DECODE, SCPresenter::onDecode)
    EVT_BUTTON(SCView::ID_DECODE, SCPresenter::onDecode)
    EVT_TEXT(SCView::ID_SECRET_MSG, SCPresenter::onSecretMessageChange)
    EVT_MENU(wxID_EXIT, SCPresenter::onExit)
    EVT_MENU(wxID_ABOUT, SCPresenter::onAbout)
wxEND_EVENT_TABLE()


/**
 * Die main-Funktion des Programms.
 */
bool SCPresenter::OnInit() {
    this->initImageHandlers();
    
    view = new SCView();
    view->showSplashScreen();
    view->SetMinSize(wxSize(800, 600));
    view->create();
    view->doLayout();
    view->Centre();
    view->Show(true);

    model = new SCModel();
    this->init();
    view->setStatusBarText(_("Welcome to SteganoMonkey!"));
    return true;
}

/**
 * Definiert den Startzustand des Programms.
 */
void SCPresenter::init() {
    this->setSaveAllowed(false);
    this->setEncodingAllowed(false);
    this->setDecodingAllowed(false);
    view->getTxtLengthOutput()->SetValue(to_string(0));
    view->getMaxTxtLengthOutput()->SetValue(to_string(0));
}

/**
 * Initialisiert die Handler für die genutzten Bildformate.
 */
void SCPresenter::initImageHandlers() {
    wxImage::AddHandler(new wxBMPHandler);
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxJPEGHandler);
    wxImage::AddHandler(new wxGIFHandler);
    wxImage::AddHandler(new wxICOHandler);
}

/**
 * Öffnet ein FileDialog zum Laden des Bildes.
 * Anhand der event id wird unterschieden welche Aktion 
 * getätigt wurde (welche Komponente wurde getätigt).
 * 
 * @param event Kommando Event mit der ID.
 */
void SCPresenter::onLoad(wxCommandEvent& event) {
    wxFileDialog openDialog(view, _T("Load Image"), wxEmptyString, wxEmptyString,
            _T("Image (*.bmp;*.jpg;*.jpeg;*.png;*.gif)|*.bmp;*.jpg;*.jpeg;*.png;*.gif"));
    openDialog.SetDirectory(wxGetHomeDir());
    openDialog.CentreOnParent();

    if (openDialog.ShowModal() == wxID_OK) {
        wxImage image = openDialog.GetPath();
        // Anzahl der Bytes = Hoehe x Breite x RGB
        size_t imageBytesCount = image.GetHeight() * image.GetWidth() * 3;
        if (event.GetId() == SCView::ID_LOAD_MOD_IMG) {
            // Es wird versucht ein Bild mit versteckter Nachricht zu laden
            view->getModStaticBitmap()->SetBitmap(image);
            model->setModCarrierBytes(image.GetData(), (size_t) imageBytesCount);
            view->getBitpatternOutput()->SetValue(model->getModBitPattern());
            // Prüfen ob das Bild eine verstekcte Nachricht enthält.
            if (model->checkForHeaderSignature()) {
                this->setDecodingAllowed(true);
            } else {
                this->setDecodingAllowed(false);
                wxMessageDialog notationDialog(NULL,
                        wxT("Geladenes Bild enthält keine versteckte Nachricht!"),
                        wxT("Info"), wxOK | wxICON_WARNING);
                notationDialog.CentreOnParent();
                notationDialog.ShowModal();
            }
            view->getUnmodStaticBitmap()->SetBitmap(wxBitmap());
            view->getSecretMsgInput()->Clear();
            this->setEncodingAllowed(false);
            view->getMaxTxtLengthOutput()->SetValue("0");
        } else {
            // Bild ohne versteckter Nachricht wurde geladen.
            view->getUnmodStaticBitmap()->SetBitmap(image);
            model->setUnmodCarrierBytes(image.GetData(), imageBytesCount);
            // Zeige maximale Länge der Nachricht.
            view->getMaxTxtLengthOutput()->SetValue(to_string(getMaxTextLength()));
            // Interaktive GUI Elemente aktualisieren.
            view->getBitpatternOutput()->Clear();
            this->setEncodingAllowed(!view->getSecretMsgInput()->GetValue().empty());
            this->setDecodingAllowed(false);
            this->setSaveAllowed(false);
            view->getModStaticBitmap()->SetBitmap(wxBitmap());
        }
        view->Layout();
    }
}

/**
 * Öffnet ein FileDialog zur Speicherung des Bildes mit versteckter Nachricht.
 * 
 * @param event Kommando Event mit der ID.
 */
void SCPresenter::onSave(wxCommandEvent& event) {
    wxFileDialog dialog(view, _T("Save Image"), wxEmptyString, _T("top_secret.bmp"),
            _T("Bitmap (*.bmp)|*.bmp"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    dialog.SetFilterIndex(1);
    if (dialog.ShowModal() == wxID_OK) {
        wxBitmap modBitmap = view->getModStaticBitmap()->GetBitmap();
        modBitmap.SaveFile(dialog.GetPath(), wxBITMAP_TYPE_BMP);
        view->setStatusBarText("Image containing the secret message saved under: " + dialog.GetPath());
    }
}

/**
 * Versteckt die gegebene Nachricht in dem gegebenem Bild.
 * 
 * @param event Kommando Event mit der ID.
 */
void SCPresenter::onEncode(wxCommandEvent& event) {
    wxString message = view->getSecretMsgInput()->GetValue();
    wxImage newImage;
    if (wxAtoi(message) > getMaxTextLength()) {
        // TODO raus, da es in die status bar geschrieben werden soll
        wxMessageDialog notationDialog(NULL,
                wxT("Tut uns leid, das Bild ist zu klein für die Nachricht!"),
                wxT("WARNING!"), wxOK | wxICON_WARNING);
        notationDialog.CentreOnParent();
        notationDialog.ShowModal();
    } else {
        // Das Bild ohne Nachricht holen.
        wxImage image = view->getUnmodStaticBitmap()->GetBitmap().ConvertToImage();
        size_t imageBytesCount = image.GetHeight() * image.GetWidth() * 3;
        // Bilddaten an das Model übergeben und den Encodingvorgang starten.
        model->setUnmodCarrierBytes(image.GetData(), imageBytesCount);
        model->encode(message.ToStdString());
        // Modifizierte Bilddaten holen und anzeigen.
        newImage = view->getUnmodStaticBitmap()->GetBitmap().ConvertToImage();
        newImage.SetData(model->getModCarrierBytes());
        view->getModStaticBitmap()->SetBitmap(newImage);
        view->getBitpatternOutput()->SetValue(model->getModBitPattern());
        this->setSaveAllowed(true);
    }
}

/**
 * Holt die Nachricht aus dem geladenen Bild.
 * 
 * @param event Kommando Event mit der ID.
 */
void SCPresenter::onDecode(wxCommandEvent& event) {
    view->getUnmodStaticBitmap()->SetBitmap(wxBitmap());
    view->getMaxTxtLengthOutput()->SetValue("0");
    view->getSecretMsgInput()->SetValue(model->decode());
    this->setEncodingAllowed(false);
}

/**
 * Überprüft das Eingabefeld auf nicht zulässige Zeichen und 
 * ersetzt diese durch gültige Zeichen. Zusätzlich wird die 
 * angezeigte Länge der einegegebener Nachricht aktualisiert.
 * 
 * @param event Kommando Event mit der ID.
 */
void SCPresenter::onSecretMessageChange(wxCommandEvent& event) {
    // TODO statusbar nachricht (warnung) ausgeben wenn zu lange nachricht!
    // Ersetze alle nicht ASCII Zeichen.
    wxTextCtrl* msgInput = view->getSecretMsgInput();
    wxString rawInput = msgInput->GetValue();
    wxString filtered = rawInput;
    wxRegEx reg;
    if (reg.Compile(wxT("[ÄÖÜäöüß*]"))) {
        reg.Replace(&filtered, wxT("?")); // gemäß der Maske ersetzen
    }
    // Aktualisiere das Eingabefeld nur wenn die Nachricht gefiltert wurde.
    if (rawInput.compare(filtered) != 0) {
        msgInput->SetValue(filtered);
        msgInput->SetInsertionPointEnd();
    }
    // Aktualisiere die angezeigte Länge der Nachricht.
    wxTextCtrl* msgLen = view->getTxtLengthOutput();
    int size = msgInput->GetValue().size();
    msgLen->SetValue(std::to_string(size));
    // Deaktiviere bzw. aktiviere encode.
    if (view->getModStaticBitmap()->GetBitmap().IsFree() 
            && (size = 0 || size > getMaxTextLength())) {
        this->setEncodingAllowed(false);
    } else {
        this->setEncodingAllowed(true);
    }
}

/**
 * Wird beim Beenden des Programms ausgeführt.
 * 
 * @param event Kommando Event mit der ID.
 */
void SCPresenter::onExit(wxCommandEvent& event) {
    view->Close(true);
}

/**
 * Zeigt ein Dialog mit Infos über das Programm, wenn die
 * GUI-Interaktionselemte "Über" betätigt wurden.
 * 
 * @param event Kommando Event mit der ID.
 */
void SCPresenter::onAbout(wxCommandEvent& event) {
    view->getAboutDialog()->Centre();
    view->getAboutDialog()->ShowModal();
}

/**
 * Ermittelt die maximal mögliche Länge der Nachricht mithilfe der Bilddaten
 * und gibt diese zurück.
 * 
 * @return unsigned int Maximale Größe der Nachricht.
 */
int SCPresenter::getMaxTextLength() const {
    unsigned int maxTxtLength = 0;
    // Maximal mögliche Länge anhand der Bilddaten ermitteln.
    if (!view->getUnmodStaticBitmap()->GetBitmap().IsNull()) {
        unsigned int height = view->getUnmodStaticBitmap()->GetBitmap().GetHeight();
        unsigned int width = view->getUnmodStaticBitmap()->GetBitmap().GetWidth();
        maxTxtLength = (height * width * 3) / 8 - model->getHeaderSize();
    }
    return maxTxtLength;
}

/**
 * Aktiviert / deaktiviert GUI-Interaktionselemente für das Encoding.
 * 
 * @param allowed
 */
void SCPresenter::setEncodingAllowed(bool allowed) {
    view->getEncodeBtn()->Enable(allowed);
    view->getEncodeMenuItem()->Enable(allowed);
}

/**
 * Aktiviert / deaktiviert GUI-Interaktionselemente für das Decoding.
 * 
 * @param allowed
 */
void SCPresenter::setDecodingAllowed(bool allowed) {
    view->getDecodeBtn()->Enable(allowed);
    view->getDecodeMenuItem()->Enable(allowed);
}

/**
 * Aktiviert / deaktiviert GUI-Interaktionselemente für das Speichern.
 * 
 * @param allowed
 */
void SCPresenter::setSaveAllowed(bool allowed) {
    view->getSaveModImgBtn()->Enable(allowed);
    view->getSaveModImgMenuItem()->Enable(allowed);
}


