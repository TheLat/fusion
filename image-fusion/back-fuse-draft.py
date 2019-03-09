import time
import sys
from PIL import Image
from multiprocessing import Process


def make_image(i, j):
    if i == j:
        Image.open("back/%s-original.png" % i).convert("RGBA").convert("P").save("out/%s-%s.png" % (i, j))
        return
    im2 = Image.open("back/%s.png" % (j)).convert("RGBA")
    px2 = im2.load()
    for x in range(0,im2.size[0]):
        for y in range(0,im2.size[1]):
            offset = 0
            r = range(0, len(data[j]['PRIMARY']))
            r.reverse()
            for index in r:
                offset = offset + 1
                if px2[(x,y)] == data[j]['PRIMARY'][index]:
                    px2[(x,y)] = (255,0,255 - offset,0)
            for index in range(0, len(data[j]['SECONDARY'])):
                offset = offset + 1
                if px2[(x,y)] == data[j]['SECONDARY'][index]:
                    px2[(x,y)] = (255,0,255 - offset,0)
            for index in range(0, len(data[j]['TERTIARY'])):
                offset = offset + 1
                if px2[(x,y)] == data[j]['TERTIARY'][index]:
                    px2[(x,y)] = (255,0,255 - offset,0)
            offset = 0
            for index in r:
                offset = offset + 1
                if px2[(x,y)] == (255,0,255 - offset,0):
                    px2[(x,y)] = data[i]['PRIMARY'][index]
            for index in range(0, len(data[j]['SECONDARY'])):
                offset = offset + 1
                if px2[(x,y)] == (255,0,255 - offset,0):
                    px2[(x,y)] = data[i]['SECONDARY'][index]
            for index in range(0, len(data[j]['TERTIARY'])):
                offset = offset + 1
                if px2[(x,y)] == (255,0,255 - offset,0):
                    px2[(x,y)] = data[i]['TERTIARY'][index]
    im2.convert("P").save("out/%s-%s.png" % (i, j))


start_time = time.time()
data = {}
key = ""
f = open("bdata.txt")
line = f.readline().strip()
while line:
    if line.startswith("NUMBER:"):
        line = line.split(":")[1]
        key = int(line)
        data[key] = {}
    elif line.startswith("PRIMARY:"):
        line = line.split(":")[1].split(" ")
        index = 0
        while index < len(line):
            line[index] = eval(line[index])
            index = index + 1
        data[key]["PRIMARY"] = line
    elif line.startswith("SECONDARY:"):
        line = line.split(":")[1].split(" ")
        index = 0
        while index < len(line):
            line[index] = eval(line[index])
            index = index + 1
        data[key]["SECONDARY"] = line
    elif line.startswith("TERTIARY:"):
        line = line.split(":")[1].split(" ")
        index = 0
        while index < len(line):
            line[index] = eval(line[index])
            index = index + 1
        data[key]["TERTIARY"] = line
    line = f.readline().strip()

f.close()

# palette smoothing, fills in intermediate colors for low-color palettes
for i in range(1, len(data) + 1):
    r = range(2, len(data[i]['PRIMARY']))
    r.reverse()
    for j in r:
        if data[i]["PRIMARY"][j] == data[i]["PRIMARY"][j-1]:
            data[i]["PRIMARY"][j-1] = (int((data[i]["PRIMARY"][j][0] + data[i]["PRIMARY"][j-2][0])/2),int((data[i]["PRIMARY"][j][1] + data[i]["PRIMARY"][j-2][1])/2),int((data[i]["PRIMARY"][j][2] + data[i]["PRIMARY"][j-2][2])/2),int((data[i]["PRIMARY"][j][3] + data[i]["PRIMARY"][j-2][3])/2))
        if data[i]["SECONDARY"][j] == data[i]["SECONDARY"][j-1]:
            data[i]["SECONDARY"][j-1] = (int((data[i]["SECONDARY"][j][0] + data[i]["SECONDARY"][j-2][0])/2),int((data[i]["SECONDARY"][j][1] + data[i]["SECONDARY"][j-2][1])/2),int((data[i]["SECONDARY"][j][2] + data[i]["SECONDARY"][j-2][2])/2),int((data[i]["SECONDARY"][j][3] + data[i]["SECONDARY"][j-2][3])/2))
        if data[i]["TERTIARY"][j] == data[i]["TERTIARY"][j-1]:
            data[i]["TERTIARY"][j-1] = (int((data[i]["TERTIARY"][j][0] + data[i]["TERTIARY"][j-2][0])/2),int((data[i]["TERTIARY"][j][1] + data[i]["TERTIARY"][j-2][1])/2),int((data[i]["TERTIARY"][j][2] + data[i]["TERTIARY"][j-2][2])/2),int((data[i]["TERTIARY"][j][3] + data[i]["TERTIARY"][j-2][3])/2))
    if data[i]["PRIMARY"][0] == data[i]["PRIMARY"][1]:
        data[i]["PRIMARY"][0] = (min(254,(data[i]["PRIMARY"][0][0] + (data[i]["PRIMARY"][1][0] - data[i]["PRIMARY"][2][0]))),
                                 min(254,(data[i]["PRIMARY"][0][1] + (data[i]["PRIMARY"][1][1] - data[i]["PRIMARY"][2][1]))),
                                 min(254,(data[i]["PRIMARY"][0][2] + (data[i]["PRIMARY"][1][2] - data[i]["PRIMARY"][2][2]))),
                                 min(254,(data[i]["PRIMARY"][0][3] + (data[i]["PRIMARY"][1][3] - data[i]["PRIMARY"][2][3]))))
    if data[i]["SECONDARY"][0] == data[i]["SECONDARY"][1]:
        data[i]["SECONDARY"][0] = (min(254,(data[i]["SECONDARY"][0][0] + (data[i]["SECONDARY"][1][0] - data[i]["SECONDARY"][2][0]))),
                                   min(254,(data[i]["SECONDARY"][0][1] + (data[i]["SECONDARY"][1][1] - data[i]["SECONDARY"][2][1]))),
                                   min(254,(data[i]["SECONDARY"][0][2] + (data[i]["SECONDARY"][1][2] - data[i]["SECONDARY"][2][2]))),
                                   min(254,(data[i]["SECONDARY"][0][3] + (data[i]["SECONDARY"][1][3] - data[i]["SECONDARY"][2][3]))))
    if data[i]["TERTIARY"][0] == data[i]["TERTIARY"][1]:
        data[i]["TERTIARY"][0] = (min(254,(data[i]["TERTIARY"][0][0] + (data[i]["TERTIARY"][1][0] - data[i]["TERTIARY"][2][0]))),
                                  min(254,(data[i]["TERTIARY"][0][1] + (data[i]["TERTIARY"][1][1] - data[i]["TERTIARY"][2][1]))),
                                  min(254,(data[i]["TERTIARY"][0][2] + (data[i]["TERTIARY"][1][2] - data[i]["TERTIARY"][2][2]))),
                                  min(254,(data[i]["TERTIARY"][0][3] + (data[i]["TERTIARY"][1][3] - data[i]["TERTIARY"][2][3]))))



for i in range(1, len(data) + 1):
    for j in range(1, len(data) + 1):
        if len(sys.argv) > 1:
            if not ((i == int(sys.argv[1]) or (j == int(sys.argv[1])))):
                continue
        p = Process(target=make_image, args=(i,j))
        p.start()

p.join()

print "Fusing complete after %f seconds." % (time.time() - start_time)
