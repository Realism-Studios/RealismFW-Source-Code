.text
.align 4

.align 4
.set sp, 1

#ifndef __MWERKS__
.set r0,0;   .set r1,1;   .set r2,2; .set r3,3;   .set r4,4
.set r5,5;   .set r6,6;   .set r7,7;   .set r8,8;   .set r9,9
.set r10,10; .set r11,11; .set r12,12; .set r13,13; .set r14,14
.set r15,15; .set r16,16; .set r17,17; .set r18,18; .set r19,19
.set r20,20; .set r21,21; .set r22,22; .set r23,23; .set r24,24
.set r25,25; .set r26,26; .set r27,27; .set r28,28; .set r29,29
.set r30,30; .set r31,31; .set f0,0; .set f2,2; .set f3,3
.set f1,1;   .set f30,30; .set f31,31
#endif

.extern ContinueFromAlternateQBlock
.extern QBlockTable
.global QBlockVariation
QBlockVariation:
	lwz r0, 4(r27)
	extrwi r0, r0, 4,24 #24 -> nybble 11 | 20 -> nybble 10 | 16 -> nybble 9 | etc...
	cmpwi r0, 0
	beq doFirstTable
	cmpwi r0, 1
	beq doSecondTable
	b ContinueFromAlternateQBlock
doFirstTable:
	lis r3, QBlockTable@h
	ori r3, r3, QBlockTable@l
	b ContinueFromAlternateQBlock
doSecondTable:
	lis r3, AlternateQBlockTable@h
	ori r3, r3, AlternateQBlockTable@l
	b ContinueFromAlternateQBlock

.extern ContinueFromAlternateQBlock2
.extern QBlockTable2
.global QBlockVariation2
QBlockVariation2:
	lwz r0, 4(r27)
	extrwi r0, r0, 4,24 #24 -> nybble 11 | 20 -> nybble 10 | 16 -> nybble 9 | etc...
	cmpwi r0, 0
	beq doFirstTable2
	cmpwi r0, 1
	beq doSecondTable2
	b ContinueFromAlternateQBlock2
doFirstTable2:
	lis r3, QBlockTable2@h
	ori r3, r3, QBlockTable2@l
	b ContinueFromAlternateQBlock2
doSecondTable2:
	lis r3, AlternateQBlockTable@h
	ori r3, r3, AlternateQBlockTable@l
	b ContinueFromAlternateQBlock2



.extern ContinueFromAlternateQBlock3
.global QBlockVariation3
QBlockVariation3:
	lwz r12, 4(r29)
	extrwi r12, r12, 4,24 #24 -> nybble 11 | 20 -> nybble 10 | 16 -> nybble 9 | etc...
	cmpwi r12, 0
	beq doFirstTable3
	cmpwi r12, 1
	beq doSecondTable3
	b ContinueFromAlternateQBlock3
doFirstTable3:
	lis r5, QBlockTable@h
	ori r5, r5, QBlockTable@l
	b ContinueFromAlternateQBlock3
doSecondTable3:
	lis r5, AlternateQBlockTable@h
	ori r5, r5, AlternateQBlockTable@l
	b ContinueFromAlternateQBlock3

.extern ContinueFromAlternateQBlock4
.global QBlockVariation4
QBlockVariation4:
	lwz r12, 4(r29)
	extrwi r12, r12, 4,24 #24 -> nybble 11 | 20 -> nybble 10 | 16 -> nybble 9 | etc...
	cmpwi r12, 0
	beq doFirstTable4
	cmpwi r12, 1
	beq doSecondTable4
	b ContinueFromAlternateQBlock4
doFirstTable4:
	lis r5, QBlockTable2@h
	ori r5, r5, QBlockTable2@l
	b ContinueFromAlternateQBlock4
doSecondTable4:
	lis r5, AlternateQBlockTable@h
	ori r5, r5, AlternateQBlockTable@l
	b ContinueFromAlternateQBlock4

.data

# DCB value stuff
# 0xF = empty, 2 = coin, 0 = fire, 0x15 = prop, 0x11 = pengi, 0x19 = mini
# 1 = star, 0x1B = continuous star, 0xC = yoshi egg, 4 = 10 coins, 7 = 1-up
# 0xA = vine, 0xD = spring, 8 = coin/mushroom, 0xE = ice, all others are mushrooms
# patched values:
# 6 = hammer, 0x10 = bubble, 0x12 = boomerang
AlternateQBlockTable: #uncommented values are empty slots.
	.long 0x6 #hammer
	.long 0x2 #coin
	.long 0x10 #bubble
	.long 0x12 #boomerang
	.long 0x15 #prop
	.long 0x13
	.long 0x14
	.long 0x16
	.long 0x17
	.long 0x18
	.long 0x4 #coin
	.long 0x1A
	.long 0xA #vine
	.long 0xD #spring
	.long 0x1E
	.long 0x1F
	.long 0xDEADBEEF #no nybble?
	.long 0xDEADBEEF #no nybble?

