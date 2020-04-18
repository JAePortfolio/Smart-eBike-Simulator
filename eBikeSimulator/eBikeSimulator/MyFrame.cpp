/**
	* @file		cMain.cpp
	* @authors	John Arena, Shamim Babul, Rona Kosumi, Ionut Rotariu
	* @license	This project is NOT released under any license.
	* @date		Sept 2019-May 2020
*/

#include "MyFrame.h"
#include "wx/bitmap.h"
#include "wx/stdpaths.h"
#include "wx/file.h"
#include <wx/log.h> 
#include "wx/event.h"
#include <chrono>
#include <thread>

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	//EVT_KEY_DOWN(turnLeftEvent)
wxEND_EVENT_TABLE()

wxStaticBitmap *image, *bike_rearViewImage;

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Smart eBike Simulator - Senior Design", wxPoint(30,30), wxSize(1366,768))
{
	wxPanel * panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1364, 766), wxWANTS_CHARS);
	panel->SetBackgroundColour(wxColour(*wxWHITE));
	panel->Connect(wxEVT_CHAR, wxKeyEventHandler(MyFrame::OnKeyDown)); //Connects the keyboard event handler to this panel
	
	/*wxString dir;
	if (wxFile::Exists(wxT("./bike_sideView.png")))
		dir = wxT("./");
	else if (wxFile::Exists(wxT("../bike_sideView.png")))
		dir = wxT("../");
	else
		wxLogWarning(wxT("Can't find image files in either '.' or '..'!"));
	*/

	image = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_sideView.png"), wxBITMAP_TYPE_PNG), wxPoint(341,191), wxSize(682,383)); // "../ means parent directory, where the SLN file is
	bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));


}

void MyFrame::OnKeyDown(wxKeyEvent& event) {
	wxChar key = event.GetKeyCode();
	if (key == 108) { // ASCII code of l (lowercase L)
		//wxLogMessage(wxT("L has been pressed")); //-You can use this to test if your key code is correct
		leftTurnSignal();
	}
	else if (key == 114) { // ASCII code of r
		rightTurnSignal();
	}
}

void MyFrame::leftTurnSignal() {
	int counter = 0;
	while (counter < 10) { // 500ms*10 = 5 seconds
		bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView_left.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
		counter++;
		std::this_thread::sleep_for (std::chrono::milliseconds(500)); // Wait half a second
		Update();
		bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Wait half a second
		counter++;
	}
	bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
	Update();
}

void MyFrame::rightTurnSignal() {
	int counter = 0;
	while (counter < 10) { // 500ms*10 = 5 seconds
		bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView_right.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
		counter++;
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Wait half a second
		Update();
		bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Wait half a second
		counter++;
	}
	bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
	Update();
}

MyFrame::~MyFrame()
{
}

