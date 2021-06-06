#include "MiniginPCH.h"
#include "ScoreObserver.h"
#include "../AliEngine/SceneManager.h"
#include "../AliEngine/Scene.h"

#include "HealthComponent.h"
#include "../AliEngine/FPSTextComponent.h"
#include "ScoreComponent.h"
#include "../AliEngine/TextComponent.h"
#include "../AliEngine/Texture2DComponent.h"
#include "../AliEngine/TransformComponent.h"

void ScoreObserver::OnNotify(const GameObject* actor, Event event)
{
	if (event == Event::CatchSlickOrSam || event == Event::CoilyHitWithFlyingDisc || event == Event::ColorChanged || event == Event::DiscLeftAtEndOfTheStage || event == Event::ScoreSet)
	{
		ChangeScore(actor);
	}
}

void ScoreObserver::ChangeScore(const GameObject* actor)
{
	std::shared_ptr<dae::SceneObject> spPointsDisplay = nullptr;
	if (actor->GetName() == "Q*Bert")
	{
		spPointsDisplay = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("ScoreDisplay");
	}
	else
	{
		spPointsDisplay = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("ScoreDisplay2");
	}

	if (!spPointsDisplay) {
		std::cout << "WARNING: pointsDisplay not found after player death" << std::endl;
		return;
	}
	auto textComp = std::static_pointer_cast<GameObject>(spPointsDisplay).get()->GetComponent<TextComponent>();

	if (!textComp) {
		std::cout << "WARNING: TextComp of pointsDisplay not found after player death" << std::endl;
		return;
	}

	auto scoreComp = actor->GetComponent<ScoreComponent>();

	if (!scoreComp) {
		std::cout << "WARNING: ScoreComp not found after player death" << std::endl;
		return;
	}
	textComp->UpdateText("Score: " + std::to_string(scoreComp->GetScore()));
}