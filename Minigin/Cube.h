#pragma once
#include "../AliEngine/Scene.h"
#include "../AliEngine/GameObject.h"
#include "glm/vec2.hpp"

class Cube final
{
public:
	enum class ColorState
	{
		FirstColor,
		SecondColor,
		ThirdColor
	};

	Cube();

	~Cube() = default;

	Cube(const Cube& other) = delete;
	Cube(Cube&& other) = delete;
	Cube& operator=(const Cube& other) = delete;
	Cube& operator=(Cube&& other) = delete;

	void Update();
	void Render();

	std::shared_ptr<GameObject> GetGameObject() const { return m_pGameObject; };

	void ChangeColor(bool& hasColored);
	void ResetColor();

	ColorState GetCurrentColorState() const { return m_ColorState; }

	bool GetHasDiscNextToIt() const;
	void SetHasDiscNextToIt(bool hasDisc);

	void ColorFlash();
	glm::vec2 GetPosition() const;
private:

	std::shared_ptr<GameObject> m_pGameObject;

	ColorState m_ColorState;
	dae::Scene::Level m_CurrentLevel;
	bool m_HasDiscNextToIt;

	const float m_MaxColorSwitchTime;
	float m_CurrentColorSwitchTime;
};
