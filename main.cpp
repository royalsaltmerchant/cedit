#include <wx/stc/stc.h>
#include <wx/wx.h>
#include <wx/file.h>
#include <iostream>

// colors
wxColour white(220, 220, 220);
wxColour grey(49, 56, 72);
wxColour light_grey(107, 116, 134);
wxColour dark_grey(36, 42, 54);

wxColour red(225, 165, 165);
wxColour purple(180, 160, 200);
wxColour light_blue(155, 205, 225);
wxColour green(160, 200, 160);
wxColour orange(235, 175, 125);

class MyTextEditor : public wxFrame {
public:
	MyTextEditor(const wxString &filename) : wxFrame(NULL, wxID_ANY, "Text Editor", wxDefaultPosition, wxSize(1000, 800)) {
		textCtrl = new wxStyledTextCtrl(this, wxID_ANY);

		// Set styles
		wxFont font(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

		textCtrl->StyleSetForeground(wxSTC_STYLE_DEFAULT, white);	  // Set default text color to white
		textCtrl->StyleSetBackground(wxSTC_STYLE_DEFAULT, dark_grey); // Set default background color to black

		textCtrl->StyleSetFont(wxFONTFAMILY_DEFAULT, font);
		// wrap
		// textCtrl->SetWrapMode(wxSTC_WRAP_WORD);

		textCtrl->SetTabWidth(4); // Set the width of a tab to be 4 spaces

		// Setting block-sized caret
		textCtrl->SetCaretWidth(5);
		textCtrl->SetCaretForeground(white);
		// Margin numbers
		textCtrl->SetMarginType(0, wxSTC_MARGIN_NUMBER);
		textCtrl->SetMarginWidth(0, textCtrl->TextWidth(wxSTC_STYLE_LINENUMBER, "_99999"));

		textCtrl->StyleClearAll(); // Apply the above set styles to all existing text

		textCtrl->StyleSetForeground(wxSTC_STYLE_LINENUMBER, light_grey); // Line number color
		textCtrl->StyleSetBackground(wxSTC_STYLE_LINENUMBER, grey);

		textCtrl->SetLexer(wxSTC_LEX_CPP);
		textCtrl->StyleSetForeground(wxSTC_C_COMMENT, light_grey);
		textCtrl->StyleSetForeground(wxSTC_C_COMMENTLINE, light_grey);
		textCtrl->StyleSetForeground(wxSTC_C_COMMENTDOC, light_grey);
		textCtrl->StyleSetForeground(wxSTC_C_NUMBER, red);
		textCtrl->StyleSetForeground(wxSTC_C_WORD, light_blue);
		textCtrl->StyleSetForeground(wxSTC_C_STRING, green);
		textCtrl->StyleSetForeground(wxSTC_C_CHARACTER, green);
		textCtrl->StyleSetForeground(wxSTC_C_OPERATOR, purple);
		textCtrl->StyleSetForeground(wxSTC_C_PREPROCESSOR, orange);

		textCtrl->StyleSetFont(wxSTC_C_COMMENT, font);
		textCtrl->StyleSetFont(wxSTC_C_COMMENTLINE, font);
		textCtrl->StyleSetFont(wxSTC_C_COMMENTDOC, font);
		textCtrl->StyleSetFont(wxSTC_C_NUMBER, font);
		textCtrl->StyleSetFont(wxSTC_C_WORD, font);
		textCtrl->StyleSetFont(wxSTC_C_STRING, font);
		textCtrl->StyleSetFont(wxSTC_C_CHARACTER, font);
		textCtrl->StyleSetFont(wxSTC_C_OPERATOR, font);

		wxString keywords = "alignas alignof and and_eq asm auto bitand bitor bool break case catch char char8_t char16_t char32_t class compl concept const consteval constexpr const_cast continue co_await co_return co_yield decltype default delete do double dynamic_cast else enum explicit export extern false float for friend goto if inline int long mutable namespace new noexcept not not_eq nullptr operator or or_eq private protected public register reinterpret_cast requires return short signed sizeof static static_assert static_cast struct switch synchronized template this thread_local throw true try typedef typeid typename union unsigned using virtual void volatile wchar_t while xor xor_eq";
		textCtrl->SetKeyWords(0, keywords);

		// load
		// Open the file if a filename was provided
		if (!filename.IsEmpty()) {
			textCtrl->LoadFile(filename);
			currentFile = filename;
		}

		// Create a menu bar
		wxMenuBar *menuBar = new wxMenuBar;
		wxMenu *fileMenu = new wxMenu;
		fileMenu->Append(wxID_SAVE, "&Save\tCtrl-S", "Save the current document");
		fileMenu->Append(wxID_OPEN, "&Open\tCtrl+O", "Open a document");
		menuBar->Append(fileMenu, "&File");
		SetMenuBar(menuBar);

		// Bind the events
		Bind(wxEVT_MENU, &MyTextEditor::OnSave, this, wxID_SAVE);
		Bind(wxEVT_MENU, &MyTextEditor::OnOpen, this, wxID_OPEN);

		textCtrl->Bind(wxEVT_STC_CHANGE, &MyTextEditor::OnTextChanged, this);
		textCtrl->Bind(wxEVT_KEY_DOWN, &MyTextEditor::OnKeyPressed, this);
	}

private:
	wxStyledTextCtrl *textCtrl;
	wxString currentFile;

	void OnTextChanged(wxStyledTextEvent &event) {
		textCtrl->SetMarginWidth(0, textCtrl->TextWidth(wxSTC_STYLE_LINENUMBER, "_99999"));
		event.Skip();
	}

	void OnSave(wxCommandEvent &event) {
		if (currentFile.IsEmpty()) {
			wxFileDialog saveFileDialog(this, "Save as", "", "", "Text files (*.txt)|*.txt|All files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
			if (saveFileDialog.ShowModal() == wxID_CANCEL)
				return; // The user cancelled the operation
			currentFile = saveFileDialog.GetPath();
		}
		textCtrl->SaveFile(currentFile);
	}

	void OnOpen(wxCommandEvent& event) {
		wxFileDialog openFileDialog(this, _("Open file"), "", "",
						   "C++ files (*.cpp)|*.cpp|Header files (*.h)|*.h|C files (*.c)|*.c|Text files (*.txt)|*.txt", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

		if (openFileDialog.ShowModal() == wxID_CANCEL)
			return;

		currentFile = openFileDialog.GetPath();
		LoadFileToEditor();
	}

    void LoadFileToEditor() {
        wxFile file(currentFile);
        if (!file.IsOpened())
            return;

        wxString fileContent;
        file.ReadAll(&fileContent);
        textCtrl->SetText(fileContent);
    }

	void OnKeyPressed(wxKeyEvent &event) {
		int keycode = event.GetKeyCode();
		wxLogDebug("Keycode: %d", keycode);

		bool cmd = event.CmdDown();
		bool isDivideKey = (keycode == '/' && cmd);

		if (isDivideKey) {
			long from, to;
			textCtrl->GetSelection(&from, &to);
			if (from == to) { // no selection
				// comment current line
				int line = textCtrl->GetCurrentLine();
				textCtrl->InsertText(textCtrl->PositionFromLine(line), "//");
			}
			else {
				// multiple lines selected
				int line_from = textCtrl->LineFromPosition(from);
				int line_to = textCtrl->LineFromPosition(to);
				for (int i = line_from; i <= line_to; ++i) {
					textCtrl->InsertText(textCtrl->PositionFromLine(i), "//");
				}
			}
		}
		else if (keycode == 57 && event.ShiftDown()) {
			// Auto-insert matching bracket
			wxString insertText = "()";
			long pos = textCtrl->GetCurrentPos();
			textCtrl->InsertText(pos, insertText);
			textCtrl->GotoPos(pos + 1);
		}
		else if (keycode == 91 && event.ShiftDown()) {
			// Auto-insert matching bracket and new line
			wxString insertText = "{}";
			long pos = textCtrl->GetCurrentPos();
			textCtrl->InsertText(pos, insertText);
			textCtrl->GotoPos(pos + 1);
		}
		else if (keycode == 91 && !event.ShiftDown()) {
			// Auto-insert matching bracket and new line
			wxString insertText = "[]";
			long pos = textCtrl->GetCurrentPos();
			textCtrl->InsertText(pos, insertText);
			textCtrl->GotoPos(pos + 1);
		}
		else {
			event.Skip();
		}
	}
};

class MyApp : public wxApp {
public:
	virtual bool OnInit() {
		wxString filename;
		if (argc > 1) {
			filename = argv[1];
		}

		MyTextEditor *editor = new MyTextEditor(filename);
		editor->Show();
		return true;
	}
};

wxIMPLEMENT_APP(MyApp);
