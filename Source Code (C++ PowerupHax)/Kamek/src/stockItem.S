.text
.align 4

#include <asm_setup.S>

.global CalcNewPowerupResult
.extern ContinueFromCalcPowerupResult
CalcNewPowerupResult:
	cmpwi r3, 6
    bgt cprIsNewPowerup
    b ContinueFromCalcPowerupResult
cprIsNewPowerup:
    mr r6, r3
    #stb r0, 0xF(r1)
	b ContinueFromCalcPowerupResult

.global FixCalcPowerupResultForMushroom
.extern ContinueFromFixCalcPowerupResultForMushroom
FixCalcPowerupResultForMushroom:
	stb r0, 0xF(r1)
	li r6, 1
	b ContinueFromFixCalcPowerupResultForMushroom


.global AddPowerupFinalValue
AddPowerupFinalValue:
	li r24, 0
	lis r22, OutputPowerupsByItemID@h
	ori r22, r22, OutputPowerupsByItemID@l
	blr

.global AddItemFinalValue
AddItemFinalValue:
	lis r4, UsedPowerupItemArray@h
	ori r4, r4, UsedPowerupItemArray@l
    or r25, r3, r3
	blr

.data
OutputPowerupsByItemID:
    .long 1, 2, 4, 6, 5, 3, -1, 7, 8, 9

UsedPowerupItemArray:
    .long 0, 1, 2, 3, 4, 5, 6, 7, 8, 9

.text
.global FixStockItemEffectPlayer
FixStockItemEffectPlayer:
	lwz r0, 0x8A0(r30)
	slwi r0, r0, 2
	add r5, r30, r0
	lwz r28, 0x958(r5)
	blr

.global FixStockItemEffectPlayer2
FixStockItemEffectPlayer2:
	lwz r0, 0x8A0(r30)
	stw r0, 0x8D0(r30)
	blr

.global FixMoreSIEffectRelatedCrap
FixMoreSIEffectRelatedCrap:
	lwz r0, 0x8D0(r31)
	slwi r0, r0, 2
	add r3, r31, r0
	lwz r29, 0x958(r3)
	blr

.extern playPowerupSound__Fi
.extern ContinueFromSIAddSound
.global NewPowerupSounds
NewPowerupSounds:
    lwz r3, 0x328(r25)
    bl playPowerupSound__Fi
    b ContinueFromSIAddSound
