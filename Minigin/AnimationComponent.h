#pragma once
#include "ComponentIncludes.h"

class AnimationComponent final : public BaseComponent
{
public:
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
		Cycle = 0,
	};

	enum class CubeColorState
	{
		Red = 0,
		Yellow = 1,
		Blue = 2
	};

	AnimationComponent(int nrOfColumns);
	void Animate();
	void Render() override;
	void SetAnimationState(AnimationState animState);
	void SetAnimationState(CubeColorState cubeAnimState);
	AnimationState GetAnimationState() const;
	CubeColorState GetCubeColorState() const;
	int GetNrOfColumns() const;
private:
	std::shared_ptr<dae::Texture2D> m_spTexture2D{};
	int m_NrOfColumns;
	//
	AnimationState m_AnimState;
	CubeColorState m_CubeColorState;
};
