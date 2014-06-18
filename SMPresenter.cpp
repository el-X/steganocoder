/* 
 * File:   SMPresenter.cpp
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
#include "SMPresenter.h"
#include <iostream>
#include <string>

using namespace std;

IMPLEMENT_APP(SMPresenter)

// Mapping für die Events vornehmen
wxBEGIN_EVENT_TABLE(SMPresenter, wxApp)
    EVT_MENU(SMView::ID_LOAD_UNMOD_IMG, SMPresenter::onLoad)
    EVT_BUTTON(SMView::ID_LOAD_UNMOD_IMG, SMPresenter::onLoad)
    EVT_MENU(SMView::ID_LOAD_MOD_IMG, SMPresenter::onLoad)
    EVT_BUTTON(SMView::ID_LOAD_MOD_IMG, SMPresenter::onLoad)
    EVT_MENU(SMView::ID_SAVE_MOD_IMG, SMPresenter::onSave)
    EVT_BUTTON(SMView::ID_SAVE_MOD_IMG, SMPresenter::onSave)
    EVT_MENU(SMView::ID_ENCODE, SMPresenter::onEncode)
    EVT_BUTTON(SMView::ID_ENCODE, SMPresenter::onEncode)
    EVT_MENU(SMView::ID_DECODE, SMPresenter::onDecode)
    EVT_BUTTON(SMView::ID_DECODE, SMPresenter::onDecode)
    EVT_TEXT(SMView::ID_SECRET_MSG, SMPresenter::onSecretMessageChange)
    EVT_MENU(wxID_EXIT, SMPresenter::onExit)
    EVT_MENU(wxID_ABOUT, SMPresenter::onAbout)
wxEND_EVENT_TABLE()


/**
 * Die main-Funktion des Programms.
 */
bool SMPresenter::OnInit() {
    this->initImageHandlers();

    view = new SMView();
    view->showSplashScreen();
    view->SetMinSize(wxSize(800, 600));
    view->create();
    view->doLayout();
    view->Centre();
    view->Show(true);

    model = new SMModel();
    this->init();
    view->setStatusBarText(welcomeMsg);
    return true;
}

/**
 * Definiert den Startzustand des Programms.
 */
void SMPresenter::init() {
    this->setSaveAllowed(false);
    this->setEncodingAllowed(false);
    this->setDecodingAllowed(false);
    view->getTxtLengthOutput()->SetValue(to_string(0));
    view->getMaxTxtLengthOutput()->SetValue(to_string(0));
    view->getSecretMsgInput()->SetEditable(false);
}

/**
 * Initialisiert die Handler für die genutzten Bildformate.
 */
void SMPresenter::initImageHandlers() {
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
void SMPresenter::onLoad(wxCommandEvent& event) {
    wxFileDialog openDialog(view, _T("Load Image"), wxEmptyString, wxEmptyString,
            _T("Image (*.bmp;*.jpg;*.jpeg;*.png;*.gif)|*.bmp;*.jpg;*.jpeg;*.png;*.gif"));
    openDialog.SetDirectory(wxGetHomeDir());
    openDialog.CentreOnParent();

    if (openDialog.ShowModal() == wxID_OK) {
        wxImage image = openDialog.GetPath();
        // Anzahl der Bytes = Hoehe x Breite x RGB
        size_t imageBytesCount = image.GetHeight() * image.GetWidth() * 3;
        if (event.GetId() == SMView::ID_LOAD_MOD_IMG) {
            // Es wird versucht ein Bild mit versteckter Nachricht zu laden
            view->getModStaticBitmap()->SetBitmap(image);
            model->setModCarrierBytes(image.GetData(), (size_t) imageBytesCount);
            view->getBitpatternOutput()->SetValue(model->getModBitPattern());
            // Prüfen ob das Bild eine verstekcte Nachricht enthält.
            if (model->checkForHeaderSignature()) {
                this->setDecodingAllowed(true);
                view->setStatusBarText("Ready to decode!");
            } else {
                this->setDecodingAllowed(false);
                view->setStatusBarErrorText("Loaded picture has no hidden message!");
            }
            view->getUnmodStaticBitmap()->SetBitmap(wxBitmap());
            view->getSecretMsgInput()->Clear();
            view->getSecretMsgInput()->SetEditable(false);
            this->setEncodingAllowed(false);
            model->resetUnmodCarrierBytes();
            view->getMaxTxtLengthOutput()->SetValue("0");
        } else {
            // Bild ohne versteckter Nachricht wurde geladen.
            view->getUnmodStaticBitmap()->SetBitmap(image);
            model->setUnmodCarrierBytes(image.GetData(), imageBytesCount);
            // Zeige maximale Länge der Nachricht.
            view->getMaxTxtLengthOutput()->SetValue(to_string(getMaxTextLength()));
            // Interaktive GUI Elemente aktualisieren.
            view->getBitpatternOutput()->Clear();
            view->getSecretMsgInput()->SetEditable(true);
            this->setDecodingAllowed(false);
            this->setSaveAllowed(false);
            model->resetModCarrierBytes();
            view->getModStaticBitmap()->SetBitmap(wxBitmap());
            bool isSecMsgEmpty = view->getSecretMsgInput()->GetValue().empty();
            
            // -----------------------------------------------------------------
            // TODO Check the maxSize! Is the picture big enough?
            // -----------------------------------------------------------------
            this->setEncodingAllowed(!isSecMsgEmpty);
            if (!isSecMsgEmpty) {
                view->setStatusBarText("Ready to encode!");
            }
        }
        view->Layout();
    }
}

/**
 * Öffnet ein FileDialog zur Speicherung des Bildes mit versteckter Nachricht.
 * 
 * @param event Kommando Event mit der ID.
 */
void SMPresenter::onSave(wxCommandEvent& event) {
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
void SMPresenter::onEncode(wxCommandEvent& event) {

    // Die eingegebene Nachricht holen.
    wxString message = view->getSecretMsgInput()->GetValue();

    // Das Bild ohne Nachricht holen.
    wxImage image = view->getUnmodStaticBitmap()->GetBitmap().ConvertToImage();
    size_t imageBytesCount = image.GetHeight() * image.GetWidth() * 3;
    
    // Bilddaten an das Model übergeben und den Encodingvorgang starten.
    model->setUnmodCarrierBytes(image.GetData(), imageBytesCount);
    model->encode(message.ToStdString());
    // Modifizierte Bilddaten setzen und anzeigen.
    image.SetData(model->getModCarrierBytes());
    view->getModStaticBitmap()->SetBitmap(image);
    view->getBitpatternOutput()->SetValue(model->getModBitPattern());
    this->setSaveAllowed(true);
    view->Layout();
}

/**
 * Holt die Nachricht aus dem geladenen Bild.
 * 
 * @param event Kommando Event mit der ID.
 */
void SMPresenter::onDecode(wxCommandEvent& event) {
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
void SMPresenter::onSecretMessageChange(wxCommandEvent& event) {

    // Ersetze alle nicht ASCII Zeichen.
    wxTextCtrl* msgInput = view->getSecretMsgInput();
    wxString rawInput = msgInput->GetValue();
    wxString filtered = rawInput;
    wxRegEx reg;
    if (reg.Compile(wxT("[^\u0000-\u007F]"))) {
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
    // Deaktiviere bzw. aktiviere encode und aktualisiere die Statusleiste.
    int maxLength = getMaxTextLength();
    bool unmodBmpSet = isUnmodBmpSet();
    if (!unmodBmpSet || size == 0 || size > maxLength) {
        this->setEncodingAllowed(false);
        if (unmodBmpSet && size > maxLength) {
            view->setStatusBarErrorText("Your message is too long for the given picture!");
        } else if (!isModBmpSet()) {
            view->setStatusBarText(welcomeMsg);
        }
    } else {
        this->setEncodingAllowed(true);
        view->setStatusBarText("Ready to encode!");
    }
    view->Layout();
}

/**
 * Wird beim Beenden des Programms ausgeführt.
 * 
 * @param event Kommando Event mit der ID.
 */
void SMPresenter::onExit(wxCommandEvent& event) {
    view->Close(true);
}

/**
 * Zeigt ein Dialog mit Infos über das Programm, wenn die
 * GUI-Interaktionselemte "Über" betätigt wurden.
 * 
 * @param event Kommando Event mit der ID.
 */
void SMPresenter::onAbout(wxCommandEvent& event) {
    view->getAboutDialog()->Centre();
    view->getAboutDialog()->ShowModal();
}

/**
 * Ermittelt die maximal mögliche Länge der Nachricht mithilfe der Bilddaten
 * und gibt diese zurück.
 * 
 * @return unsigned int Maximale Größe der Nachricht.
 */
int SMPresenter::getMaxTextLength() const {
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
void SMPresenter::setEncodingAllowed(bool allowed) {
    view->getEncodeBtn()->Enable(allowed);
    view->getEncodeMenuItem()->Enable(allowed);
}

/**
 * Aktiviert / deaktiviert GUI-Interaktionselemente für das Decoding.
 * 
 * @param allowed
 */
void SMPresenter::setDecodingAllowed(bool allowed) {
    view->getDecodeBtn()->Enable(allowed);
    view->getDecodeMenuItem()->Enable(allowed);
}

/**
 * Aktiviert / deaktiviert GUI-Interaktionselemente für das Speichern.
 * 
 * @param allowed
 */
void SMPresenter::setSaveAllowed(bool allowed) {
    view->getSaveModImgBtn()->Enable(allowed);
    view->getSaveModImgMenuItem()->Enable(allowed);
}

/**
 * Liefert TRUE zurück, wenn ein Bild fürs encode geladen ist.
 * 
 * @return bool
 */
bool SMPresenter::isUnmodBmpSet() {
    return (model->getUnmodCarrierBytesLength() > 0);
}

/**
 * Liefert TRUE zurück, wenn ein Bild fürs decode geladen ist.
 * 
 * @return bool
 */
bool SMPresenter::isModBmpSet() {
    return (model->getModCarrierBytesLength() > 0);
}
