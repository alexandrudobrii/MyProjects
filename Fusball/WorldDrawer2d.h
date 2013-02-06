#pragma once
#include "HeadersAndDefines.h"
#include "Support2d.h"

//-----------------------------------------------------------------------------------------------------
//	WorldDrawer2d
//	ca un obiect sa fie desenat trebuie sa fie atasat ori la cs_basis ori la un sistem de coordonate
//	din lista cs_used.
//-----------------------------------------------------------------------------------------------------
class WorldDrawer2d{
	public:
		//implemented in worldDrawer2d_gl .. not for lab1
		WorldDrawer2d();
		WorldDrawer2d(int argc, char **argv, int windowWidth, int windowHeight, int windowStartX, int windowStartY, std::string windowName);
		~WorldDrawer2d();
		static void run();	
		static void displayCallbackFunction();
		static void reshapeCallbackFunction(int width, int height);
		static void idleCallbackFunction();
		static void keyboardCallbackFunction(unsigned char key, int x, int y);


		//implemented in worldDrawer2d_logic .. for lab1
		static void init();
		static void onIdle();
		static void onKey(unsigned char key);

	public:
		static bool animation;
		static CoordinateSystem2d cs_basis;
		static std::vector<CoordinateSystem2d*> cs_used;
};