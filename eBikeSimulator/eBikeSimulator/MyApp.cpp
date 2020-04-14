/**
	* @file		eBikeSimulator.cpp
	* @authors	John Arena, Shamim Babul, Rona Kosumi, Ionut Rotariu
	* @license	This project is NOT released under any license.
	* @date		Sept 2019-May 2020
*/

#include "MyApp.h"
#include "wx/image.h"


// Think of eBikeSimulator as the launcher
wxIMPLEMENT_APP(MyApp); // Think of it as int main that works across different OS


MyApp::MyApp()  // Constructor - Class name::Function 
{
}


MyApp::~MyApp() // Destructor - Class name::Function
{
}

bool MyApp::OnInit() {
	wxInitAllImageHandlers;

	m_frame1 = new MyFrame();
	m_frame1->Show();



	return true;
}