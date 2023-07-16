#include <wx/wx.h>
#include <wx/stc/stc.h>

wxColour white(220, 220, 220);
wxColour grey(49, 56, 72);
wxColour light_grey(87, 96, 114);
wxColour dark_grey(36, 42, 54);

wxColour red(255, 145, 145);
wxColour blue(50, 150, 255);
wxColour light_blue(0, 225, 255);
wxColour green(0, 255, 0);
wxColour orange(255, 165, 50);



class MyTextEditor : public wxFrame {
public:
    MyTextEditor() : wxFrame(NULL, wxID_ANY, "Text Editor", wxDefaultPosition, wxSize(600, 400)) {
        textCtrl = new wxStyledTextCtrl(this, wxID_ANY);

        // Set styles
        wxFont font(14, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

        textCtrl->StyleSetForeground(wxSTC_STYLE_DEFAULT, white); // Set default text color to white
        textCtrl->StyleSetBackground(wxSTC_STYLE_DEFAULT, dark_grey); // Set default background color to black


        textCtrl->StyleSetFont(wxFONTFAMILY_TELETYPE, font);
        // textCtrl->SetWrapMode(wxSTC_WRAP_WORD);

                // Setting block-sized caret
        textCtrl->SetCaretWidth(5);
        textCtrl->SetCaretForeground(white);

        textCtrl->SetMarginType(0, wxSTC_MARGIN_NUMBER);
        textCtrl->SetMarginWidth(0, textCtrl->TextWidth(wxSTC_STYLE_LINENUMBER, "_99999"));

        // set
        textCtrl->StyleClearAll(); // Apply the above set styles to all existing text

        textCtrl->StyleSetForeground(wxSTC_STYLE_LINENUMBER, light_grey); // Line number color
        textCtrl->StyleSetBackground(wxSTC_STYLE_LINENUMBER, grey);


        textCtrl->SetLexer(wxSTC_LEX_CPP);
        textCtrl->StyleSetForeground(wxSTC_C_COMMENT, light_grey);
        textCtrl->StyleSetForeground(wxSTC_C_COMMENTLINE, light_grey);
        textCtrl->StyleSetForeground(wxSTC_C_COMMENTDOC, light_grey);
        textCtrl->StyleSetForeground(wxSTC_C_NUMBER, red);
        textCtrl->StyleSetForeground(wxSTC_C_WORD, light_blue);
        textCtrl->StyleSetForeground(wxSTC_C_STRING, orange);
        textCtrl->StyleSetForeground(wxSTC_C_CHARACTER, orange);
        textCtrl->StyleSetForeground(wxSTC_C_OPERATOR, blue);

        textCtrl->StyleSetFont(wxSTC_C_COMMENT, font);
        textCtrl->StyleSetFont(wxSTC_C_COMMENTLINE, font);
        textCtrl->StyleSetFont(wxSTC_C_COMMENTDOC, font);
        textCtrl->StyleSetFont(wxSTC_C_NUMBER, font);
        textCtrl->StyleSetFont(wxSTC_C_WORD, font);
        textCtrl->StyleSetFont(wxSTC_C_STRING, font);
        textCtrl->StyleSetFont(wxSTC_C_CHARACTER, font);
        textCtrl->StyleSetFont(wxSTC_C_OPERATOR, font);

        wxString keywords = "class int float while private char catch import module export virtual operator sizeof "
                            "dynamic_cast typedef const_cast const for static_cast union namespace unsigned long "
                            "volatile static protected bool template mutable if public friend do return goto auto "
                            "void enum else break extern using asm case typeid short reinterpret_cast default "
                            "double register explicit signed typename try this switch continue wchar_t inline "
                            "delete alignas alignof char16_t char32_t constexpr decltype noexcept nullptr "
                            "static_assert thread_local restrict _Bool complex _Complex _Imaginary atomic_bool "
                            "atomic_char atomic_schar atomic_uchar atomic_short atomic_ushort atomic_int atomic_uint "
                            "atomic_long atomic_ulong atomic_llong atomic_ullong new throw";
        textCtrl->SetKeyWords(0, keywords);

        // Events
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
