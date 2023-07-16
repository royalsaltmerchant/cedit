#include <wx/wx.h>
#include <wx/stc/stc.h>

class MyTextEditor : public wxFrame {
public:
    MyTextEditor() : wxFrame(NULL, wxID_ANY, "Text Editor", wxDefaultPosition, wxSize(600, 400)) {
        textCtrl = new wxStyledTextCtrl(this, wxID_ANY);

        // Set styles
        textCtrl->StyleSetForeground(wxSTC_STYLE_DEFAULT, *wxWHITE); // Set default text color to white
        textCtrl->StyleSetBackground(wxSTC_STYLE_DEFAULT, *wxBLACK); // Set default background color to black
        textCtrl->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(128, 128, 128)); // Set line number color to grey
        textCtrl->StyleSetBackground(wxSTC_STYLE_LINENUMBER, *wxBLACK); // Set line number background color to black
        textCtrl->StyleClearAll(); // Apply the above set styles to all existing text

        wxFont font(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
        textCtrl->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
        // textCtrl->SetWrapMode(wxSTC_WRAP_WORD);

                // Setting block-sized caret
        textCtrl->SetCaretWidth(5);
        textCtrl->SetCaretForeground(*wxWHITE);

        textCtrl->SetMarginType(0, wxSTC_MARGIN_NUMBER);
        textCtrl->SetMarginWidth(0, textCtrl->TextWidth(wxSTC_STYLE_LINENUMBER, "_99999"));

        textCtrl->Bind(wxEVT_STC_CHANGE, &MyTextEditor::OnTextChanged, this);
    }

private:
    wxStyledTextCtrl* textCtrl;

    void OnTextChanged(wxStyledTextEvent& event) {
        textCtrl->SetMarginWidth(0, textCtrl->TextWidth(wxSTC_STYLE_LINENUMBER, "_99999"));
        event.Skip();
    }
};

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MyTextEditor* editor = new MyTextEditor();
        editor->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
