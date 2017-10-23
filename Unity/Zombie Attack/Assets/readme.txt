Notes:

The game must be started from the “Main Menu” scene to work properly.

To save the game while playing level 1, press the “v” key at any point.

To load the game you saved, you must stop the game entirely and then restart it from the main menu scene.

Particles in the game:
-When you shoot the gun, particles burst from the tip of it
-When a zombie is shot, it has a burst of blood particles
-When the player is attacked by zombies, he has a burst of blood particles
-During the four GUI scenes (main menu, next level, you died, you win), particle rain falls
-During gameplay, there is a different particle rainfall that stretches over the entire level

Tweening is used on all four fading effects in the game. Both levels have fading objects that use the tweening function:
x += (1.0f - x) * 0.1f
to fade into the next scene beautifully when you win or die. The game also incorporated built-in gravity.

Juiciness:
-Moving, never-ending background in GUI scenes
-Sound effects: zombie moans when they approach the player, shooting noise, button clicking noise, different background music for GUI scenes and gameplay scenes
-All buttons change color if touching mouse
-the blood particle effects add juiciness to attacking zombies and zombies attacking you

Zombie Behavior:
-they can jump over obstacles when they detect them
-move toward the player at a speed depending on how far away the player is
-change between their “attacking”, “walking”, and “dying” animations when necessary
-not implemented using behaviour tree, but I tried to for multiple hours with no success

Besides the above changes, the in-class feedback forms showed me that my game was a bit too hard because health went down too quickly and it was hard to tell when the player's health was going down.
I fixed this by incorporating the player blood particles when damage is taken, and also made the zombies wait 20 frames before inflicting more damage (instead of continuously inflicting it).