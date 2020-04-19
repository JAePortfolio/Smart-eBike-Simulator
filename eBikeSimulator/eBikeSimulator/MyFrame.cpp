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

wxStaticBitmap *image, *bike_rearViewImage, *frontWheel;
wxStaticText *textForControls;
bool headlightOn = false;

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Smart eBike Simulator - Senior Design", wxPoint(30,30), wxSize(1366,768))
{
    brakeLevel = 0;
	wxPanel * panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1364, 766), wxWANTS_CHARS);
	panel->SetBackgroundColour(wxColour(*wxWHITE));
	panel->Connect(wxEVT_CHAR, wxKeyEventHandler(MyFrame::OnKeyDown)); //Connects the keyboard event handler to this panel
	SetFont(wxFont(18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	/*wxString dir;
	if (wxFile::Exists(wxT("./bike_sideView.png")))
		dir = wxT("./");
	else if (wxFile::Exists(wxT("../bike_sideView.png")))
		dir = wxT("../");
	else
		wxLogWarning(wxT("Can't find image files in either '.' or '..'!"));
	*/

	textForControls = new wxStaticText(this, wxID_ANY, "Control Bindings", wxPoint(1065, 574), wxSize(299, 192), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls = new wxStaticText(this, wxID_ANY, "L-Signal Left", wxPoint(1065, 605), wxSize(299, 192), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls = new wxStaticText(this, wxID_ANY, "R-Signal Right", wxPoint(1065, 635), wxSize(299, 192), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls = new wxStaticText(this, wxID_ANY, "H-Headlight", wxPoint(1065, 665), wxSize(299, 192), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	image = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/blueprint.png"), wxBITMAP_TYPE_PNG), wxPoint(341,191), wxSize(682,383)); // "../ means parent directory, where the SLN file is
	bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
    brakePicture = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));

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

	else if (key == 104) { // ASCII code of h
		headlightActivation();
	}
    else if (key == 122) // Z for increasing brake level
    {
        controlBrake(0, brakeLevel + 15);
    }
    else if (key == 120) //X for decreasing brake level 
    {
        controlBrake(0, brakeLevel - 15);
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

void MyFrame::headlightActivation() {
	if (headlightOn == false) {
		headlightOn = !headlightOn;
		image = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/blueprint_headlight.png"), wxBITMAP_TYPE_PNG), wxPoint(341, 191), wxSize(682, 383)); // "../ means parent directory, where the SLN file is
		Update();
	}
	else if (headlightOn == true) {
		headlightOn = !headlightOn;
		image = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/blueprint.png"), wxBITMAP_TYPE_PNG), wxPoint(341, 191), wxSize(682, 383)); // "../ means parent directory, where the SLN file is
		Update();
	}
}

void MyFrame::controlBrake(int _throttle, int _brake)
{
    if (brakeLevel < _brake)
    {
        if (brakeLevel > 100)
        {
            SetBrakePicture(true);
            return;
        }
        brakeLevel = _brake;
    }
    else
    {
        if (brakeLevel < 0)
        {
            SetBrakePicture(false);
            return;
        }
        brakeLevel = _brake;
    }

    
    if (brakeLevel > 0)
    {
        SetBrakePicture(true);
    }
    else
    {
        SetBrakePicture(false);
    }
}

void MyFrame::SetBrakePicture(bool _bstatus)
{
    //Update UI
    if (_bstatus)
    {
        //brakeGif = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_brakeon1.gif"), wxBITMAP_TYPE_GIF), wxPoint(1065, 50), wxSize(255, 287));
        brakePicture = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_brakeon.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
        Update();
    }
    else
    {
        brakePicture = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_brake.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
        Update();
    }
   
}

MyFrame::~MyFrame()
{
}

