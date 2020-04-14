/**
	* @file		eBikeSimulator.h
	* @authors	John Arena, Shamim Babul, Rona Kosumi, Ionut Rotariu
	* @license	This project is NOT released under any license.
	* @date		Sept 2019-May 2020
*/


#pragma once

#include "wx/wx.h"
#include "MyFrame.h"

class MyApp : public wxApp
{
public:
	MyApp();
	~MyApp();

private:
	MyFrame* m_frame1 = nullptr;

public:
	virtual bool OnInit();
};

