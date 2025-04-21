# This is used for replacing references to pane names so they can either be deleted, or to remove any kind of pane hardcode
.set sp,1
.set rtoc,2

.text

.data
.global N_dummy_00
N_dummy_00: .string "N_dummy_00"

.global P_dummy_00
P_dummy_00: .string "P_dummy_00"

.global T_dummy_00
T_dummy_00: .string "T_dummy_00"

.global W_dummy_00
W_dummy_00: .string "W_dummy_00"

# Probably won't ever need this but just in case
.global B_dummy_00
B_dummy_00: .string "B_dummy_00"