#include "CommandList.h"

void CommandList::AddCommand(Command* commmand)
{
	if (m_CommandPointer != m_Commands.size())
	{
		m_Commands.erase(m_Commands.begin() + m_CommandPointer, m_Commands.end());
	}
	m_CommandPointer++;
	m_Commands.push_back(commmand);
}

void CommandList::Redo()
{
	if (m_CommandPointer < m_Commands.size())
	{
		m_Commands[m_CommandPointer]->Execute();
		m_CommandPointer++;
	}
}

void CommandList::Undo()
{
	if (m_CommandPointer > 0)
	{
		m_Commands[m_CommandPointer - 1]->Undo();
		m_CommandPointer--;
	}
}
