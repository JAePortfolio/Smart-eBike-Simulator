/**
	* @file		eBikeSimulator.h
	* @authors	John Arena, Shamim Babul, Rona Kosumi, Ionut Rotariu
	* @license	This project is NOT released under any license.
	* @date		Sept 2019-May 2020
*/


#pragma once

#include "wx/wx.h"
#include "cMain.h"

class eBikeSimulator : public wxApp
{
public:
	eBikeSimulator();
	~eBikeSimulator();

private:
	cMain* m_frame1 = nullptr;

public:
	virtual bool OnInit();
};

