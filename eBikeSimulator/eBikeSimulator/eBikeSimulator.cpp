/**
	* @file		eBikeSimulator.cpp
	* @authors	John Arena, Shamim Babul, Rona Kosumi, Ionut Rotariu
	* @license	This project is NOT released under any license.
	* @date		Sept 2019-May 2020
*/



#include "eBikeSimulator.h"


// Think of eBikeSimulator as the launcher
wxIMPLEMENT_APP(eBikeSimulator); // Think of it as int main that works across different OS


eBikeSimulator::eBikeSimulator()
{
}


eBikeSimulator::~eBikeSimulator()
{
}

bool eBikeSimulator::OnInit() {
	m_frame1 = new cMain();
	m_frame1->Show();
	return true;
}