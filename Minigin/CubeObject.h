#pragma once
#include "GameObject.h"

class CubeObject
{
public:
	CubeObject();
	void Color();

	~CubeObject() = default;

	CubeObject(const CubeObject& other) = delete;
	CubeObject(CubeObject&& other) = delete;
	CubeObject& operator=(const CubeObject& other) = delete;
	CubeObject& operator=(CubeObject&& other) = delete;

	void Update();
	void Render();

	std::shared_ptr<GameObject> GetGameObject() const { return m_pGameObject; };
private:

	std::shared_ptr<GameObject> m_pGameObject;

	bool m_Colored;
};
