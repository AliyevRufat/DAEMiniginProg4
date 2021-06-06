#pragma once
#include "../AliEngine/Observer.h"
#include "../AliEngine/GameObject.h"

class LivesObserver : public Observer
{
public:
	void OnNotify(const GameObject* actor, Event event) override;

private:
	void ChangeLives(const GameObject* actor);
};
