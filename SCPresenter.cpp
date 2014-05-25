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

/**
 * Main of steganocoder. 
 */
bool SCPresenter::OnInit() {
    std::cout << "``::: Initialize SteganoCoder :::´´" << std::endl;

    this->init();
    view->getFrame()->Layout();
    view->getFrame()->CreateStatusBar();
    view->getFrame()->SetStatusText(_T("Welcome to SteganoCoder!"));

    view->getFrame()->Show(true);
    view->getFrame()->Centre(wxBOTH);
    SetTopWindow(view->getFrame());

    std::cout << "´´::: SteganoCoder initialized :::``" << std::endl;
    return true;
}

/**
 * Additional initialisation steps.
 * Connects gui actions with kernel actions.
 */
void SCPresenter::init() {
    // Open button clicked -> onOpen
    view->getFrame()->Connect(view->getFrame()->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) & SCPresenter::onOpen());

    wxYieldIfNeeded(); // Do we need this?
    wxInitAllImageHandlers();

    // view->getSaveBtn()->disable();

}

/**
 * Opens FileDialog to load an Bitmap where the encoded message should be stored.
 * @param event created on the gui.
 * @return true if no fatal errors occured.
 */
bool SCPresenter::onOpen(wxCommandEvent& event) {
    wxFileDialog openDialog(view->getFrame(), _T("Load Image"), wxEmptyString, wxEmptyString, getWXMOTIF());
    openDialog.SetDirectory(wxGetHomeDir()); // OS independency
    openDialog.CentreOnParent();

    if (openDialog.ShowModal() == wxID_OK) {
        //view->setUnmodCarrierBitmap((openDialog.GetPath(), event.GetId());
    }
}

/**
 * Called when gui frame was closed.
 * @param event created on the gui.
 * @return true if no fatal errors occured.
 */
bool SCPresenter::onClose(wxCommandEvent& event) {
    return true;
}

/**
 * Called when exit button was clicked.
 * @param event created on the gui.
 * @return true if no fatal errors occured.
 */
bool SCPresenter::onExit(wxCommandEvent& event) {
    bool frameClosed = view->getFrame()->Close();
    bool others = true;
    return frameClosed && others;
}

/**
 * Called when an Bitmap was loaded.
 * @param event created on the gui.
 * @return true if no fatal errors occured.
 */
bool SCPresenter::onLoad(wxCommandEvent& event) {
    return true;
}

/**
 * Saves Bitmap with encoded message in filesystem when save button was clicked.
 * @param event created on the gui.
 * @return true if no fatal errors occured.
 */
bool SCPresenter::onSave(wxCommandEvent& event) {
    // Save the Bitmap with encoded message as *.bmp
    wxFileDialog dialog(view->getFrame(), _T("Save Image"), wxEmptyString,
            _T("top_secret.bmp"),
            _T("Bitmap (*.bmp)|*.bmp"),
            wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    dialog.SetFilterIndex(1);

    if (dialog.ShowModal() == wxID_OK) {
        wxBitmap encBitmap;
        //encBitmap = view->getModCarrierBmp()->GetBitmap();
        encBitmap.SaveFile(dialog.GetPath(), wxBITMAP_TYPE_BMP);
        view->getFrame()->GetStatusBar()->SetStatusText("Image saved: " + dialog.GetPath(), 1);
    }
    return true;
}

/**
 * Encodes the given message and sets the new Bitmap when encode button was clicked.
 * @param event created on the gui.
 * @return true if no fatal errors occured.
 */
bool SCPresenter::onEncode(wxCommandEvent& event) {
    wxString message, wxMaxTxtLen;

    //message = view->getSecretMessageBox()->GetValue();
    //wxMaxTxtLen = TextCtrl3->GetValue();

    if (message->IsEmpty()) { // enthält der eingegebene Text Zeichen?
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
                wxT("Sorry but your message is too long.\nSelect either a bigger image or type in a briefer message."),
                wxT("Notation"),
                wxOK | wxICON_EXCLAMATION);
        notationDialog.CentreOnParent();
        notationDialog.ShowModal();
    } else {
        //model->encode();
    }
    return true;
}

/**
 * Decodes the message from an Bitmap when decode button was clicked.
 * @param event created on the gui.
 * @return true if no fatal errors occured.
 */
bool SCPresenter::onDecode(wxCommandEvent& event) {
    //model->decode();
    //setMessage...
    return true;
}

/**
 * Sets actual message length in the length label.
 * @param event created on the gui.
 * @return true if no fatal errors occured.
 */
bool SCPresenter::onSecretMessageChange(wxCommandEvent& event) {
    // view->getSecretMessageBox()->getValue()->getLength()...
    // view->setLength..
    return true;
}

_T getWXMOTIF() {
#ifdef __WXMOTIF__
    return _T("Bitmap (*.bmp)|*.bmp");
#else
    return _T("Bitmap (*.bmp;*.dib)|*.bmp;*.dib");
#endif
}