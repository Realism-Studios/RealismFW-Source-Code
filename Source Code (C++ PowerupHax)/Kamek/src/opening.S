.text
#include <asm_setup.S>
.align 4

.extern __ptmf_scall

.global callOpeningState__FP12dScOpening_cUi
callOpeningState__FP12dScOpening_cUi:
	stwu sp, -0x10(sp)
	mflr r0
	stw r0, 0x14(sp)
	stw r31, 0xC(sp)

	lis r5, OpeningStates@h
	ori r5, r5, OpeningStates@l
	mulli r4, r4, 0xC
	add r12, r5, r4
	bl __ptmf_scall

	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 16
	blr


.data

.extern state0_startsPage__12dScOpening_cFv, state1_writesDialog__12dScOpening_cFv, state2_showsButton__12dScOpening_cFv
.extern state3_idlesButton__12dScOpening_cFv, state4_waitsForInput__12dScOpening_cFv, state5_hidesButton__12dScOpening_cFv
.extern state6_flipsPageNext__12dScOpening_cFv, state7_flipsPageBack__12dScOpening_cFv, state8_endsOpening__12dScOpening_cFv

.global OpeningStates
OpeningStates:
	.long 0, -1, state0_startsPage__12dScOpening_cFv
	.long 0, -1, state1_writesDialog__12dScOpening_cFv
	.long 0, -1, state2_showsButton__12dScOpening_cFv
	.long 0, -1, state3_idlesButton__12dScOpening_cFv
	.long 0, -1, state4_waitsForInput__12dScOpening_cFv
	.long 0, -1, state5_hidesButton__12dScOpening_cFv
	.long 0, -1, state6_flipsPageNext__12dScOpening_cFv
	.long 0, -1, state7_flipsPageBack__12dScOpening_cFv
	.long 0, -1, state8_endsOpening__12dScOpening_cFv