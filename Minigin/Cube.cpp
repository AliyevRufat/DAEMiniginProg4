#include "MiniginPCH.h"
#include "Cube.h"

Cube::Cube()
	:m_IsActive{ false }
{
	m_pCube = std::make_shared<GameObject>("Cube");
}

void Cube::IsActive()
{
}