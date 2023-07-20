#ifndef EDITOR_H
#define EDITOR_H

#include <wx/stc/stc.h>
#include <wx/wx.h>
#include <wx/file.h>
#include <wx/clipbrd.h>
#include <iostream>

class TextEdit : public wxStyledTextCtrl {
public:
	TextEdit(const wxString &filename, wxWindow* parent);
  void OnSave(wxCommandEvent &event);
	void OnOpen(wxCommandEvent& event);
  void LoadFileToEditor();

private:
	wxString currentFile;
  void InitCppStyle();
	void OnTextChanged(wxStyledTextEvent &event);
	void OnKeyPressed(wxKeyEvent &event);
};

#endif /* EDITOR_H */
