#ifndef __KOOPATLAS_CAMERA_H
#define __KOOPATLAS_CAMERA_H

#include "koopatlas/koopatlasCore.hpp"

#define STD_ZOOM 2.8f

class dKPCamera_c : public dActor_c {
	public:
		int onCreate();
		int onDelete();
		int onExecute();
		int onDraw();

		dKPCamera_c();

		EGG::Screen screen;
		EGG::LookAtCamera camera3d;

		EGG::ProjectOrtho projection2d;
		EGG::LookAtCamera camera2d;

		Vec camPos, camTarget, camUp;

		float currentX, currentY, zoomLevel;

		void calculateScreenGeometry();


		float zoomDivisor, screenLeft, screenTop, screenWidth, screenHeight;

		bool followPlayer;
		void panToBounds(float left, float top, float right, float bottom);
		void panToPosition(float x, float y, float zoom=STD_ZOOM);
		bool panning;
		float panFromX, panFromY, panToX, panToY;
		float panFromZoom, panToZoom;
		float panCurrentStep;
		float panTotalSteps;

		void doStuff(float);
		void generateCameraMatrices();
		void updateCameras();

		static dActor_c *build();
		static dKPCamera_c *instance;
};

#endif

