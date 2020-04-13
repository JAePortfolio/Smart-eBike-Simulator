/**
	* @file		cMain.cpp
	* @authors	John Arena, Shamim Babul, Rona Kosumi, Ionut Rotariu
	* @license	This project is NOT released under any license.
	* @date		Sept 2019-May 2020
*/

#include "cMain.h"


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Smart eBike Simulator - Senior Design", wxPoint(30,30), wxSize(1366,768))
{
	m_btn1 = new wxButton(this, wxID_ANY, "Click Me", wxPoint(10, 10), wxSize(100,25));
	m_txt1 = new wxTextCtrl(this, wxID_ANY, "Type Here", wxPoint(10, 70), wxSize(300, 30));
}


cMain::~cMain()
{
}
