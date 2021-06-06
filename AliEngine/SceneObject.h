#pragma once
#include <string>

namespace dae
{
	class SceneObject
	{
	public:
		virtual void Update() = 0;
		virtual void Render() const = 0;
		//
		SceneObject(const std::string& name) :m_Name{ name } {};
		virtual ~SceneObject() = default;
		SceneObject(const SceneObject& other) = delete;
		SceneObject(SceneObject&& other) = delete;
		SceneObject& operator=(const SceneObject& other) = delete;
		SceneObject& operator=(SceneObject&& other) = delete;
		//
		const std::string& GetName() const { return m_Name; };

		void SetIsActive(bool isActive) { m_IsActive = isActive; }
		bool GetIsActive() const { return m_IsActive; }

		void SetMarkForDelete(bool markFordelete) { m_MarkForDelete = markFordelete; }
		bool GetMarkForDelete() const { return m_MarkForDelete; }
	protected:
		bool m_IsActive = true;
		const std::string m_Name;
		bool m_MarkForDelete = false;
	};
}
