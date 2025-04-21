Realism Freelous Worlds source code

[IMPORTANT] Credits go to the following people for the code present here:
- MandyIGuess           - doing the majority of the new code here
- Ryguy                 - NSMBWer+, P1 change fileselect feature, custom powerups, levelinfo hacks, lots and lots of help and assistance
- Nin0 & the NSMLW Team - Discord rich presence, Dolphin emulator detection feature, general assistance
- mkwcat                - P1 character change feature
- RedStoneMatt          - P1 character change feature (Kamek port)
- and to anybody else I may have forgotten

I'm releasing this mainly because it still has some use, even if not much.
What you're probably after is all of the documentation I did for the retail game, a lot of that can be found in game.hpp (see also, dInfo_c.hpp, dGameCom.hpp, etc.)

If you are here for documentation, then I'd recommend you just check out this repo instead (https://github.com/MandyIGuess/NSMBW-Documentation),
I've been putting stuff there in a much more organized form than what you'll find in this code, so consider looking there instead.
(If you need something documented then you can request it by making a GitHub Issue)

Don't try to use this code to base your mod off of, this won't work for it and it didn't even work for Realism, soo...
Also some bugs that got introduced near the end were never fixed, so the mod might genuinely be unplayable.

Additionally, most features are unfinished or in some state of being broken. Be very careful when using stuff, and don't expect it to work.

There's two copies of the code here, they're mostly similar but have a major difference:
 - The C++ PowerupHax version contains an unfinished rewrite of the custom powerups code in C++.
   This was never pushed to the repo, as it wasn't finished, but it shouldn't impact anything too much.
   Also, it may be lacking a few things from the other version, I can't remember though.

 - The "Last GitHub Commit" version is as it says, the version of the code present in the most recent
   commit to the private GitHub repo. This version is much more broken since I never had the chance to
   fix the issues introduced in the last few commits (since I was working on the PowerupHax rewrite).

For the most part, it doesn't matter which one you use, since it's not like either of them even really work.


And one last thing, don't ask for support for this, you will get none.