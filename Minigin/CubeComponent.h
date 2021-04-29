#pragma once
#include "GameObject.h"

class CubeComponent
{
public:
	CubeComponent();
	void IsActive();
private:
	//std::shared_ptr<GameObject> m_pCube;
	bool m_IsActive;
};
