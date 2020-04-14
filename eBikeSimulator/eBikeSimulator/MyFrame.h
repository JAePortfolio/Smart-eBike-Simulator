/**
	* @file		cMain.h
	* @authors	John Arena, Shamim Babul, Rona Kosumi, Ionut Rotariu
	* @license	This project is NOT released under any license.
	* @date		Sept 2019-May 2020
*/


#pragma once


#include "wx/wx.h"

class MyFrame : public wxFrame
{
public:							// Function declarations
	MyFrame();
	~MyFrame();
public:							// Variable declarations? Objects?
	wxButton *m_btn1 = nullptr;
	wxTextCtrl *m_txt1 = nullptr;

};

