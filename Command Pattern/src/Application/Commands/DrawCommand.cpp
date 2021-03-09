#include "Application/Commands/DrawCommand.h"
#include "Application/Application.h"

DrawCommand::DrawCommand(std::vector<glm::vec2> locations, unsigned char* bitMap)
	: m_StrokeLocations(locations), m_BitMap(bitMap)
{

	float minx =  Application::WIDTH;
	float maxx =  0;
	float miny =  Application::HEIGHT;
	float maxy =  0;

	for (auto location : m_StrokeLocations)
	{
		if      (location.x < minx) minx = location.x;
		if (location.x > maxx) maxx = location.x;
		if		(location.y < miny) miny = location.y;
		if (location.y > maxy) maxy = location.y;
	}
	minx -= 10;
	maxx += 10;
	miny -= 10;
	maxy += 10;
	
	if (minx < 0) minx = 0;
	if (maxx > Application::WIDTH) maxx = Application::WIDTH;
	if (miny < 0) miny = 0;
	if (maxy > Application::HEIGHT) maxy =	Application::HEIGHT;

	w = maxx - minx;
	h = maxy - miny;
	m_SavedBytes = new unsigned char[w*h*4];
	for (int x = minx; x < maxx; x++)
	{
		for (int y = miny; y < maxy; y++)
		{
			int _x = x - minx;
			int _y = y - miny;
			int index = (x + y * Application::WIDTH) * 4;
			int savedIndex = (_x+_y*w) * 4;
			m_SavedBytes[savedIndex + 0] = m_BitMap[index + 0];
			m_SavedBytes[savedIndex + 1] = m_BitMap[index + 1];
			m_SavedBytes[savedIndex + 2] = m_BitMap[index + 2];
			m_SavedBytes[savedIndex + 3] = m_BitMap[index + 3];

		}
	}

	xOffset = minx;
	yOffset = miny;

	Execute();
}

DrawCommand::~DrawCommand()
{
	delete[] m_SavedBytes;
}

void DrawCommand::Execute()
{
	for (auto location : m_StrokeLocations)
	{
		for (int x = -10; x < 10; x++)
		{
			for (int y = -10; y < 10; y++)
			{
				if (x * x + y * y < 10 * 10)
				{
					int _x = x + location.x;
					int _y = y + location.y;

					if (_x >= 0 && _y >= 0 && _x < Application::WIDTH && _y < Application::HEIGHT)
					{
						int index = (_x + _y * Application::WIDTH) * 4;
						m_BitMap[index + 0] = 255;
						m_BitMap[index + 1] = 255;
						m_BitMap[index + 2] = 255;
						m_BitMap[index + 3] = 255;
					}
				}
			}
		}
	}
}

void DrawCommand::Undo()
{
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			int _x = x + xOffset;
			int _y = y + yOffset;
			int index = (_x + _y * Application::WIDTH) * 4;
			int savedIndex = (x + y * w) * 4;
			m_BitMap[index + 0] = m_SavedBytes[savedIndex + 0];
			m_BitMap[index + 1] = m_SavedBytes[savedIndex + 1];
			m_BitMap[index + 2] = m_SavedBytes[savedIndex + 2];
			m_BitMap[index + 3] = m_SavedBytes[savedIndex + 3];
		}
	}
}
