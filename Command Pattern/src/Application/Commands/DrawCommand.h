#pragma once
#include "Application/Command.h"

#include <vector>
#include "glm/vec2.hpp"

class DrawCommand : public Command
{
public:
	DrawCommand(std::vector<glm::vec2> locations, unsigned char* bitMap);
	~DrawCommand();

	virtual void Execute() override;

	virtual void Undo() override;
private:
	std::vector<glm::vec2> m_StrokeLocations;
	unsigned char* m_BitMap;
	unsigned char* m_SavedBytes;
	unsigned int xOffset;
	unsigned int yOffset;
	unsigned int w;
	unsigned int h;
};