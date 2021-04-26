#pragma once
#include "ComponentIncludes.h"

enum class AnimationState
{
	IdleRightTop = 0,
	JumpRightTop = 1,
	IdleLeftTop = 2,
	JumpLeftTop = 3,
	IdleRightDown = 4,
	JumpRightDown = 5,
	IdleLeftDown = 6,
	JumpLeftDown = 7,
	IdleLeft = 0,
	JumpLeft = 1,
	IdleRight = 2,
	JumpRight = 3,
	//
	Cycle = 8,
};

class AnimationComponent final : public BaseComponent
{
public:
	AnimationComponent(int nrOfColumns);
	SDL_Rect Animate(AnimationState animState);
private:
	std::shared_ptr<dae::Texture2D> m_spTexture2D{};
	int m_NrOfColumns;
};
