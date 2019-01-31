from PIL import Image

count = 0
files = []

for i in range(1,28):
    for j in range(1,28):
        files.append("out/%s-%s.png" % (i, j))

images = map(Image.open, files)
widths, heights = zip(*(i.size for i in images))

max_height = max(heights)
max_width = 725
new_im = Image.new('RGBA', (max_width, max_height))

xoffset = 0
for im in images:
    im = im.convert("RGBA")
    if xoffset + im.size[0] > max_width:
        new_im.save("out2/%s.png" % count)
        count = count + 1
        new_im = Image.new('RGBA', (max_width, max_height))
        xoffset = 0
    new_im.paste(im, (xoffset,0))
    xoffset += im.size[0]

new_im.save("out2/%s.png" % count)
