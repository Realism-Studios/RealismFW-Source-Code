# @SFNP (shit for new powerups) = section you'd need to edit to add a new powerup, yes this is stupid and yes i unfortunately need this to not dementia and forget something - Kirblue

.text
.align 4

#include <asm_setup.S>

.extern returnFromGPSFASixth
.extern daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
.extern dAcPy_c__ChangePowerupWithAnimation
.extern playPlayerSound__FP10daPlBase_c3SFXl # don't change this
.extern CreateActor
.extern PlayerProjectileShooting
.extern Actor_SearchByName
.extern SomeTable_802F5440
.extern SomeTable_802F5580
.extern SomeTable_802F56C0
.extern BlahTable
.extern continuePlumberSetPowerupTexture
.extern doneSettingThePowerupTexture
.extern doneSettingThePowerupTexture2
.extern returnFromPowerupSoundChange

.global DisableHatForHammer
DisableHatForHammer:
	lwz r0, 0x17C(r31)
	clrlwi. r0, r0, 31
	beq DontDrawTheFuckingHat_
	lbz r0, 0x152(r31)
	cmpwi r0, 7
	beq DontDrawTheFuckingHat_
	b DrawTheFuckingHat
DontDrawTheFuckingHat_:
	b DontDrawTheFuckingHat # this is a hack to avoid implementing R_PPC_REL14
.extern DontDrawTheFuckingHat
.extern DrawTheFuckingHat

.global GetInfoFromDumbTable
GetInfoFromDumbTable:
	lis r3, SomeTable@h
	ori r3, r3, SomeTable@l

	slwi r4, r4, 2
	slwi r0, r5, 6

	lwzx r3, r3, r4
	add r3, r3, r0
	blr




.global FukidashiFix
FukidashiFix:
	lis r7, ZeroFloat@h
	ori r7, r7, ZeroFloat@l
	
	lis r8, FukidashiTable2@h
	ori r8, r8, FukidashiTable2@l
	
	mulli r6, r4, 0x20
	slwi r0, r5, 2
	add r0, r0, r6
	
	lfs f0, 0(r7)
	lfsx f1, r8, r0
	
	stfs f0, 0x1DC(r3)
	stfs f1, 0x1E0(r3)
	stfs f0, 0x1E4(r3)
	
	blr




.global PlayerGetHeight
PlayerGetHeight:
	lwz r0, 0x14D4(r3) # r0 = this->characterID
	
	lwz r4, 0x1090(r3) # r4 = this->powerup
	slwi r4, r4, 3
	
	cmpwi r0, 0
	beq useNormal
	cmpwi r0, 1
	bne useDucking
	
useNormal:
	lis r3, PlayerHeightsNormal@h
	ori r3, r3, PlayerHeightsNormal@l
	b gotOffset
	
useDucking:
	lis r3, PlayerHeightsDucking@h
	ori r3, r3, PlayerHeightsDucking@l
	
gotOffset:
	lfsx f1, r3, r4
	blr



.global WtfEver
WtfEver:
	lis r3, WtfEverTable@h
	ori r3, r3, WtfEverTable@l
	lfsx f0, r3, r0
	blr



.global YetAnother
YetAnother:
	lis r3, YetAnotherTable@h
	ori r3, r3, YetAnotherTable@l
	lfsx f0, r3, r0
	blr




.global GetWeirdScaleTable
GetWeirdScaleTable:
	lis r5, WeirdScaleTable@h
	ori r5, r5, WeirdScaleTable@l
	lfsx f1, r5, r0
	blr





.global GetAsdfTable
GetAsdfTable:
	extsb r0, r4
	cmpwi r0, -1
	bne dontGetThisOne
	
	lwz r0, 0x1090(r3)	#r0 = this->powerup
	extsb r4, r0
	
dontGetThisOne:
	extsb r0, r4
	
	lis r3, AsdfTable@h
	ori r3, r3, AsdfTable@l
	lbzx r3, r3, r0
	
	blr





.global GetBlahTableOffset
GetBlahTableOffset:
	lis r5, BlahTableOffsets@h
	ori r5, r5, BlahTableOffsets@l
	lwzx r30, r5, r4
	blr


.global DealWithBlahTable
DealWithBlahTable:
	lis r5, BlahTableOffsets@h
	ori r5, r5, BlahTableOffsets@l
	
	# points to BlahTable
	lis r4, BlahTable@h
	ori r4, r4, BlahTable@l
	
	# get blahtableoffset entry based on powerup
	lwz r0, 0x1090(r3)	#r0 = this->powerup
	slwi r0, r0, 2
	lwzx r0, r5, r0
	
	# now get the entry from the blahtable
	slwi r0, r0, 2
	lwzx r4, r4, r0
	
	# do shit here
	
	lfs f3, 0(r4)
	stfs f3, 0x14E4(r3)
	stfs f3, 0x164(r3)
	
	lfs f1, 0x2A70(r3)
	lfs f0, 0x2A74(r3)
	
	lfs f2, 4(r4)
	fadds f1, f2, f1
	fadds f0, f0, f1
	stfs f0, 0x14E8(r3)
	stfs f0, 0x168(r3)
	
	lfs f0, 0x8(r4)
	lfs f1, 0xC(r4)
	stfs f0, 0x16C(r3)
	stfs f1, 0x170(r3)
	
	# hope I did this right
	blr





.global GetPowerupScaleFloatAddr_r6_trash_r0_valshl2_r4_dest
GetPowerupScaleFloatAddr_r6_trash_r0_valshl2_r4_dest:
	lis r6, PowerupScaleFloats@h
	ori r6, r6, PowerupScaleFloats@l
	add r4, r6, r0
	blr

.global GetPowerupScaleFloatAddr_r6_trash_r0_valshl2_r3_dest
GetPowerupScaleFloatAddr_r6_trash_r0_valshl2_r3_dest:
	lis r6, PowerupScaleFloats@h
	ori r6, r6, PowerupScaleFloats@l
	add r3, r6, r0
	blr

.global GetPowerupScaleFloatAddr_r8_trash_r0_valshl2_r7_dest
GetPowerupScaleFloatAddr_r8_trash_r0_valshl2_r7_dest:
	lis r8, PowerupScaleFloats@h
	ori r8, r8, PowerupScaleFloats@l
	add r7, r8, r0
	blr

.global GetPowerupScaleFloatAddr_fixForSixth
GetPowerupScaleFloatAddr_fixForSixth:
	lis r5, PowerupScaleFloats@h
	ori r5, r5, PowerupScaleFloats@l
	add r4, r5, r4
	b returnFromGPSFASixth



.global PlumberSetPowerupFix #@SFNP
PlumberSetPowerupFix:
	lbz r0, 0x152(r31) # get powerup id
	
	cmpwi r0, 2
	beq doFireTex
	
	cmpwi r0, 6
	beq doIceTex
	
	cmpwi r0, 7
	beq doHammerTex

	cmpwi r0, 8
	beq doBubbleTex

	cmpwi r0, 9
	beq doLemmyTex

	cmpwi r0, 0xA
	beq doMandyTex
	
	cmpwi r0, 0xB
	beq doBombTex
	
	cmpwi r0, 0xC
	beq doMegaTex
	
	cmpwi r0, 0xD
	beq doBoomerangTex
	
	cmpwi r0, 0xE
	beq doEvilTex

	li r4, 0
	b gotTexID
	
doFireTex:
	li r4, 1	#These are Pat0 frame ID
	b gotTexID
	
doIceTex:
	li r4, 2
	b gotTexID
	
doHammerTex:
	li r4, 3
	b gotTexID

doBubbleTex:
	li r4, 4
	b gotTexID

doLemmyTex:
	li r4, 5
	b gotTexID

doMandyTex:
	li r4, 6
	b gotTexID
	
doBombTex:
	li r4, 7
	b gotTexID

doMegaTex:
	li r4, 0 # regular player textures, sorry for breaking the counting-by-one chain :pensive:
	b gotTexID

doBoomerangTex:
	li r4, 8 # wow new textures again!
	b gotTexID

doEvilTex:
	li r4, 9
	b gotTexID
	
gotTexID:
	lwz r12, 0(r31)
	mr r3, r31
	lwz r12, 0x34(r12)
	mtctr r12
	bctrl
	
	b doneSettingThePowerupTexture



.global PlumberSetPowerupFix2 #same as the first one, like, literally. @SFNP
PlumberSetPowerupFix2:
	cmpwi r31, 2
	beq doFireTex2
	
	cmpwi r31, 6
	beq doIceTex2
	
	cmpwi r31, 7
	beq doHammerTex2

	cmpwi r31, 8
	beq doBubbleTex2

	cmpwi r31, 9
	beq doLemmyTex2

	cmpwi r31, 0xA
	beq doMandyTex2
	
	cmpwi r31, 0xB
	beq doBombTex2
	
	cmpwi r31, 0xC
	beq doMegaTex2
	
	cmpwi r31, 0xD
	beq doBoomerangTex2
	
	cmpwi r31, 0xE
	beq doEvilTex2

	li r4, 0
	b gotTexID2
	
doFireTex2:
	li r4, 1	#These are Pat0 frame ID
	b gotTexID2
	
doIceTex2:
	li r4, 2
	b gotTexID2
	
doHammerTex2:
	li r4, 3
	b gotTexID2

doBubbleTex2:
	li r4, 4
	b gotTexID2

doLemmyTex2:
	li r4, 5
	b gotTexID2

doMandyTex2:
	li r4, 6
	b gotTexID2
	
doBombTex2:
	li r4, 7
	b gotTexID2

doMegaTex2:
	li r4, 0
	b gotTexID2

doBoomerangTex2:
	li r4, 8
	b gotTexID2
	
doEvilTex2:
	li r4, 9
	b gotTexID2
	
gotTexID2:
	lwz r3, 0x2A64(r30)
	lwz r12, 0(r3)
	lwz r12, 0x34(r12)
	mtctr r12
	bctrl
	
	b doneSettingThePowerupTexture2




.global PlumberSetPowerupTextureFix
PlumberSetPowerupTextureFix:
	stb r4, 0x153(r3)
	
	lis r5, PowerupTextures@h
	ori r5, r5, PowerupTextures@l
	
	slwi r4, r4, 2
	lfsx f31, r5, r4
	
	b continuePlumberSetPowerupTexture



.global SetHammerToEnItemDCA #@SFNP
SetHammerToEnItemDCA:
	bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetHammer
	
	li r0, 5
	sth r0, 0xDCA(r31) #So basically DCA is the value in EN_ITEM that stores the internal powerup type
	
DontSetHammer:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr

.global SetBubbleToEnItemDCA
SetBubbleToEnItemDCA:
	bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetBubble

	li r0, 7
	sth r0, 0xDCA(r31)

DontSetBubble:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr

.global SetLemmyToEnItemDCA
SetLemmyToEnItemDCA:
	bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetLemmy

	li r0, 8
	sth r0, 0xDCA(r31)

DontSetLemmy:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr

.global SetMandyToEnItemDCA
SetMandyToEnItemDCA:
	bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetMandy

	li r0, 9
	sth r0, 0xDCA(r31)

DontSetMandy:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr
	
.global SetBombToEnItemDCA
SetBombToEnItemDCA:
	bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetBomb

	li r0, 0xA
	sth r0, 0xDCA(r31)

DontSetBomb:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr
	
.global SetMegaToEnItemDCA
SetMegaToEnItemDCA:
	bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetMega

	li r0, 0xB
	sth r0, 0xDCA(r31)

DontSetMega:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr
	
.global SetBoomerangToEnItemDCA
SetBoomerangToEnItemDCA:
	bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetBoomerang

	li r0, 0xC
	sth r0, 0xDCA(r31)

DontSetBoomerang:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr
	
.global SetEvilToEnItemDCA
SetEvilToEnItemDCA:
	bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetEvil

	li r0, 0xD
	sth r0, 0xDCA(r31)

DontSetEvil:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr

.global SetHammerToEnItemDCASpecial #@SFNP
SetHammerToEnItemDCASpecial: #sooo this is a big mess so lemme explain it
	#just so you know: 0xD is the mini shroom's DCA internal value, 5 is the hammer's
	#also, 0x19 is the mini shroom's DCB internal value, 6 is the hammer's
	cmplwi r4, 0x19			#if the current DCB isn't a mini shroom
	bne _not19				#then go to _not19
	li r0, 0xD				#else, store 0xD in r0
	sth r0, 0xDCA(r3)		#then store r0 in the DCA
_not19:
	cmplwi r4, 6
	bne notHammerEither
	li r0, 5
	sth r0, 0xDCA(r3)
	blr
notHammerEither:
	cmplwi r4, 0x10
	bne notBubbleEither
	li r0, 7
	sth r0, 0xDCA(r3)
	blr
notBubbleEither:
	cmplwi r4, 0x12
	bne notLemmyEither
	li r0, 8
	sth r0, 0xDCA(r3)
	blr
notLemmyEither:
	cmplwi r4, 0x14
	bne notMandyEither
	li r0, 9
	sth r0, 0xDCA(r3)
	blr
notMandyEither:
	cmplwi r4, 0x16
	bne notBombEither
	li r0, 0xA
	sth r0, 0xDCA(r3)
	blr
notBombEither:
	cmplwi r4, 0x18
	bne notMegaEither
	li r0, 0xB
	sth r0, 0xDCA(r3)
	blr
notMegaEither:
	cmplwi r4, 0x1A
	bne notBoomerangEither
	li r0, 0xC
	sth r0, 0xDCA(r3)
	blr
notBoomerangEither:
	cmplwi r4, 0x1C
	bnelr
	li r0, 0xD
	sth r0, 0xDCA(r3)
	blr

#.global EnItem_BindAnimation_Fix
#EnItem_BindAnimation_Fix:
#	lhz r0, 0xDCA(r3)
#	cmpwi r0, 5
#	beqlr
#	stwu sp, -0x20(sp)
#	b EnItem_BindAnimation_Continued


.global FixPowerupSound
FixPowerupSound:
	#lis r3, DBString@h
	#ori r3, r3, DBString@l
	#lwz r4, 0x1090(r29)
	#crclr 4*cr1+eq
	#bl OSReport

	lwz r5, 0x14DC(r29) # Original powerup
	lwz r0, 0x1090(r29) # New powerup
	# If NEW is 1 (big) and OLD is not 0 or 3, then play 273 (powerdown)
	cmpwi r0, 1
	bne dontConsiderPowerdown

	cmpwi r5, 0
	beq dontConsiderPowerdown
	cmpwi r5, 3
	beq dontConsiderPowerdown
	# If the player is here, he was downgraded from a higher powerup to Big Mario
	# So play the powerdown sound
	mr r3, r29
	li r4, 273
	li r5, 0
	b returnFromPowerupSoundChange

dontConsiderPowerdown:
	lis r3, PowerupSounds@h
	ori r3, r3, PowerupSounds@l
	slwi r4, r0, 2
	lwzx r4, r3, r4
	mr r3, r29
	li r5, 0

	b returnFromPowerupSoundChange


.global WeirdAnimLoadHack #@SFNP
WeirdAnimLoadHack:
	cmplwi r4, 5   # Hammer
	beqlr
	cmplwi r4, 6   # 1up
	beqlr
	cmplwi r4, 7   # Bubble
	beqlr
	cmplwi r4, 8   # Lemmy
	beqlr
	cmplwi r4, 9   # Mandy
	beqlr
	cmplwi r4, 0xA # Bomb
	beqlr
	cmplwi r4, 0xB # Mega
	beqlr
	cmplwi r4, 0xC # Boomerang
	beqlr
	cmplwi r4, 0xD # Evil
	beqlr
	# none of those succeeded
	crclr 4*cr0+eq
	blr

.global TryToGiveMarioPowerup
TryToGiveMarioPowerup:
	stwu sp, -0x10(sp)
	mflr r0
	stw r0, 0x14(sp)
	
	# check the obtained item number
	b checkDCA

checkDCA: #@SFNP
	cmplwi r3, 5
	beq doCheckForHammer
	cmplwi r3, 7
	beq doCheckForBubble
	cmplwi r3, 8
	beq doCheckForLemmy
	cmplwi r3, 9
	beq doCheckForMandy
	cmplwi r3, 0xA
	beq doCheckForBomb
	cmplwi r3, 0xB
	beq doCheckForMega
	cmplwi r3, 0xC
	beq doCheckForBoomerang
	cmplwi r3, 0xD
	beq doCheckForEvil
	cmplwi r3, 0xE
	b IsntGettingAnySuit

doCheckForHammer: #@SFNP
	cmplwi r4, 7
	bne DontHaveHammerSuit
	
	li r28, 0
	li r29, 2
	b ReturnFromTTGMHS
	
doCheckForBubble:
	cmplwi r4, 8
	bne DontHaveBubble

	li r28, 0
	li r29, 2
	b ReturnFromTTGMHS

doCheckForLemmy:
	cmplwi r4, 9
	bne DontHaveLemmy

	li r28, 0
	li r29, 2
	b ReturnFromTTGMHS

doCheckForMandy:
	cmplwi r4, 0xA
	bne DontHaveMandy

	li r28, 0
	li r29, 2
	b ReturnFromTTGMHS
	
doCheckForBomb:
	cmplwi r4, 0xB
	bne DontHaveBomb

	li r28, 0
	li r29, 2
	b ReturnFromTTGMHS
	
doCheckForMega:
	cmplwi r4, 0xC
	bne DontHaveMega

	li r28, 0
	li r29, 2
	b ReturnFromTTGMHS
	
doCheckForBoomerang:
	cmplwi r4, 0xD
	bne DontHaveBoomerang

	li r28, 0
	li r29, 2
	b ReturnFromTTGMHS

doCheckForEvil:
	cmplwi r4, 0xE
	bne DontHaveEvil

	li r28, 0
	li r29, 2
	b ReturnFromTTGMHS

DontHaveHammerSuit: #@SFNP
	mr r3, r30
	li r4, 7
	bl dAcPy_c__ChangePowerupWithAnimation
	
	cmpwi r3, 0
	beq ReturnFromTTGMHS
	
	li r29, 1
	b ReturnFromTTGMHS

DontHaveBubble:
	mr r3, r30
	li r4, 8
	bl dAcPy_c__ChangePowerupWithAnimation
	
	cmpwi r3, 0
	beq ReturnFromTTGMHS

	li r29, 1
	b ReturnFromTTGMHS

DontHaveLemmy:
	mr r3, r30
	li r4, 9
	bl dAcPy_c__ChangePowerupWithAnimation
	
	cmpwi r3, 0
	beq ReturnFromTTGMHS

	li r29, 1
	b ReturnFromTTGMHS

DontHaveMandy:
	mr r3, r30
	li r4, 0xA
	bl dAcPy_c__ChangePowerupWithAnimation
	
	cmpwi r3, 0
	beq ReturnFromTTGMHS

	li r29, 1
	b ReturnFromTTGMHS
	
DontHaveBomb:
	mr r3, r30
	li r4, 0xB
	bl dAcPy_c__ChangePowerupWithAnimation
	
	cmpwi r3, 0
	beq ReturnFromTTGMHS

	li r29, 1
	b ReturnFromTTGMHS

DontHaveMega:
	mr r3, r30
	li r4, 0xC
	bl dAcPy_c__ChangePowerupWithAnimation
	
	cmpwi r3, 0
	beq ReturnFromTTGMHS

	li r29, 1
	b ReturnFromTTGMHS

DontHaveBoomerang:
	mr r3, r30
	li r4, 0xD
	bl dAcPy_c__ChangePowerupWithAnimation
	
	cmpwi r3, 0
	beq ReturnFromTTGMHS

	li r29, 1
	b ReturnFromTTGMHS

DontHaveEvil:
	mr r3, r30
	li r4, 0xE
	bl dAcPy_c__ChangePowerupWithAnimation
	
	cmpwi r3, 0
	beq ReturnFromTTGMHS

	li r29, 1
	b ReturnFromTTGMHS

IsntGettingAnySuit:
ReturnFromTTGMHS:
	# end of function
	lwz r0, 0x14(sp)
	mtlr r0
	addi sp, sp, 0x10
	
	# leftover instruction from what we replaced to add the bl
	cmpwi r29, 0
	blr

#include <asm_profiles.S>
.global ThrowNewProjectile #@SFNP
ThrowNewProjectile:
	lwz r3, 0x1090(r30)

	cmplwi r3, 7
	beq ThrowHammer

	cmplwi r3, 8
	beq ThrowBubble

	cmplwi r3, 9
	beq ThrowLemmy

	cmplwi r3, 0xA
	beq ThrowMandy
	
	cmplwi r3, 0xB
	beq ThrowBomb
	
	#skip 0xC, mega does not throw anything
	#i wonder if this is causing errors? check later maybe
	
	cmplwi r3, 0xD
	beq ThrowBoomerang
	
	cmplwi r3, 0xE
	beq ThrowEvil
	
	b ReturnFromThrowProjectile

ThrowHammer: #@SFNP
	mr r3, r30
	li r4, 498
	li r5, 0
	bl playPlayerSound__FP10daPlBase_c3SFXl
	
	li r3, 0x73				# Hammer
	li r4, 0				# Settings
	addi r5, sp, 0x2C		# Position
	li r6, 0				# Rotation
	lbz r7, 0x38F(r30)		# Layer
	bl CreateActor
	
	# Store player ID
	lwz r4, 0(r30)
	stw r4, 0x450(r3)
	
	# store player number, but add 0x100 to differentiate it from 0
	lbz r4, 0x38D(r30)
	addi r4, r4, 0x100
	stw r4, 0x458(r3)
	
	# Fix scale
	lis r4, OneFloat@h
	ori r4, r4, OneFloat@l
	lfs f0, 0(r4)
	stfs f0, 0xDC(r3)
	stfs f0, 0xE0(r3)
	stfs f0, 0xE4(r3)
	
	mr r4, r30
	lwz r12, 0x60(r3)
	lwz r12, 0x8C(r12)
	mtctr r12
	bctrl
	b ReturnFromThrowProjectile

ThrowBubble:
	mr r3, r30
	li r4, 498
	li r5, 0
	bl playPlayerSound__FP10daPlBase_c3SFXl
	

	li r3, BubbleSuit

	lwz r4, 0x38D(r30)		# Settings
	addi r4, r4, 50

	addi r5, sp, 0x2C		# Position
	li r6, 0				# Rotation
	lbz r7, 0x38F(r30)		# Layer
	bl CreateActor

	# Store player ID
	lwz r4, 0(r30)
	stw r4, 0x540(r3)
	
	# store player number, but add 0x100 to differentiate it from 0
	lbz r4, 0x38D(r30)
	addi r4, r4, 0x100
	stw r4, 0x544(r3)

	b ReturnFromThrowProjectile

ThrowLemmy:
	mr r3, r30
	li r4, 2003
	li r5, 0
	bl playPlayerSound__FP10daPlBase_c3SFXl
	
	frsp f1, f31
	lfs f2, 0x20(r1) #0xC0+another_x
	frsp f0, f30
	stfs f2, 0x2C(r1) #0xC0+VEC3_x
	slwi r4, r31, 16
	addi r5, r1, 0x2C #0xC0+VEC3_x			# pos
	stfs f1, 0x30(r1) #0xC0+VEC3_y

	li r3, 562								# classID					#old is 0x69
	li r6, 0								# rot
	li r7, 0								# layer

	stfs f0, 0x34(r1) #0xC0+VEC3_z
	lbz r0, 0x1035(r30)
	lbz r10, 0x38D(r30) #dAcPy_c._._.associatedPlayerID
	lbz r9, 0x27CC(r30)
	slwi r0, r0, 12
	lbz r8, 0x38F(r30)
	or r0, r4, r0
	extsb r10, r10
	slwi r9, r9, 4
	slwi r4, r8, 8
	or r8, r10, r9
	or r0, r4, r0

	or r4, r8, r0							# settings
	bl CreateActor							# 105 PL_FIREBALL

	b ReturnFromThrowProjectile

ThrowMandy:
	mr r3, r30
	li r4, 1 # SE_DEMO_OP_JUMP_JR_0954f
	li r5, 0
	bl playPlayerSound__FP10daPlBase_c3SFXl
	

	li r3, MandyIGuess
	lwz r4, 0x38D(r30)		# Settings
	addi r4, r4, 50

	addi r5, sp, 0x2C		# Position
	li r6, 0				# Rotation
	lbz r7, 0x38F(r30)		# Layer
	bl CreateActor

	# Store player ID
	lwz r4, 0(r30)
	stw r4, 0x630(r3)
	
	# store player number, but add 0x100 to differentiate it from 0
	lbz r4, 0x38D(r30)
	addi r4, r4, 0x100
	stw r4, 0x630(r3)

	b ReturnFromThrowProjectile
	
ThrowBomb:
	mr r3, r30
	li r4, 498
	li r5, 0
	bl playPlayerSound__FP10daPlBase_c3SFXl
	
	li r3, BrosBomb			# Bomb
	li r4, 0				# Settings
	addi r5, sp, 0x2C		# Position
	li r6, 0				# Rotation
	lbz r7, 0x38F(r30)		# Layer
	bl CreateActor
	
	# Store player ID
	lwz r4, 0(r30)
	stw r4, 0x720(r3) # +0xF0
	
	# store player number, but add 0x100 to differentiate it from 0
	lbz r4, 0x38D(r30)
	addi r4, r4, 0x100
	stw r4, 0x71C(r3) # +0xEC
	
	# Fix scale
	lis r4, OneFloat@h
	ori r4, r4, OneFloat@l
	lfs f0, 0(r4)
	stfs f0, 0xDC(r3)
	stfs f0, 0xE0(r3)
	stfs f0, 0xE4(r3)
	
	mr r4, r30
	lwz r12, 0x60(r3)
	lwz r12, 0x8C(r12)
	mtctr r12
	bctrl
	b ReturnFromThrowProjectile
	
ThrowBoomerang:
	mr r3, r30
	li r4, 498
	li r5, 0
	bl playPlayerSound__FP10daPlBase_c3SFXl
	

	li r3, 0x176				# Custom Boomerang

	lwz r4, 0x38D(r30)		# Settings => dStageActor_c->associatedPlayerID (in nybble 6 for some reasons) + 2 (in nybble 12 because yes)
	addi r4, r4, 2

	addi r5, sp, 0x2C		# Position
	li r6, 0				# Rotation
	lbz r7, 0x38F(r30)		# Layer
	bl CreateActor

	
	# Store player ID
	lwz r4, 0(r30)
	stw r4, 0x810(r3) # +0xF0
	
	# store player number, but add 0x100 to differentiate it from 0
	lbz r4, 0x38D(r30)
	addi r4, r4, 0x100
	stw r4, 0x808(r3) # +0xEC
	
	# Fix scale
	lis r4, OneFloat@h
	ori r4, r4, OneFloat@l
	lfs f0, 0(r4)
	stfs f0, 0xDC(r3)
	stfs f0, 0xE0(r3)
	stfs f0, 0xE4(r3)
	
	mr r4, r30
	lwz r12, 0x60(r3)
	lwz r12, 0x8C(r12)
	mtctr r12
	bctrl

ReturnFromThrowProjectile:
	lwz r0, 0xC4(sp)
	psq_l f31, 0xB8(sp), 1, 0
	lfd f31, 0xB0(sp)
	psq_l f30, 0xA8(sp), 1, 0
	lfd f30, 0xA0(sp)
	lwz r31, 0x9C(sp)
	lwz r30, 0x98(sp)
	mtlr r0
	addi sp, sp, 0xC0
	blr

ThrowEvil:
	mr r3, r30
	li r4, 498
	li r5, 0
	bl playPlayerSound__FP10daPlBase_c3SFXl
	

	li r3, BubbleSuit

	lwz r4, 0x38D(r30)		# Settings
	addi r4, r4, 50

	addi r5, sp, 0x2C		# Position
	li r6, 0				# Rotation
	lbz r7, 0x38F(r30)		# Layer
	bl CreateActor

	# Store player ID
	lwz r4, 0(r30)
	stw r4, 0x900(r3) # +0xF0
	
	# store player number, but add 0x100 to differentiate it from 0
	lbz r4, 0x38D(r30)
	addi r4, r4, 0x100
	stw r4, 0x8F4(r3) # +0xEC

	b ReturnFromThrowProjectile

.global MoreProjectileStuff_Fix
MoreProjectileStuff_Fix:
	lwz r3, 0x1090(r31)
	cmplwi r3, 7
	blt ReturnFromMPSFix
	
	mr r3, r31
	li r4, 1
	bl PlayerProjectileShooting
	
ReturnFromMPSFix:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr



.global ProjectileShootCheck_Fix #@SFNP
ProjectileShootCheck_Fix:
	lwz r3, 0x1090(r31)
	cmplwi r3, 7
	blt Return0fromPSCFix
	
	cmplwi r3, 7
	beq DoHammerFromProjectileShootCheck_Fix

	cmplwi r3, 8
	beq DoBubbleFromProjectileShootCheck_Fix

	cmplwi r3, 9
	beq DoLemmyFromProjectileShootCheck_Fix

	cmplwi r3, 0xA
	beq DoMandyFromProjectileShootCheck_Fix
	
	cmplwi r3, 0xB
	beq DoBombFromProjectileShootCheck_Fix
	
	#skip 0xC, mega does not throw anything
	
	cmplwi r3, 0xD
	beq DoBoomerangFromProjectileShootCheck_Fix
	
	cmplwi r3, 0xE
	beq DoEvilFromProjectileShootCheck_Fix

DoHammerFromProjectileShootCheck_Fix: #@SFNP
	lbz r3, 0x38D(r31)
	li r4, 0
	extsb r3, r3
	bl CheckHammerLimit
	b FinishFromProjectileShootCheck_Fix

DoBubbleFromProjectileShootCheck_Fix:
	lbz r3, 0x38D(r31)
	li r4, 0
	extsb r3, r3
	bl CheckBubbleLimit
	b FinishFromProjectileShootCheck_Fix

DoLemmyFromProjectileShootCheck_Fix:
	lbz r3, 0x38D(r31)
	li r4, 0
	extsb r3, r3
	bl CheckLemmyLimit
	b FinishFromProjectileShootCheck_Fix

DoMandyFromProjectileShootCheck_Fix:
	lbz r3, 0x38D(r31)
	li r4, 0
	extsb r3, r3
	bl CheckMandyLimit
	b FinishFromProjectileShootCheck_Fix
	
DoBombFromProjectileShootCheck_Fix:
	lbz r3, 0x38D(r31)
	li r4, 0
	extsb r3, r3
	bl CheckBombLimit
	b FinishFromProjectileShootCheck_Fix

DoBoomerangFromProjectileShootCheck_Fix:
	lbz r3, 0x38D(r31)
	li r4, 0
	extsb r3, r3
	bl CheckBoomerangLimit
	b FinishFromProjectileShootCheck_Fix

DoEvilFromProjectileShootCheck_Fix:
	lbz r3, 0x38D(r31)
	li r4, 0
	extsb r3, r3
	bl CheckBoomerangLimit
	b FinishFromProjectileShootCheck_Fix

Return0fromPSCFix:
	li r3, 0
	b ReturnFromPSCFix
	
Return1fromPSCFix:
	li r3, 1
	
ReturnFromPSCFix:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr

FinishFromProjectileShootCheck_Fix:
	cmpwi r3, 0
	beq Return0fromPSCFix
	b Return1fromPSCFix


.global CheckHammerLimit #@SFNP
CheckHammerLimit:
	stwu sp, -0x10(sp)
	mflr r0
	stw r0, 0x14(sp)
	stw r31, 0xC(sp)
	stw r30, 0x8(sp)
	
	# r30 = player number
	mr r30, r3
	addi r30, r30, 0x100
	
	# r31 = hammer count so far
	li r31, 0
	
	# search for every Hammer
	
	li r3, 0
	b startOrContinueLoop
loop:
	
	lwz r4, 0x458(r3)
	cmpw r4, r30
	bne startOrContinueLoop
	
	addi r31, r31, 1
	
startOrContinueLoop:
	mr r4, r3
	li r3, 0x73
	bl Actor_SearchByName
	
	cmpwi r3, 0
	bne loop
	
	# now if the current count is >= 2, return 0
	# otherwise return 1
	cmpwi r31, 2
	bge tooManyHammers
	
	li r3, 1
	b gotHammerStatus
	
tooManyHammers:
	li r3, 0
	
gotHammerStatus:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	lwz r30, 0x8(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr




.global CheckBubbleLimit
CheckBubbleLimit:
	stwu sp, -0x10(sp)
	mflr r0
	stw r0, 0x14(sp)
	stw r31, 0xC(sp)
	stw r30, 0x8(sp)
	
	# r30 = player number
	mr r30, r3
	addi r30, r30, 0x100
	
	# r31 = bubble count so far
	li r31, 0
	
	# search for every bubble
	
	li r3, 0
	b startOrContinueLoopBubble
loopBubble:
	
	lwz r4, 0x544(r3)
	cmpw r4, r30
	bne startOrContinueLoopBubble
	
	addi r31, r31, 1
	
startOrContinueLoopBubble:
	mr r4, r3
	li r3, BubbleSuit
	bl Actor_SearchByName
	
	cmpwi r3, 0
	bne loopBubble
	
	# now if the current count is >= 2, return 0
	# otherwise return 1
	cmpwi r31, 32767 #minecraft referenced
	bge tooManyBubble
	
	li r3, 1
	b gotBubbleStatus
	
tooManyBubble:
	li r3, 0
	
gotBubbleStatus:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	lwz r30, 0x8(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr

.global CheckLemmyLimit
CheckLemmyLimit:
	stwu sp, -0x10(sp)
	mflr r0
	stw r0, 0x14(sp)
	stw r31, 0xC(sp)
	stw r30, 0x8(sp)
	
	# r30 = player number
	mr r30, r3
	addi r30, r30, 0x100
	
	# r31 = lemmy ball count so far
	li r31, 0
	
	# search for every lemmy ball
	
	li r3, 0
	b startOrContinueLoopLemmy
loopLemmy:
	
	lwz r4, 0x544(r3)
	cmpw r4, r30
	bne startOrContinueLoopLemmy
	
	addi r31, r31, 1
	
startOrContinueLoopLemmy:
	mr r4, r3
	li r3, 562
	bl Actor_SearchByName
	
	cmpwi r3, 0
	bne loopLemmy
	
	# now if the current count is >= 2, return 0
	# otherwise return 1
	cmpwi r31, 5 #minecraft referenced
	bge tooManyLemmy
	
	li r3, 1
	b gotLemmyStatus
	
tooManyLemmy:
	li r3, 0
	
gotLemmyStatus:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	lwz r30, 0x8(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr




.global CheckMandyLimit
CheckMandyLimit:
	stwu sp, -0x10(sp)
	mflr r0
	stw r0, 0x14(sp)
	stw r31, 0xC(sp)
	stw r30, 0x8(sp)
	
	# r30 = player number
	mr r30, r3
	addi r30, r30, 0x100
	
	# r31 = count so far
	li r31, 0
	
	# search for me
	
	li r3, 0
	b startOrContinueLoopMandy
loopMandy:
	
	lwz r4, 0x630(r3)
	cmpw r4, r30
	bne startOrContinueLoopMandy
	
	addi r31, r31, 1
	
startOrContinueLoopMandy:
	mr r4, r3
	li r3, MandyIGuess
	bl Actor_SearchByName
	
	cmpwi r3, 0
	bne loopMandy
	
	# now if the current count is >= 5, return 0
	# otherwise return 1
	cmpwi r31, 5
	bge tooManyMandy
	
	li r3, 1
	b gotMandyStatus
	
tooManyMandy:
	li r3, 0
	
gotMandyStatus:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	lwz r30, 0x8(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr



.global CheckBombLimit
CheckBombLimit:
	stwu sp, -0x10(sp)
	mflr r0
	stw r0, 0x14(sp)
	stw r31, 0xC(sp)
	stw r30, 0x8(sp)
	
	# r30 = player number
	mr r30, r3
	addi r30, r30, 0x100
	
	# r31 = Bomb count so far
	li r31, 0
	
	# search for every Bomb
	
	li r3, 0
	b startOrContinueLoopBomb
loopBomb:
	
	lwz r4, 0x71C(r3)
	cmpw r4, r30
	bne startOrContinueLoopBomb
	
	addi r31, r31, 1
	
startOrContinueLoopBomb:
	mr r4, r3
	li r3, BrosBomb
	bl Actor_SearchByName
	
	cmpwi r3, 0
	bne loop
	
	# now if the current count is >= 2, return 0
	# otherwise return 1
	cmpwi r31, 2
	bge tooManyBombs
	
	li r3, 1
	b gotBombStatus
	
tooManyBombs:
	li r3, 0
	
gotBombStatus:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	lwz r30, 0x8(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr

.data
.align 4
.global doWait
doWait: .byte 0x0, 0x0, 0x0, 0x0

.text
.align 4
.global CheckBoomerangLimit
CheckBoomerangLimit:
	stwu sp, -0x10(sp)
	mflr r0
	stw r0, 0x14(sp)
	stw r31, 0xC(sp)
	stw r30, 0x8(sp)
	
	# r3 = player number

	lis r4, doWait@h
	ori r4, r4, doWait@l
	add r4, r4, r3
	lbz r4, 0(r4) #store doWait[player number] in r4
	
	# now if the frames to wait are > 0, return 0
	# otherwise return 1
	cmpwi r4, 0
	bgt tooManyBoomerangs
	
	li r3, 1
	b gotBoomerangStatus
	
tooManyBoomerangs:
	li r3, 0
	
gotBoomerangStatus:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	lwz r30, 0x8(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr
	
.global CheckEvilLimit
CheckEvilLimit:
	stwu sp, -0x10(sp)
	mflr r0
	stw r0, 0x14(sp)
	stw r31, 0xC(sp)
	stw r30, 0x8(sp)
	
	# r30 = player number
	mr r30, r3
	addi r30, r30, 0x100
	
	# r31 = bubble count so far
	li r31, 0
	
	# search for every bubble
	
	li r3, 0
	b startOrContinueLoopEvil
loopEvil:
	
	lwz r4, 0x8F4(r3)
	cmpw r4, r30
	bne startOrContinueLoopEvil
	
	addi r31, r31, 1
	
startOrContinueLoopEvil:
	mr r4, r3
	li r3, BubbleSuit
	bl Actor_SearchByName
	
	cmpwi r3, 0
	bne loopEvil
	
	# now if the current count is >= 1, return 0
	# otherwise return 1
	cmpwi r31, 1 # yeah no way there should be more than one of this :sob:
	bge tooManyEvil
	
	li r3, 1
	b gotEvilStatus
	
tooManyEvil:
	li r3, 0
	
gotEvilStatus:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	lwz r30, 0x8(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr


.extern continueToadPowerupSet
.global ToadPowerupFix
ToadPowerupFix: #@SFNP
	cmpwi r4, 3 # is this Pat0 frame ID?
	beq isHammerForToad
	cmpwi r4, 4
	beq isBubbleForToad
	cmpwi r4, 5
	beq isLemmyForToad
	cmpwi r4, 6
	beq isMandyForToad
	cmpwi r4, 7
	beq isBombForToad
	cmpwi r4, 8
	beq isMegaForToad
	cmpwi r4, 9
	beq isBoomerangForToad
	cmpwi r4, 0xA
	beq isEvilForToad

	lis r8, PowerupTextures@h
	ori r8, r8, PowerupTextures@l
	slwi r9, r4, 2
	lfsx f1, r8, r9
	fadds f31, f31, f1
	b continueToadPowerupSet

isHammerForToad: #@SFNP
	lis r8, ToadTextures@h
	ori r8, r8, ToadTextures@l
	slwi r9, r0, 2
	lfsx f31, r8, r9
	b continueToadPowerupSet

isBubbleForToad:
	lis r8, ToadBubbleTextures@h
	ori r8, r8, ToadBubbleTextures@l
	slwi r9, r0, 2
	lfsx f31, r8, r9
	b continueToadPowerupSet

isLemmyForToad:
	lis r8, ToadLemmyTextures@h
	ori r8, r8, ToadLemmyTextures@l
	slwi r9, r0, 2
	lfsx f31, r8, r9
	b continueToadPowerupSet

isMandyForToad:
	lis r8, ToadMandyTextures@h
	ori r8, r8, ToadMandyTextures@l
	slwi r9, r0, 2
	lfsx f31, r8, r9
	b continueToadPowerupSet
	
isBombForToad:
	lis r8, ToadBombTextures@h
	ori r8, r8, ToadBombTextures@l
	slwi r9, r0, 2
	lfsx f31, r8, r9
	b continueToadPowerupSet

isMegaForToad:
	lis r8, ToadMegaTextures@h
	ori r8, r8, ToadMegaTextures@l
	slwi r9, r0, 2
	lfsx f31, r8, r9
	b continueToadPowerupSet

isBoomerangForToad:
	lis r8, ToadBoomerangTextures@h
	ori r8, r8, ToadBoomerangTextures@l
	slwi r9, r0, 2
	lfsx f31, r8, r9
	b continueToadPowerupSet
	
isEvilForToad:
	lis r8, ToadEvilTextures@h
	ori r8, r8, ToadEvilTextures@l
	slwi r9, r0, 2
	lfsx f31, r8, r9
	b continueToadPowerupSet

.global CheckExistingPowerup # Why is this even here? It's not used anywhere...
CheckExistingPowerup:

	lwz r3, 0x14E0(r3)
	blr

.global AcPyCtorHS
AcPyCtorHS:
	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	mr r31, r3

	bl build__21dHammerSuitRenderer_cFv
	stw r3, 0x2D08(r31)

	mr r3, r31
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.global AcPyDtorHS
AcPyDtorHS:
	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	stw r30, 8(r1)

	mr r31, r4
	mr r30, r3

	cmpwi r3, 0
	beq oops2
	lwz r3, 0x2D08(r30)
	li r4, 1
	bl __dt__21dHammerSuitRenderer_cFv
oops2:
	
	mr r3, r30
	mr r4, r31
	bl RealAcPyDtor

	lwz r30, 8(r1)
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.global AcPySetMeUpHS
AcPySetMeUpHS:
	# partway in: r31 = this
	lwz r3, 0x2D08(r31)
	addi r4, r31, 0x2A60
	bl setup__21dHammerSuitRenderer_cFP21dPlayerModelHandler_c

	# epilogue
	li r3, 1
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.global AcPyDrawHS
AcPyDrawHS:
	# partway in: r31 = this
	lwz r3, 0x2D08(r31)
	bl draw__21dHammerSuitRenderer_cFv

	# epilogue
	li r3, 1
	lwz r0, 0x14(r1)
	lwz r31, 0xC(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr



# DO IT AGAIN
.global Wm2dPlayerCtorHS
Wm2dPlayerCtorHS:
	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	mr r31, r3

	bl build__21dHammerSuitRenderer_cFv
	stw r3, 0x384(r31)

	mr r3, r31
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.global Wm2dPlayerDtorHS
Wm2dPlayerDtorHS:
	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	stw r30, 8(r1)

	mr r31, r4
	mr r30, r3

	cmpwi r3, 0
	beq oops
	lwz r3, 0x384(r30)
	li r4, 1
	bl __dt__21dHammerSuitRenderer_cFv
oops:
	
	mr r3, r30
	mr r4, r31
	bl RealWm2dPlayerDtor

	lwz r30, 8(r1)
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.global Wm2dPlayerSetMeUpHS
Wm2dPlayerSetMeUpHS:
	# partway in: r27 = this
	lwz r3, 0x384(r27)
	lwz r4, 0x128(r27)
	li r5, 1
	bl setup__21dHammerSuitRenderer_cFP21dPlayerModelHandler_ci

	# epilogue
	addi r11, r1, 0x20
	bl _restgpr_27
	lwz r0, 0x24(r1)
	mtlr r0
	addi r1, r1, 0x20
	blr

.global Wm2dPlayerDrawHS
Wm2dPlayerDrawHS:
	# dicks (a very insightful comment)
	lbz r0, 0x261(r3)
	cmpwi r0, 0
	beqlr

	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	mr r31, r3

	lwz r3, 0x384(r31)
	bl draw__21dHammerSuitRenderer_cFv

	lwz r3, 0x128(r31)
	bl draw__21dPlayerModelHandler_cFv

	lwz r0, 0x14(r1)
	lwz r31, 0xC(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr



#DO IT A THIRD TIME
.global WmPlayerCtorHS
WmPlayerCtorHS:
	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	mr r31, r3

	bl build__21dHammerSuitRenderer_cFv
	stw r3, 0x308(r31)

	mr r3, r31
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.global WmPlayerDtorHS
WmPlayerDtorHS:
	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	stw r30, 8(r1)

	mr r31, r4
	mr r30, r3

	cmpwi r3, 0
	beq oops3
	lwz r3, 0x308(r30)
	li r4, 1
	bl __dt__21dHammerSuitRenderer_cFv
oops3:
	
	mr r3, r30
	mr r4, r31
	bl RealWmPlayerDtor

	lwz r30, 8(r1)
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.global WmPlayerSetMeUpHS
WmPlayerSetMeUpHS:
	# partway in: r31 = this
	lwz r3, 0x308(r31)
	addi r4, r31, 0x1cc
	bl setup__21dHammerSuitRenderer_cFP21dPlayerModelHandler_c

	# epilogue
	li r3, 1
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.global WmPlayerDrawHS
WmPlayerDrawHS:
	# partway in: r31 = this
	lwz r3, 0x308(r31)
	bl draw__21dHammerSuitRenderer_cFv

	# epilogue
	li r3, 1
	lwz r0, 0x14(r1)
	lwz r31, 0xC(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.extern _restgpr_27
.extern RealAcPyDtor
.extern RealWm2dPlayerDtor
.extern RealWmPlayerDtor
.extern build__21dHammerSuitRenderer_cFv
.extern __dt__21dHammerSuitRenderer_cFv
.extern setup__21dHammerSuitRenderer_cFP21dPlayerModelHandler_ci
.extern setup__21dHammerSuitRenderer_cFP21dPlayerModelHandler_c
.extern draw__21dHammerSuitRenderer_cFv
.extern draw__21dPlayerModelHandler_cFv

.extern GabonRock_DropOutState
.extern GabonRock_CrushState


.global HammerGabonRock
HammerGabonRock:
	# r27 = apOther
	# r28 = apThis
	# r29 = objOther
	# r31 = objThis
	lbz r0, 0x2D(r27)
	cmpwi r0, 19
	bne hgrExit

	addi r3, r31, 0x394
	lis r4, GabonRock_CrushState@h
	ori r4, r4, GabonRock_CrushState@l
	lwz r12, 0(r3)
	lwz r12, 0x18(r12)
	mtctr r12
	bctrl

	lbz r0, 0xA1(r28)
	ori r0, r0, 2
	stb r0, 0xA1(r28)
hgrExit:
	lwz r27, 0x2C(r1)
	lwz r28, 0x30(r1)
	lwz r29, 0x34(r1)
	lwz r30, 0x38(r1)
	lwz r31, 0x3C(r1)
	lwz r0, 0x44(r1)
	mtlr r0
	addi r1, r1, 0x40
	blr

.global HammerCallbackGabonRockOverride
.extern forceHCGROPushBack
.extern continueFromHCGRO
HammerCallbackGabonRockOverride:
	lhz r3, 8(r30) #objThis
	cmpwi r3, 115 #HAMMER
	bne continueHCGRO

	lhz r3, 8(r30) #objOther
	cmpwi r3, 374 #GABON_ROCK
	bne continueHCGRO

	b forceHCGROPushBack

continueHCGRO:
	lbz r0, 0x2D(r4) #to fix the branch we nuked
	b continueFromHCGRO


.data

PowerupSounds:
	.long 273, 274, 274, 275, 276, 277, 274, 274, 274, 274, 274, 274, 274, 274, 1634
	
	#vanilla sound effect IDs:
	#273 = player shrink
	#274 = normal powerup sound
	#275 = mini mushroom
	#276 = propeller
	#277 = penguin
	#1634 = SE_SYS_GEN_W9, very special cool sound that i want to use for a very bad useless powerup
	
	#custom sound effect IDs:
	#2019 = lemmy
	#2020 = mega mushroom
	#and so on...

SomeTable:
	.long SomeTable_802F5580
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F5440
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0

ZeroFloat:
	.float 0.0

OneFloat:
	.float 1.0

FukidashiTable2:
	.float 20.0, 32.0, 32.0, 12.0, 38.0, 33.0, 32.0, 32.0, 32.0, 32.0, 32.0, 33.0, 64.0, 32.0, 32.0
	.float 20.0, 34.0, 34.0, 12.0, 39.0, 35.0, 34.0, 34.0, 34.0, 34.0, 34.0, 35.0, 64.0, 34.0, 34.0
	.float 20.0, 29.0, 29.0, 11.0, 34.0, 29.0, 29.0, 29.0, 29.0, 29.0, 29.0, 29.0, 58.0, 29.0, 29.0
	.float 20.0, 29.0, 29.0, 11.0, 34.0, 29.0, 29.0, 29.0, 29.0, 29.0, 29.0, 29.0, 58.0, 29.0, 29.0

PlayerHeightsNormal:
	.float 16.0, 27.0, 27.0, 12.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 54.0, 27.0, 27.0

PlayerHeightsDucking:
	.float 13.0, 21.0, 21.0, 9.0, 21.0, 21.0, 21.0, 21.0, 21.0, 21.0, 21.0, 21.0, 42.0, 21.0, 21.0

WtfEverTable:
	.float 4.5, 10.0, 10.0, 1.0, 10.0, 9.0, 10.0, 10.0, 10.0, 10.0, 10.0, 9.0, 20.0, 10.0, 10.0

YetAnotherTable:
	.float 0.0, -4.0, -4.0, 4.0, -6.0, -2.0, -4.0, -4.0, -4.0, -4.0, -4.0, -4.0, -8.0, -4.0, -4.0

WeirdScaleTable:
	.float 16.0, 31.0, 31.0, 6.0, 34.0, 31.0, 31.0, 31.0, 31.0, 31.0, 31.0, 31.0, 62.0, 31.0, 31.0

.align 4
AsdfTable:
	.byte 1, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2

.align 4
BlahTableOffsets:
	.long 1, 2, 2, 0, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2

PowerupScaleFloats: #@SFNP
	.long 0xDEADBEEF, 0xDEADBEEF # padding
	.float 1.0
	.float 1.0
	.float 1.0
	.float 0.6
	.float 1.0
	.float 1.0
	.float 1.0
	.float 1.0
	.float 1.0
	.float 1.0
	.float 1.0
	.float 1.0
	.float 2.0
	.float 1.0
	.float 1.0

PowerupTextures: #@SFNP
	.float 0.0
	.float 1.0
	.float 2.0
	.float 3.0
	.float 4.0
	.float 5.0  # Hammer
	.float 6.0  # Bubble
	.float 7.0  # Lemmy
	.float 8.0  # Mandy
	.float 9.0  # Bomb
	.float 0.0  # Mega
	.float 10.0 # Boomerang
	.float 11.0 # Evil
ToadTextures: #@SFNP
	.long 0xDEADBEEF, 0xDEADBEEF
	.float 7.0, 8.0, 6.0
ToadBubbleTextures:
	.long 0xDEADBEEF, 0xDEADBEEF
	.float 9.0, 10.0, 6.0
ToadLemmyTextures:
	.long 0xDEADBEEF, 0xDEADBEEF
	.float 11.0, 12.0, 7.0
ToadMandyTextures:
	.long 0xDEADBEEF, 0xDEADBEEF
	.float 13.0, 14.0, 8.0
ToadBombTextures:
	.long 0xDEADBEEF, 0xDEADBEEF
	.float 14.0, 15.0, 9.0
ToadMegaTextures:
	.long 0xDEADBEEF, 0xDEADBEEF
	.float 4.0, 7.0, 10.0
ToadBoomerangTextures:
	.long 0xDEADBEEF, 0xDEADBEEF
	.float 16.0, 17.0, 11.0
ToadEvilTextures:
	.long 0xDEADBEEF, 0xDEADBEEF
	.float 18.0, 19.0, 12.0

.global BrosArcFileName
BrosArcFileName:
	.string "hammerM"

.global I_hammerResFile
I_hammerResFile:
	.string "g3d/I_fireflower.brres"

.global I_hammerModelName
I_hammerModelName:
	.string "I_fireflower"

.global g3d_I_kinoko_brres
g3d_I_kinoko_brres: .string "g3d/I_kinoko.brres"

.global I_propeller_model
I_propeller_model: .string "I_propeller_model"

.global SI_kinoko
SI_kinoko: .string "SI_kinoko"

.global SI_hammer #@SFNP - optional, only add if you want to put this new powerup in stockItem
SI_hammer: .string "SI_hammer"

.global SI_bubble
SI_bubble: .string "SI_bubble"

.global SI_lemmy
SI_lemmy: .string "SI_lemmy"

.global I_hammerArcFilename #@SFNP
I_hammerArcFilename:
	.string "I_hammer"

.global I_bubbleArcFilename
I_bubbleArcFilename:
	.string "I_bubble"

.global I_lemmyArcFilename
I_lemmyArcFilename:
	.string "I_lemmy"

.global I_mandyArcFilename
I_mandyArcFilename:
	.string "I_mandy"
	
.global I_bombArcFilename
I_bombArcFilename:
	.string "I_bomb"
	
.global I_megaArcFilename
I_megaArcFilename:
	.string "I_mega"
	
.global I_boomerangArcFilename
I_boomerangArcFilename:
	.string "I_boomerang"

.global I_evilArcFilename
I_evilArcFilename:
	.string "I_evil"

g3d: .string "g3d/"

brres: .string ".brres"

g3dmodel: .string "g3d/model.brres"

.text
.global AddHSToBattleItem_FixWait2Anim
AddHSToBattleItem_FixWait2Anim:
	lis r4, wait2@h
	ori r4, r4, wait2@l
	blr

.global AddBSToBattleItem_FixKinoSwitch
AddBSToBattleItem_FixKinoSwitch:
	lis r4, I_kinoko_switch@h
	ori r4, r4, I_kinoko_switch@l
	blr

.global InsertLostG3D
InsertLostG3D:
	lis r4, g3d@h
	ori r4, r4, g3d@l
	blr

.global InsertLostBRRES
InsertLostBRRES:
	lis r4, brres@h
	ori r4, r4, brres@l
	blr

.global InsertLostG3DModel
InsertLostG3DModel:
	lis r5, g3dmodel@h
	ori r5, r5, g3dmodel@l
	blr
.data
wait2: .string "wait2"
I_kinoko_switch: .string "I_kinoko_switch"

#PowerupTexDebugStr:
#	.string "Plumber::SetPowerupTexture() called on class %p with texnum %d lr=%p\n"

#DBString:
#	.string "PW:%d\n"

#.global BrosModelFilename
#BrosModelFilename:
#	.string "g3d/bros.brres"

#.global HammerModelName
#HammerModelName:
#	.string "bros_hammer"

.text
.extern continueFromStrongBox
.extern continueFromOrigStrongBox
.global StrongboxInit
StrongboxInit:
	#first we do a check on nybble 7
	lwz r12, 0x4(r3) 					#settings
	rlwinm r12,r12,0xc,0x1c,0x1f 		#nybble 7
	cmplwi r12, 1						#if nybble 7 is set
	beq StrongboxNewer
	# so if it isn't set, we do vanilla game behavior
	rlwinm. r0,r7,0x4,0x1c,0x1f 		#instruction we replaced
	b continueFromOrigStrongBox

StrongboxNewer:
	# MODE
	li r4, 2
	stw r4, 0x8E0(r30)
	# ITEM COUNT (nybble 8)
	extrwi r4, r7, 4, 12
	stw r4, 0x8E8(r30)
	b continueFromStrongBox

.extern continueFromStrongBox2
.extern continueFromOrigStrongBox2
.extern AddStockPowerup
.extern create__13dStageActor_cF6ActorsUiP7Point3dP6S16VecUc
.global StrongboxGiveBattleItem
StrongboxGiveBattleItem:
	#first we do a check on nybble 7
	lwz r12, 0x4(r30) 					#settings
	rlwinm r12,r12,0xc,0x1c,0x1f 		#nybble 7
	cmplwi r12, 1						#if nybble 7 is set
	beq GiveBattleItemNewer
	## if it isn't set, we do vanilla game behavior
	bl AddStockPowerup 					#instruction we replaced
	b continueFromOrigStrongBox2

GiveBattleItemNewer:
	# save a reg we can use
	stw r29, 0x24(r1)

	# get number of current item
	lhz r6, 0x8FA(r30)
	# create a shift amount...
	slwi r6, r6, 2
	lwz r7, 4(r30)
	# ..and use it to get the item
	srw r7, r7, r6
	clrlwi r29, r7, 28

	# now give it
	lis r4, IndexItemsForStockItem@h
	ori r4, r4, IndexItemsForStockItem@l
	lbzx r4, r4, r29
	bl AddStockPowerup

	# and then copy the position
	lfs f1, 0xAC(r30)
	stfs f1, 0x10(r1)
	lfs f1, 0xB0(r30)
	lfs f2, 0x8D8(r30)
	fsubs f1, f1, f2
	stfs f1, 0x14(r1)
	lfs f1, 0xB4(r30)
	stfs f1, 0x18(r1)

	li r3, 0x25
	slwi r4, r31, 4
	or r4, r4, r29
	addi r5, r1, 0x10
	li r6, 0
	li r7, 0
	bl create__13dStageActor_cF6ActorsUiP7Point3dP6S16VecUc

	# restore our reg
	lwz r29, 0x24(r1)
	b continueFromStrongBox2

.global StrongboxMushroomCheck
StrongboxMushroomCheck:
	#first we do a check on nybble 7
	lwz r12, 0x4(r30) 					#settings
	rlwinm r12,r12,0xc,0x1c,0x1f 		#nybble 7
	cmplwi r12, 1						#if nybble 7 is set
	beq NoMushroom
	## if it isn't set, we do vanilla game behavior
	lis r5, -0x7fbd						#instruction we replaced
	blr

NoMushroom:
	blr

.global PassSettingsToToad
PassSettingsToToad:
	lwz r12, 0x4(r3)
	li r3, 0x24
	mr r4, r12
	blr

.extern continueFromKinopio
.global StrongboxKinopio
StrongboxKinopio:
	#first we do a check on nybble 7
	lwz r12, 0x4(r28) 					#settings
	rlwinm r12,r12,0xc,0x1c,0x1f 		#nybble 7
	cmplwi r12, 0 						#if nybble 7 isn't set
	beq ShowKinopio
	## if it is set, don't do anything (don't show toad)
	b continueFromKinopio

ShowKinopio:
	bctrl
	lwz r12, 0x3b4(r31)
	addi r3, r31, 0x3b4
	lwz r12, 0x14(r12)
	mtspr CTR, r12
	bctrl
	b continueFromKinopio

.extern continueToFanfare
.global StrongboxFanfare
StrongboxFanfare:
	lwz r12, 0x4(r28) 					#settings
	rlwinm r12,r12,0xc,0x1c,0x1f 		#nybble 7
	cmplwi r12, 0 						#if nybble 7 isn't set
	beq doFanfare
	blr

doFanfare:
	stwu r1, -0x10(r1) # instruction we replaced
	b continueToFanfare

.data
IndexItemsForStockItem:
.byte 0,1,2,3,5,6,4,7,8,9

.text

.global WMItem_AddBrres, WMItem_AddMdl0

WMItem_AddBrres:
	lwzx r4, r27, r0
	cmpwi r0, 28
	beq doFireFlowerBrres
	cmpwi r0, 32
	beq doFireFlowerBrres
	cmpwi r0, 36
	beq doFireFlowerBrres
	blr
doFireFlowerBrres:
	lis r5, FireflowerFileName@h
	ori r5, r5, FireflowerFileName@l
	blr
WMItem_AddMdl0:
	lwzx r4, r4, r0
	cmpwi r0, 28
	beq doFireFlowerMdl0
	cmpwi r0, 32
	beq doFireFlowerMdl0
	cmpwi r0, 36
	beq doFireFlowerMdl0
	blr
doFireFlowerMdl0:
	lis r4, FireflowerModelName@h
	ori r4, r4, FireflowerModelName@l
	blr

.data
FireflowerFileName: .string "g3d/I_fireflower.brres"
FireflowerModelName: .string "I_fireflower"
.align 4

.text

#thanks rsm
.extern ContinueFromThatFix
.global TryToFixThis
TryToFixThis:
	lbz r0, 0x152(r31)		#r0 = this->powerupId
	cmpwi r0, 8				#if(r0 == 8)
	bge StopBeingAnAss		# { StopBeingAnAss() }
	lwz r0, 0x770(r31)		#else { r0 = this->current_player_model_id
	b ContinueFromThatFix	# ContinueFromThatFix() }
StopBeingAnAss:
	li r0, 0				#r0 = 0
	stw r0, 0x770(r31)		#this->current_player_model_id = r0
	b ContinueFromThatFix	#ContinueFromThatFix()


# from stockItemFix.S

# Add this to the bl hook!
.global StockItemHack
.global StockItemHackR3
.global ItemNamesHack
.global ItemNamesHackR26

# Add these to the symbol map!
.extern SI_Kinoko, SI_Fireflower, SI_Propeller, SI_Iceflower, SI_Penguin, SI_Star

# From loading.S
.extern aI_kinoko_3, aI_fireflower_2, aI_iceflower_0, aI_star_0, aI_propeller_0, aI_penguin, I_hammer, I_bubble, I_lemmy

.text
StockItemHack:
  lis r28, StockItemList@h
  ori r28, r28, StockItemList@l
  blr

.text
StockItemHackR3:
  lis r3, StockItemListR3@h
  ori r3, r3, StockItemListR3@l
  blr

.text
ItemNamesHack:
  lis r31, ItemNamesList@h
  ori r31, r31, ItemNamesList@l
  blr

.text
ItemNamesHackR26:
  lis r26, ItemNamesList@h
  ori r26, r26, ItemNamesList@l
  blr

.data
.align 4
StockItemList:
	.long SI_Kinoko
	.long SI_Fireflower
	.long SI_Propeller
	.long SI_Iceflower
	.long SI_Penguin
	.long SI_Star
	.long SI_hammer
	.long SI_bubble
	.long SI_lemmy

StockItemListR3:
	.long SI_Kinoko
	.long SI_Fireflower
	.long SI_Propeller
	.long SI_Iceflower
	.long SI_Penguin
	.long SI_Kinoko
	.long SI_Star
	.long SI_hammer
	.long SI_bubble
	.long SI_lemmy

ItemNamesList:
	.long teresa
	.long aI_kinoko_3							# "I_kinoko"
	.long aI_fireflower_2						# "I_fireflower"
	.long aI_iceflower_0						# "I_iceflower"
	.long aI_star_0								# "I_star"
	.long aI_propeller_0						# "I_propeller"
	.long aI_penguin							# "I_penguin"
	.long I_hammer								# "I_hammer"
	.long I_bubble								# "I_bubble"
	.long I_lemmy								# "I_lemmy"

teresa:
	.string "teresa"