#include "cMain.h"
//Shamim Babul
// Think of cMain as the GUI. Structure and Event Handlers

//Rona Kosumi 
//test123

//Ionut Rotariu

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Smart eBike Simulator - Senior Design", wxPoint(30,30), wxSize(800,600))
{
	m_btn1 = new wxButton(this, wxID_ANY, "Click Me", wxPoint(10, 10), wxSize(100,25));
	m_txt1 = new wxTextCtrl(this, wxID_ANY, "Type Here", wxPoint(10, 70), wxSize(300, 30));
}


cMain::~cMain()
{
}
