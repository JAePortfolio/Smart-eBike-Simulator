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
#include <wx/gauge.h>
//#include "kwic/LCDWindow.h"

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_KEY_DOWN(MyFrame::OnKeyDown)
    EVT_KEY_UP(MyFrame::OnKeyUp)
wxEND_EVENT_TABLE()

wxStaticBitmap *image, *bike_rearViewImage, *frontWheel,*keyImage;
wxStaticText *textForControls,*batteryPercentageText;
wxListBox *raspberryPi;
bool headlightOn = false;
bool isLocked = true;
int brk_lvl = 0;
wxGauge *batteryGauge;
//kwxLCDDisplay* test; Leave disabled
int batteryPercentage=100;
double batteryVoltage=42.0;
int xWidth = 1366;
int yWidth = 768;

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Smart eBike Simulator - Senior Design", wxPoint(30,30), wxSize(xWidth, yWidth), wxDEFAULT_FRAME_STYLE & ~wxMAXIMIZE_BOX){

	//wxPanel * panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1364, 766), wxWANTS_CHARS);
	this->SetBackgroundColour(wxColour(*wxWHITE));
    this->Bind(wxEVT_KEY_DOWN,&MyFrame::OnKeyDown,this);
    this->Bind(wxEVT_KEY_UP,&MyFrame::OnKeyUp,this);
	this->Bind(wxEVT_IDLE, &MyFrame::IdleEv, this);

	//this->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MyFrame::OnKeyDown)); //Connects the keyboard event handler to this panel
    //this->Connect(wxEVT_KEY_UP, wxKeyEventHandler(MyFrame::OnKeyUp)); //Connects the keyboard event handler to this panel
	SetFont(wxFont(18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	//test = new kwxLCDDisplay(this, wxPoint(0, 0), wxSize(100, 100)); Leave disabled
	textForControlsSetup();
	raspberryPiSetup();
	initialImageDisplaySetup();
	throttleTextSetup();
	timeElapsedSetup();
    keySetup();
    batteryGaugeSetup();
    lidarGaugeSetup();
}

void MyFrame::lidarGaugeSetup()
{
    lidarGauge = new wxGauge(this, wxID_ANY, 100, wxPoint(10, 300), wxSize(200, 50));
    lidarTxt = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("LiDar Distance: %dm"), 0), wxPoint(10, 350), wxSize(200, 50), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
    lidarTxt->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    lidarGauge->SetValue(0);
}

void MyFrame::setLidarLevel(int val)
{
    lidarGauge->SetValue(val);
}

void MyFrame::keySetup() {
 
    keyImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/key_lock.png"), wxBITMAP_TYPE_PNG), wxPoint(43, 24), wxSize(124, 131)); // "../ means parent directory, where the SLN file is
    
}
void MyFrame::keyLock() {
    keyImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/key_lock.png"), wxBITMAP_TYPE_PNG));
    isLocked=true;
}
void MyFrame::keyUnlock() {
    keyImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/key_unlock.png"), wxBITMAP_TYPE_PNG));
    isLocked=false;
}
void MyFrame::batteryGaugeSetup() {
    batteryGauge= new wxGauge(this, wxID_ANY, 100, wxPoint(275, 72), wxSize(200, 50) );
    batteryPercentageText = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("Battery level: %d%%"), batteryPercentage), wxPoint(285, 123), wxSize(200, 50), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
    batteryPercentageText->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    batteryGauge->SetValue(batteryPercentage);
}
void MyFrame::textForControlsSetup() {
	textForControls = new wxStaticText(this, wxID_ANY, "Control Bindings", wxPoint(1065, 575), wxSize(299, 192), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	textForControls = new wxStaticText(this, wxID_ANY, "K-Unlock/Lock", wxPoint(1065, 600), wxSize(299, 175), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	textForControls = new wxStaticText(this, wxID_ANY, "L-Signal Left", wxPoint(1065, 620), wxSize(299, 175), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	textForControls = new wxStaticText(this, wxID_ANY, "R-Signal Right", wxPoint(1065, 640), wxSize(299, 175), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	textForControls = new wxStaticText(this, wxID_ANY, "H-Headlight", wxPoint(1065, 660), wxSize(299, 175), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	textForControls = new wxStaticText(this, wxID_ANY, "Z-BrakeIncr + 15%", wxPoint(1065, 680), wxSize(299, 175), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	textForControls = new wxStaticText(this, wxID_ANY, "X-BrakeDecr - 15%", wxPoint(1065, 700), wxSize(299, 175), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
}

void MyFrame::raspberryPiSetup() {
	raspberryPi = new wxListBox(this, wxID_ANY, wxPoint(594, 24), wxSize(255, 143));
	raspberryPi->SetFont(wxFont(13, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	raspberryPi->SetBackgroundColour(*wxBLACK);
	raspberryPi->SetForegroundColour(*wxWHITE);
	raspberryPiConsole("Raspberry Pi: ON");
	raspberryPiConsole("Blynk Initialized");

}
void MyFrame::initialImageDisplaySetup() {
	image = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/blueprint.png"), wxBITMAP_TYPE_PNG), wxPoint(341, 191), wxSize(682, 383)); // "../ means parent directory, where the SLN file is
	bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
	brakePicture = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
	brakingAnim = new wxAnimationCtrl(this, wxID_ANY, wxAnimation(wxT("../eBikeSimulator/gifs/bike_braking.gif")), wxPoint(895, 24), wxSize(426, 143));
	brakeGif = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/blueprint.png"), wxBITMAP_TYPE_PNG), wxPoint(341, 191), wxSize(682, 383)); // How is this a GIF?

}
void MyFrame::throttleTextSetup() {
	//Throttle 
	throttleSlider = new wxSlider(this, wxID_ANY, 0, 0, 35, wxPoint(1025, 574), wxSize(20, 150), wxSL_VERTICAL | wxSL_INVERSE);
	wxStaticText* throttleText = new wxStaticText(this, wxID_ANY, "T\nh\nr\no\nt\nt\nl\ne", wxPoint(1045, 585), wxDefaultSize, wxALIGN_CENTER);
	throttleText->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	throttleSliderValue = new wxStaticText(this, wxID_ANY, "0.7", wxPoint(1010, 649), wxDefaultSize, wxALIGN_TOP);
	throttleSliderValue->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	throttleSlider->Bind(wxEVT_SLIDER, &MyFrame::OnThrottleSliderScrolled, this);
	//temporary Speed Display
	speedText = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("Speed: %.1f MPH"), currentSpeed), wxPoint(43, 623), wxDefaultSize, wxALIGN_TOP);
}
void MyFrame::timeElapsedSetup() {
	timeElapsedText = new wxStaticText(this, wxID_ANY, "Time Elapsed:", wxPoint(341, 650), wxDefaultSize, wxALIGN_TOP);
	timeElapsedHours = new wxStaticText(this, wxID_ANY, "Hours", wxPoint(527, 700), wxDefaultSize, wxALIGN_TOP);
	timeElapsedMins = new wxStaticText(this, wxID_ANY, "Mins", wxPoint(607, 700), wxDefaultSize, wxALIGN_TOP);
	timeElapsed_mins = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxPoint(607, 650), wxSize(60, 40));
	timeElapsed_mins->SetRange(0, 59);
	timeElapsed_hours = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxPoint(527, 650), wxSize(60, 40));
	timeElapsed_hours->SetRange(0, 3);
	setTimeElapsedButton = new wxButton(this, wxID_ANY, "Set Time", wxPoint(341, 685), wxDefaultSize); // Functionality needs to be connected to this button 
      //batteryPercentageCharged();
    setTimeElapsedButton->Bind(wxEVT_BUTTON, &MyFrame::batteryPercentageCharged, this);
   
}
void MyFrame::batteryPercentageCharged(wxCommandEvent& ) {
    //wxLogMessage(wxString::Format(wxT("%d%"), timeElapsed_hours->GetValue()));
    int totaltime= (timeElapsed_hours->GetValue()*60)+timeElapsed_mins->GetValue();
    double tmp= (double(totaltime)/180.0)*100.0;
    batteryPercentage= 100-tmp;
    setBatteryPercentage();
    
}
void MyFrame::setBatteryPercentage(){
    batteryGauge->SetValue(batteryPercentage);
    batteryPercentageText->SetLabel(wxString::Format(wxT("Battery level: %d%%"), batteryPercentage));
    batteryVoltage=(30.4 + ((double(batteryPercentage)/100.0)*11.6));
    raspberryPiConsole("Battery Percentage:"+std::to_string(batteryPercentage)+"%");
    raspberryPiConsole("Battery Voltage:" +std::to_string(batteryVoltage)+"V");
    if (batteryVoltage<31 && batteryVoltage>30.4){
        raspberryPiConsole("DC-DC Converter will" );
        raspberryPiConsole("shutdown soon" );
    }
 
  else  if (batteryVoltage*(10.7/(10.7+255))<1.224){
           raspberryPiConsole("RaspberryPi is OFF" );
       }
}

void MyFrame::raspberryPiConsole(std::string outputMessage) {
	raspberryPi->AppendString(outputMessage);
	raspberryPi->SetSelection(raspberryPi->GetCount()-1);
}

void MyFrame::OnKeyDown(wxKeyEvent& event) {
	wxChar key = event.GetKeyCode();
	//LowerCase ASCII don't work once I have changed the EVENT for KEY DOWN and UP. SO Use uppercase ones
    //wxLogMessage(wxString::Format(wxT("%d"),key));
    if (key == 76 && !isLocked) { // ASCII code of l (lowercase L)
		leftTurnSignal();
	}
	else if (key == 82 && !isLocked) { // ASCII code of r
		rightTurnSignal();
	}

	else if (key == 72 && !isLocked) { // ASCII code of h
		headlightActivation();
	}
    else if (key == 90 && !isLocked) // Z for increasing brake level
    {
        controlBrake(0, brk_lvl + 15);
    }
    else if (key == 88 && !isLocked) //X for decreasing brake level
    {
        controlBrake(0, brk_lvl - 15);
    }
	else if (key == 315 && !isLocked) {//ASCI code for up Arrow
		if (!upKeyPressed) {
			upKeyPressed = true;
			keyPressedTime = clock();
		}
		//Temporarily keep increasing speed until key is released
		tmpSpeed = (bikeAcceleration* (digitalThrottleValue / 1024.0)*(1.0 / 31.0)) + tmpSpeed;
		tmpSpeed = std::min(tmpSpeed, 25.0);
		speedText->SetLabel(wxString::Format(wxT("Speed: %.1f MPH"), tmpSpeed));
	}
	else if (key == 317 && !isLocked) {//ASCI code for up Arrow
		if (!upKeyPressed) {
			upKeyPressed = true;
			keyPressedTime = clock();
		}
		//Temporarily keep increasing speed until key is released
		tmpSpeed = tmpSpeed-(bikeAcceleration* (1-(digitalThrottleValue / 1024.0))*(1.0 / 31.0)) ;
		tmpSpeed = std::max(tmpSpeed, 0.0);
		speedText->SetLabel(wxString::Format(wxT("Speed: %.1f MPH"), tmpSpeed));
	}
    else if (key== 75) {//ASCI code for k
        if (isLocked) {
            keyUnlock();
        }
        else if (!isLocked){
            keyLock();
        }
    }
    
}
void MyFrame::OnKeyUp(wxKeyEvent& event) {
	wxChar key = event.GetKeyCode();
//<<<<<<< HEAD

    // :  Always avoid else if-- while you have switch branching
	if (key == 315) {//ASCI code for up Arrow

	if (key == 315 && !isLocked) {//ASCI code for up Arrow
//>>>>>>> 302d9945b4cad576bebbdd80b42bee9f27e24364
		keyReleasedTime = clock();

		totalKeyPressedTime = double(keyReleasedTime - keyPressedTime) / 1000.0;
		upKeyPressed = false;
		increaseSpeed();
	}
	else if (key == 317 && !isLocked) {//ASCI code for Down Arrow
		keyReleasedTime = clock();

		totalKeyPressedTime = double(keyReleasedTime - keyPressedTime) / 1000.0;
		upKeyPressed = false;
		decreaseSpeed();
	}

    // If c is pressed then take cursor's value and update lidar.
    //It's like Lidar is ON when c is pressed.
    if (key == 'C') 
    {
        double lidVal; //0-100;
        POINT point;
        GetCursorPos(&point);   //40 - 1386 range of x values on the screen
        lidVal = Mapping(40, xWidth, 100, 0, point.x);
        int val2 = (int)Mapping(40, xWidth, 0, 40, point.x);
        if (lidVal < 0) lidVal = 0;
        if (val2 > 40)
        {
            raspberryPiConsole("Obstacle Distance:");
            raspberryPiConsole("OUT OF RANGE");
            lidarTxt->SetLabel("LiDar Distance: OUT OF RANGE");
        }
        else
        {
            raspberryPiConsole("Obstacle Distance: " + std::to_string(val2) + "m");
            lidarTxt->SetLabel(wxString::Format("LiDar Distance: %dm", val2));
        }
        setLidarLevel((int)lidVal);
    }
}
                       //0, 100, 40, xWdith, posx
double MyFrame::Mapping(int a1, int a2, int b1, int b2, int _percentage)
{
    return (b1 + (((_percentage)*(b2 - b1)) / (a2 - a1)));
}



void MyFrame::leftTurnSignal() {
	raspberryPiConsole("Turning Left");
	int counter = 0;
	while (counter < 10) { // 500ms*10 = 5 seconds
		bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_rearView_left.png"), wxBITMAP_TYPE_PNG));
		counter++;
		std::this_thread::sleep_for (std::chrono::milliseconds(500)); // Wait half a second
		Update();
		bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG));
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Wait half a second
		counter++;
	}
	bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG));
	Update();
}

void MyFrame::rightTurnSignal() {
	raspberryPiConsole("Turning Right");
	int counter = 0;
	while (counter < 10) { // 500ms*10 = 5 seconds
		bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_rearView_right.png"), wxBITMAP_TYPE_PNG));
		counter++;
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Wait half a second
		Update();
		bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG));
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Wait half a second
		counter++;
	}
	bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG));
	Update();
}

void MyFrame::headlightActivation() {
	if (headlightOn == false) {
		raspberryPiConsole("Headlight ON");
		headlightOn = !headlightOn;
		image->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/blueprint_headlight.png"), wxBITMAP_TYPE_PNG));
		Update();
	}
	else if (headlightOn == true) {
		raspberryPiConsole("Headlight OFF");
		headlightOn = !headlightOn;
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
		raspberryPiConsole("Braking");
        // optional blueprint image with light in the back on
		brakeGif->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/blueprintbrake.png"), wxBITMAP_TYPE_PNG));
		brakePicture->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_brakeon.png"), wxBITMAP_TYPE_PNG));
		Update();
		brakingAnim->Play(true);
    }
    else
    {
		brakeGif->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/blueprint.png"), wxBITMAP_TYPE_PNG));
		brakePicture->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_brake.png"), wxBITMAP_TYPE_PNG));
		Update();
		brakingAnim->Play(false);
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
void MyFrame::IdleEv(wxIdleEvent&) {
	if (brk_lvl > 0) {
		currentSpeed = std::max(currentSpeed - 2.5, 0.0);

		tmpSpeed = currentSpeed;
		speedText->SetLabel(wxString::Format(wxT("Speed: %.1f MPH"), currentSpeed));
	}

}



MyFrame::~MyFrame()
{
}


// This function cal help with directory finding. 
/*wxString dir;
if (wxFile::Exists(wxT("./bike_sideView.png")))
	dir = wxT("./");
else if (wxFile::Exists(wxT("../bike_sideView.png")))
	dir = wxT("../");
else
	wxLogWarning(wxT("Can't find image files in either '.' or '..'!"));
*/
