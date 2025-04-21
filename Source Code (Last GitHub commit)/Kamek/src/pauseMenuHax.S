.text
.extern fixPauseCounterBug__Fv

# since the pause counter increments when the window is opened, this causes an issue when
# opening/closing the window for the exit course prompt, since it closes and reopens the pause menu.
# so if we deincrement the counter when the menu opens, the window incrementing it will return it to the former value

.global decrementCounter
decrementCounter:
    stw r0, 4(r3) # instruction we replaced
    b fixPauseCounterBug__Fv
    blr