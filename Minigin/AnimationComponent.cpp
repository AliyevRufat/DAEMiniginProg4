#include "MiniginPCH.h"
#include "AnimationComponent.h"
#include "Texture2DComponent.h"

AnimationComponent::AnimationComponent(int nrOfColumns)
	:m_NrOfColumns{ nrOfColumns }
	, m_AnimState{}
{
}

SDL_Rect AnimationComponent::Animate()
{
	if (!m_IsInitialized)
	{
		m_IsInitialized = true;
		m_spTexture2D = m_pGameObject->GetComponent<Texture2DComponent>()->GetTexture2D();
	}

	if ((int)m_AnimState <= m_NrOfColumns)
	{
		int textureWidth, textureHeight;
		SDL_QueryTexture(m_spTexture2D.get()->GetSDLTexture(), nullptr, nullptr, &textureWidth, &textureHeight);
		SDL_Rect srcRect;
		srcRect.h = textureHeight;
		srcRect.w = textureWidth / m_NrOfColumns;
		srcRect.y = 0;
		srcRect.x = srcRect.w * (int)m_AnimState;
		return srcRect;
	}
	return SDL_Rect();
}

void AnimationComponent::SetAnimationState(AnimationState animState)
{
	m_AnimState = animState;
}