#pragma once
#include "ComponentIncludes.h"

class ScoreComponent final : public BaseComponent
{
public:
	ScoreComponent(const unsigned int& score);

	const unsigned int& GetScore() const;
	void SetScore(const unsigned int&);
	void IncreaseScore(const int score);
private:
	unsigned int m_Score;
};