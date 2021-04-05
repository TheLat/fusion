from PIL import Image
import os

count = 0
files = []

for i in range(1,152):
    for j in range(1,152):
        if os.path.isfile("out/%s-%s-back.png" % (i, j)):
            files.append("out/%s-%s-back.png" % (i, j))
            if os.path.isfile("out/%s-%s.png" % (i, j)):
                files.append("out/%s-%s.png" % (i, j))

images = []
for f in files:
    images.append(Image.open(f).convert("RGBA"))

widths, heights = zip(*(i.size for i in images))

max_height = 0
max_width = 660
lines = 19
new_im = Image.new('RGBA', (max_width, 80*lines))

xoffset = 0
yoffset = 0
j = 0
tempxoffset = 0
while j < len(images) and tempxoffset < max_width:
    if max_height < images[j].size[1]:
        max_height = images[j].size[1]
    tempxoffset += images[j].size[0]
    j = j + 1
oldmax = max_height
for i in range(0, len(images)):
    if xoffset + images[i].size[0] >= max_width:
        oldmax = max_height
        j = 0
        tempxoffset = 0
        max_height = 0
        while j + i < len(images) and tempxoffset < max_width:
            if max_height < images[i + j].size[1]:
                max_height = images[i + j].size[1]
            tempxoffset += images[i + j].size[0]
            j = j + 1
        if yoffset + (2*max(max_height, oldmax)) > new_im.size[1]:
            new_im.save("out2/%s.png" % count)
            count = count + 1
            new_im = Image.new('RGBA', (max_width, 80*lines))
            yoffset = 0
        else:
            yoffset += max(max_height, oldmax)
        xoffset = 0
    new_im.paste(images[i], (xoffset,yoffset))
    xoffset += images[i].size[0]

new_im.save("out2/%s.png" % count)
