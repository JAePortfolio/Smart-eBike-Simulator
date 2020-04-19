/**
	* @file		cMain.h
	* @authors	John Arena, Shamim Babul, Rona Kosumi, Ionut Rotariu
	* @license	This project is NOT released under any license.
	* @date		Sept 2019-May 2020
*/


#pragma once

#include "wx/wx.h"
#include <chrono>

class MyFrame : public wxFrame
{
public:							// Frame declaration
	MyFrame();
	~MyFrame();
public:							// Global variables, other function delcarations, etc
	wxButton *m_btn1 = nullptr;
	wxTextCtrl *m_txt1 = nullptr;
    //Define names of break pictures ON and OFF
	wxStaticBitmap *image, *bike_rearViewImage, *brakePicture, *brakeGif;
	wxStaticText *textForControls;
	bool headlightOn;
	void OnKeyDown(wxKeyEvent& event);
	wxDECLARE_EVENT_TABLE();
	void leftTurnSignal();
	void rightTurnSignal();
	void headlightActivation();
    void controlBrake(int _throttle, int _break);
    void SetBrakePicture(bool);
private:     int brakeLevel;
};

