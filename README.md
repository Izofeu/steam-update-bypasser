# steam-update-bypasser
Allows gamers to skip annoying forced Steam game / software updates.


# What's Steam update bypasser?
Steam update bypasser is an educational C++ project of mine that I used to learn file read/write operations on. This means it probably is slightly spaghetti and not super organized, but I learned a lot while coding this application. It took me almost 4 hours to write the 1st version of the software and additional 4 to create this one.

# How do you use it?
You can compile it from scratch or use the provided exe. It was compiled with GCC 4.9.2 64-bit release. Make sure Steam is closed. Simply follow the program's instructions. As for manifest, the way Steam detects if a game needs updating is by checking the Steam's version of game version. We can skip the update by telling Steam that the game is 1. fully updated and 2. is on the newest version. To do this, we need to get the newest app's (manifest) version from https://steamdb.info. Search for your game's name, click Depots, choose one you're using (it's ID is likely app ID +1), click Manifests, and use the newest ManifestID in my app.

# Is this permanent?
Sadly, no. You'll need to redo it every time an update comes out.
