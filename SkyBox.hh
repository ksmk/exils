#pragma once

#include "Definitions.hh"

class SkyBox
{
public:
	SkyBox();
	~SkyBox();
	void Load(Float zfar);
	void Display(Int tid, Float player_x, Float player_y, Float player_z);

private:
	Int SkyBox_id;
};


