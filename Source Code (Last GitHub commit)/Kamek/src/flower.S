#include <asm_setup.S>

# Sys functions
.extern DVDClass
.extern GetRes
.extern strcmp

# Code
.extern HasFlowers, NoFlowers
.extern returnFromFlowerBuild1
.extern returnFromFlowerBuild2, returnFromFlowerBuild2ItsGrassland, returnFromFlowerBuild2ItsDaishizen
.extern returnFromFlowerBuild3, returnFromFlowerBuild3ItsGrassland, returnFromFlowerBuild3ItsDaishizen
.extern returnFromFlowerBuild4

# Data
.extern ObjHana
.extern ObjKusa, ObjKusaBrres
.extern Pa1Daishizen

# Globals
.global FlowerCheck
.global FlowerBuild1, FlowerBuild2, FlowerBuild3, FlowerBuild4

.text
.align 4

# This function checks whether the flowers should be loaded. To make
# your own block, copy the instructions from "mr r3, r26" to "beq true",
# add your tileset's name in the lis and ori instructions, then add
# your tileset to the tileset name list at the end of this file.

FlowerCheck:
	mr r3, r26
	lis r4, Pa1YeOlde@h
	ori r4, r4, Pa1YeOlde@l
	bl strcmp
	cmpwi r3, 0
	beq true
	
	mr r3, r26
	lis r4, Pa1YeOldeRain@h
	ori r4, r4, Pa1YeOldeRain@l
	bl strcmp
	cmpwi r3, 0
	beq true

	# No flowers detected, sir.
	b NoFlowers

true:
	b HasFlowers


# First branch in the ObjGrass onCreate, which checks the tileset name (again)
# Not gonna write an essay here as well.

FlowerBuild1:
	# Copy of the game's original compare, don't use this as a base!
	mr r26, r3
	addi r4, r31, Pa1Daishizen@l
	bl strcmp
	cmpwi r3, 0
	beq IsPa1Daishizen

	# Use this one instead ;)
	mr r3, r26
	lis r4, Pa1YeOlde@h
	ori r4, r4, Pa1YeOlde@l
	bl strcmp
	cmpwi r3, 0
	beq IsPa1YeOlde
	
	mr r3, r26
	lis r4, Pa1YeOldeRain@h
	ori r4, r4, Pa1YeOldeRain@l
	bl strcmp
	cmpwi r3, 0
	beq IsPa1YeOldeRain
	
	# Return otherwise
	b returnFromFlowerBuild1

# The value in r28 will be reused in later checks, so add a copy of this as well
IsPa1Daishizen:
	li r28, 1
	b returnFromFlowerBuild1

IsPa1YeOlde:
	li r28, 2
	b returnFromFlowerBuild1
	
IsPa1YeOldeRain:
	li r28, 3
	b returnFromFlowerBuild1

# First of the three arc file loaders. Loads the brres file based on r28's content.
FlowerBuild2:
	cmpwi r28, 0
	beq ItsGrassland
	cmpwi r28, 1
	beq ItsDaishizen
	cmpwi r28, 2
	beq ItsYeOlde
	cmpwi r28, 3
	beq ItsYeOldeRain
	b returnFromFlowerBuild2

ItsGrassland:
	b returnFromFlowerBuild2ItsGrassland

ItsDaishizen:
	b returnFromFlowerBuild2ItsDaishizen

# Copy this block!
ItsYeOlde:
	lis r3, DVDClass@ha
	lwz r3, DVDClass@l(r3)
	addi r3, r3, 4
	lis r4, ObjHana@h
	ori r4, r4, ObjHana@l
	lis r5, ObjHanaYeOlde@h
	ori r5, r5, ObjHanaYeOlde@l
	bl GetRes
	b returnFromFlowerBuild2
	
ItsYeOldeRain:
	lis r3, DVDClass@ha
	lwz r3, DVDClass@l(r3)
	addi r3, r3, 4
	lis r4, ObjHana@h
	ori r4, r4, ObjHana@l
	lis r5, ObjHanaYeOldeRain@h
	ori r5, r5, ObjHanaYeOldeRain@l
	bl GetRes
	b returnFromFlowerBuild2

# Another arc file swapper, no idea why there are two.
FlowerBuild3:
	cmpwi r28, 0
	beq ItsGrassland3
	cmpwi r28, 1
	beq ItsDaishizen3
	cmpwi r28, 2
	beq ItsYeOlde3
	cmpwi r28, 3
	beq ItsYeOldeRain3
	b returnFromFlowerBuild2

ItsGrassland3:
	b returnFromFlowerBuild3ItsGrassland

ItsDaishizen3:
	b returnFromFlowerBuild3ItsDaishizen

ItsYeOlde3:
	lis r3, DVDClass@ha
	lwz r3, DVDClass@l(r3)
	addi r3, r3, 4
	lis r4, ObjHana@h
	ori r4, r4, ObjHana@l
	lis r5, ObjHanaYeOlde@h
	ori r5, r5, ObjHanaYeOlde@l
	bl GetRes
	b returnFromFlowerBuild3
	
ItsYeOldeRain3:
	lis r3, DVDClass@ha
	lwz r3, DVDClass@l(r3)
	addi r3, r3, 4
	lis r4, ObjHana@h
	ori r4, r4, ObjHana@l
	lis r5, ObjHanaYeOldeRain@h
	ori r5, r5, ObjHanaYeOldeRain@l
	bl GetRes
	b returnFromFlowerBuild3

# This check is used for the grass. Since the game originally doesn't perform
# a check on r28, we'll do it ourselves.
FlowerBuild4:
	cmpwi r28, 1
	ble ItsGrassOrDaishizen
	cmpwi r28, 2
	beq ItsYeOlde4
	cmpwi r28, 3
	beq ItsYeOldeRain4
	b returnFromFlowerBuild2

# Copypasted code, do NOT use!
ItsGrassOrDaishizen:
	lis r3, DVDClass@ha
	lwz r3, DVDClass@l(r3)
	addi r3, r3, 4
	lis r4, ObjKusa@h
	ori r4, r4, ObjKusa@l
	lis r5, ObjKusaBrres@h
	ori r5, r5, ObjKusaBrres@l
	bl GetRes
	b returnFromFlowerBuild4

# Use this block instead ;)
ItsYeOlde4:
	lis r3, DVDClass@ha
	lwz r3, DVDClass@l(r3)
	addi r3, r3, 4
	lis r4, ObjKusa@h
	ori r4, r4, ObjKusa@l
	lis r5, ObjKusaYeOlde@h
	ori r5, r5, ObjKusaYeOlde@l
	bl GetRes
	b returnFromFlowerBuild4

ItsYeOldeRain4:
	lis r3, DVDClass@ha
	lwz r3, DVDClass@l(r3)
	addi r3, r3, 4
	lis r4, ObjKusa@h
	ori r4, r4, ObjKusa@l
	lis r5, ObjKusaYeOldeRain@h
	ori r5, r5, ObjKusaYeOldeRain@l
	bl GetRes
	b returnFromFlowerBuild4
	
.data
.align 4

# Tilesets go here
Pa1YeOlde: .string 'Pa1_ye_olde'
Pa1YeOldeRain: .string 'Pa1_ye_olde_rain'

# Flower brres files go here
ObjHanaYeOlde: .string 'g3d/obj_hana_ye_olde.brres'
ObjHanaYeOldeRain: .string 'g3d/obj_hana_ye_olde_rain.brres'

# Grass brres files go here
ObjKusaYeOlde: .string 'g3d/obj_kusa_ye_olde.brres'
ObjKusaYeOldeRain: .string 'g3d/obj_kusa_ye_olde.brres'