#include "define.h"
#include "DXGraphic.h"
class ECSystem{
public:
	ECSystem::ECSystem();
	ECSystem::~ECSystem();

	void ECSystem::drawsystem();
	void ECSystem::drawkey(int key, int y);
	
private:
	DXGraphic frame, lane, notes;
};