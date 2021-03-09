#pragma once

#include "Command.h"

#include <vector>

class CommandList
{
public:

	void AddCommand(Command* commmand);

	void Redo();
	void Undo();

private:
	unsigned int m_CommandPointer;

	std::vector<Command*> m_Commands;
};