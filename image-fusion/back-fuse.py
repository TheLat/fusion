import time
import sys
import math
from PIL import Image
from multiprocessing import Process

def transform(x11, x21, y11, y21, l1, l2, thetas, thetac, x, y):
    xtarg = x - x21
    ytarg = y - y21
    xtarg *= l1 / l2
    ytarg *= l1 / l2
    xh = (thetac * xtarg) - (thetas * ytarg)
    yh = (thetas * xtarg) + (thetac * ytarg)
    xtarg = xh + x11
    ytarg = yh + y11
    return (xtarg, ytarg)

def get_subsampling_pixel(px, x1, y1, x2, y2):
    if (abs(x2 - x1) < 2.0) and (abs(y2 - y1) < 2.0):
        return get_pixel(px, (x1 + x2) / 2.0, (y1 + y2) / 2.0)
    dx = 1.0 / float(int(abs(x2 - x1) + 1.0))
    dy = 1.0 / float(int(abs(y2 - y1) + 1.0))
    total = 0.0
    colors = [0.0, 0.0, 0.0, 0.0]
    xt = 0.0
    yt = 0.0
    while yt < 1.0:
        xt = 0.0
        while xt < 1.0:
            p = px[(x1 + (xt*(x2 - x1)), y1 + (yt*(y2 - y1)))]
            if p[3] == 255:
                for i in range(0,4):
                    colors[i] = colors[i] + float(p[i])
                total = total + 1.0
            xt += dx
        yt += dy
    if total > 0.0:
        for i in range(0, 4):
            colors[i] = colors[i]/total
    return (int(colors[0]),int(colors[1]),int(colors[2]),int(colors[3]))

def get_pixel(px, x, y):
    x1 = int(x)
    x2 = x1 + 1
    xt = x - (float(int(x)))
    y1 = int(y)
    y2 = y1 + 1
    yt = y - (float(int(y)))
    try:
        p1 = px[(x1,y1)]
    except:
        p1 = (0,0,0,0)
    try:
        p2 = px[(x2,y1)]
    except:
        p2 = (0,0,0,0)
    try:
        p3 = px[(x1,y2)]
    except:
        p3 = (0,0,0,0)
    try:
        p4 = px[(x2,y2)]
    except:
        p4 = (0,0,0,0)
    scores = [(1.0 - xt) * (1.0 - yt), (xt)*(1.0 - yt), (1.0 - xt)*(yt), (xt)*(yt)]
    points = [p1, p2, p3, p4]
    pointmap = {}
    for i in range(0,4):
        if points[i] not in pointmap.keys():
            pointmap[points[i]] = scores[i]
        else:
            pointmap[points[i]] += scores[i]
    pout = points[0]
    pscore = scores[0]
    for i in range(1,4):
        if pointmap[points[i]] > pscore:
            pout = points[i]
            pscore = pointmap[points[i]]
    return pout

def make_image(i, j):
    if i == j:
        Image.open("back/%s-original.png" % i).convert("RGBA").convert("P").save("out/%s-%s-back.png" % (i, j))
        return
    imt = Image.open("back/%s-%s.png" % (j,data[i]["FACE_USES"])).convert("RGBA")
    pxt = imt.load()
    im2 = Image.new("RGBA", (160,160), (0,0,0,0))
    px2 = im2.load()
    for x in range(0,imt.size[0]):
        for y in range(0,imt.size[1]):
            if data[j]["VFLIP"]:
                px2[(x, y)] = pxt[(x, y)]
            else:
                px2[(x,y+80)] = pxt[(x,y)]
    imt = Image.open("back/%s-face.png" % (i)).convert("RGBA")
    pxt = imt.load()
    im1 = Image.new("RGBA", (160,160), (0,0,0,0))
    px1 = im1.load()
    for x in range(0,imt.size[0]):
        for y in range(0,imt.size[1]):
            px1[(x,y+80)] = pxt[(x,y)]
    imt = Image.open("back/%s-mask.png" % (j)).convert("RGBA")
    pxt = imt.load()
    im3 = Image.new("RGBA", (160,160), (0,0,0,0))
    px3 = im3.load()
    for x in range(0,imt.size[0]):
        for y in range(0,imt.size[1]):
            if data[j]["VFLIP"]:
                px3[(x,y)] = pxt[(x,y)]
            else:
                px3[(x,y+80)] = pxt[(x,y)]
    imt = Image.open("back/%s-deepmask.png" % (j)).convert("RGBA")
    pxt = imt.load()
    im4 = Image.new("RGBA", (160,160), (0,0,0,0))
    px4 = im4.load()
    for x in range(0,imt.size[0]):
        for y in range(0,imt.size[1]):
            if data[j]["VFLIP"]:
                px4[(x,y)] = pxt[(x,y)]
            else:
                px4[(x,y+80)] = pxt[(x,y)]
    imt = Image.open("back/%s-crest.png" % (i)).convert("RGBA")
    pxt = imt.load()
    im5 = Image.new("RGBA", (160,160), (0,0,0,0))
    px5 = im5.load()
    for x in range(0,imt.size[0]):
        for y in range(0,imt.size[1]):
            px5[(x,y+80)] = pxt[(x,y)]
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
    if "EYEBOUNDS" in data[i].keys():
        for e in data[j]["EYE"]:
            x11 = float(data[i]["EYEBOUNDS"]["X1"])
            x12 = float(data[i]["EYEBOUNDS"]["X2"])
            dx1 = (x12 - x11)
            x21 = float(e["X1"])
            x22 = float(e["X2"])
            dx2 = (x22 - x21)
            y11 = float(data[i]["EYEBOUNDS"]["Y1"] + 80)
            y12 = float(data[i]["EYEBOUNDS"]["Y2"] + 80)
            dy1 = (y12 - y11)
            y21 = float(e["Y1"] + 80)
            y22 = float(e["Y2"] + 80)
            if data[j]["VFLIP"]:
                y21 = float(e["Y1"])
                y22 = float(e["Y2"])
            dy2 = (y22 - y21)
            l1 = math.sqrt(dx1*dx1 + dy1*dy1)
            l2 = math.sqrt(dx2*dx2 + dy2*dy2)
            theta = math.acos(max(min(((dx1*dx2) + (dy1*dy2))/(l1*l2),1.0),-1.0))
            if (dx1*dy2 - dx2*dy1) > 0:
                theta *= -1.0
            thetas = math.sin(theta)
            thetac = math.cos(theta)
            for x in range(0,im2.size[0]):
                for y in range(0,im2.size[1]):
                    if px3[(x, y)] == (0, 0, 0, 255) and px2[(x,y)][3] != 0:
                        continue
                    xtarg1, ytarg1 = transform(x11, x21, y11, y21, l1, l2, thetas, thetac, float(x) - 0.5, float(y) - 0.5)
                    xtarg2, ytarg2 = transform(x11, x21, y11, y21, l1, l2, thetas, thetac, float(x) + 0.5, float(y) + 0.5)
                    if xtarg1 < 0:
                        continue
                    if xtarg2 < 0:
                        continue
                    if ytarg1 < 0:
                        continue
                    if ytarg2 < 0:
                        continue
                    if xtarg1 > im1.size[0]:
                        continue
                    if xtarg2 > im1.size[0]:
                        continue
                    if ytarg1 > im1.size[1]:
                        continue
                    if ytarg2 > im1.size[1]:
                        continue
                    try:
                        px = get_subsampling_pixel(px1, xtarg1, ytarg1, xtarg2, ytarg2)
                        if px[3] == 0:
                            continue
                        px2[(x,y)] = px
                    except:
                        pass
    if "CRESTBOUNDS" in data[i].keys():
        for b in data[j]['CREST']:
            x1 = float(b['X'])
            x2 = float(data[i]['CRESTBOUNDS']['X'])
            y1 = float(b['Y'] + 80)
            y2 = float(data[i]['CRESTBOUNDS']['Y'] + 80)
            if data[j]["VFLIP"]:
                y1 = float(b['Y'])
            w1 = float(b['WIDTH'])
            w2 = float(data[i]['CRESTBOUNDS']['WIDTH'])
            for x in range(0,im2.size[0]):
                for y in range(0,im2.size[1]):
                    if px4[(x, y)] == (0, 0, 0, 255) and px2[(x,y)][3] != 0:
                        continue
                    xtarg1 = ((w2/w1)*float((x - 0.5) - x1) + (x2))
                    xtarg2 = ((w2/w1)*float((x + 0.5) - x1) + (x2))
                    ytarg1 = (abs((w2/w1))*float((y - 0.5) - y1) + (y2))
                    ytarg2 = (abs((w2/w1))*float((y + 0.5) - y1) + (y2))
                    if xtarg1 < 0:
                        continue
                    if xtarg2 < 0:
                        continue
                    if ytarg1 < 0:
                        continue
                    if ytarg2 < 0:
                        continue
                    if xtarg1 > im1.size[0]:
                        continue
                    if xtarg2 > im1.size[0]:
                        continue
                    if ytarg1 > im1.size[1]:
                        continue
                    if ytarg2 > im1.size[1]:
                        continue
                    try:
                        px = get_subsampling_pixel(px5, xtarg1, ytarg1, xtarg2, ytarg2)
                        if px[3] == 0:
                            continue
                        px2[(x,y)] = px
                    except:
                        pass
    if data[j]["VFLIP"]:
        im2 = im2.transpose(Image.FLIP_TOP_BOTTOM)
        px2 = im2.load()
    xmin = 80
    ymin = 160
    for x in range(0,im2.size[0]):
        for y in range(0,im2.size[1]):
            if px2[(x,y)][3] == 255:
                if x > xmin:
                    xmin = x
                if y < ymin:
                    ymin = y
    if ymin > 80:
        ymin = 80
    if xmin > 160 - ymin:
        ymin = 160 - xmin
    if 160 - ymin > xmin:
        xmin = 160 - ymin
    im2 = im2.crop((0,ymin,xmin,160))
    im2.convert("P").save("out/%s-%s-back.png" % (i, j))
    if im2.size[0] != im2.size[1]:
        print "%s-%s" % (i,j)


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
        data[key]["VFLIP"] = False
    elif line.startswith("VFLIP:YES"):
        data[key]["VFLIP"] = True
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
    elif line.startswith("EYEBOUNDS:"):
        line = line.split(":")[1]
        data[key]["EYEBOUNDS"] = {}
        line = line.replace(",", " ").replace("(", " ").replace(")", " ").replace("  ", " ").strip().split(" ")
        data[key]["EYEBOUNDS"]["X1"] = int(line[0])
        data[key]["EYEBOUNDS"]["Y1"] = int(line[1])
        data[key]["EYEBOUNDS"]["X2"] = int(line[2])
        data[key]["EYEBOUNDS"]["Y2"] = int(line[3])
    elif line.startswith("CRESTBOUNDS:"):
        line = line.split(":")[1]
        line = line.replace(",", " ").replace("(", " ").replace(")", " ").replace("  ", " ").strip().split(" ")
        data[key]["CRESTBOUNDS"] = {}
        data[key]["CRESTBOUNDS"]["X"] = int(line[0])
        data[key]["CRESTBOUNDS"]["Y"] = int(line[1])
        data[key]["CRESTBOUNDS"]["WIDTH"] = int(line[2])
    elif line.startswith("EYE:"):
        line = line.split(":")[1]
        data[key]["EYE"] = []
        line = line.split(" ")
        index = 0
        for l in line:
            data[key]["EYE"].append({})
            l = l.replace(",", " ").replace("(", " ").replace(")", " ").replace("  ", " ").strip().split(" ")
            data[key]["EYE"][index]["X1"]=int(l[0])
            data[key]["EYE"][index]["Y1"]=int(l[1])
            data[key]["EYE"][index]["X2"]=int(l[2])
            data[key]["EYE"][index]["Y2"]=int(l[3])
            index = index + 1
    elif line.startswith("SOCKET:"):
        line = line.split(":")[1]
        data[key]["SOCKET"] = []
        line = line.split(" ")
        index = 0
        for l in line:
            data[key]["SOCKET"].append({})
            l = l.replace(",", " ").replace("(", " ").replace(")", " ").replace("  ", " ").strip().split(" ")
            data[key]["SOCKET"][index]["X"]=int(l[0])
            data[key]["SOCKET"][index]["Y"]=int(l[1])
            data[key]["SOCKET"][index]["WIDTH"]=int(l[2])
            index = index + 1
    elif line.startswith("CREST:"):
        line = line.split(":")[1]
        data[key]["CREST"] = []
        line = line.split(" ")
        index = 0
        for l in line:
            data[key]["CREST"].append({})
            l = l.replace(",", " ").replace("(", " ").replace(")", " ").replace("  ", " ").strip().split(" ")
            data[key]["CREST"][index]["X"]=int(l[0])
            data[key]["CREST"][index]["Y"]=int(l[1])
            data[key]["CREST"][index]["WIDTH"]=int(l[2])
            index = index + 1
    elif line.startswith("FACE_USES:"):
        line = line.split(":")[1]
        data[key]["FACE_USES"] = line
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
