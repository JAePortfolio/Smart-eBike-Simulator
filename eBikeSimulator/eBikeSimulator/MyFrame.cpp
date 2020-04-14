/**
	* @file		cMain.cpp
	* @authors	John Arena, Shamim Babul, Rona Kosumi, Ionut Rotariu
	* @license	This project is NOT released under any license.
	* @date		Sept 2019-May 2020
*/

#include "MyFrame.h"
#include "wx/bitmap.h"
#include "wx/stdpaths.h"
#include "wx/file.h"
#include <wx/log.h> 


MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Smart eBike Simulator - Senior Design", wxPoint(30,30), wxSize(1366,768))
{
	wxPanel * panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1364, 766));
	panel->SetBackgroundColour(wxColour(*wxWHITE));

	wxString dir;
	if (wxFile::Exists(wxT("./bike_sideView.png")))
		dir = wxT("./");
	else if (wxFile::Exists(wxT("../bike_sideView.png")))
		dir = wxT("../");
	else
		wxLogWarning(wxT("Can't find image files in either '.' or '..'!"));

	wxInitAllImageHandlers;
	wxPNGHandler *handler = new wxPNGHandler;
	wxStaticBitmap *image;
	image = new wxStaticBitmap(this, wxID_ANY, wxBitmap("../bike_sideView.png", wxBITMAP_TYPE_PNG), wxPoint(50,100), wxSize(500,500));
}



MyFrame::~MyFrame()
{
}
