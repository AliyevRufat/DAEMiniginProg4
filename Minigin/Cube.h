#pragma once
#include "GameObject.h"

class Cube
{
public:
	Cube();
	void IsActive();
private:
	std::shared_ptr<GameObject> m_pCube;
	bool m_IsActive;
};
