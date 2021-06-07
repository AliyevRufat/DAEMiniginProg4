#include "MiniginPCH.h"
#include "PyramidLoader.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

PyramidLoader::PyramidLoader(const std::string& filePath)
	:m_FilePath{ filePath }
{
}

std::vector<glm::vec3> PyramidLoader::ReadGetPositions()
{
	m_Positions.clear();

	std::ifstream input;
	std::string stringBuf;
	std::string currentLine;
	float bufX = 0;
	float bufY = 0;

	input.open(m_FilePath, std::ios::in | std::ios::binary);
	if (input.is_open())
	{
		while (std::getline(input, currentLine))
		{
			for (size_t i = 0; i < currentLine.size(); i++)
			{
				if (currentLine[i] != ',' && currentLine[i] != '\r')
				{
					stringBuf += currentLine[i];
				}
				else if (currentLine[i] == ',')
				{
					bufX = std::stof(stringBuf);
					stringBuf = "";
				}
			}
			bufY = std::stof(stringBuf);
			stringBuf = "";
			m_Positions.push_back(glm::vec3{ bufX, bufY, 0 });
		}
	}

	return m_Positions;
}