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

	wxStaticBitmap *image, *bike_rearViewImage;
	void OnKeyDown(wxKeyEvent& event);
	void leftTurnSignal();
	void rightTurnSignal();
	wxDECLARE_EVENT_TABLE();
};

