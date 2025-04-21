// goomba boss from that one newer dev video - "Newer Project - 4b - 9-8Proto"
#include <daEnKuribo_c.hpp>
#include <profile.hpp>
#include "boss.hpp"

class daEnGoombaBoss_c : public daEnKuribo_c {
    public:
        daBoss boss;
        
        //void setWalkSpeed();
        
        static daEnGoombaBoss_c *instance;
        static dActor_c *build();

        //USING_STATES(daEnGoombaBoss_c);
        //DECLARE_STATE(Init);
        //DECLARE_STATE(Die);
};

const char *GoombaBossFileList[] = {"kuribo"};
const SpriteData GoombaBossSpriteData = {ProfileId::GoombaBoss, 8, -16, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile GoombaBossProfile(&daEnGoombaBoss_c::build, SpriteId::GoombaBoss, &GoombaBossSpriteData, ProfileId::AC_DOKAN_CANNON, ProfileId::GoombaBoss, "GoombaBoss", GoombaBossFileList);

daEnGoombaBoss_c *daEnGoombaBoss_c::instance = 0;

dActor_c *daEnGoombaBoss_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnGoombaBoss_c));
	daEnGoombaBoss_c *c = new(buffer) daEnGoombaBoss_c;

	instance = c;
	return c;
}

// int daEnGoombaBoss_c::onCreate() {
//     Vec actorScale = {1.5, 1.5, 1.5};
//     scale = actorScale;
// }

//void daEnGoombaBoss_c::setWalkSpeed() {
//    speed.x = (direction == RIGHT) ? 0.5 : -0.5;
//}