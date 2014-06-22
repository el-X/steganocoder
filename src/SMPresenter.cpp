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
    this->initGUIElements();
    return true;
}

/**
 * Definiert den Startzustand des Programms.
 */
void SMPresenter::initGUIElements() {
    this->setSaveAllowed(false);
    this->setEncodingAllowed(false);
    this->setDecodingAllowed(false);
    view->getTxtLengthOutput()->SetValue("0");
    view->getMaxTxtLengthOutput()->SetValue("0");
    view->getSecretMsgInput()->SetEditable(false);
    view->setStatusBarText(MSG_WELCOME);
}

/**
 * Initialisiert die Handler für die genutzten Bildformate.
 */
void SMPresenter::initImageHandlers() {
    wxImage::AddHandler(new wxBMPHandler);
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxJPEGHandler);
    wxImage::AddHandler(new wxICOHandler);
    wxImage::AddHandler(new wxTGAHandler());
}

/**
 * Öffnet ein FileDialog zum Laden des Bildes.
 * Anhand der event-id wird unterschieden, welche Aktion 
 * getätigt wurde (welcher Button wurde getätigt).
 * 
 * @param event Kommando Event mit der ID.
 */
void SMPresenter::onLoad(wxCommandEvent& event) {
    
    // Zeige ein Dialog, um ein Bild zu laden.
    wxFileDialog openDialog(view, TXT_LOAD_IMG, wxEmptyString, wxEmptyString, IMG_IN_FORMATS);
    openDialog.SetDirectory(wxGetHomeDir());
    openDialog.CentreOnParent();
    
    // Warte bis der Dialog geschlossen wird. 
    // Falls OK gedrückt wurde, versuche das Bild zu laden.
    if (openDialog.ShowModal() == wxID_OK) {
        wxImage image = openDialog.GetPath();
        
        // Prüfe ob das Bild in Ordnung ist.
        if (!image.IsOk()) {
            view->setStatusBarErrorText(MSG_COR_IMG);
            return;
        }
        // Anzahl der Bytes = Hoehe x Breite x RGB
        size_t imageBytesCount = image.GetHeight() * image.GetWidth() * 3;
        
        // Prüfen welcher Button betätigt wurde.
        if (event.GetId() == SMView::ID_LOAD_MOD_IMG) {
            
            // Es wird ein Bild mit versteckter Nachricht geladen.
            view->getModStaticBitmap()->SetBitmap(image);
            model->setModCarrierBytes(image.GetData(), (size_t) imageBytesCount);
            view->getBitpatternOutput()->SetValue(model->getModBitPattern());
            
            // Prüfen ob das Bild eine versteckte Nachricht enthält.
            if (model->checkForHeaderSignature()) {
                this->setDecodingAllowed(true);
                view->setStatusBarText(MSG_RDY_DEC);
            } else {
                this->setDecodingAllowed(false);
                view->setStatusBarErrorText(MSG_NO_HDN_INF);
            }
            view->getUnmodStaticBitmap()->SetBitmap(wxBitmap());
            view->getSecretMsgInput()->Clear();
            view->getSecretMsgInput()->SetEditable(false);
            this->setEncodingAllowed(false);
            model->resetUnmodCarrierBytes();
            view->getMaxTxtLengthOutput()->SetValue("0");
        } else {
            
            // Es wird ein Bild ohne versteckte Nachricht geladen.
            view->getUnmodStaticBitmap()->SetBitmap(image);
            model->setUnmodCarrierBytes(image.GetData(), imageBytesCount);
            
            // Zeige maximale Länge der Nachricht.
            view->getMaxTxtLengthOutput()->SetValue(to_string(getMaxTextLength()));
            
            // Interaktive GUI Elemente aktualisieren.
            view->getBitpatternOutput()->Clear();
            view->getSecretMsgInput()->SetEditable(true);
            this->setDecodingAllowed(false);
            this->setSaveAllowed(false);
            
            // Setze die Daten und das Bild mit einer Nachricht zurück.
            model->resetModCarrierBytes();
            view->getModStaticBitmap()->SetBitmap(wxBitmap());
            
            // Im Falle dass schon vorher ein Bild da war..
            int secMsgLen = view->getSecretMsgInput()->GetValue().size();
            if (secMsgLen > getMaxTextLength()) {
                this->setEncodingAllowed(false);
                view->setStatusBarErrorText(MSG_TOO_LONG);
            } else if (secMsgLen == 0){
                this->setEncodingAllowed(false);
                view->setStatusBarText(MSG_WAIT_INPUT);
            } else {
                this->setEncodingAllowed(true);
                view->setStatusBarText(MSG_RDY_ENC);
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
    
    // Zeige ein Dialog zum Speichern des Bildes.
    wxFileDialog dialog(view, TXT_SAVE_IMG, wxEmptyString, IMG_OUT_NAME,
            IMG_OUT_FORMATS, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    dialog.SetFilterIndex(1);
    
    // Falls OK gedrückt wurde, speichere das Bild im gegebenen Pfad.
    if (dialog.ShowModal() == wxID_OK) {
        wxBitmap modBitmap = view->getModStaticBitmap()->GetBitmap();
        modBitmap.SaveFile(dialog.GetPath(), wxBITMAP_TYPE_BMP);
        view->setStatusBarText(_(MSG_SAVE_PATH + dialog.GetPath()));
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
    // Zweiter Parameter auf TRUE, damit die Kontrolle über 
    // den Pointer nicht dem wxImage-Objekt überlassen wird.
    image.SetData(model->getModCarrierBytes(), true);
    view->getModStaticBitmap()->SetBitmap(image);
    view->getBitpatternOutput()->SetValue(model->getModBitPattern());
    this->setSaveAllowed(true);
    view->setStatusBarText(MSG_ENC_SUCC);
    view->Layout();
}

/**
 * Holt die Nachricht aus dem geladenen Bild.
 * 
 * @param event Kommando Event mit der ID.
 */
void SMPresenter::onDecode(wxCommandEvent& event) {
    
    // Setze unmodifiziertes Bild zurück (im Falle das eins schon da war).
    view->getUnmodStaticBitmap()->SetBitmap(wxBitmap());
    view->getMaxTxtLengthOutput()->SetValue("0");
    this->setEncodingAllowed(false);
    
    // Starte den Decodingvorgang und zeige die Nachricht.
    view->getSecretMsgInput()->SetValue(model->decode());
    view->setStatusBarText(MSG_DEC_SUCC);
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
    
    // Regex erlaubt nur ASCII Zeichen 0x(0-7F) => 0-127
    if (reg.Compile(wxT("[^\u0000-\u007F]"))) {
        // Setzt ein Fragezeichen, falls Zeichen nicht ASCII Zeichen ist.
        reg.Replace(&filtered, wxT("?"));
    }
    // Aktualisiere das Eingabefeld nur wenn die Nachricht gefiltert wurde,
    // also die Nachricht im Textfeld nicht mehr der aktuellen Nachricht entspricht
    if (rawInput.compare(filtered) != 0) {
        msgInput->SetValue(filtered);
        msgInput->SetInsertionPointEnd();
    }
    // Aktualisiere die angezeigte Länge der Nachricht.
    wxTextCtrl* msgLen = view->getTxtLengthOutput();
    int size = msgInput->GetValue().size();
    msgLen->SetValue(std::to_string(size));
    
    // Deaktiviere bzw. aktiviere Encode-Elemente und aktualisiere die Statusleiste.
    int maxLength = getMaxTextLength();
    bool unmodBmpSet = isUnmodBmpSet();
    if (!unmodBmpSet || size == 0 || size > maxLength) {
        this->setEncodingAllowed(false);
        if (unmodBmpSet && size > maxLength) {
            view->setStatusBarErrorText(MSG_TOO_LONG);
        } else if (unmodBmpSet && !isModBmpSet()) {
            view->setStatusBarText(MSG_WAIT_INPUT);
        }
    } else {
        this->setEncodingAllowed(true);
        view->setStatusBarText(MSG_RDY_ENC);
    }
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
