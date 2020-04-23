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
#include <string>
#include <algorithm>

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	//EVT_KEY_DOWN(turnLeftEvent)
wxEND_EVENT_TABLE()

wxStaticBitmap *image, *bike_rearViewImage, *frontWheel;
wxStaticText *textForControls;
wxListBox *raspberryPi;
bool headlightOn = false;
int brk_lvl = 0;

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Smart eBike Simulator - Senior Design", wxPoint(30,30), wxSize(1366,768))
{
    brk_lvl = 0;
	//wxPanel * panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1364, 766), wxWANTS_CHARS);
	this->SetBackgroundColour(wxColour(*wxWHITE));
	this->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MyFrame::OnKeyDown)); //Connects the keyboard event handler to this panel
	this->Connect(wxEVT_KEY_UP, wxKeyEventHandler(MyFrame::OnKeyUp)); //Connects the keyboard event handler to this panel
	SetFont(wxFont(18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	/*wxString dir;
	if (wxFile::Exists(wxT("./bike_sideView.png")))
		dir = wxT("./");
	else if (wxFile::Exists(wxT("../bike_sideView.png")))
		dir = wxT("../");
	else
		wxLogWarning(wxT("Can't find image files in either '.' or '..'!"));
	*/

	textForControls = new wxStaticText(this, wxID_ANY, "Control Bindings", wxPoint(1065, 550), wxSize(299, 192), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls = new wxStaticText(this, wxID_ANY, "L-Signal Left", wxPoint(1065, 580), wxSize(299, 192), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls = new wxStaticText(this, wxID_ANY, "R-Signal Right", wxPoint(1065, 610), wxSize(299, 192), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls = new wxStaticText(this, wxID_ANY, "H-Headlight", wxPoint(1065, 640), wxSize(299, 192), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
    textForControls = new wxStaticText(this, wxID_ANY, "Z-BrakeIncr + 15%", wxPoint(1065, 670), wxSize(299, 192), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
    textForControls = new wxStaticText(this, wxID_ANY, "X-BrakeDecr - 15%", wxPoint(1065, 700), wxSize(299, 192), wxALIGN_LEFT | wxST_NO_AUTORESIZE);

	raspberryPi = new wxListBox(this, wxID_ANY, wxPoint(594, 24), wxSize(255, 143));

	image = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/blueprint.png"), wxBITMAP_TYPE_PNG), wxPoint(341,191), wxSize(682,383)); // "../ means parent directory, where the SLN file is
	bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
    brakePicture = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
	//initialized brakeGif
	brakeGif = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/blueprint.png"), wxBITMAP_TYPE_PNG), wxPoint(341, 191), wxSize(682, 383));
	//Throttle 
	throttleSlider = new wxSlider(this, wxID_ANY, 0, 0, 35, wxPoint(1025, 574), wxSize(20, 150), wxSL_VERTICAL | wxSL_INVERSE);
	wxStaticText* throttleText = new wxStaticText(this, wxID_ANY, "T\nh\nr\no\nt\nt\nl\ne",wxPoint(1045, 585), wxDefaultSize, wxALIGN_CENTER);
	throttleText->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	throttleSliderValue = new wxStaticText(this, wxID_ANY, "0.7",wxPoint(1010, 649), wxDefaultSize, wxALIGN_TOP);
	throttleSliderValue->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	throttleSlider->Bind(wxEVT_SLIDER, &MyFrame::OnThrottleSliderScrolled, this);
	//temporary Speed Display
	speedText = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("Speed: %.1f MPH"), currentSpeed),wxPoint(800, 649), wxDefaultSize, wxALIGN_TOP);
}

void MyFrame::raspberryPiConsole(std::string outputMessage) {
	raspberryPi->AppendString(outputMessage); // Do not use this function for now, it crashes.
}

void MyFrame::OnKeyDown(wxKeyEvent& event) {
	wxChar key = event.GetKeyCode();
	//LowerCase ASCII don't work once I have changed the EVENT for KEY DOWN and UP. SO Use uppercase ones
	if (key == 76) { // ASCII code of l (lowercase L)
		//raspberryPiConsole("Turning Left");
		leftTurnSignal();
	}
	else if (key == 82) { // ASCII code of r
		rightTurnSignal();
	}

	else if (key == 72) { // ASCII code of h
		headlightActivation();
	}
    else if (key == 90) // Z for increasing brake level
    {
        controlBrake(0, brk_lvl + 15);
    }
    else if (key == 88) //X for decreasing brake level 
    {
        controlBrake(0, brk_lvl - 15);
    }
	else if (key == 315) {//ASCI code for up Arrow
		if (!upKeyPressed) {
			upKeyPressed = true;
			keyPressedTime = clock();
		}
		//Temporarily keep increasing speed until key is released
		tmpSpeed = (bikeAcceleration* (digitalThrottleValue / 1024.0)*(1.0 / 31.0)) + tmpSpeed;
		tmpSpeed = std::min(tmpSpeed, 25.0);
		speedText->SetLabel(wxString::Format(wxT("Speed: %.1f MPH"), tmpSpeed));
	}
	else if (key == 317) {//ASCI code for up Arrow
		if (!upKeyPressed) {
			upKeyPressed = true;
			keyPressedTime = clock();
		}
		//Temporarily keep increasing speed until key is released
		tmpSpeed = tmpSpeed-(bikeAcceleration* (1-(digitalThrottleValue / 1024.0))*(1.0 / 31.0)) ;
		tmpSpeed = std::max(tmpSpeed, 0.0);
		speedText->SetLabel(wxString::Format(wxT("Speed: %.1f MPH"), tmpSpeed));
	}
}
void MyFrame::OnKeyUp(wxKeyEvent& event) {
	wxChar key = event.GetKeyCode();
	if (key == 315) {//ASCI code for up Arrow
		keyReleasedTime = clock();

		totalKeyPressedTime = double(keyReleasedTime - keyPressedTime) / 1000.0;
		upKeyPressed = false;
		increaseSpeed();
	}
	else if (key == 317) {//ASCI code for Down Arrow
		keyReleasedTime = clock();

		totalKeyPressedTime = double(keyReleasedTime - keyPressedTime) / 1000.0;
		upKeyPressed = false;
		decreaseSpeed();
	}
}

void MyFrame::leftTurnSignal() {
	int counter = 0;
	while (counter < 10) { // 500ms*10 = 5 seconds
		//new wxStaticBitmap will increase memory each time. Rather modify the current image.
		//bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView_left.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
		bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_rearView_left.png"), wxBITMAP_TYPE_PNG));
		counter++;
		std::this_thread::sleep_for (std::chrono::milliseconds(500)); // Wait half a second
		Update();
		//bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
		bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG));

		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Wait half a second
		counter++;
	}
	//bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
	bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG));
	Update();
}

void MyFrame::rightTurnSignal() {
	int counter = 0;
	while (counter < 10) { // 500ms*10 = 5 seconds
		//bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView_right.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
		bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_rearView_right.png"), wxBITMAP_TYPE_PNG));
		counter++;
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Wait half a second
		Update();
		//bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
		bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG));
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Wait half a second
		counter++;
	}
	//bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
	bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG));
	Update();
}

void MyFrame::headlightActivation() {
	if (headlightOn == false) {
		headlightOn = !headlightOn;
		//image = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/blueprint_headlight.png"), wxBITMAP_TYPE_PNG), wxPoint(341, 191), wxSize(682, 383)); // "../ means parent directory, where the SLN file is
		image->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/blueprint_headlight.png"), wxBITMAP_TYPE_PNG));
		Update();
	}
	else if (headlightOn == true) {
		headlightOn = !headlightOn;
		//image = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/blueprint.png"), wxBITMAP_TYPE_PNG), wxPoint(341, 191), wxSize(682, 383)); // "../ means parent directory, where the SLN file is
		image->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/blueprint.png"), wxBITMAP_TYPE_PNG));
		Update();
	}
}

void MyFrame::controlBrake(int _throttle, int _brake)
{    if (brk_lvl < _brake)
    {
        if (brk_lvl > 100)
        {
            SetBrakePicture(true);
            return;
        }
        brk_lvl = _brake;
    }
    else
    {
        if (brk_lvl <= 0)
        {
            SetBrakePicture(false);
            return;
        }
        brk_lvl = _brake;
    }

    
    if (brk_lvl > 0)
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
        // optional blueprint image with light in the back on
       // brakeGif = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/blueprintbrake.png"), wxBITMAP_TYPE_PNG), wxPoint(341, 191), wxSize(682, 383)); // "../ means 
		brakeGif->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/blueprintbrake.png"), wxBITMAP_TYPE_PNG));
        //brakePicture = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_brakeon.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
		brakePicture->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_brakeon.png"), wxBITMAP_TYPE_PNG));

		Update();
    }
    else
    {
        //optional blueprintimage with light in the back on
       // brakeGif= new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/blueprint.png"), wxBITMAP_TYPE_PNG), wxPoint(341, 191), wxSize(682, 383)); // "
		brakeGif->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/blueprint.png"), wxBITMAP_TYPE_PNG));

       // brakePicture = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_brake.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
		brakePicture->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_brake.png"), wxBITMAP_TYPE_PNG));

		Update();
    }
   
}

void MyFrame::OnThrottleSliderScrolled(wxCommandEvent&) {
	double currValue = double(throttleSlider->GetValue()) / 10 + 0.7;
	digitalThrottleValue = (((float)currValue - 0.7) / .00341796875) + 1;
	throttleSliderValue->SetLabel(wxString::Format(wxT("%.1f"), currValue));

}
void MyFrame::increaseSpeed() {
	currentSpeed = currentSpeed+(bikeAcceleration * (digitalThrottleValue / 1024.0)*totalKeyPressedTime);
	
	currentSpeed = std::min(currentSpeed, 25.0);
	speedText->SetLabel(wxString::Format(wxT("Speed: %.1f MPH"), currentSpeed));
	tmpSpeed = currentSpeed;

}
void MyFrame::decreaseSpeed() {
	currentSpeed = currentSpeed - (bikeAcceleration * (1-(digitalThrottleValue / 1024.0))*totalKeyPressedTime);

	currentSpeed = std::max(currentSpeed, 0.0);
	speedText->SetLabel(wxString::Format(wxT("Speed: %.1f MPH"), currentSpeed));
	tmpSpeed = currentSpeed;

}

MyFrame::~MyFrame()
{
}

