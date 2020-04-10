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