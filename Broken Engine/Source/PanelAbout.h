#ifndef __PANEL_ABOUT_H__
#define __PANEL_ABOUT_H__

#include "BrokenEngine.h"

class PanelAbout : public Broken::Panel
{
public:

	PanelAbout(char* name);
	~PanelAbout();

	bool Draw();
};

#endif