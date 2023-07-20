#include "Editor.h"

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
// font
wxFont font(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

TextEdit::TextEdit(const wxString &filename, wxWindow* parent): wxStyledTextCtrl(parent, wxID_ANY) {
  this->StyleSetForeground(wxSTC_STYLE_DEFAULT, white);	  // Set default text color to white
  this->StyleSetBackground(wxSTC_STYLE_DEFAULT, dark_grey); // Set default background color to black

  this->StyleSetFont(wxFONTFAMILY_DEFAULT, font);
  // wrap
  // this->SetWrapMode(wxSTC_WRAP_WORD);

  this->SetTabWidth(4); // Set the width of a tab to be 4 spaces

  // Setting block-sized caret
  this->SetCaretWidth(5);
  this->SetCaretForeground(white);
  // Margin numbers
  this->SetMarginType(0, wxSTC_MARGIN_NUMBER);
  this->SetMarginWidth(0, this->TextWidth(wxSTC_STYLE_LINENUMBER, "_99999"));

  this->StyleClearAll(); // Apply the above set styles to all existing text

  this->StyleSetForeground(wxSTC_STYLE_LINENUMBER, light_grey); // Line number color
  this->StyleSetBackground(wxSTC_STYLE_LINENUMBER, grey);

  TextEdit::InitCppStyle();

  // load
  // Open the file if a filename was provided
  if (!filename.IsEmpty()) {
    this->LoadFile(filename);
    currentFile = filename;
  }


  this->Bind(wxEVT_STC_CHANGE, &TextEdit::OnTextChanged, this);
  this->Bind(wxEVT_KEY_DOWN, &TextEdit::OnKeyPressed, this);
}

  // save the current file
void TextEdit::OnSave(wxCommandEvent &event) {
  if (currentFile.IsEmpty()) {
    wxFileDialog saveFileDialog(this, "Save as", "", "", "Text files (*.txt)|*.txt|All files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
      return; // The user cancelled the operation
    currentFile = saveFileDialog.GetPath();
  }
  this->SaveFile(currentFile);
}
// open a new file
void TextEdit::OnOpen(wxCommandEvent& event) {
  wxFileDialog openFileDialog(this, _("Open file"), "", "",
              "C++ files (*.cpp)|*.cpp|Header files (*.h)|*.h|C files (*.c)|*.c|Text files (*.txt)|*.txt", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

  if (openFileDialog.ShowModal() == wxID_CANCEL)
    return;

  currentFile = openFileDialog.GetPath();
  LoadFileToEditor();
}
// load in the file
void TextEdit::LoadFileToEditor() {
  wxFile file(currentFile);
  if (!file.IsOpened())
      return;

  wxString fileContent;
  file.ReadAll(&fileContent);
  this->SetText(fileContent);
}

wxString currentFile;

//init the cpp format
void TextEdit::InitCppStyle() {
  this->SetLexer(wxSTC_LEX_CPP);
  this->StyleSetForeground(wxSTC_C_COMMENT, light_grey);
  this->StyleSetForeground(wxSTC_C_COMMENTLINE, light_grey);
  this->StyleSetForeground(wxSTC_C_COMMENTDOC, light_grey);
  this->StyleSetForeground(wxSTC_C_NUMBER, red);
  this->StyleSetForeground(wxSTC_C_WORD, light_blue);
  this->StyleSetForeground(wxSTC_C_STRING, green);
  this->StyleSetForeground(wxSTC_C_CHARACTER, green);
  this->StyleSetForeground(wxSTC_C_OPERATOR, purple);
  this->StyleSetForeground(wxSTC_C_PREPROCESSOR, orange);

  this->StyleSetFont(wxSTC_C_COMMENT, font);
  this->StyleSetFont(wxSTC_C_COMMENTLINE, font);
  this->StyleSetFont(wxSTC_C_COMMENTDOC, font);
  this->StyleSetFont(wxSTC_C_NUMBER, font);
  this->StyleSetFont(wxSTC_C_WORD, font);
  this->StyleSetFont(wxSTC_C_STRING, font);
  this->StyleSetFont(wxSTC_C_CHARACTER, font);
  this->StyleSetFont(wxSTC_C_OPERATOR, font);

  wxString keywords = "alignas alignof and and_eq asm auto bitand bitor bool break case catch char char8_t char16_t char32_t class compl concept const consteval constexpr const_cast continue co_await co_return co_yield decltype default delete do double dynamic_cast else enum explicit export extern false float for friend goto if inline int long mutable namespace new noexcept not not_eq nullptr operator or or_eq private protected public register reinterpret_cast requires return short signed sizeof static static_assert static_cast struct switch synchronized template this thread_local throw true try typedef typeid typename union unsigned using virtual void volatile wchar_t while xor xor_eq";
  this->SetKeyWords(0, keywords);
}

// handle numbers column in textwidth
void TextEdit::OnTextChanged(wxStyledTextEvent &event) {
  this->SetMarginWidth(0, this->TextWidth(wxSTC_STYLE_LINENUMBER, "_99999"));
  event.Skip();
}
// handle key commands
void TextEdit::OnKeyPressed(wxKeyEvent &event) {
  int keycode = event.GetKeyCode();
  wxLogDebug("Keycode: %d", keycode);

  bool cmd = event.CmdDown();
  bool isDivideKey = (keycode == '/' && cmd);
  bool isCutKey = (keycode == 88 && cmd);

  if (isDivideKey) {
    long from, to;
    this->GetSelection(&from, &to);
    if (from == to) { // no selection
      // comment current line
      int line = this->GetCurrentLine();
      this->InsertText(this->PositionFromLine(line), "//");
    }
    else {
      // multiple lines selected
      int line_from = this->LineFromPosition(from);
      int line_to = this->LineFromPosition(to);
      for (int i = line_from; i <= line_to; ++i) {
        this->InsertText(this->PositionFromLine(i), "//");
      }
    }
  }
  else if (isCutKey) {
    // Cmd+x: cut current line
    int currentLine = this->GetCurrentLine();
    wxString lineText = this->GetLine(currentLine);

    if (wxTheClipboard->Open()) {
      wxTheClipboard->SetData(new wxTextDataObject(lineText));
      wxTheClipboard->Close();
    }
    // Remove
    this->GotoLine(currentLine);
    this->LineDelete();
  }
  else if (keycode == 57 && event.ShiftDown()) {
    // Auto-insert matching bracket
    wxString insertText = "()";
    long pos = this->GetCurrentPos();
    this->InsertText(pos, insertText);
    this->GotoPos(pos + 1);
  }
  else if (keycode == 91 && event.ShiftDown()) {
    // Auto-insert matching bracket and new line
    wxString insertText = "{}";
    long pos = this->GetCurrentPos();
    this->InsertText(pos, insertText);
    this->GotoPos(pos + 1);
  }
  else if (keycode == 91 && !event.ShiftDown()) {
    // Auto-insert matching bracket and new line
    wxString insertText = "[]";
    long pos = this->GetCurrentPos();
    this->InsertText(pos, insertText);
    this->GotoPos(pos + 1);
  }		
  else if (keycode == 315 && event.AltDown()) {
    // Alt+UpArrow: move current line up
    int currentLine = this->GetCurrentLine();
    if (currentLine > 0) {
      wxString tempLine = this->GetLine(currentLine);
      this->BeginUndoAction();
      this->DeleteRange(this->PositionFromLine(currentLine), tempLine.Length());
      this->InsertText(this->PositionFromLine(currentLine - 1), tempLine);
      this->EndUndoAction();
      this->GotoLine(currentLine - 1);
    }
  }	
  else if (keycode == 317 && event.AltDown()) {
    // Alt+DownArrow: move current line down
    int currentLine = this->GetCurrentLine();
    if (currentLine < this->GetLineCount() - 1) {
      wxString tempLine = this->GetLine(currentLine);
      this->BeginUndoAction();
      this->DeleteRange(this->PositionFromLine(currentLine), tempLine.Length());
      this->InsertText(this->PositionFromLine(currentLine + 1), tempLine);
      this->EndUndoAction();
      this->GotoLine(currentLine + 1);
    }
  }
  else if (keycode == 90 && cmd && event.ShiftDown()) { // 90 is the keycode for 'Z'
    // Cmd + Shift + Z: redo
    if (this->CanRedo())
      this->Redo();
  }
  else {
    event.Skip();
  }
}