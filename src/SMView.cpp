/* 
 * File:   SMView.cpp
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#include "SMView.h"

/**
 * Erstellt alle GUI-Elemente des Fensters.
 */
void SMView::create() {
    aboutDialog = new SMAboutDialog(this);
    mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

    this->SetTitle(TEXT_TITLE);
    this->SetIcon(wxICON(appicon));
    this->createMenuBar();
    this->createUpperLeftBox();
    this->createLowerLeftBox();
    this->createMiddleBox();
    this->createUpperRightBox();
    this->createLowerRightBox();

    statusBar = new SMStatusBar(this, wxSTB_DEFAULT_STYLE);
    this->setStatusBarText(TEXT_WELCOME);
    this->SetStatusBar(statusBar);
}

/**
 * Sorgt dafür, dass die erzeugten Widgets an ihren rechten Platz gelangen.
 */
void SMView::doLayout() {
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

/**
 * Funktion zum Anzeigen eines SplashScreens, die zum Start des Programms 
 * aufgerufen wird.
 */
void SMView::showSplashScreen() {
    wxBitmap bitmap(wxBITMAP_PNG(monkey_splash));
    // Code::Blocks SplashScreen zum Darstellen von transparenten Bitmaps
    cbSplashScreen* splash = new cbSplashScreen(bitmap, -1, this, wxID_ANY);
    ::wxMilliSleep(3000);
    splash->Destroy();
}

/**
 * Erzeugt eine MenuBar, die dem Benutzer Programm-Interaktionsmögichkeiten 
 * bereitstellt.
 */
void SMView::createMenuBar() {
    // File
    wxMenu *menuFile = new wxMenu;
    wxMenuItem *loadUnmodImgMenuItem = new wxMenuItem(menuFile, ID_LOAD_UNMOD_IMG,
            MENUITEM_LOAD_UNMOD_IMG_TEXT, MENUITEM_LOAD_UNMOD_IMG_HELP);
    loadUnmodImgMenuItem->SetBitmap(wxBITMAP_PNG(open16x16));
    menuFile->Append(loadUnmodImgMenuItem);
    menuFile->AppendSeparator();

    wxMenuItem *loadModImgMenuItem = new wxMenuItem(menuFile, ID_LOAD_MOD_IMG,
            MENUITEM_LOAD_MOD_IMG_TEXT, MENUITEM_LOAD_MOD_IMG_HELP);
    loadModImgMenuItem->SetBitmap(wxBITMAP_PNG(open16x16));
    menuFile->Append(loadModImgMenuItem);

    saveModImgMenuItem = new wxMenuItem(menuFile, ID_SAVE_MOD_IMG,
            MENUITEM_SAVE_MOD_IMG_TEXT, MENUITEM_SAVE_MOD_IMG_HELP);
    saveModImgMenuItem->SetBitmap(wxBITMAP_PNG(save16x16));
    menuFile->Append(saveModImgMenuItem);
    menuFile->AppendSeparator();

    wxMenuItem *exitMenuItem = new wxMenuItem(menuFile, wxID_EXIT);
    exitMenuItem->SetBitmap(wxBITMAP_PNG(exit16x16));
    menuFile->Append(exitMenuItem);

    // Edit
    wxMenu *menuEdit = new wxMenu;
    encodeMenuItem = new wxMenuItem(menuEdit, ID_ENCODE, MENUITEM_ENCODE_TEXT, MENUITEM_ENCODE_HELP);
    encodeMenuItem->SetBitmap(wxBITMAP_PNG(encode16x16));
    menuEdit->Append(encodeMenuItem);
    decodeMenuItem = new wxMenuItem(menuEdit, ID_DECODE, MENUITEM_DECODE_TEXT, MENUITEM_DECODE_HELP);
    decodeMenuItem->SetBitmap(wxBITMAP_PNG(decode16x16));
    menuEdit->Append(decodeMenuItem);

    // Help
    wxMenu *menuHelp = new wxMenu;
    wxMenuItem *aboutMenuItem = new wxMenuItem(menuHelp, wxID_ABOUT);
    aboutMenuItem->SetBitmap(wxBITMAP_PNG(about16x16));
    menuHelp->Append(aboutMenuItem);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, MENU_FILE);
    menuBar->Append(menuEdit, MENU_EDIT);
    menuBar->Append(menuHelp, MENU_HELP);
    SetMenuBar(menuBar);
}

/**
 * Dient zum Erstellen der GUI-Elemente des oberen linken Areals; dort, wo
 * das unmodifizierte Bild seinen Platz findet.
 */
void SMView::createUpperLeftBox() {
    unmodImgScrolledWindow = new wxScrolledWindow(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxSTATIC_BORDER | wxVSCROLL);
    unmodImgScrolledWindow->SetScrollRate(1, 1);
    unmodStaticBitmap = new wxStaticBitmap(unmodImgScrolledWindow, wxID_ANY, wxBitmap(0, 0, -1), wxDefaultPosition, wxDefaultSize, 0);
    loadUnmodImgBtn = new wxButton(mainPanel, ID_LOAD_UNMOD_IMG, BTN_LOAD_IMG, wxDefaultPosition, wxDefaultSize, 0);
    loadUnmodImgBtn->SetDefault();
    unmodImgSeparator = new wxStaticLine(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
    maxTxtLengthLabel = new wxStaticText(mainPanel, wxID_ANY, TEXT_MAX_TXT_LENGTH, wxDefaultPosition, wxDefaultSize, 0);
    maxTxtLengthLabel->Wrap(-1);
    maxTxtLengthOutput = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(0, -1), wxTE_READONLY);
}

/**
 * Erzeugung der GUI-Elemente unten links. Hierzu zählt ein TextCtrl mit der zu
 * verschlüsselnden bzw. entschlüsselten Nachricht.
 */
void SMView::createLowerLeftBox() {
    secretMsgInput = new wxTextCtrl(mainPanel, ID_SECRET_MSG, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    txtLengthLabel = new wxStaticText(mainPanel, wxID_ANY, TEXT_TXT_LENGTH, wxDefaultPosition, wxSize(-1, -1), 0);
    txtLengthOutput = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1), wxTE_READONLY);
    txtLengthLabel->Wrap(-1); // kein Wrapping
}

/**
 * Erzeugt die Buttons zum En- und Dekodieren von Bildern.
 */
void SMView::createMiddleBox() {
    encodeBtn = new wxButton(mainPanel, ID_ENCODE, BTN_ENCODE, wxDefaultPosition, wxDefaultSize, 0);
    encodeBtn->SetBitmap(wxBITMAP_PNG(encode16x16), wxRIGHT);
    decodeBtn = new wxButton(mainPanel, ID_DECODE, BTN_DECODE, wxDefaultPosition, wxDefaultSize, 0);
    decodeBtn->SetBitmap(wxBITMAP_PNG(decode16x16), wxLEFT);
}

/**
 * Erstellt die GUI-Elemente für das verschlüsselte Bild.
 */
void SMView::createUpperRightBox() {
    modImgScrolledWindow = new wxScrolledWindow(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxSTATIC_BORDER | wxVSCROLL);
    modImgScrolledWindow->SetScrollRate(1, 1);
    modStaticBitmap = new wxStaticBitmap(modImgScrolledWindow, wxID_ANY, wxBitmap(0, 0, -1), wxDefaultPosition, wxDefaultSize, 0);
    loadModImgBtn = new wxButton(mainPanel, ID_LOAD_MOD_IMG, BTN_LOAD_IMG, wxDefaultPosition, wxDefaultSize, 0);
    modImgSeparator = new wxStaticLine(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
    saveModImgBtn = new wxButton(mainPanel, ID_SAVE_MOD_IMG, BTN_SAVE_IMG, wxDefaultPosition, wxDefaultSize, 0);
}

/**
 * Das Bitmuster des modifizierten Bildes wird hier erzeugt.
 */
void SMView::createLowerRightBox() {
    bitPatternOutput = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    bitPatternOutput->SetEditable(false);
}

/**
 * Erstellt das Layout für das unmodifizierte Bild.
 */
void SMView::layoutUpperLeftBox() {
    unmodImgSizer = new wxStaticBoxSizer(new wxStaticBox(mainPanel, wxID_ANY, TEXT_UNMOD_IMG), wxVERTICAL);
    wxBoxSizer* unmodImgScrolledSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* unmodImgInfoSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* maxTxtLengthSizer = new wxBoxSizer(wxHORIZONTAL);

    unmodImgScrolledSizer->Add(unmodStaticBitmap, 1, wxALIGN_CENTER | wxEXPAND, 0);
    unmodImgScrolledWindow->SetSizer(unmodImgScrolledSizer);

    maxTxtLengthSizer->Add(maxTxtLengthLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    maxTxtLengthSizer->Add(maxTxtLengthOutput, 1, wxALL | wxEXPAND, 5);

    unmodImgInfoSizer->Add(loadUnmodImgBtn, 1, wxALL, 5);
    unmodImgInfoSizer->Add(unmodImgSeparator, 0, wxEXPAND | wxALL, 5);
    unmodImgInfoSizer->Add(maxTxtLengthSizer, 1, wxALIGN_CENTER_VERTICAL, 5);

    unmodImgSizer->Add(unmodImgScrolledWindow, 1, wxEXPAND | wxALL | wxALIGN_CENTER, 5);
    unmodImgSizer->Add(unmodImgInfoSizer, 0, wxEXPAND, 5);
}

/**
 * Hier erfolgt das Layout für die zu verschlüsselnde/entschlüsselte Nachricht.
 */
void SMView::layoutLowerLeftBox() {
    secretMsgSizer = new wxStaticBoxSizer(new wxStaticBox(mainPanel, wxID_ANY, TEXT_SECRET_MSG), wxVERTICAL);
    wxBoxSizer* txtLengthSizer = new wxBoxSizer(wxHORIZONTAL);

    txtLengthSizer->Add(txtLengthLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    txtLengthSizer->Add(txtLengthOutput, 1, wxALL | wxEXPAND, 5);
    txtLengthSizer->Add(0, 0, 1, wxEXPAND, 5); // Spacer hinzufügen

    secretMsgSizer->Add(secretMsgInput, 1, wxALL | wxEXPAND, 5);
    secretMsgSizer->Add(txtLengthSizer, 0, wxEXPAND, 5);
}

/**
 * Das Layout für das modifizierte Bild wird hier erstellt.
 */
void SMView::layoutUpperRightBox() {
    modImgSizer = new wxStaticBoxSizer(new wxStaticBox(mainPanel, wxID_ANY, TEXT_MOD_IMG), wxVERTICAL);
    wxBoxSizer* modImgScrolledSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* modImgBtnSizer = new wxBoxSizer(wxHORIZONTAL);

    modImgScrolledSizer->Add(modStaticBitmap, 1, wxALIGN_CENTER | wxEXPAND, 0);
    modImgScrolledWindow->SetSizer(modImgScrolledSizer);

    modImgBtnSizer->Add(loadModImgBtn, 1, wxALL | wxEXPAND, 5);
    modImgBtnSizer->Add(modImgSeparator, 0, wxEXPAND | wxALL, 5);
    modImgBtnSizer->Add(saveModImgBtn, 1, wxALL | wxEXPAND, 5);

    modImgSizer->Add(modImgScrolledWindow, 1, wxEXPAND | wxALL, 5);
    modImgSizer->Add(modImgBtnSizer, 0, wxEXPAND, 5);
}

/**
 * Das TextCtrl für das Bitmuster kommt hier an seinen Platz.
 */
void SMView::layoutLowerRightBox() {
    bitPatternSizer = new wxStaticBoxSizer(new wxStaticBox(mainPanel, wxID_ANY, TEXT_BIT_PATERN), wxVERTICAL);
    bitPatternSizer->Add(bitPatternOutput, 1, wxALL | wxEXPAND, 5);
}

/**
 * Dient zum Setzen eines ganz gewönlichen Textes in der StatusBar.
 * @param text der darzustellende StatusBar-Text
 */
void SMView::setStatusBarText(const wxString& text) {
    statusBar->setStatusStyle(SMStatusBar::STATUS_NORMAL);
    statusBar->SetStatusText(text);
}

/**
 * Erzeugt eine Fehlerausgabe mit einer rot hinterlegten StatusBar und stellt
 * somit eine alternative Herangehensweise zu den alt hergebrachten 
 * Fehlerdialogen dar.
 * @param text der darzustellende Fehlertext
 */
void SMView::setStatusBarErrorText(const wxString& text) {
    statusBar->setStatusStyle(SMStatusBar::STATUS_ERROR);
    statusBar->SetStatusText(text);
}

/**
 * Überschriebene Methode von wxFrame. Dient zur Darstellung von Hilfetexten
 * in der StatusBar, die beim Hovern über ein MenuItem ausgelöst werden. 
 * Hierbei muss sichergestellt werden, dass Text, der vorher explizit auf der
 * StatusBar gesetzt wurde, nach Beendigung des Hovervorgangs auch wieder
 * zurückgesetzt wird.
 * @param help der anzuzeigende Hilfetext
 * @param show gibt an, ob ein Menü geöffnet wurde
 */
void SMView::DoGiveHelp(const wxString& help, bool menuShown) {
    // nur anzeigen, wenn gerade keine Fehlermeldung auf der StatusBar ausgegeben wird
    if (statusBar->getStatusStyle() != SMStatusBar::STATUS_ERROR) {
        wxString text;  // der zu setzende StatusText
        if (menuShown) {  // ein Menü wurde geöffnet
            if (oldStatusText.empty()) { 
                // den alten Wert abspeichern, wenn noch nicht gesetzt
                oldStatusText = statusBar->GetStatusText();
                if (oldStatusText.empty()) {  
                    // wenn der momentane StatusText leer ist, schließen wir
                    // diesen mit einem Nullzeichen ab, sodass 
                    // "oldStatusText.empty() = false" wird
                    oldStatusText += _('\0');
                }
            }
            this->lastHelpShown = text = help;
        } else {    // ein Menü wurde geschlossen
            wxString lastHelpShown;
            // den zuletzt angezeigten Hilfetext löschen, den Wert jedoch noch 
            // merken
            lastHelpShown.swap(this->lastHelpShown);

            // den alten Text löschen und den Wert merken, um ihn weiter unten 
            // setzen zu können
            text.swap(oldStatusText);

            // wenn der StatusText nicht mit dem zuletzt angezeigten Hilfetext
            // übereinstimmt, bedeutet es, dass dieser währenddessen explizit
            // gesetzt wurde und somit nicht überschrieben werden sollte
            if (statusBar->GetStatusText() != lastHelpShown) {
                return;
            }
        }
        statusBar->SetStatusText(text);
    }
}

