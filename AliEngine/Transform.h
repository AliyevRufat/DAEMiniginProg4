#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#pragma warning(pop)

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec2& GetPosition() const { return m_Position; }
		const glm::vec2& GetScale() const { return m_Scale; }
		void SetPosition(float x, float y);
		void SetScale(float x, float y);
	private:
		glm::vec2 m_Position;
		glm::vec2 m_Scale;
	};
}
