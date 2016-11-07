import os

sprites = []
levels = []
for filename in os.listdir("resources/levels"):
    levels.append(filename[:-4])

for filename in os.listdir("resources/level_sprites"):
    if filename != ".DS_Store":
        sprites.append(filename)


f = open("resources/data/levels.dat", 'w')
for l in levels:
    f.write(l)
    f.write("\n")

f.close()

f = open("resources/data/level_sprites.dat", 'w')
f.write(str(len(sprites)))
f.close()
