.text
#include <asm_setup.S>

.extern doneWithPatchAnimNameArray
.extern doneWithPatchAnimByte34Array

.global PatchAnimNameArray
PatchAnimNameArray:
	lis r4, AnimNameArray@h
	ori r4, r4, AnimNameArray@l
	lwzx r4, r28, r4
	b doneWithPatchAnimNameArray

.global PatchAnimByte34Array
PatchAnimByte34Array:
	lis r4, AnimByte34Array@h
	ori r4, r4, AnimByte34Array@l
	lwzx r4, r26, r4
	b doneWithPatchAnimByte34Array


.data

.global AnimNameArray
AnimNameArray:
	.long aCobcourseclear
	.long aCobcoursehelp
	.long aCobcourseopen
	.long aCobcoursehalf
	.long 0

.global AnimByte34Array
AnimByte34Array:
	.long 1, 0, 0, 1

aCobcourseclear:
	.string "cobCourseClear"

aCobcoursehelp:
	.string "cobCourseHelp"

aCobcourseopen:
	.string "cobCourseOpen"

aCobcoursehalf:
	.string "cobCourseHalf"

.text

.set sp, 1

.extern cobCourseChooseFilename

.global cobCourseSpriteTex
cobCourseSpriteTex:
	stwu sp, -0x20(sp)
	mflr r0
	stw r0, 0x24(sp)
	stw r3, 0x1C(sp)
	stw r4, 0x18(sp)
	stw r6, 0x14(sp)
	stw r7, 0x10(sp)
	stw r8, 0xC(sp)
	stw r9, 0x8(sp)

	lis r3, cobCourseFilename@h
	ori r3, r3, cobCourseFilename@l 

	bl cobCourseChooseFilename

	mr r5, r3 # move filename to r5 from r3

	lwz r0, 0x24(sp)
	lwz r3, 0x1C(sp)
	lwz r4, 0x18(sp)
	lwz r6, 0x14(sp)
	lwz r7, 0x10(sp)
	lwz r8, 0xC(sp)
	lwz r9, 0x8(sp)
	mtlr r0
	addi sp, sp, 0x20
	blr

.data

.global cobCourseFilename
cobCourseFilename:
	.string "g3d/model.brres"
.align 4