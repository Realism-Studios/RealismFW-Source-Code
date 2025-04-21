.text
.global newClimbSounds
newClimbSounds:
	li r7, 0x134
	b effectNewClimbSounds
.global newClimbSoundsJump
newClimbSoundsJump:
	li r7, 0x136
	b effectNewClimbSounds
.global newClimbSoundsJumpS
newClimbSoundsJumpS:
	li r7, 0x137
	b effectNewClimbSounds
.global newClimbSoundsJumpSS
newClimbSoundsJumpSS:
	li r7, 0x138
	b effectNewClimbSounds
.global newClimbSoundsLand
newClimbSoundsLand:
	li r7, 0x139
	b effectNewClimbSounds
.global newClimbSoundsAttack
newClimbSoundsAttack:
	li r7, 0x13A
	b effectNewClimbSounds


.extern StagePtr
effectNewClimbSounds:
	lis r6, StagePtr@h
	ori r6, r6, StagePtr@l
	lwz r6, 0(r6) # World number?
	lhz r6, 0x120C(r6)

	cmpwi r6, 14 # Stage Number?
	beq pngn
	cmpwi r6, 0x903
	beq pngn
	cmpwi r6, 0x40E
	beq climb
	mr r4, r7
	blr
pngn:
	li r4, 0xE1 # Sfx ID 225
	blr
climb:
	li r4, 0xC9 # Sfx ID 201
	blr
