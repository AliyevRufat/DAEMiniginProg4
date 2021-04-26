#include "MiniginPCH.h"
#include "AnimationComponent.h"
#include "Texture2DComponent.h"

AnimationComponent::AnimationComponent(int nrOfColumns)
	:m_NrOfColumns{ nrOfColumns }
{
}

SDL_Rect AnimationComponent::Animate(AnimationState animState)
{
	if (!m_IsInitialized)
	{
		m_IsInitialized = true;
		m_spTexture2D = m_pGameObject->GetComponent<Texture2DComponent>()->GetTexture2D();
	}

	if ((int)animState <= m_NrOfColumns)
	{
		int textureWidth, textureHeight;
		SDL_QueryTexture(m_spTexture2D.get()->GetSDLTexture(), nullptr, nullptr, &textureWidth, &textureHeight);
		SDL_Rect srcRect;
		srcRect.h = textureHeight;
		srcRect.w = textureWidth / m_NrOfColumns;
		srcRect.y = 0;
		srcRect.x = srcRect.h * (int)animState;
		return srcRect;
	}
	return SDL_Rect();
}