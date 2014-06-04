/* 
 * File:   SCView.cpp
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#include "SCView.h"

/**
 * Erstellt alle GUI-Elemente des Fensters.
 */
void SCView::create() {
    mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

    this->createMenuBar();
    this->createUpperLeftBox();
    this->createLowerLeftBox();
    this->createMiddleBox();
    this->createUpperRightBox();
    this->createLowerRightBox();

    statusBar = this->CreateStatusBar(1, wxST_SIZEGRIP, wxID_ANY);
}

/**
 * Sorgt dafür, dass die erzeugten Widgets an ihren rechten Platz gelangen.
 */
void SCView::doLayout() {
    wxBoxSizer* mainPanelSizer;
    wxBoxSizer* mainSizer;
    wxBoxSizer* leftSizer;
    wxGridSizer* middleSizer;
    wxBoxSizer* rightSizer;

    // Linke Seite
    this->layoutUpperLeftBox();
    this->layoutLowerLeftBox();
    leftSizer = new wxBoxSizer(wxVERTICAL);
    leftSizer->Add(unmodImgSizer, 1, wxALL | wxEXPAND, 5);
    leftSizer->Add(secretMsgSizer, 1, wxALL | wxEXPAND, 5);

    // Mitte
    middleSizer = new wxGridSizer(2, 1, 0, 0); // zwei Zeilen, eine Spalte
    middleSizer->Add(encodeBtn, 0, wxALIGN_CENTER | wxALL, 5);
    middleSizer->Add(decodeBtn, 0, wxALIGN_CENTER | wxALL, 5);

    // Rechte Seite
    this->layoutUpperRightBox();
    this->layoutLowerRightBox();
    rightSizer = new wxBoxSizer(wxVERTICAL);
    rightSizer->Add(modImgSizer, 1, wxALL | wxEXPAND, 5);
    rightSizer->Add(bitPatternSizer, 1, wxALL | wxEXPAND, 5);

    // Main Sizer
    mainSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(leftSizer, 1, wxEXPAND, 5);
    mainSizer->Add(middleSizer, 0, wxEXPAND, 5);
    mainSizer->Add(rightSizer, 1, wxEXPAND, 5);
    mainPanel->SetSizer(mainSizer);
    mainPanel->Layout();

    mainSizer->Fit(mainPanel);
    mainPanelSizer = new wxBoxSizer(wxHORIZONTAL);
    mainPanelSizer->Add(mainPanel, 1, wxEXPAND, 0);
    this->SetSizer(mainPanelSizer);
    this->Layout();
}

void SCView::createMenuBar() {
    // File
    wxMenu *menuFile = new wxMenu;    
    wxMenuItem *loadUnmodImgMenuItem = new wxMenuItem(menuFile, ID_LOAD_UNMOD_IMG, 
            "&Load Unmodified Image...\tCtrl-U", "Load an image that you want to encode");
    loadUnmodImgMenuItem->SetBitmap(wxImage(_("images/open16x16.png")));
    menuFile->Append(loadUnmodImgMenuItem);
    
    menuFile->AppendSeparator();
    wxMenuItem *loadModImgMenuItem = new wxMenuItem(menuFile, ID_LOAD_MOD_IMG, 
                "&Load Modified Image...\tCtrl-M", "Load an image that contains a secret message");
    loadModImgMenuItem->SetBitmap(wxImage(_("images/open16x16.png")));
    menuFile->Append(loadModImgMenuItem);
    
    saveModImgMenuItem = new wxMenuItem(menuFile, ID_SAVE_MOD_IMG, 
            "&Save Modified Image...\tCtrl-S", "Save an image that contains a secret message");
    saveModImgMenuItem->SetBitmap(wxImage(_("images/save16x16.png")));
    menuFile->Append(saveModImgMenuItem);

    menuFile->AppendSeparator();
    wxMenuItem *exitMenuItem = new wxMenuItem(menuFile, wxID_EXIT);
    exitMenuItem->SetBitmap(wxImage(_("images/exit16x16.png")));
    menuFile->Append(exitMenuItem);

    // Edit
    wxMenu *menuEdit = new wxMenu;
    encodeMenuItem = new wxMenuItem(menuEdit, ID_ENCODE, "&Encode\tCtrl-E",
            "Encode the unmodified image with the secret message");
    encodeMenuItem->SetBitmap(wxImage(_("images/encode16x16.png")));
    menuEdit->Append(encodeMenuItem);
    decodeMenuItem = new wxMenuItem(menuEdit, ID_DECODE, "&Decode\tCtrl-D",
            "Decode the modified image which contains a secret message");
    decodeMenuItem->SetBitmap(wxImage(_("images/decode16x16.png")));
    menuEdit->Append(decodeMenuItem);

    // Help
    wxMenu *menuHelp = new wxMenu;
    wxMenuItem *aboutMenuItem = new wxMenuItem(menuHelp, wxID_ABOUT);
    aboutMenuItem->SetBitmap(wxImage(_("images/about16x16.png")));
    menuHelp->Append(aboutMenuItem);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuEdit, "&Edit");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
}

/**
 * Dient zum Erstellen der GUI-Elemente des oberen linken Areals; dort, wo
 * das unmodifizierte Bild seinen Platz findet.
 */
void SCView::createUpperLeftBox() {
    unmodImgScrolledWindow = new wxScrolledWindow(this->mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxSTATIC_BORDER | wxVSCROLL);
    unmodImgScrolledWindow->SetScrollRate(5, 5);
    unmodStaticBitmap = new wxStaticBitmap(unmodImgScrolledWindow, wxID_ANY, wxBitmap(0, 0, -1), wxDefaultPosition, wxDefaultSize, 0);
    loadUnmodImgBtn = new wxButton(mainPanel, ID_LOAD_UNMOD_IMG, _("Load Image..."), wxDefaultPosition, wxDefaultSize, 0);
    loadUnmodImgBtn->SetDefault();
    unmodImgSeparator = new wxStaticLine(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
    maxTxtLengthLabel = new wxStaticText(mainPanel, wxID_ANY, _("Max Text Length:"), wxDefaultPosition, wxDefaultSize, 0);
    maxTxtLengthLabel->Wrap(-1);
    maxTxtLengthOutput = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(0, -1), wxTE_READONLY);
}

/**
 * Erzeugung der GUI-Elemente unten links. Hierzu zählt ein TextCtrl mit der zu
 * verschlüsselnden bzw. entschlüsselten Nachricht.
 */
void SCView::createLowerLeftBox() {
    secretMsgInput = new wxTextCtrl(mainPanel, ID_SECRET_MSG, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    txtLengthLabel = new wxStaticText(mainPanel, wxID_ANY, _("Text Length:"), wxDefaultPosition, wxSize( -1,-1 ), 0);
    txtLengthOutput = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1), wxTE_READONLY);
    txtLengthLabel->Wrap(-1); // kein Wrapping
}

/**
 * Erzeugt die Buttons zum Ko- und Dekodieren von Bildern.
 */
void SCView::createMiddleBox() {
    encodeBtn = new wxButton(mainPanel, ID_ENCODE, _("Encode"), wxDefaultPosition, wxDefaultSize, 0);
    encodeBtn->SetBitmap(wxImage(_("images/encode16x16.png")), wxRIGHT);
    decodeBtn = new wxButton(mainPanel, ID_DECODE, _("Decode"), wxDefaultPosition, wxDefaultSize, 0);
    decodeBtn->SetBitmap(wxImage(_("images/decode16x16.png")), wxLEFT);
}

/**
 * Die GUI-Elemente für das verschlüsselte Bild werden hier erstellt.
 */
void SCView::createUpperRightBox() {
    modImgScrolledWindow = new wxScrolledWindow(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxSTATIC_BORDER | wxVSCROLL);
    modImgScrolledWindow->SetScrollRate(5, 5);
    modStaticBitmap = new wxStaticBitmap(modImgScrolledWindow, wxID_ANY, wxBitmap(0, 0, -1), wxDefaultPosition, wxDefaultSize, 0);
    loadModImgBtn = new wxButton(mainPanel, ID_LOAD_MOD_IMG, _("Load Image..."), wxDefaultPosition, wxDefaultSize, 0);
    modImgSeparator = new wxStaticLine(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
    saveModImgBtn = new wxButton(mainPanel, ID_SAVE_MOD_IMG, _("Save Image..."), wxDefaultPosition, wxDefaultSize, 0);
}

/**
 * Das Bitmuster des modifizierten Bilds wird hier erzeugt.
 */
void SCView::createLowerRightBox() {
    bitPatternOutput = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
}

/**
 * Erstellt das Layout für das unmodidzierte Bild.
 */
void SCView::layoutUpperLeftBox() {
    unmodImgSizer = new wxStaticBoxSizer(new wxStaticBox(mainPanel, wxID_ANY, _("Unmodified Image")), wxVERTICAL);
    wxBoxSizer* unmodImgScrolledSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* unmodImgInfoSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* maxTxtLengthSizer = new wxBoxSizer(wxHORIZONTAL);

    unmodImgScrolledSizer->Add(unmodStaticBitmap, 1, wxALL | wxEXPAND, 5);
    unmodImgScrolledWindow->SetSizer(unmodImgScrolledSizer);
    unmodImgScrolledWindow->Layout();
    unmodImgScrolledSizer->Fit(unmodImgScrolledWindow);

    maxTxtLengthSizer->Add(maxTxtLengthLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    maxTxtLengthSizer->Add(maxTxtLengthOutput, 1, wxALL | wxEXPAND, 5);

    unmodImgInfoSizer->Add(loadUnmodImgBtn, 1, wxALL, 5);
    unmodImgInfoSizer->Add(unmodImgSeparator, 0, wxEXPAND | wxALL, 5);
    unmodImgInfoSizer->Add(maxTxtLengthSizer, 1, wxALIGN_CENTER_VERTICAL, 5);

    unmodImgSizer->Add(unmodImgScrolledWindow, 1, wxEXPAND | wxALL, 5);
    unmodImgSizer->Add(unmodImgInfoSizer, 0, wxEXPAND, 5);
}

/**
 * Hier erfolgt das Layout für die zu verschlüsselnde/entschlüsselte Nachricht.
 */
void SCView::layoutLowerLeftBox() {
    secretMsgSizer = new wxStaticBoxSizer(new wxStaticBox(mainPanel, wxID_ANY, _("Secret Message")), wxVERTICAL);
    wxBoxSizer* txtLengthSizer = new wxBoxSizer(wxHORIZONTAL);

    txtLengthSizer->Add(txtLengthLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    txtLengthSizer->Add(txtLengthOutput, 1, wxALL | wxEXPAND, 5);
    txtLengthSizer->Add(0, 0, 1, wxEXPAND, 5); // Spacer hinzufügen

    secretMsgSizer->Add(secretMsgInput, 1, wxALL | wxEXPAND, 5);
    secretMsgSizer->Add(txtLengthSizer, 0, wxEXPAND, 5);
}

/**
 * Das Layout für das modifizierte Bild wir hier erstellt.
 */
void SCView::layoutUpperRightBox() {
    modImgSizer = new wxStaticBoxSizer(new wxStaticBox(mainPanel, wxID_ANY, _("Modified Image")), wxVERTICAL);
    wxBoxSizer* modImgScrolledSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* modImgBtnSizer = new wxBoxSizer(wxHORIZONTAL);

    modImgScrolledSizer->Add(modStaticBitmap, 1, wxALL | wxEXPAND, 5);
    modImgScrolledWindow->SetSizer(modImgScrolledSizer);
    modImgScrolledWindow->Layout();
    modImgScrolledSizer->Fit(modImgScrolledWindow);

    modImgBtnSizer->Add(loadModImgBtn, 1, wxALL | wxEXPAND, 5);
    modImgBtnSizer->Add(modImgSeparator, 0, wxEXPAND | wxALL, 5);
    modImgBtnSizer->Add(saveModImgBtn, 1, wxALL | wxEXPAND, 5);

    modImgSizer->Add(modImgScrolledWindow, 1, wxEXPAND | wxALL, 5);
    modImgSizer->Add(modImgBtnSizer, 0, wxEXPAND, 5);
}

/**
 * Das TextCtrl für das Bitmuster kommt hier an seinen Platz.
 */
void SCView::layoutLowerRightBox() {
    bitPatternSizer = new wxStaticBoxSizer(new wxStaticBox(mainPanel, wxID_ANY, _("Bit Pattern")), wxVERTICAL);
    bitPatternSizer->Add(bitPatternOutput, 1, wxALL | wxEXPAND, 5);
}

SCView::~SCView() {
}

