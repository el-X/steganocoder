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

IMPLEMENT_APP(SCPresenter)

// Mapping für die Events vornehmen
wxBEGIN_EVENT_TABLE(SCPresenter, wxApp)
EVT_MENU(ID_LOAD_UNMOD_IMG, SCPresenter::onLoad)
EVT_BUTTON(ID_LOAD_UNMOD_IMG, SCPresenter::onLoad)
EVT_MENU(ID_LOAD_MOD_IMG, SCPresenter::onLoad)
EVT_BUTTON(ID_LOAD_MOD_IMG, SCPresenter::onLoad)
EVT_MENU(ID_SAVE_MOD_IMG, SCPresenter::onSave)
EVT_BUTTON(ID_SAVE_MOD_IMG, SCPresenter::onSave)
EVT_MENU(ID_ENCODE, SCPresenter::onEncode)
EVT_BUTTON(ID_ENCODE, SCPresenter::onEncode)
EVT_MENU(ID_DECODE, SCPresenter::onDecode)
EVT_BUTTON(ID_DECODE, SCPresenter::onDecode)
EVT_TEXT(ID_SECRET_MSG, SCPresenter::onSecretMessageChange)
EVT_MENU(wxID_EXIT, SCPresenter::onExit)
EVT_MENU(wxID_ABOUT, SCPresenter::onAbout)
wxEND_EVENT_TABLE()

/**
 * Die main-Funktion des Programms.
 */
bool SCPresenter::OnInit() {
    wxInitAllImageHandlers();
    view = new SCView();
    view->SetMinSize(wxSize(800, 600));
    view->create();
    view->doLayout();
    view->Centre();
    view->Show(true);

    model = new SCModel();
    this->init();
    view->getStatusBar()->SetStatusText(_("Welcome to SteganoCoder!"));
    return true;
}

/**
 * Definiert den Startzustand des Programms.
 */
void SCPresenter::init() {
    view->getSaveModImgMenuItem()->Enable(false);
    view->getSaveModImgBtn()->Disable();
    view->getEncodeMenuItem()->Enable(false);
    view->getEncodeBtn()->Disable();
    view->getDecodeMenuItem()->Enable(false);
    view->getDecodeBtn()->Disable();
    *(view->getTxtLengthOutput()) << 0;
    *(view->getMaxTxtLengthOutput()) << 0;
}

/**
 * Öffnet ein FileDialog zum Laden des Bildes.
 * Anhand der event id wird unterschieden welche Aktion 
 * getätigt wurde (welche Komponente wurde getätigt).
 * 
 * @param event Kommando Event mit der ID
 */
void SCPresenter::onLoad(wxCommandEvent& event) {
    wxFileDialog openDialog(view, _T("Load Image"), wxEmptyString, wxEmptyString,
            _T("Image (*.bmp;*.jpg;*.jpeg;*.png;*.gif)|*.bmp;*.jpg;*.jpeg;*.png;*.gif"));
    openDialog.SetDirectory(wxGetHomeDir()); // OS independency
    openDialog.CentreOnParent();

    if (openDialog.ShowModal() == wxID_OK) {
        wxImage image = openDialog.GetPath();
        size_t imageBytesCount = image.GetHeight() * image.GetWidth() * 3;
        if (event.GetId() == ID_LOAD_MOD_IMG) {
            // Es wird versucht ein Bild mit versteckter Nachricht zu geladen
            view->getModStaticBitmap()->SetBitmap(image);
            model->setModCarrierBytes(image.GetData());
            model->setModCarrierBytesLength((size_t) imageBytesCount);
            wxString bitPattern = _(model->getModBitPattern());
            view->getBitpatternOutput()->SetValue(bitPattern);
            
            if (model->checkForHeaderSignature()) {
                std::cout << " found header with signature! " << std::endl;
                view->getDecodeBtn()->Enable(true);
                view->getDecodeMenuItem()->Enable(true);
            } else {
                wxMessageDialog notationDialog(NULL,
                        wxT("Geladenes Bild enthält keine versteckte Nachricht!"),
                        wxT("Info"), wxOK | wxICON_WARNING);
                notationDialog.CentreOnParent();
                notationDialog.ShowModal();
            }
        } else {
            // Bild ohne versteckter Nachricht wurde geladen
            view->getUnmodStaticBitmap()->SetBitmap(image);
            model->setUnmodCarrierBytes(image.GetData());
            model->setUnmodCarrierBytesLength(imageBytesCount);
            view->getEncodeBtn()->Enable(true);
            view->getEncodeMenuItem()->Enable(true);
            // Zeige maximale Länge der Nachricht
            view->getMaxTxtLengthOutput()->Clear();
            *(view->getMaxTxtLengthOutput()) << getMaxTextLength();
        }
        // FIXME: Show scrollbars in the right way!
        wxSize size = view->GetSize();
        view->SetSize(size.GetWidth() + 1, size.GetHeight() + 1);
        view->SetSize(size);
    }
}

/**
 * Öffnet ein FileDialog zur Speicherung des Bildes mit versteckter Nachricht.
 * 
 * @param event created on the gui.
 */
void SCPresenter::onSave(wxCommandEvent& event) {
    wxFileDialog dialog(view, _T("Save Image"), wxEmptyString, _T("top_secret.bmp"),
            _T("Bitmap (*.bmp)|*.bmp"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    dialog.SetFilterIndex(1);
    if (dialog.ShowModal() == wxID_OK) {
        wxBitmap modBitmap = view->getModStaticBitmap()->GetBitmap();
        modBitmap.SaveFile(dialog.GetPath(), wxBITMAP_TYPE_BMP);
        view->GetStatusBar()->SetStatusText("Image with secret massage saved - " + dialog.GetPath());
    }
}

/**
 * Versteckt die gegebene Nachricht in dem gegebenem Bild.
 * 
 * @param event created on the gui.
 */
void SCPresenter::onEncode(wxCommandEvent& event) {
    wxString message = view->getSecretMsgInput()->GetValue();
    wxImage newImage;
    if (message.IsEmpty()) {
        wxMessageDialog notationDialog(NULL,
                wxT("You are about to encrypt an empty message.\nAre you sure you want to continue?"),
                wxT("Notation"),
                wxYES_NO | wxICON_EXCLAMATION);
        notationDialog.CentreOnParent();

        switch (notationDialog.ShowModal()) {
            case wxID_YES:
                model->encode(message.ToStdString());
                newImage = view->getUnmodStaticBitmap()->GetBitmap().ConvertToImage();
                newImage.SetData(model->getModCarrierBytes());
                view->getModStaticBitmap()->SetBitmap(newImage);
                view->getSaveModImgBtn()->Enable(true);
                view->getSaveModImgMenuItem()->Enable(true);
                break;
            case wxID_NO:
                break;
        }
    } else if (wxAtoi(message) > getMaxTextLength()) {
        wxMessageDialog notationDialog(NULL,
                wxT("Sorry but your message is too long.\nSelect either a bigger image or type in a shorter message."),
                wxT("WARNING!"), wxOK | wxICON_WARNING);
        notationDialog.CentreOnParent();
        notationDialog.ShowModal();
    } else {
        std::cout << "get data for encode.." << std::endl;
        wxImage image = view->getUnmodStaticBitmap()->GetBitmap().ConvertToImage();
        std::cout << "get data.. " << std::endl;
        unsigned char* data = image.GetData();
        std::cout << "parse data.. " << std::endl;
        model->setUnmodCarrierBytes(data);
        model->encode(message.ToStdString());
        std::cout << " pattern: \n " << std::endl;
        wxString bitpattern = _(model->getModBitPattern());
        // FIXME: Bitpatterncreation takes too much time
        std::cout << bitpattern.ToStdString() << std::endl;
        view->getBitpatternOutput()->SetValue(bitpattern);
        std::cout << " set new image.. " << std::endl;
        view->getUnmodStaticBitmap()->GetSize().GetHeight();
        newImage = view->getUnmodStaticBitmap()->GetBitmap().ConvertToImage();
        newImage.SetData(model->getModCarrierBytes());
        view->getModStaticBitmap()->SetBitmap(newImage);
        std::cout << " new image is set.. " << std::endl;
        view->getSaveModImgBtn()->Enable(true);
        view->getSaveModImgMenuItem()->Enable(true);
    }
}

/**
 * Holt die Nachricht aus dem geladenen Bild.
 * 
 * @param event created on the gui.
 */
void SCPresenter::onDecode(wxCommandEvent& event) {
    std::string message = model->decode();
    view->getSecretMsgInput()->SetValue(_(message));
}

/**
 * Überprüft das Eingabefeld auf nicht zulässige Zeichen und 
 * ersetzt diese durch gültige Zeichen. Zusätzlich wird die 
 * angezeigte Länge der einegegebener Nachricht aktualisiert.
 * 
 * @param event created on the gui.
 */
void SCPresenter::onSecretMessageChange(wxCommandEvent& event) {
    // Ersetze alle nicht ASCII Zeichen
    wxString rawInput = view->getSecretMsgInput()->GetValue();
    wxString filtered = view->getSecretMsgInput()->GetValue();
    wxRegEx reg;
    if (reg.Compile(wxT("[ÄÖÜäöüß*]"))) {
        reg.Replace(&filtered, wxT("?")); // gemäß der Maske ersetzen
    }
    if (rawInput.compare(filtered) != 0) {
        view->getSecretMsgInput()->SetValue(filtered);
        view->getSecretMsgInput()->SetInsertionPointEnd();
    }
    // Aktualisiere die angezeigte Länge der Nachricht
    view->getTxtLengthOutput()->Clear();
    int size = view->getSecretMsgInput()->GetValue().size();
    *(view->getTxtLengthOutput()) << size;
}

void SCPresenter::onExit(wxCommandEvent& event) {
    view->Close(true);
}

void SCPresenter::onAbout(wxCommandEvent& event) {
    SCAboutDialog about(view);
    about.ShowModal();
}

int SCPresenter::getMaxTextLength() const {
    unsigned int maxTxtLength = 0;
    if (!view->getUnmodStaticBitmap()->GetBitmap().IsNull()) {
        unsigned int height = view->getUnmodStaticBitmap()->GetBitmap().GetHeight();
        unsigned int width = view->getUnmodStaticBitmap()->GetBitmap().GetWidth();
        maxTxtLength = (height * width * 3) / 8 - model->getHeaderSize();
    }
    return maxTxtLength;
}