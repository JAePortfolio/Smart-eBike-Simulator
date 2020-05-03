/**
	* @file		cMain.h
	* @authors	John Arena, Shamim Babul, Rona Kosumi, Ionut Rotariu
	* @license	This project is NOT released under any license.
	* @date		Sept 2019-May 2020
*/


#pragma once

#include "wx/wx.h"
#include <chrono>
#include <string>
//#include "kwic/LCDWindow.h"
#include "wx/spinctrl.h"
#include "wx/button.h"
#include <wx/animate.h>
#include <wx/gauge.h>
class MyFrame : public wxFrame
{
public:							// Frame declaration
	MyFrame();
	~MyFrame();
public:							// Global variables, other function delcarations, etc
    LPPOINT* cursorPos;            // Point to save cursor's position
	wxButton *m_btn1 = nullptr;
	wxTextCtrl *m_txt1 = nullptr;
	wxListBox *raspberryPi = nullptr;
	wxStaticBitmap *bike_sideViewImage, *bike_rearViewImage, *brakePicture, *brakeGif;
	wxStaticText *textForControls;
	wxSlider* throttleSlider;
	wxStaticText* throttleSliderValue;

	wxSpinCtrl* timeElapsed_hours;
	wxSpinCtrl* timeElapsed_mins;
	wxStaticText* timeElapsedText;
	wxStaticText* timeElapsedHours;
	wxStaticText* timeElapsedMins;
    wxStaticText* lidarTxt;
	wxButton* setTimeElapsedButton;
	wxAnimationCtrl* brakingAnim;
	wxAnimationCtrl* bike_wheelAnim;
	

	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	wxDECLARE_EVENT_TABLE();

	void textForControlsSetup();
	void raspberryPiSetup();
	void initialImageDisplaySetup();
	void throttleTextSetup();
	void timeElapsedSetup();
	void raspberryPiConsole(std::string outputMessage);
	void leftTurnSignal();
	void rightTurnSignal();
	void headlightActivation();
    void controlBrake(int _throttle, int _break);
    void SetBrakePicture(bool);
	void OnThrottleSliderScrolled(wxCommandEvent&);
	void increaseSpeed();
	void decreaseSpeed();
    void keyLockSetup();
    void keyLock();
    void keyUnlock();
    void batteryGaugeSetup();
    void batteryPercentageCharged(wxCommandEvent& );
    void setBatteryPercentage();
    void lidarGaugeSetup();
	void speedometerSetup();
	void speedometerUpdate(double speed);
    void setLidarLevel(int);
	void lidar();
    double Mapping(int a1, int a2, int b1, int b2, int _percentage);   //map values 40 to 1386 to a 0-100% range for lidar
	void IdleEv(wxIdleEvent&);
	void speedBrake();

private:     
	int brakeLevel;
	double digitalThrottleValue = 0.0; //Digital Throttle Value from 0 to 1024
	double currentSpeed,tmpSpeed = 0.0; //current speed and temporary speed 
	wxStaticText* speedText;//Temporary Speed Display
	bool upKeyPressed = false;
	clock_t keyPressedTime, keyReleasedTime;//to keep track how long Up/Down key was pressed
	clock_t ntime1, ntime2;
	double totalKeyPressedTime;
	const double bikeAcceleration = 25.0 / 13.0; //Accelearion of bike at FUll throttle
	const double analogToDigitalRatio = 35.0 / 1024.0;
	bool headlightOn;

};

