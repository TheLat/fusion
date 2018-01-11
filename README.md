This is a reimplementation of Pokemon Red and Blue, but with the implementation of the idea presented at this website:  http://pokemon.alexonsager.net/

This work is not done with his permission.

This is intended to be a hobby project and will never be released.

The game is more or less the same as Pokemon Red and Blue, but with changes made so that it is technically possible to capture all 22,801 pokemon.  God help you if you try.  The game is built with data pulled from Bulbapedia (http://bulbapedia.bulbagarden.net/), The VG Resource (https://www.vg-resource.com/), and Veekun (https://veekun.com/).

If you want to get into the raw data, it is all stored in plain text format in the resoures/data directory.  You can alter this, if you want.  Stability cannot be guaranteed if you do.  The basic idea behind the fusions is that it's half of each pokemon slammed together.  The stats are the average of the two parents.  The moveset will be every odd move from the first parent and every even move from the second parent, unless the developer has deliberately altered the data.  The types of the fusion will be the first type of each parent.  If those are the same, it will use the second type of the second parent, if it exists.  To deal with the possibility of a fusion never learning a damage-dealing move, a fusion will be able to learn every move from a TM or HM that either of its parents could learn.

Bugs in Pokemon Red and Blue will be fixed, such as the fact that Focus Energy would divide your critical chance by 4 instead of multiplying it.

The developer of this game wishes to remain anonymous and will not answer any questions.  If, for some reason, people want to contribute art assets, the developer may take them and include them in the game.  Credits to the game will be the original Pokemon Red and Blue credits.

List of changes:
- Fusion machine in Professor Oak's Lab.
- Major status effects (Burn, Freeze, Paralyze, Poison, Sleep) are on longer mututally exclusive.
- Toxic is now a major status effect that persists outside of combat.  Swapping out a pokemon while it has Toxic on it will reset its counter.  A pokemon that suffers from Toxic can't be poisoned.  A pokemon that is poisoned and has Toxic applied to it will no longer be poisoned.
- Any story event that asks you to choose between pokemon that can't be found in the wild (the fossils, Hitmonlee and Hitmonchan, etc.) will grant you all the pokemon instead.  This does not apply to the starter pokemon at the beginning.
- Beating the Elite 4 will give you the option of getting more money or another pick from the starter pokemon.
- Beating the Elite 4 will allow you to challenge them in Hardcore Mode, where all of their pokemon have leveled up once for every time you've beaten them.
- TMs will be rechargeable instead of one-shots.  Not sure where to put the TM recharger yet.
- Critical hit chance will be done on an individual attack basis for multi-attacks
- Focus Energy will multiply your critical chance by 4 instead of dividing it by 4.
- You will be able to explore the S.S. Anne area after the boat leaves and you will be able to push the truck to get Mew.  I am unsure of the validity of this in the original game, but it seems as good a place as any to let you get Mew.
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
- Maximum name length is now 14 characters.
- Inventory screen now displays 5 items, because the original game is doing something weird with the spacing and the developer doesn't care enough to figure it out.
- Moltres learns Fire Blast.
- Gust is a flying move.  This makes early life hard for Bulbasaur.
- Butterfree weighs 12.5 pounds, used to be 70 pounds.  What the hell?
- Beedrill weights 15 pounds.
- Changed Rage to greatly increase attack, defense, and special when attacked.
- Increased damage to self on miss of Jump Kick and Hi Jump Kick.
- Lowered Jump Kick's power.
- Increased Hi Jump Kick's power.
- Moves that deal set damage (Sonicboom, Seismic Toss) are now subject to type multipliers and can score critical hits.  This may be removed in the final version.
- Critical hits will instead deal x1.5 damage, but ignore all buffs on the target and all debuffs on the user.
- CUT no longer shows up in your menu options for a pokemon.  Instead, you just have to walk up to a bush and interact with it and if something in your party knows CUT, it will be cut down.
- Bushes don't grow back.
- Gym badges no longer do anything, aside from being required to pass certain NPCs.  You will be able to use all HMs from the beginning.
- Aerodactyl natively learns Rock Slide

Concerning Fusions:
  There are 22,801 pokemon that can appear in Pokemon: Fusion.  The developer can't inspect all of them.  Some may be duds with few or no attack moves (Pikachu-Bulbasaur is quite terrible).  Some things may happen that make no sense (things fused with Gengar will learn Hypnosis from Gengar or Dream Eater from Gengar, but not both).  As a way to balance this, fusions are compatible with every TM and HM that either parent could learn.
  While there are 22,801 possible fusions in the game, only 19,667 can be evolved to naturally.  A Bulbasaur-Charmander will always evolve into an Ivysaur-Charmander, and a Bulbasaur-Charmeleon will not exist unless you specifically fuse those two.

