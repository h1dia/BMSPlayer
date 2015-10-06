#include "DXGraphic.h"

DXGraphic::DXGraphic() {
	draw_pos.first = 0;
	draw_pos.second = 0;
}

DXGraphic::DXGraphic(std::string path) {
	handle = LoadGraph(path.c_str());
	draw_pos.first = 0;
	draw_pos.second = 0;
}

DXGraphic::~DXGraphic(){
	DeleteGraph(handle);
}

void DXGraphic::setHandle(std::string path){
	DeleteGraph(handle);
	handle = LoadGraph(path.c_str());
}

void DXGraphic::draw(){
	DrawGraph(draw_pos.first, draw_pos.second, handle, TRUE);
}

void DXGraphic::draw(int x, int y){
	DrawGraph(x, y, handle, TRUE);
}

void DXGraphic::drawRect(){
	DrawRectGraph(draw_pos.first, draw_pos.second, rect_pos.first, rect_pos.second,
		rect_size.first, rect_size.second, handle, TRUE, FALSE);
}

void DXGraphic::drawRect(int x, int y){
	DrawRectGraph(x, y, rect_pos.first, rect_pos.second,
		rect_size.first, rect_size.second, handle, TRUE, FALSE);
}

void DXGraphic::setRect(int x, int y, int w, int h){
	rect_pos.first = x;
	rect_pos.second = y;
	rect_size.first = w;
	rect_size.second = h;
}

void DXGraphic::setPos(int x, int y){
	rect_pos.first = x;
	rect_pos.second = y;
}