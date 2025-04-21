#include <game.hpp>
#include <sfx.hpp>
#include <player.hpp>

extern "C" void ChangeCoinCollectionSound(SoundPlayingClass *self, int soundID, Vec2 *pos, int remoteRelatedValue) {
    SoundPlayingClass *sound = SoundPlayingClass::instance2;
    u8 world = m_startGameInfo.world1 + 1;
    u8 level = m_startGameInfo.level1 + 1;

    // right now sound ID is SE_OBJ_GET_COIN (567)

    if ((world == 5) && (level == 5))
        soundID = SE_OBJ_GET_COIN_OLD;

    // play sound
    sound->PlaySoundAtPosition(soundID, pos, remoteRelatedValue);
}

extern "C" void ChangeJumpSound(daPlBase_c *self, int soundID, long shouldPlay) {
    u8 world = m_startGameInfo.world1 + 1;
    u8 level = m_startGameInfo.level1 + 1;

    if ((world == 4) && (level == 5))
        soundID = SE_OBJ_GET_ONPU;

    self->playPlayerSound(soundID, 0);
}