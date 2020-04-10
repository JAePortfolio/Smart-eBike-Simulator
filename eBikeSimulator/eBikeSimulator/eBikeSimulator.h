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

