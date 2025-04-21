#include <game.hpp>
#include <common.hpp>
#include <sfx.hpp>

extern "C" void cvtSndObjctPos(Vec2 *out, Vec *stage_pos);

void landBarrelEffectFix(dStageActor_c *self) {
    Vec2 soundPos;
    Vec effectPos;
    Vec effectScale1;
    Vec effectScale2;

    effectPos.y = self->pos.y + 14.0;
    effectPos.x = self->pos.x;
    effectPos.z = self->pos.z;

    effectScale1 = (Vec){0.6f, 0.6f, 0.6f};
    effectScale2 = (Vec){0.4f, 0.4f, 0.4f};

    SpawnEffect("Wm_ob_cmnboxsmoke", 0, &effectPos, &(S16Vec){0,0,0}, &effectScale1);
    SpawnEffect("Wm_ob_cmnboxgrain", 0, &effectPos, &(S16Vec){0,0,0}, &effectScale2);
    SpawnEffect("Wm_ob_cmnboxpiece", 0, &effectPos, &(S16Vec){0,0,0}, &effectScale1);

    cvtSndObjctPos(&soundPos, &self->pos);
    SoundPlayingClass::instance2->PlaySoundAtPosition(SE_OBJ_TARU_BREAK, &soundPos, 1);

    self->Delete(0);
    return;
}