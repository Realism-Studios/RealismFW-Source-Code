#include <game.hpp>
#include <dCourse.hpp>
#include <stage.hpp>
#include "tryAgain.hpp"

extern void FUN_80069E50();
extern void FreeBackgroundList();
extern void FUN_80926210(dScStage_c *);
bool StageThing1, StageThing2, StageThing3, StageThing4, StageThing5, StageThing6, StageThing7, StageThing8;

void dScStage_c::loadNewStageActors() {
	FUN_80069E50(); // what we replaced to call this function

	dScTryAgain_c::instance = (dScTryAgain_c*)CreateParentedObject(ProfileId::TryAgain, this, 0, 0);
}

void dScStage_c::unloadNewStageActors() {
	FreeBackgroundList(); // what we replaced

	dScTryAgain_c *tryAgain = dScTryAgain_c::instance;
	tryAgain->Delete();
}

void StageDrawFunc() {
	if (!StageThing1)
		StageThing1 = true;
	if (!StageThing2)
		StageThing2 = true;
	if (!StageThing3)
		StageThing3 = true;
	if (!StageThing4)
		StageThing4 = true;
	if (!StageThing5)
		StageThing5 = true;
	if (!StageThing6)
		StageThing6 = true;
	if (!StageThing7)
		StageThing7 = true;
	if (!StageThing8)
		StageThing8 = true;

	dCamera_c *stageCam = dScStage_c::instance->getCamera(0);
	DoSomethingCameraRelatedWithEGGScreen(GetCurrentCameraID(), &stageCam->screen);

	// setup Scene 0
	LinkScene(0);
	ChangeAlphaUpdate(0);
	SceneCalcWorld(0);
	SceneCameraStuff(0);
	DrawOpa();
	DrawXlu();

	// leaving out a massive section here, this handles the looping for the 2-Castle bullshit
	// since we don't use it, i'm not gonna bother reimplementing it

	FUN_80926210(dScStage_c::instance);

	// crashes, idk why
	//CameraGXSetViewport(GetCurrentCamera());
	//CameraGXSetScissor(GetCurrentCamera());

	RenderEffects(0, 6);
	SetupLYTDrawing();
	DrawAllLayoutsBeforeX(0x81);
	RenderEffects(0, 3);
	RenderEffects(0, 2);
	UnlinkScene(0);

	GXDrawDone();
	RemoveAllFromScnRoot();
	Reset3DState();

	// setup Scene 1
	SetCurrentCameraID(1);
	DoSpecialDrawing1();
	LinkScene(1);
	SceneCalcWorld(1);
	SceneCameraStuff(1);
	CalcMaterial();
	DrawOpa();
	DrawXlu();
	UnlinkScene(1);

	GXDrawDone();
	RemoveAllFromScnRoot();
	Reset3DState();
	DrawAllLayoutsAfterX(0x80);
	ClearLayoutDrawList();

	SetCurrentCameraID(0);
	FUN_80926210(dScStage_c::instance);
}