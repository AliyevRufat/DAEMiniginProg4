#pragma once
#include <glm\vec3.hpp>
#include <vector>

class PyramidLoader final
{
public:
	PyramidLoader(const std::string& filePath);
	~PyramidLoader() = default;

	PyramidLoader(const PyramidLoader&) = delete;
	PyramidLoader(PyramidLoader&&) noexcept = delete;
	PyramidLoader& operator=(const PyramidLoader&) = delete;
	PyramidLoader& operator=(PyramidLoader&&) noexcept = delete;

	std::vector<glm::vec3> ReadGetPositions();
private:
	std::vector<glm::vec3> m_Positions;
	std::string m_FilePath;
};
