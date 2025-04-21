.text
.extern AXSetMasterVolume

.global muteAudioOnCrash
muteAudioOnCrash:
    #nofralloc

    // Original instruction
    mr r28, r3

    // Mute audio
    li r3, 0
    b AXSetMasterVolume