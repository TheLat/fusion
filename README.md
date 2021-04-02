This is a reimplementation of Pokemon Red and Blue, but with the implementation of the idea presented at this website:  http://pokemon.alexonsager.net/

This work is not done with his permission.

This is intended to be a hobby project and will never be publicly released.

The game is more or less the same as Pokemon Red and Blue, but with changes made so that it is technically possible to capture all 22,801 pokemon.  God help you if you try.  Content has been rebalanced as the developer saw fit.  The game is built with data pulled from Bulbapedia (http://bulbapedia.bulbagarden.net/), The VG Resource (https://www.vg-resource.com/), and Veekun (https://veekun.com/).  The developer does not recall where they got the sound files from, only that they were found with basic Googling.

If you want to get into the raw data, it is all stored in plain text format in the resoures/data directory.  You can alter this, if you want.  Stability cannot be guaranteed if you do.  The basic idea behind the fusions is that it's half of each pokemon slammed together.  The stats are the average of the two parents.  The moveset will be every odd move from the first parent and every even move from the second parent, unless the developer has deliberately altered the data.  The types of the fusion will be the first type of each parent.  If those are the same, it will use the second type of the second parent, if it exists.  To deal with the possibility of a fusion never learning a damage-dealing move, a fusion will be able to learn every move from a TM or HM that either of its parents could learn.

Bugs in Pokemon Red and Blue will be fixed, such as the fact that Focus Energy would divide your critical chance by 4 instead of multiplying it.

The developer of this game wishes to remain anonymous and will not answer any questions.  Credits to the game will be the original Pokemon Red and Blue credits.

List of changes:
- Fusion machine in Saffron City.
- Major status effects (Burn, Freeze, Paralyze, Poison, Sleep) are on longer mututally exclusive.
- Toxic is now a major status effect that persists outside of combat.  Swapping out a pokemon while it has Toxic on it will reset its counter.  A pokemon that suffers from Toxic can't be poisoned.  A pokemon that is poisoned and has Toxic applied to it will no longer be poisoned.
- Any story event that asks you to choose between pokemon that can't be found in the wild (the fossils, Hitmonlee and Hitmonchan, etc.) will grant you all the pokemon instead.  This does not apply to the starter pokemon at the beginning.
- Beating the Elite 4 will give you the option of getting more money or another pick from the starter pokemon.
- TMs will be rechargeable instead of one-shots.  TM Recharging shop is in Saffron City.
- Critical hit chance will be done on an individual attack basis for multi-attacks
- Focus Energy will multiply your critical chance by 4 instead of dividing it by 4.
- The S.S. Anne will not leave and you will be able to push the truck to get Mew.  The developer is unsure of the validity of this in the original game, but it seems as good a place as any to let you get Mew.
- Pokemon that can be found in the wild are a combination of everything you could find in Red or Blue, along with all possible fusions of those.
- The Safari Zone will be normal combat.  That area was bullshit.
- To deal with the fact that this game is not multiplayer, the Trade Stone item is added to allow you to evolve anything that contains at least part Kadabra, Machoke, Haunter, or Graveler.  You will need two trade stones to fully evolve fusions of those 4.
- If a pokemon has two seperate evolution triggers from an elemental stone, such as a Growlithe-Vulpix, it will always use the first parent's evolution first.  The exception to this is anything that is part Eevee.  If something is part Eevee, the Eevee will always take priority for stone evolutions.
- Ghost attacks will deal double-damage against psychics, like they're supposed to.
- Ghost attacks deal damage to normal types.  That never made sense.
- Item duplication and MISSINGNO will not exist.  Use that master ball carefully.
- TMs and HMs will be forgettable.
- Bind, Wrap, Fire Spin, and Clamp will not prevent a pokemon's actions on the final hit.
- Fire Spin now has a chance to inflict BURN.
- Increased damage of Pin Missile.
- Increased damage of Lick.
- Licktung (and everything with a Lickitung head) learns Lick.
- Dizzy Punch has a chance to confuse enemies.
- Tri Attack has a chance to burn, freeze, and paralyze the target.
- Fire types can't be burned.
- Ice types can't be frozen.
- Electric types can't be paralyzed by electric attacks.
- Grass types can't be put to sleep or paralyzed by grass attacks.
- Poison types can't be poisoned or suffer Toxic.
- Leveling rates are gone.  Everything is on the fast path now.
- Pokemon use the current-gen EXP yields.
- In addition to the normal EXP scaling, there is now a bonus and penalty for fighting things above and below your level.  Pokemon will receive a bonus 10% EXP for every level above their own a defeated enemy is, to a maximum bonus of 100%.  Pokemon will receive a 10% EXP penalty for every level they are above a defeated enemy pokemon, to a maximum of 50%.
- Different messages for x4 and x0.25 damage multipliers.
- Main character draws at center of screen.
- Speed buffs affect critical hit chance, so that there's a reason to use them.
- Maximum name length is now 13 characters.
- Inventory screen now displays 5 items, because the original game is doing something weird with the spacing and the developer doesn't care enough to figure it out.
- Moltres learns Fire Blast.
- Gust is a flying move.  This makes early life hard for Bulbasaur.
- Butterfree weighs 12.5 pounds, used to be 70 pounds.  What the hell?
- Beedrill weighs 15 pounds.
- Changed Rage to greatly increase attack, defense, and special when attacked.
- Increased damage to self on miss of Jump Kick and Hi Jump Kick.
- Lowered Jump Kick's power.
- Increased Hi Jump Kick's power.
- Moves that deal set damage (Sonicboom, Seismic Toss) are now subject to type multipliers and can score critical hits.
- Critical hits will instead deal x1.5 damage, but ignore all buffs on the target and all debuffs on the user.
- CUT no longer shows up in your menu options for a pokemon.  Instead, you just have to walk up to a bush and interact with it and if something in your party knows CUT, it will be cut down.
- Bushes don't grow back.
- Gym badges no longer do anything, aside from being required to pass certain NPCs.  You will be able to use all HMs from the beginning.
- Aerodactyl natively learns Rock Slide
- Increased Ember's chance to burn to 30%.
- Reduced Fire Blast's chance to burn to 10%.
- Confused, paralyzed, sleep, freeze, and flinch will only clear the move queue if the move is one that that pokemon knows.  Moves like the second turn component of Hyper Beam will still happen.
- Confused only checks for the first part of multi-turn moves.
- Anything that is part Drowzee or Hypno will learn Dream Eater by leveling up.  The pokedex entries in all the games talk about it eating dreams.
- Increased the status effect chances of Aurora Beam, Bubble, Confusion, and Thundershock.
- Changed EXP.ALL to consider everything in your party that is not KO'ed to have fought in the battle, but reduces total exp by 15%.  This is in line with how the NPC that gives you EXP.ALL warns that it reduces the total amount of EXP.
- The Bulbasaur line learns Body Slam because fuck you, Bulbasaur needs love.
- Solar Beam and Sky Attack both go first on the turn they attack.  They are terrible moves and this doesn't make them good, but it does improve them.
- HP IV is its own random number, not the collection of the first bits of the other IVs.
- Bug types are resistant to psychic moves.
- Ghost types are resistant to psychic moves.
- Grass types are resistant to psychic moves.
- Mimic just copies the last move now because the developer is lazy.
- Buffed power of Razor Leaf.
- Buffed accuracy of Rock Throw.
- Switched locations of ITEMFINDER and EXP.ALL.

Concerning Fusions:
  There are 22,801 pokemon that can appear in Pokemon: Fusion.  The developer can't inspect all of them.  Some may be duds with few or no attack moves (Pikachu-Bulbasaur is quite terrible).  Some things may happen that make no sense (things fused with Gengar will learn Hypnosis from Gengar or Dream Eater from Gengar, but not both).  As a way to balance this, fusions are compatible with every TM and HM that either parent could learn.
  While there are 22,801 possible fusions in the game, only 19,667 can be evolved to naturally.  A Bulbasaur-Charmander will always evolve into an Ivysaur-Charmander, and a Bulbasaur-Charmeleon will not exist unless you specifically fuse those two.

A note to modders:
  In the unlikely event that someone takes this engine and sets out to make it work for later generations of the games, here is how things work:
  - All mon data is defined in resources/original/pokemon.dat and the format is fairly straight-forward.  Tricky parts are "BODY" which references which body is shown in the team view screen, found in resources/images.  There is also "SWITCH: YES" which specifies that the type order should be reversed if it's the second parent.  This is used to make things more intuitive with things that have wings.  Zubat has "SWITCH: YES" so a Zubat-Charmander is Poison-fire, but a Charmander-Zubat is Fire-Flying.  If a pokemon is the first parent in a fusion, it contributes every even move, if it's the second parent, every odd move.  I've changed the order of some moves to make it more intuitive (see Gyarados).  Once you've added things, just run the fusion.py script in the same folder and it will generate resources/data/mon.dat, which is an enormous file.  You will need to change code and the fusion.py script to support Special Attack and Special Defense, which appeared in later games.  Likewise, you'll need to update menus.
  - Menus are all loaded when invoked and reloaded every time.  This is true for animations, too.  This makes testing a lot faster.  Some menus, such as the pokedex and the shop, are dynamically generated in menus.cpp.
  - resources/data/types.dat is where vulnerabilities and resistances are defined.
  - Level data and scripting is complicated at the best of times.
    - MAP: Where the location shows up on x-y on your map, as well as what its index is.
    - DUNGEON specifies that it is a dungeon and you can exit out of it by using DIG.
    - INSIDE specifies that you are inside and can't ride a bike.  Also, you can't fly inside.
    - MUSIC:  Specifies the music to play.  If the music has a loop at a different point than the beginning, you can specify a different intro than the main loop.
    - LEAVESOUND:  Sound to play when you exit the level into a non-neighbor level.
    - NPCS:  Complicated.  The first line takes 4 parameters, split by spaced.  The first is their unique signature, which is how the game stores their state.  The second specifies their sprite in resources/characters.  The 3rd and 4th are their X-Y coordinates in the level.  INTERACTIONS specify what they do.  Each NPC acts on their current interaction and moves on to the next unless they hit a NO_ADVANCE tag.  You can chain together actions on an interaction by splitting them with the | character.  FORCE_INTERACTION allows you to specify relative spacing that, if the player moves into, they're forced to interact with them.  MOVE_TO_PLAYER is how trainers approach you.  Some things, such as menus and battles, return a value.  In the case of menus, it returns which choice you selected in the menu.  Battles always go in this format:  BATTLE:[trainer battle ID]|{NO_ADVANCE}{DISABLE_FORCE}  which will start a battle and make the NPC stay in their current interaction state if you lose and disable the forced interaction if you win.  You can call any menu from an INTERACTION.  SET and GET allow you to store variables from dialog.
    - TRAINERS take the format of "[trainer ID] [money multiplier] [skill level] [unused float value] [name as it appears in the fight (make sure to use {SPACE} for spaces in the name] [image from resources/images/ that the trainer appears as] {[defeated message]}{[victory message]} [team definition]
      - Team definitions are in the formate of [pokemon number]:[level]:[MOVE,MOVE,MOVE,MOVE] with the move definition being optional and the team split by |
    - TELEPORT specifies what tiles on this level will teleport to another level.  If you teleport to a level that is a neighbor, the screen will not fade in transition between the two.
    - NEIGHBORS specifies other levels to be drawn at an offset.  This allows you to seamlessly travel between two levels.
    - ENCOUNTERS specifies which of the original 151 you can encounter.  What you may fight in random encounters is a randomly generated fusion from the list of regular encouners.
    - WATER_ENCOUNTERS_* specifies what you'll face with fishing and SUPER is also your surfing encounters.  What you will fight is guaranteed to be part something from the water pool and the other part will be randomly selected from the counters + the appropriate water pool.
    - LEVELS specifies the level range of random encounters.
    - DATA sucks and I'm sorry.  It's all the level tiles in resources/level_sprites, defined in rows.  Number of spaces don't matter.  New lines define new rows.  You'll have to change how level tiles are parsed if you have more than 1000 tiles because of the assumed starting 0's.

 