#ifdef _MSC_VER
#pragma warning(suppress: 28251)
#endif
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/event.h>
#endif
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <ctime>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class MainFrame;  // Forward declaration

class CafeApp : public wxApp {
public:
    virtual bool OnInit();
};
bool CafeApp::OnInit() {
    MainFrame* frame = new MainFrame("BAM Cafe Management System");
    frame->Show(true);
    return true;
}

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

private:
    // Event Handlers
    void OnCustomerInfo(wxCommandEvent& event);
    void OnGameOption(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    void RiddleGame();
    void TeacherGuessGame();

    wxPanel* panel;
    wxBoxSizer* sizer;

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(1001, MainFrame::OnCustomerInfo)
EVT_BUTTON(1002, MainFrame::OnGameOption)
EVT_BUTTON(1003, MainFrame::OnExit)
END_EVENT_TABLE()


IMPLEMENT_APP(CafeApp)

bool CafeApp::OnInit() {
    MainFrame* frame = new MainFrame("BAM Cafe Management System");
    frame->Show(true);
    return true;
}

MainFrame::MainFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 400)) {
    panel = new wxPanel(this, wxID_ANY);
    sizer = new wxBoxSizer(wxVERTICAL);

    // Welcome Message
    wxStaticText* welcomeText = new wxStaticText(panel, wxID_ANY, "Welcome to BAM Cafe Management System", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    sizer->Add(welcomeText, 0, wxALL | wxEXPAND | wxALIGN_CENTER, 10);

    // Buttons
    wxButton* customerInfoButton = new wxButton(panel, 1001, "Customer Information");
    wxButton* gameOptionButton = new wxButton(panel, 1002, "Game Options");
    wxButton* exitButton = new wxButton(panel, 1003, "Exit");

    sizer->Add(customerInfoButton, 0, wxALL | wxEXPAND | wxALIGN_CENTER, 5);
    sizer->Add(gameOptionButton, 0, wxALL | wxEXPAND | wxALIGN_CENTER, 5);
    sizer->Add(exitButton, 0, wxALL | wxEXPAND | wxALIGN_CENTER, 5);

    panel->SetSizer(sizer);
}

void MainFrame::OnCustomerInfo(wxCommandEvent& event) {
    wxTextEntryDialog dialog(this, "Enter Customer Name:", "Customer Information");
    if (dialog.ShowModal() == wxID_OK) {
        wxString name = dialog.GetValue();
        wxMessageBox("Thank you, " + name + "!", "Information Received", wxOK | wxICON_INFORMATION);
    }
}

void MainFrame::OnGameOption(wxCommandEvent& event) {
    wxArrayString games;
    games.Add("Riddle Game");
    games.Add("Guess the Teacher");

    wxSingleChoiceDialog gameDialog(this, "Choose a game to play:", "Game Options", games);
    if (gameDialog.ShowModal() == wxID_OK) {
        wxString selectedGame = gameDialog.GetStringSelection();
        if (selectedGame == "Riddle Game") {
            RiddleGame();
        }
        else if (selectedGame == "Guess the Teacher") {
            TeacherGuessGame();
        }
    }
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MainFrame::RiddleGame() {
    struct Riddle {
        wxString question;
        wxString answer;
    };

    std::vector<Riddle> riddles = {
        {"What has to be broken before you can use it?", "egg"},
        {"I speak without a mouth and hear without ears. What am I?", "echo"},
        {"The more of me you take, the more you leave behind. What am I?", "footsteps"}
    };

    srand(static_cast<unsigned>(time(nullptr)));
    Riddle currentRiddle = riddles[rand() % riddles.size()];

    wxString userAnswer;
    int attempts = 3;

    while (attempts > 0) {
        userAnswer = wxGetTextFromUser(currentRiddle.question + "\n\nAttempts left: " + std::to_string(attempts), "Riddle Game");

        if (userAnswer.IsEmpty()) {
            wxMessageBox("Game cancelled!", "Riddle Game", wxOK | wxICON_INFORMATION);
            return;
        }

        if (userAnswer.CmpNoCase(currentRiddle.answer) == 0) {
            wxMessageBox("Correct! You solved the riddle!", "Riddle Game", wxOK | wxICON_INFORMATION);
            return;
        }
        else {
            attempts--;
            wxMessageBox("Incorrect! Try again.", "Riddle Game", wxOK | wxICON_WARNING);
        }
    }

    wxMessageBox("Out of attempts! The answer was: " + currentRiddle.answer, "Riddle Game", wxOK | wxICON_INFORMATION);
}

void MainFrame::TeacherGuessGame() {
    struct Teacher {
        wxString clue;
        wxString name;
    };

    std::vector<Teacher> teachers = {
        {"Teaches Programming Fundamentals and makes it fun.", "Sir Mansoor"},
        {"Expert in Object Oriented Programming and loves to challenge students.", "Sir Farzeen"},
        {"Artificial Intelligence enthusiast with a passion for machine learning.", "Dr Rizwan"}
    };

    srand(static_cast<unsigned>(time(nullptr)));
    Teacher currentTeacher = teachers[rand() % teachers.size()];
    wxString userGuess;
    int attempts = 3;

    while (attempts > 0) {
        userGuess = wxGetTextFromUser("Clue: " + currentTeacher.clue +
            "\n\nWho am I? (Attempts left: " + wxString::Format("%d", attempts) + ")",
            "Guess the Teacher");

        if (userGuess.IsEmpty()) {
            wxMessageBox("Game cancelled!", "Guess the Teacher", wxOK | wxICON_INFORMATION);
            return;
        }

        if (userGuess.CmpNoCase(currentTeacher.name) == 0) {
            wxMessageBox("Correct! The teacher is " + currentTeacher.name + "!",
                "Congratulations", wxOK | wxICON_INFORMATION);
            return;
        }
        else {
            attempts--;
            if (attempts > 0) {
                wxMessageBox("Incorrect! Try again.", "Wrong Answer", wxOK | wxICON_WARNING);
            }
        }
    }

    wxMessageBox("Out of attempts! The teacher was: " + currentTeacher.name,
        "Game Over", wxOK | wxICON_INFORMATION);
}