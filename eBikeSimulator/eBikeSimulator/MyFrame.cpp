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
#include "kwic/LinearMeter.cpp"//Lidar,Battery Gauge
#include "kwic/AngularMeter.cpp"//For Speedometer


wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_KEY_DOWN(MyFrame::OnKeyDown)
wxEND_EVENT_TABLE()
//Assigning Gauges and Meter Vairables
wxStaticBitmap *bike_sideViewImage, *bike_rearViewImage, *frontWheel,*keyImage,*lidarBox;
wxStaticText *textForControls,*batteryPercentageText;
wxListBox *raspberryPi;
kwxLinearMeter* batteryGauge;
kwxLinearMeter* lidarGauge;
kwxAngularMeter* speedometer;
wxStaticText *speedometerText, *speedValueText;
//Booleans to keep track which button/function is on
bool headlightOn = false;
bool isLocked = true;
bool isBatteryShutdown = false;
bool doSimulate = false;
bool isPedalling = false;
bool isincreaseSpeed = false;
bool isdecreaseSpeed = false;

//Initialization of variables
int brk_lvl = 0;
int batteryPercentage=100;
int wetModeReduction = 0;//For Wet/Dry Mode
double batteryVoltage = 42.0;
double pedalPercantage = 1;

//Window Width/Height
int xWidth = 1366;
int yWidth = 768;

clock_t batTime1, batTime2;//Time for battery discharge rate calculation;

//Initial Frame and Window Setup
MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Smart eBike Simulator - Senior Design", wxPoint(30,30), wxSize(xWidth, yWidth), wxDEFAULT_FRAME_STYLE & ~wxMAXIMIZE_BOX){
	
	//wxPanel * panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1364, 766), wxWANTS_CHARS);
	this->SetBackgroundColour(wxColour(*wxWHITE));
    this->Bind(wxEVT_KEY_DOWN,&MyFrame::OnKeyDown,this);
	this->Bind(wxEVT_IDLE, &MyFrame::IdleEv, this);

	//this->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MyFrame::OnKeyDown)); //Connects the keyboard event handler to this panel
    //this->Connect(wxEVT_KEY_UP, wxKeyEventHandler(MyFrame::OnKeyUp)); //Connects the keyboard event handler to this panel
	SetFont(wxFont(18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	//Functions call to setup all the system on the window
	initialImageDisplaySetup();
	textForControlsSetup();
	keyLockSetup();
	raspberryPiSetup();
	throttleTextSetup();
	timeElapsedSetup();
    batteryGaugeSetup();
    lidarGaugeSetup();
	speedometerSetup();
	ButtonSetup();


}

void MyFrame::initialImageDisplaySetup() {
	//Initial Pictures(wxBitmap) and their locations(wxPoint), Size(wxSize) on the simulator display
	bike_sideViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/blueprint.png"), wxBITMAP_TYPE_PNG), wxPoint(341, 191), wxSize(682, 383)); // "../ means parent directory, where the SLN file is
	bike_rearViewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG), wxPoint(1065, 191), wxSize(255, 287));
	brakingAnim = new wxAnimationCtrl(this, wxID_ANY, wxAnimation(wxT("../eBikeSimulator/gifs/bike_braking.gif")), wxPoint(895, 24), wxSize(426, 143));
	bike_wheelAnim = new wxAnimationCtrl(this, wxID_ANY, wxAnimation(wxT("../eBikeSimulator/gifs/wheel_animation/wheel_still.gif")), wxPoint(43, 303), wxSize(192,192));
	lidarBox = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/lidarBox.png"), wxBITMAP_TYPE_PNG), wxPoint(0, 191), wxSize(342, 105));

}
void MyFrame::textForControlsSetup() {
	//All the simulator User Control is displayed on the window
	textForControls = new wxStaticText(this, wxID_ANY, "Control Bindings", wxPoint(1065, 575), wxSize(299, 192), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	textForControls = new wxStaticText(this, wxID_ANY, "K - Unlock/Lock", wxPoint(1065, 600), wxSize(299, 175), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls->SetFont(wxFont(13, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	textForControls = new wxStaticText(this, wxID_ANY, "L - Signal Left", wxPoint(1065, 620), wxSize(299, 175), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls->SetFont(wxFont(13, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	textForControls = new wxStaticText(this, wxID_ANY, "R - Signal Right", wxPoint(1065, 640), wxSize(299, 175), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls->SetFont(wxFont(13, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	textForControls = new wxStaticText(this, wxID_ANY, "H - Headlight", wxPoint(1065, 660), wxSize(299, 175), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls->SetFont(wxFont(13, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	textForControls = new wxStaticText(this, wxID_ANY, "Z - BrakeIncr + 25%", wxPoint(1200, 600), wxSize(299, 175), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls->SetFont(wxFont(13, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	textForControls = new wxStaticText(this, wxID_ANY, "X - BrakeDecr - 25%", wxPoint(1195, 620), wxSize(299, 175), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls->SetFont(wxFont(13, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	textForControls = new wxStaticText(this, wxID_ANY, "C - LiDAR", wxPoint(1195, 640), wxSize(299, 175), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	textForControls->SetFont(wxFont(13, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
}
void MyFrame::keyLockSetup() {
	//Initial  Key (Locked) image is displayed
	keyImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("../eBikeSimulator/images/key_lock.png"), wxBITMAP_TYPE_PNG), wxPoint(43, 24), wxSize(124, 131));

}
void MyFrame::batteryGaugeSetup() {
	//Sets Up the Battery Gauge, its postion, Size, initial percentage and colors,font
	batteryGauge = new kwxLinearMeter(this, wxID_ANY, wxPoint(275, 72), wxSize(200, 50));
	//text Setup
	batteryPercentageText = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("Battery level: %d%%"), batteryPercentage),
		wxPoint(285, 123), wxSize(200, 50), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	batteryPercentageText->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	batteryGauge->SetValue(batteryPercentage);
	batteryGauge->SetBorderColour(*wxBLACK);

}
void MyFrame::raspberryPiSetup() {
	//Sets up the Raspberry Pi console 
	raspberryPi = new wxListBox(this, wxID_ANY, wxPoint(594, 24), wxSize(255, 143));
	raspberryPi->SetFont(wxFont(13, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	//Background color is set to black to look like a terminal
	raspberryPi->SetBackgroundColour(*wxBLACK);
	raspberryPi->SetForegroundColour(*wxWHITE);
}

void MyFrame::timeElapsedSetup() {
	//Time Elapsed control is setup 
	timeElapsedText = new wxStaticText(this, wxID_ANY, "Time Elapsed:", wxPoint(341, 650), wxDefaultSize, wxALIGN_TOP);
	timeElapsedHours = new wxStaticText(this, wxID_ANY, "Hours", wxPoint(527, 700), wxDefaultSize, wxALIGN_TOP);
	timeElapsedMins = new wxStaticText(this, wxID_ANY, "Mins", wxPoint(607, 700), wxDefaultSize, wxALIGN_TOP);
	timeElapsed_mins = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxPoint(607, 650), wxSize(60, 40));
	//Limits the Mins to 0 to 59
	timeElapsed_mins->SetRange(0, 59);
	timeElapsed_hours = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxPoint(527, 650), wxSize(60, 40));
	//Limits the Hours to 0 to 3
	timeElapsed_hours->SetRange(0, 3);
}

void MyFrame::lidarGaugeSetup()
{
	//Sets up Lidar Gauge, its position,size,text,range and colors
	lidarGauge = new kwxLinearMeter(this, wxID_ANY, wxPoint(1065, 520), wxSize(255, 50));
	lidarTxt = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("LiDar Distance: %dm"), 0), wxPoint(1065, 490), wxSize(200, 30), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	lidarTxt->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	lidarBoxTxt = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("LiDAR Area Box"), 0), wxPoint(0, 170), wxSize(200, 20), wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	lidarBoxTxt->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	lidarGauge->SetValue(0);
	lidarGauge->SetRangeVal(0, 40);
	lidarGauge->SetBorderColour(*wxBLACK);
}
void MyFrame::throttleTextSetup() {
	//Throttle Slider Setup
	throttleSlider = new wxSlider(this, wxID_ANY, 0, 0, 35, wxPoint(1025, 580), wxSize(20, 150), wxSL_VERTICAL | wxSL_INVERSE);
	throttleSlider->SetOwnBackgroundColour(*wxGREEN);
	//Throttle Text and Voltage Value Text
	wxStaticText* throttleText = new wxStaticText(this, wxID_ANY, "Throttle", wxPoint(950, 620), wxDefaultSize, wxALIGN_CENTER | wxBOLD);
	throttleText->SetFont(wxFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)); //Font,Size Setup
	throttleSliderValue = new wxStaticText(this, wxID_ANY, "0.7V", wxPoint(965, 649), wxDefaultSize, wxALIGN_TOP);
	throttleSliderValue->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	throttleSlider->Bind(wxEVT_SLIDER, &MyFrame::OnThrottleSliderScrolled, this);//function will be executed when slider is scrolled
	throttleSlider->Bind(wxEVT_SCROLL_THUMBRELEASE, &MyFrame::OnThrottleSliderReleased, this);//function will be executed when slider is released
}

void MyFrame::speedometerSetup() {
	//Sets up Speedometer Gauge position,size,colors,text,range
	speedometerText = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("Speed"), 0), wxPoint(129, 575));
	speedometer = new kwxAngularMeter(this, wxID_ANY, wxPoint(75, 600), wxSize(180, 180));
	
	speedometer->SetRange(0, 25);
	speedometer->SetNumSectors(3);
	
	speedometer->SetNeedleColour(*wxBLUE);
	speedometer->SetSectorColor(0, *wxGREEN);
	speedometer->SetSectorColor(1, *wxYELLOW);
	speedometer->SetSectorColor(2, *wxRED);
	speedometer->SetNumTick(5);
	speedValueText = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("%.1f"), currentSpeed), wxPoint(150, 700), wxSize(35, 100));
	speedValueText->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
}
void MyFrame::ButtonSetup() {
	//All the buttons are initialized on the window
	//Set Time Button
	setTimeElapsedButton = new wxButton(this, wxID_ANY, "Set Time", wxPoint(341, 685), wxDefaultSize); // Functionality needs to be connected to this button 
	setTimeElapsedButton->Bind(wxEVT_BUTTON, &MyFrame::batteryPercentageCharged, this);//Binding for setTime Button
	
	pedalButton = new wxButton(this, wxID_ANY, "Pedal", wxPoint(520, 610), wxSize(180, 35));//button for pedal
	pedalButton->SetOwnBackgroundColour(*wxGREEN);
	pedalButton->Bind(wxEVT_BUTTON, &MyFrame::pedalButtonClicked, this);
	
	simulateButton = new wxButton(this, wxID_ANY, "Simulate", wxPoint(341, 610), wxDefaultSize); // button for simulate 
	simulateButton->Bind(wxEVT_BUTTON, &MyFrame::simulateButtonClicked, this);
	
	wetDryModeBtn = new wxButton(this, wxID_ANY, "Dry", wxPoint(740, 610), wxSize(180, 35)); // button for Wet/Dry Mode
	wetDryModeBtn->Bind(wxEVT_BUTTON, &MyFrame::wetDryModeClicked, this);

}

void MyFrame::pedalButtonClicked(wxCommandEvent&) {
	//When Pedal button is clicked
	//Checks key is unlocked and battery is greater than 60%
	if (!isLocked && batteryPercentage >= 60) {
		raspberryPiConsole("Can't Pedal Right Now");
	}
	//If it is less than 30% and not Pedalling
	else if (!isLocked && !isPedalling) {
		isPedalling = true;//Set pedaling to true
		pedalButton->SetLabel("Stop Pedalling");
		pedalButton->SetOwnBackgroundColour(*wxRED);
		pedalPercantage = 0.6;//Set pedaling percentage to 40% causing motor uses to 60% of original

	}
	else if (!isLocked && isPedalling) {
		//If it is already pedalling 
		isPedalling = false;
		pedalButton->SetLabel("Pedal");
		pedalButton->SetOwnBackgroundColour(*wxGREEN);
		pedalPercantage = 1;//Set 1 to obtain original motor usage

	}
}
void MyFrame::simulateButtonClicked(wxCommandEvent&) {
	//Function to start simulate
	//Checks if Key is unlocked
	//Setting doSimulate to True would start
	//The simulation

	if (!isLocked && !doSimulate) {
		doSimulate = true;
		simulateButton->SetLabel("Stop");
	}
	else if (!isLocked && doSimulate) {
		doSimulate = false;
		simulateButton->SetLabel("Simulate");
	}
}

void MyFrame::wetDryModeClicked(wxCommandEvent&) {

	//When key unlocked and wet mode is off
	if (!isLocked && wetModeReduction == 0) {
		raspberryPiConsole("Wet Mode Activated");
		wetModeReduction = 1;
		wetDryModeBtn->SetLabel("Wet");
		digitalThrottleValue = digitalThrottleValue - (wetModeReduction*(int(digitalThrottleValue*.3)));
		isdecreaseSpeed = false;

	}
	//when key unlocked and  wet mode is on
	else if (!isLocked && wetModeReduction == 1) {
		raspberryPiConsole("Dry Mode Activated");
		wetModeReduction = 0;
		wetDryModeBtn->SetLabel("Dry");
		int throttleSliderNumber = throttleSlider->GetValue();
		double currValue = double(throttleSliderNumber) / 10 + 0.7;
		digitalThrottleValue = (((float)currValue - 0.7) / analogToDigitalRatio) + 1;
		isincreaseSpeed = false;
	}
}

void MyFrame::OnKeyDown(wxKeyEvent& event) {
	//Everytime a key is pressed this function is activated
	wxChar key = event.GetKeyCode();//Gets the current Key
	//Uses ASCII Upercase Letters
	 if (key == 75) {//ASCI code for k (Lock/Unlock)
		//if it is locked 
		if (isLocked) {
			keyUnlock();
		}
		//if it is already unlocked
		else if (!isLocked) {
			keyLock();
		}
	}
	 //Checks if key is unlock
	 //If it is unlocked then all function will works
	else if (key == 76 && !isLocked) { // ASCII code of L 
		leftTurnSignal();
	}

	else if (key == 82 && !isLocked) { // ASCII code of R
		rightTurnSignal();
	}

	else if (key == 72 && !isLocked) { // ASCII code of H
		headlightActivation();
	}

	else if (key == 90 && !isLocked) // Z for increasing brake level
	{
        brk_lvl += 25;
		controlBrake(0,brk_lvl );
	}
	else if (key == 88 && !isLocked) //X for decreasing brake level
	{
        brk_lvl -= 25;
		controlBrake(0, brk_lvl);
	}
    // If c is pressed then take cursor's value and update lidar.
    //It's like Lidar is ON when c is pressed.
    else if (key == 67 && !isLocked) // ASCII code for c 
    {
        lidar();
    }

}


void MyFrame::raspberryPiConsole(std::string outputMessage) {
	//Takes the message input and displays to Pi console
	raspberryPi->AppendString(outputMessage);
	raspberryPi->SetSelection(raspberryPi->GetCount() - 1);
}

void MyFrame::leftTurnSignal() {
	//Blinks the left turn signal light for 5 second
	raspberryPiConsole("Turning Left");
	int counter = 0;
	while (counter < 10) { // 500ms*10 = 5 seconds
		bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_rearView_left.png"), wxBITMAP_TYPE_PNG));
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

void MyFrame::rightTurnSignal() {
	//Blinks the right turn signal light for 5 second
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
	//Activates headlight if it is off
	if (headlightOn == false) {
		raspberryPiConsole("Headlight ON");
		headlightOn = !headlightOn;
		bike_sideViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/blueprint_headlight.png"), wxBITMAP_TYPE_PNG));
		Update();
	}
	//Deactivates headlight if it is on
	else if (headlightOn == true) {
		raspberryPiConsole("Headlight OFF");
		headlightOn = !headlightOn;
		bike_sideViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/blueprint.png"), wxBITMAP_TYPE_PNG));
		Update();
	}
}

void MyFrame::controlBrake(int _throttle, int _brake)
{
	if (brk_lvl > 100)
	{
		brk_lvl = 100;
		SetBrakePicture(true);
		return;
	}
	else if (brk_lvl <= 0)
	{
        brk_lvl = 0;
		SetBrakePicture(false);
		return;
	}
    else
    {
        SetBrakePicture(true);
        return;
    }
}

void MyFrame::SetBrakePicture(bool _bstatus)
{
	//Update UI
	if (_bstatus)
	{
		// optional blueprint image with light in the back on
		if(!headlightOn) bike_sideViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/blueprintbrake.png"), wxBITMAP_TYPE_PNG));
		if(headlightOn)bike_sideViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/headlightandbrake.png"), wxBITMAP_TYPE_PNG));
		bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_brakeon.png"), wxBITMAP_TYPE_PNG));
		Update();
		brakingAnim->Play(true);
	}
	else
	{
		if(!headlightOn) bike_sideViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/blueprint.png"), wxBITMAP_TYPE_PNG));
		if(headlightOn) bike_sideViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/blueprint_headlight.png"), wxBITMAP_TYPE_PNG));
		bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_brake.png"), wxBITMAP_TYPE_PNG));
		Update();
		brakingAnim->Play(false);
	}
	raspberryPiConsole("Braking: " + std::to_string(brk_lvl) + "%");

}

void MyFrame::keyLock() {
	//Locks the System and Puts back everything to initial setup
	isLocked = true;//Set lock mode
	//Show Locked Image
	keyImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/key_lock.png"), wxBITMAP_TYPE_PNG));
	raspberryPiConsole("Raspberry Pi is Shutting Down");
	std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Wait 2 second
	raspberryPi->Clear();//Clear Raspberry Pi messages

	//Default Images/
	bike_sideViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/blueprint.png"), wxBITMAP_TYPE_PNG));
	bike_rearViewImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/bike_rearView.png"), wxBITMAP_TYPE_PNG));
	//Stop GIFS
	brakingAnim->Stop();//Braking animation stops
	bike_wheelAnim->Stop();//Rotating Wheel stops moving

	speedometerUpdate(0);// Turn off Speedometer
	doSimulate = false;//turn Simulation OFF
	simulateButton->SetLabel("Simulate");
	isincreaseSpeed = false;//Turn off increasing
	isdecreaseSpeed = false;//or decreasing of speed
	digitalThrottleValue = 0.0;//Set throttle to 0
	throttleSlider->SetValue(0);//Slider to 0 postion
	throttleSlider->SetOwnBackgroundColour(*wxGREEN);
	throttleSliderValue->SetLabel(wxString("0.7V"));
	
}
void MyFrame::keyUnlock() {
	//Unlocks the Battery unit
	//Display the Unlocked Key
	keyImage->SetBitmap(wxBitmap(wxT("../eBikeSimulator/images/key_unlock.png"), wxBITMAP_TYPE_PNG));
	isLocked = false;//Set unlock Mode
	//Show Raspberry pi is on and Blynk(APP) initialized on Console
	raspberryPiConsole("Raspberry Pi: ON");
	raspberryPiConsole("Blynk Initialized");

	//Set throttle to 0
	throttleSlider->SetValue(0);
	digitalThrottleValue = 0.0;

}

void MyFrame::OnThrottleSliderScrolled(wxCommandEvent&) {
	// Each time Slider is scrolled display the current voltage and change digital throttle value
	if (!isLocked) {
		int throttleSliderNumber = throttleSlider->GetValue();//Get Slider postion
		//Throttle Voltage goes from 0.7 to 4.2 (Range=3.5), Total Slider postion=10
		//Divide the current Slider Position by 10 (35/3.5) and add 0.7 since slider starts from 0
		double currValue = double(throttleSliderNumber) / 10 + 0.7;//Set current Voltage
		double prevDigitalValue = digitalThrottleValue;
		digitalThrottleValue = (((float)currValue - 0.7) / analogToDigitalRatio) + 1;
		//Subtract 30% if wetMode is on
		digitalThrottleValue = digitalThrottleValue - (wetModeReduction*(int(digitalThrottleValue*.3)));
		if (prevDigitalValue > digitalThrottleValue) {
			isincreaseSpeed = false;
			isdecreaseSpeed = true;
		}
		if (prevDigitalValue < digitalThrottleValue) {
			isincreaseSpeed = true;
			isdecreaseSpeed = false;
		}
		throttleSliderValue->SetLabel(wxString::Format(wxT("%.1fV"), currValue));
		if (throttleSliderNumber < 12) throttleSlider->SetOwnBackgroundColour(*wxGREEN);
		else if (throttleSliderNumber < 24) throttleSlider->SetOwnBackgroundColour(*wxYELLOW);
		else if (throttleSliderNumber < 36) throttleSlider->SetOwnBackgroundColour(*wxRED);

	}

}
void MyFrame::OnThrottleSliderReleased(wxCommandEvent&) {
	if (!isLocked) {
		int dutyCycle = double(digitalThrottleValue / 1024.0)*100;
		raspberryPiConsole("PWM DUTY CYCLE:"+std::to_string(dutyCycle)+"%");
	}
}
void MyFrame::increaseSpeed(double totalTime) {
	//Increase speed based on total time key was pressed for and current throttle value
	currentSpeed = currentSpeed + (bikeAcceleration * (digitalThrottleValue / 1024.0)*totalTime);
	double maxSpeed = (25.0 / 1024.0)*digitalThrottleValue;
	currentSpeed = std::min(currentSpeed, maxSpeed);//Sets the Highest speed limit based on throttle postion
	//Display Current speed value
	if (currentSpeed == maxSpeed) isincreaseSpeed = false;
	speedometerUpdate(currentSpeed);
	setWheelAnimationSpeed(currentSpeed);
}
void MyFrame::decreaseSpeed(double totalTime) {
	//Decrease speed based on total time key was pressed for and current throttle value
	currentSpeed = currentSpeed - (bikeAcceleration * (1 - (digitalThrottleValue / 1024.0))*totalTime);
	double minSpeed = (25.0 / 1024.0)*digitalThrottleValue;
	currentSpeed = std::max(currentSpeed,minSpeed );//Sets the lowest speed limit based on throttle postion
	//Display Current speed value
	if (currentSpeed == minSpeed) isdecreaseSpeed = false;
	speedometerUpdate(currentSpeed);
	setWheelAnimationSpeed(currentSpeed);
}
//Updates Speedometer Value
void MyFrame::speedometerUpdate(double speed) {
	speedometer->SetValue(speed);
	speedValueText->SetLabelText(wxString::Format(wxT("%.1f"), speed));
	speedValueText->Refresh();

}
void MyFrame::setWheelAnimationSpeed(double currentSpeed) {
	//Selects the animated GIFS based on the current Speed.
	if (currentSpeed <0.1) {
		//Stop Animation
		bike_wheelAnim->SetAnimation(wxAnimation(wxT("../eBikeSimulator/gifs/wheel_animation/wheel_still.gif")));
		bike_wheelAnim->Play(false);
	}
	else if (currentSpeed < 5) {
		bike_wheelAnim->SetAnimation(wxAnimation(wxT("../eBikeSimulator/gifs/wheel_animation/wheel_1.gif")));
		bike_wheelAnim->Play(true);
	}
	else if (currentSpeed < 9) {
		bike_wheelAnim->SetAnimation(wxAnimation(wxT("../eBikeSimulator/gifs/wheel_animation/wheel_2.gif")));
		bike_wheelAnim->Play(true);
	}
	else if (currentSpeed < 13) {
		bike_wheelAnim->SetAnimation(wxAnimation(wxT("../eBikeSimulator/gifs/wheel_animation/wheel_3.gif")));
		bike_wheelAnim->Play(true);
	}
	else if (currentSpeed < 17) {
		bike_wheelAnim->SetAnimation(wxAnimation(wxT("../eBikeSimulator/gifs/wheel_animation/wheel_4.gif")));
		bike_wheelAnim->Play(true);
	}
	else if (currentSpeed < 21) {
		bike_wheelAnim->SetAnimation(wxAnimation(wxT("../eBikeSimulator/gifs/wheel_animation/wheel_6.gif")));
		bike_wheelAnim->Play(true);
	}
	else if (currentSpeed < 23) {
		bike_wheelAnim->SetAnimation(wxAnimation(wxT("../eBikeSimulator/gifs/wheel_animation/wheel_7.gif")));
		bike_wheelAnim->Play(true);
	}
	else if (currentSpeed < 25) {
		bike_wheelAnim->SetAnimation(wxAnimation(wxT("../eBikeSimulator/gifs/wheel_animation/wheel_8.gif")));
		bike_wheelAnim->Play(true);
	}

}

void MyFrame::lidar()
{
   const wxPoint pt = wxGetMousePosition();
   int mouseX = pt.x - this->GetScreenPosition().x;
   if (mouseX > 490) mouseX = 490;
	double lidVal; //0-100;
   // POINT point;
   //	GetCursorPos(&point);   //40 - 1386 range of x values on the screen
	lidVal = Mapping(40, 490, 40, 0, mouseX);//point.x
	int val2 = (int)Mapping(40, 490, 40, 0, mouseX);
    if (val2 < 0) val2 = 0;
	if (lidVal < 0) lidVal = 0;
	if (val2 > 40)
	{
		raspberryPiConsole("Obstacle Distance:");
		raspberryPiConsole("OUT OF RANGE");
		lidarTxt->SetLabel("LiDar Dst:OUT OF RANGE");
        
	}
	else
	{
		raspberryPiConsole("Obstacle Distance: " + std::to_string(val2) + "m");
		lidarTxt->SetLabel(wxString::Format("LiDar Distance: %dm", val2));
	}
	setLidarLevel((int)lidVal);
}
double MyFrame::Mapping(int a1, int a2, int b1, int b2, int _percentage)
{
	return (b1 + (((_percentage)*(b2 - b1)) / (a2 - a1)));
}
void MyFrame::setLidarLevel(int val)
{
    if (val> 30)
    {
        lidarGauge->SetActiveBarColour(*wxGREEN);
    }
    else if (val <= 30 && val  >= 15)
    {
        lidarGauge->SetActiveBarColour(*wxYELLOW);
    }
    else
    {
        lidarGauge->SetActiveBarColour(*wxRED);
    }
    if (val < 2) val = 2;
    lidarGauge->SetValue(val);
}

void MyFrame::batteryPercentageCharged(wxCommandEvent& ) {
	//When Set Time is Pressed this function is enabled
	//Sets the Battery Percentage  
	//Displays the Battery Voltage on Pi
	if (!isLocked) {
		
		setBatteryPercentage();
		raspberryPiConsole("Battery Percentage:" + std::to_string(batteryPercentage) + "%");
		raspberryPiConsole("Battery Voltage:" + std::to_string(batteryVoltage) + "V");
	}
    
}
void MyFrame::setBatteryPercentage(){
	//Calculate the Battery Percnetage based on time Elapsed
	//Obtain Total Time in minutes
	int currentTotaltime = (timeElapsed_hours->GetValue() * 60) + timeElapsed_mins->GetValue();
	//Get the percentage of current total time in relation with 180(max  time)
	double dischargedValue = (double(currentTotaltime) / 180.0)*100.0;
	batteryPercentage = 100 - dischargedValue;

	//Setup background color based on percentage
	if (batteryPercentage > 66) {
		batteryGauge->SetActiveBarColour(*wxGREEN);
	}
	else if (batteryPercentage <= 66 && batteryPercentage > 33) {
		batteryGauge->SetActiveBarColour(*wxYELLOW);
	}
	else if (batteryPercentage <= 33) {
		batteryGauge->SetActiveBarColour(*wxRED);
	}
	else {
		wxLogWarning(wxT("Improper Battery Percentage value"));
	}
	//Display the current Percentage of the battery on the Battery Gauge
    batteryGauge->SetValue(batteryPercentage);
    batteryPercentageText->SetLabel(wxString::Format(wxT("Battery level: %d%%"), batteryPercentage));

	//Get the current Battery Voltage  Based on percentage 
	//30.4V is minimum, 42V is the maximum. Range is 11.6=42-30.4
    batteryVoltage=(30.4 + ((double(batteryPercentage)/100.0)*11.6));
    batteryVoltage=(30.4 + ((double(batteryPercentage)/100.0)*11.6));

	//Display Message when Battery Voltage is when battery almost reaching 
	//the minimum voltage of the battery
    if (!isBatteryShutdown && batteryVoltage<31 && batteryVoltage>30.4){
        raspberryPiConsole("DC-DC Converter will" );
        raspberryPiConsole("shutdown soon" );
		isBatteryShutdown = true;
    }
	//Calculate and compare the Voltage going to the LT3748 DC-DC converter
  else  if (batteryVoltage*(10.7/(10.7+255))< minVoltageLT3748){
           raspberryPiConsole("RaspberryPi is OFF" );
		   keyLock();//Lock the Battery
       }
}

void MyFrame::IdleEv(wxIdleEvent&) {
	//This function will always occur when system is Idle
	//Gets the Current times
	ntime2 = clock();
	batTime2 = clock();
	if ( ntime2 - ntime1 >= 500) {//Updates around every 0.5 seconds 
		if (brk_lvl > 0) speedBrake();//If braking is applied Decrease Speed
		if (isincreaseSpeed == true)increaseSpeed(0.5);//increase speed 
		else  if (isdecreaseSpeed == true)decreaseSpeed(0.5);//decrease speed
		//if(!isLocked) lidar();
		ntime1 = ntime2;//Replace ntime1 with ntime2 
	}
	//Depending on throttle position the formula calculates how fast to discharge the battery
	//Time elapsed will change 1 mins every 3 second when throttle is at min
	//It will change 1 min every 0.5 second when throttle is at max
	double timeReduction = 0;
	if(brk_lvl==0)timeReduction=(((2.5 / 1024.0)*digitalThrottleValue) * 1000)*pedalPercantage;
	if (batTime2 - batTime1 >= (3000-timeReduction)) {
		if (doSimulate) {
			int curr_min = timeElapsed_mins->GetValue();
			int curr_hour = timeElapsed_hours->GetValue();
			timeElapsed_mins->SetValue((curr_min + 1) % 60);
			if (timeElapsed_mins->GetValue() == 0) timeElapsed_hours->SetValue(curr_hour + 1);
			setBatteryPercentage();//Sets the current Battery Percentage based on the current Elapsed Time
			if (batteryPercentage == 0) {
				//When it reaches 0 it shuts down
				doSimulate = false;
				keyLock();
			}
		}
		batTime1 = batTime2;
	}
}
void MyFrame::speedBrake() {
	//Brakes the bike reducing the speed with different rate based on brake level
	switch (brk_lvl) {
	case 25:currentSpeed = std::max(currentSpeed - 1.5, 0.0);
		break;
	case 50:currentSpeed = std::max(currentSpeed - 2.5, 0.0);
		break;
	case 75:currentSpeed = std::max(currentSpeed - 3, 0.0);
		break;
	case 100:currentSpeed = std::max(currentSpeed - 4, 0.0);
		break;
	}
	//Updates Current Speed
	speedometerUpdate(currentSpeed);
	setWheelAnimationSpeed(currentSpeed);
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
