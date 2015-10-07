#pragma once
#include "define.h"

class DXGraphic
{
public:
	DXGraphic();
	DXGraphic(std::string path);
	~DXGraphic();
	
	void draw();
	void draw(int, int);
	void drawRect();
	void drawRect(int, int);
	void setRect(int x, int y, int w, int h);
	void setPos(int x, int y);
	void setHandle(std::string path);

protected:
	std::string path;
	std::pair<int, int> draw_pos;

private:
	int handle;

	std::pair<int, int> rect_pos;
	std::pair<int, int> rect_size;
};

