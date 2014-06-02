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
#include "SCPresenter.h"
#include <iostream>


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
    view->SetMinSize(wxSize(1152, 864));
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
        view->getSaveModImgBtn()->Disable();
}

/**
 * Opens FileDialog to load an Bitmap where the encoded message should be stored.
 * @param event created on the gui.
 * @return true if no fatal errors occured.
 */
void SCPresenter::onLoad(wxCommandEvent& event) {
    wxFileDialog openDialog(view, _T("Load Image"), wxEmptyString, wxEmptyString,
#ifdef __WXMOTIF__
            _T("Image (*.bmp;*.jpg;*.jpeg;*.png;*.gif)|*.bmp;*.jpg;*.jpeg;*.png;*.gif")
#else
            _T("Image (*.bmp;*.jpg;*.jpeg;*.png;*.gif)|*.bmp;*.jpg;*.jpeg;*.png;*.gif")
#endif
            );
    openDialog.SetDirectory(wxGetHomeDir()); // OS independency
    openDialog.CentreOnParent();

    if (openDialog.ShowModal() == wxID_OK) {
        //view->setUnmodCarrierBitmap((openDialog.GetPath(), event.GetId());
    }
}

/**
 * Saves Bitmap with encoded message in filesystem when save button was clicked.
 * @param event created on the gui.
 * @return true if no fatal errors occured.
 */
void SCPresenter::onSave(wxCommandEvent& event) {
    // Save the Bitmap with encoded message as *.bmp
    wxFileDialog dialog(view, _T("Save Image"), wxEmptyString,
            _T("top_secret.bmp"),
            _T("Bitmap (*.bmp)|*.bmp"),
            wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    dialog.SetFilterIndex(1);

    if (dialog.ShowModal() == wxID_OK) {
        wxBitmap encBitmap;
        //encBitmap = view->getModCarrierBmp()->GetBitmap();
        encBitmap.SaveFile(dialog.GetPath(), wxBITMAP_TYPE_BMP);
        view->GetStatusBar()->SetStatusText("Image saved: " + dialog.GetPath(), 1);
    }
}

/**
 * Encodes the given message and sets the new Bitmap when encode button was clicked.
 * @param event created on the gui.
 * @return true if no fatal errors occured.
 */
void SCPresenter::onEncode(wxCommandEvent& event) {
    wxString message, wxMaxTxtLen;

    //message = view->getSecretMessageBox()->GetValue();
    //wxMaxTxtLen = TextCtrl3->GetValue();

    if (message.IsEmpty()) { // enthält der eingegebene Text Zeichen?
        wxMessageDialog notationDialog(NULL,
                wxT("You are about to encrypt an empty message.\nAre you sure you want to continue?"),
                wxT("Notation"),
                wxYES_NO | wxICON_EXCLAMATION);
        notationDialog.CentreOnParent();

        switch (notationDialog.ShowModal()) {
            case wxID_YES:
                //model->encode();
                break;
            case wxID_NO:
                break;
        }

    } else if (wxAtoi(message) > wxAtoi(wxMaxTxtLen)) { // passt der eingegebene Text ins Bild?
        wxMessageDialog notationDialog(NULL,
                wxT("Sorry but your message is too long.\nSelect either a bigger image or type in a shorter message."),
                wxT("Notation"),
                wxOK | wxICON_EXCLAMATION);
        notationDialog.CentreOnParent();
        notationDialog.ShowModal();
    } else {
        //model->encode();
    }
}

/**
 * Decodes the message from an Bitmap when decode button was clicked.
 * @param event created on the gui.
 * @return true if no fatal errors occured.
 */
void SCPresenter::onDecode(wxCommandEvent& event) {
    //model->decode();
    //setMessage...
}

/**
 * Sets actual message length in the length label.
 * @param event created on the gui.
 * @return true if no fatal errors occured.
 */
void SCPresenter::onSecretMessageChange(wxCommandEvent& event) {
    // view->getSecretMessageBox()->getValue()->getLength()...
    // view->setLength..
    std::cout << ".";
}

void SCPresenter::onExit(wxCommandEvent& event) {
    view->Close(true);
}

void SCPresenter::onAbout(wxCommandEvent& event) {
    wxMessageBox("SteganoCoder", "About SteganoCoder", wxOK | wxICON_INFORMATION);
}

std::string SCPresenter::getWXMOTIF() {
#ifdef __WXMOTIF__
    return "Bitmap (*.bmp)|*.bmp";
#else
    return "Bitmap (*.bmp;*.dib)|*.bmp;*.dib";
#endif
}