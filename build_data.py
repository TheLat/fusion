import os

sprites = []
levels = []
menu_sprites = []
character_sprites = []


for filename in os.listdir("resources/levels"):
    if filename != ".DS_Store":
        levels.append(filename[:-4])

for filename in os.listdir("resources/level_sprites"):
    if filename != ".DS_Store":
        sprites.append(filename)

for filename in os.listdir("resources/menu_sprites"):
    if filename != ".DS_Store":
        menu_sprites.append(filename)

for filename in os.listdir("resources/characters"):
    if filename != ".DS_Store":
        character_sprites.append(filename)

f = open("resources/data/levels.dat", 'w')
for l in levels:
    f.write(l)
    f.write("\n")

f.close()

f = open("resources/data/level_sprites.dat", 'w')
f.write(str(len(sprites)))
f.close()

f = open("resources/data/menu_sprites.dat", 'w')
for m in menu_sprites:
    f.write(m)
    f.write("\n")

f.close()


f = open("resources/data/character_sprites.dat", 'w')
for c in character_sprites:
    f.write(c)
    f.write("\n")

f.close()

