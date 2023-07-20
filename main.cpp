#include "Editor.h"

class Frame : public wxFrame {
public:
  Frame(const wxString &filename) : wxFrame(NULL, wxID_ANY, "Text Editor", wxDefaultPosition, wxSize(1000, 800)) {
    // setup text editor
    textEdit = new TextEdit(filename, this);
    // setup menu
    Frame::InitMenu();
  };
private:
  TextEdit* textEdit;

  void InitMenu() {
    		// Create a menu bar
		wxMenuBar *menuBar = new wxMenuBar;
		wxMenu *fileMenu = new wxMenu;
		fileMenu->Append(wxID_SAVE, "&Save\tCtrl-S", "Save the current document");
		fileMenu->Append(wxID_OPEN, "&Open\tCtrl+O", "Open a document");
		menuBar->Append(fileMenu, "&File");
		SetMenuBar(menuBar);

		// Bind the events
		Bind(wxEVT_MENU, &TextEdit::OnSave, textEdit, wxID_SAVE);
		Bind(wxEVT_MENU, &TextEdit::OnOpen, textEdit, wxID_OPEN);
  }
};

class MyApp : public wxApp {
public:
	virtual bool OnInit() {
		wxString filename;
		if (argc > 1) {
			filename = argv[1];
		}

		Frame* frame = new Frame(filename);
		frame->Show();
		return true;
	}
};

wxIMPLEMENT_APP(MyApp);
