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


class MyFrame : public wxFrame
{
public:							// Frame declaration
	MyFrame();
	~MyFrame();
public:							// Global variables, other function delcarations, etc
	wxButton *m_btn1 = nullptr;
	wxTextCtrl *m_txt1 = nullptr;
	wxListBox *raspberryPi = nullptr;
    //Define names of break pictures ON and OFF
	wxStaticBitmap *image, *bike_rearViewImage, *brakePicture, *brakeGif;
	wxStaticText *textForControls;
	bool headlightOn;
	wxSlider* throttleSlider;
	wxStaticText* throttleSliderValue;

	//kwxLCDDisplay* test; Leave disabled
	wxSpinCtrl* timeElapsed_hours;
	wxSpinCtrl* timeElapsed_mins;
	wxStaticText* timeElapsedText;
	wxStaticText* timeElapsedHours;
	wxStaticText* timeElapsedMins;
	wxButton* setTimeElapsedButton;
	
	wxAnimationCtrl* brakingAnim;
	

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

private:     
	int brakeLevel;
	double digitalThrottleValue = 0.0;
	double currentSpeed,tmpSpeed = 0.0;
	wxStaticText* speedText;//Temporary Speed Display
	bool upKeyPressed = false;
	clock_t keyPressedTime, keyReleasedTime;
	double totalKeyPressedTime;
	const double bikeAcceleration = 25.0 / 13.0;
};

