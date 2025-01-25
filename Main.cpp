#include <wx/wx.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>

class MainFrame : public wxFrame {
public:
    MainFrame();

private:
    // Menu selection items
    wxButton* menuButton;
    wxButton* placeOrderButton;
    wxStaticText* totalBillText;
    wxButton* playGameButton;
    wxButton* exitButton;

    // Prices and menu options
    std::vector<std::pair<wxString, int>> menuItems;
    int totalBill;
    int attempts;
    std::map<wxString, wxString> puzzleQuestions;
    wxString currentAnswer;

    void OnMenuClick(wxCommandEvent& event);
    void OnItemSelect(wxCommandEvent& event);
    void OnPlaceOrder(wxCommandEvent& event);
    void OnGamePrompt(wxCommandEvent& event);
    void OnPlayGame(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void PuzzleGame(wxDialog* parentDialog);

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(1001, MainFrame::OnMenuClick)
EVT_BUTTON(2001, MainFrame::OnPlaceOrder)
EVT_BUTTON(3001, MainFrame::OnPlayGame)
EVT_BUTTON(3002, MainFrame::OnExit)
wxEND_EVENT_TABLE()

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MainFrame* frame = new MainFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "Cafeteria Management System", wxDefaultPosition, wxSize(400, 300)) {

    totalBill = 0;
    attempts = 3;

    // Initial menu options and prices
    menuItems = { {"Coffee", 5}, {"Steak", 20}, {"Pasta", 15} };

    // Puzzle questions and answers
    puzzleQuestions = {
        {"What is the shortcut key for copying text?", "ctrl+c"},
        {"What does 'RAM' stand for?", "random access memory"},
        {"What is the primary language used for web development?", "html"}
    };

    // Welcome message and button to go to menu
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    wxStaticText* welcomeText = new wxStaticText(panel, wxID_ANY, "Welcome to BAM Cafe!", wxPoint(120, 20));
    menuButton = new wxButton(panel, 1001, "Menu", wxPoint(150, 80), wxSize(100, 50));
}

void MainFrame::OnMenuClick(wxCommandEvent& event) {
    wxDialog* menuDialog = new wxDialog(this, wxID_ANY, "Menu", wxDefaultPosition, wxSize(300, 300));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Create buttons for each menu item
    for (size_t i = 0; i < menuItems.size(); ++i) {
        wxButton* itemButton = new wxButton(menuDialog, wxID_HIGHEST + i, menuItems[i].first + " - $" + std::to_string(menuItems[i].second));
        sizer->Add(itemButton, 0, wxALL | wxEXPAND, 5);
        itemButton->Bind(wxEVT_BUTTON, &MainFrame::OnItemSelect, this);
    }

    // Fix Place Order button
    placeOrderButton = new wxButton(menuDialog, 2001, "Place Order", wxDefaultPosition, wxSize(100, 30));
    placeOrderButton->Bind(wxEVT_BUTTON, &MainFrame::OnPlaceOrder, this);  // Bind properly
    sizer->Add(placeOrderButton, 0, wxALL | wxALIGN_CENTER, 10);

    menuDialog->SetSizer(sizer);
    menuDialog->ShowModal();
}

void MainFrame::OnItemSelect(wxCommandEvent& event) {
    int id = event.GetId() - wxID_HIGHEST;
    if (id >= 0 && id < (int)menuItems.size()) {
        totalBill += menuItems[id].second;
        wxMessageBox(menuItems[id].first + " added to your order.", "Item Selected", wxOK | wxICON_INFORMATION);
    }
}

void MainFrame::OnPlaceOrder(wxCommandEvent& event) {
    wxMessageBox("Your total bill is $" + std::to_string(totalBill) + ".\nYour order will take 45 minutes.",
        "Order Placed", wxOK | wxICON_INFORMATION);

    wxDialog* promptDialog = new wxDialog(this, wxID_ANY, "Play a Game?", wxDefaultPosition, wxSize(300, 200));
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* promptText = new wxStaticText(promptDialog, wxID_ANY, "Would you like to play a game while waiting?", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    sizer->Add(promptText, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);

    playGameButton = new wxButton(promptDialog, 3001, "Yes, Play Game", wxDefaultPosition, wxSize(120, 30));
    exitButton = new wxButton(promptDialog, 3002, "No, Exit", wxDefaultPosition, wxSize(120, 30));

    sizer->Add(playGameButton, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
    sizer->Add(exitButton, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

    // Bind the buttons to their respective handlers
    playGameButton->Bind(wxEVT_BUTTON, &MainFrame::OnPlayGame, this);
    exitButton->Bind(wxEVT_BUTTON, &MainFrame::OnExit, this);

    promptDialog->SetSizer(sizer);
    promptDialog->ShowModal();
}

void MainFrame::OnGamePrompt(wxCommandEvent& event) {
    wxDialog* puzzleDialog = new wxDialog(this, wxID_ANY, "Puzzle Game", wxDefaultPosition, wxSize(400, 300));
    PuzzleGame(puzzleDialog);
    puzzleDialog->ShowModal();
}

void MainFrame::OnPlayGame(wxCommandEvent& event) {
    wxDialog* puzzleDialog = new wxDialog(this, wxID_ANY, "Puzzle Game", wxDefaultPosition, wxSize(400, 300));
    PuzzleGame(puzzleDialog);
    puzzleDialog->ShowModal();
}

void MainFrame::OnExit(wxCommandEvent& event) {
    wxMessageBox("Thank you for ordering from BAM Cafe!", "Goodbye", wxOK | wxICON_INFORMATION);
    Close();
}

void MainFrame::PuzzleGame(wxDialog* parentDialog) {
    wxPanel* panel = new wxPanel(parentDialog, wxID_ANY);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    auto it = puzzleQuestions.begin();
    std::advance(it, rand() % puzzleQuestions.size());
    wxString question = it->first;
    currentAnswer = it->second;

    wxStaticText* questionText = new wxStaticText(panel, wxID_ANY, question, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    wxTextCtrl* answerInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 25));
    wxButton* submitButton = new wxButton(panel, wxID_ANY, "Submit", wxDefaultPosition, wxSize(100, 30));

    sizer->Add(questionText, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
    sizer->Add(answerInput, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
    sizer->Add(submitButton, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

    panel->SetSizer(sizer);

    submitButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
        wxString userAnswer = answerInput->GetValue().Lower();
        if (userAnswer == currentAnswer) {
            wxMessageBox("Correct! Enjoy your time!", "Success", wxOK | wxICON_INFORMATION);
            parentDialog->Destroy();
        }
        else {
            attempts--;
            if (attempts > 0) {
                wxMessageBox("Incorrect. Try again. Attempts left: " + std::to_string(attempts), "Try Again", wxOK | wxICON_WARNING);
            }
            else {
                wxMessageBox("Out of attempts! Better luck next time!", "Game Over", wxOK | wxICON_ERROR);
                parentDialog->Destroy();
            }
        }
        });
}
