import time
import sys
from PIL import Image

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
    p1f = p1
    p2f = p2
    p3f = p3
    p4f = p4
    if p1[3] == 0:
        if p2[3] == 0 and p3[3] == 0:
            p1f = (p4[0], p4[1], p4[2], p4[3]/4)
        elif p2[3] == 0:
            p1f = (p3[0], p3[1], p3[2], p3[3]/2)
        elif p3[3] == 0:
            p1f = (p2[0], p2[1], p2[2], p2[3]/2)
        else:
            p1f = ((p2[0] + p3[0]) / 2,
                  (p2[1] + p3[1]) / 2,
                  (p2[2] + p3[2]) / 2,
                  (p2[3] + p3[3]) / 4)
    if p2[3] == 0:
        if p1[3] == 0 and p4[3] == 0:
            p2f = (p3[0], p3[1], p3[2], p3[3]/4)
        elif p1[3] == 0:
            p2f = (p4[0], p4[1], p4[2], p4[3]/2)
        elif p4[3] == 0:
            p2f = (p1[0], p1[1], p1[2], p1[3]/2)
        else:
            p2f = ((p1[0] + p4[0]) / 2,
                  (p1[1] + p4[1]) / 2,
                  (p1[2] + p4[2]) / 2,
                  (p1[3] + p4[3]) / 4)
    if p3[3] == 0:
        if p1[3] == 0 and p4[3] == 0:
            p3f = (p2[0], p2[1], p2[2], p2[3]/4)
        elif p1[3] == 0:
            p3f = (p4[0], p4[1], p4[2], p4[3]/2)
        elif p4[3] == 0:
            p3f = (p1[0], p1[1], p1[2], p1[3]/2)
        else:
            p3f = ((p1[0] + p4[0]) / 2,
                  (p1[1] + p4[1]) / 2,
                  (p1[2] + p4[2]) / 2,
                  (p1[3] + p4[3]) / 4)
    if p4[3] == 0:
        if p2[3] == 0 and p3[3] == 0:
            p4f = (p1[0], p1[1], p1[2], p1[3]/4)
        elif p2[3] == 0:
            p4f = (p3[0], p3[1], p3[2], p3[3]/2)
        elif p3[3] == 0:
            p4f = (p2[0], p2[1], p2[2], p2[3]/2)
        else:
            p4f = ((p2[0] + p3[0]) / 2,
                  (p2[1] + p3[1]) / 2,
                  (p2[2] + p3[2]) / 2,
                  (p2[3] + p3[3]) / 4)
    p1 = (int(float(p1f[0]) + xt*float(p2f[0] - p1f[0])),
          int(float(p1f[1]) + xt*float(p2f[1] - p1f[1])),
          int(float(p1f[2]) + xt*float(p2f[2] - p1f[2])),
          int(float(p1f[3]) + xt*float(p2f[3] - p1f[3])))
    p3 = (int(float(p3f[0]) + xt*float(p4f[0] - p3f[0])),
          int(float(p3f[1]) + xt*float(p4f[1] - p3f[1])),
          int(float(p3f[2]) + xt*float(p4f[2] - p3f[2])),
          int(float(p3f[3]) + xt*float(p4f[3] - p3f[3])))
    p1 = (int(float(p1[0]) + yt*float(p3[0] - p1[0])),
          int(float(p1[1]) + yt*float(p3[1] - p1[1])),
          int(float(p1[2]) + yt*float(p3[2] - p1[2])),
          int(float(p1[3]) + xt*float(p3[3] - p1[3])))
    return p1


start_time = time.time()
data = {}
key = ""
f = open("fdata.txt")
line = f.readline().strip()
while line:
    if line.startswith("NUMBER:"):
        line = line.split(":")[1]
        key = int(line)
        data[key] = {}
        data[key]["MASK"] = False
        data[key]["HFLIP"] = False
        data[key]["VFLIP"] = False
    elif line.startswith("FACE_USES:"):
        data[key]["BODY"] = line.split(":")[1] + ".png"
    elif line == "USE_MASK:YES":
        data[key]["MASK"] = True
    elif line == "VFLIP:YES":
        data[key]["VFLIP"] = True
    elif line == "HFLIP:YES":
        data[key]["HFLIP"] = True
    elif line.startswith("FACEBOUNDS:"):
        line = line.split(":")[1]
        line = line.replace(",", " ").replace("(", " ").replace(")", " ").replace("  ", " ").strip().split(" ")
        data[key]["FACEBOUNDS"] = {}
        data[key]["FACEBOUNDS"]["XMIN"] = int(line[0])
        data[key]["FACEBOUNDS"]["YMIN"] = int(line[1])
        data[key]["FACEBOUNDS"]["XMAX"] = int(line[2])
        data[key]["FACEBOUNDS"]["YMAX"] = int(line[3])
    elif line.startswith("JAWBOUNDS:"):
        line = line.split(":")[1]
        line = line.replace(",", " ").replace("(", " ").replace(")", " ").replace("  ", " ").strip().split(" ")
        data[key]["JAWBOUNDS"] = {}
        data[key]["JAWBOUNDS"]["X1"] = int(line[0])
        data[key]["JAWBOUNDS"]["Y1"] = int(line[1])
        data[key]["JAWBOUNDS"]["X2"] = int(line[2])
        data[key]["JAWBOUNDS"]["Y2"] = int(line[3])
    elif line.startswith("HEADBOUNDS:"):
        line = line.split(":")[1]
        line = line.replace(",", " ").replace("(", " ").replace(")", " ").replace("  ", " ").strip().split(" ")
        data[key]["HEADBOUNDS"] = {}
        data[key]["HEADBOUNDS"]["X"] = int(line[0])
        data[key]["HEADBOUNDS"]["Y"] = int(line[1])
        data[key]["HEADBOUNDS"]["WIDTH"] = int(line[2])
    elif line.startswith("BOUNDS:"):
        line = line.split(":")[1]
        data[key]["BOUNDS"] = []
        line = line.split(" ")
        index = 0
        for l in line:
            data[key]["BOUNDS"].append({})
            l = l.replace(",", " ").replace("(", " ").replace(")", " ").replace("  ", " ").strip().split(" ")
            data[key]["BOUNDS"][index]["XMIN"]=int(l[0])
            data[key]["BOUNDS"][index]["YMIN"]=int(l[1])
            data[key]["BOUNDS"][index]["XMAX"]=int(l[2])
            data[key]["BOUNDS"][index]["YMAX"]=int(l[3])
            index = index + 1
    elif line.startswith("JAW:"):
        line = line.split(":")[1]
        data[key]["JAW"] = []
        line = line.split(" ")
        index = 0
        for l in line:
            data[key]["JAW"].append({})
            l = l.replace(",", " ").replace("(", " ").replace(")", " ").replace("  ", " ").strip().split(" ")
            data[key]["JAW"][index]["X1"]=int(l[0])
            data[key]["JAW"][index]["Y1"]=int(l[1])
            data[key]["JAW"][index]["X2"]=int(l[2])
            data[key]["JAW"][index]["Y2"]=int(l[3])
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
        if i == j:
            if data[i]["HFLIP"]:
                Image.open("front/%s.png" % i).convert("RGBA").transpose(Image.FLIP_LEFT_RIGHT).convert("P").save("out/%s-%s.png" % (i, j))
            if data[i]["VFLIP"]:
                Image.open("front/%s.png" % i).convert("RGBA").transpose(Image.FLIP_TOP_BOTTOM).convert("P").save("out/%s-%s.png" % (i, j))
            else:
                Image.open("front/%s.png" % i).convert("RGBA").convert("P").save("out/%s-%s.png" % (i, j))
            continue
        im1 = Image.open("front/%s-face.png" % i).convert("RGBA")
        im2 = Image.open("front/%s-%s" % (j, data[i]['BODY'])).convert("RGBA")
        im3 = Image.open("front/%s-mask.png" % j).convert("RGBA")
        im4 = Image.open("front/%s-deepmask.png" % j).convert("RGBA")
        px1 = im1.load()
        px2 = im2.load()
        px3 = im3.load()
        px4 = im4.load()
        for x in range(0,im2.size[0]):
            for y in range(0,im2.size[1]):
                r = range(0, len(data[j]['PRIMARY']))
                r.reverse()
                for index in r:
                    if px2[(x,y)] == data[j]['PRIMARY'][index]:
                        px2[(x,y)] = data[i]['PRIMARY'][index]
                for index in range(0, len(data[j]['SECONDARY'])):
                    if px2[(x,y)] == data[j]['SECONDARY'][index]:
                        px2[(x,y)] = data[i]['SECONDARY'][index]
                for index in range(0, len(data[j]['TERTIARY'])):
                    if px2[(x,y)] == data[j]['TERTIARY'][index]:
                        px2[(x,y)] = data[i]['TERTIARY'][index]
        if "FACEBOUNDS" in data[i].keys():
            for b in data[j]['BOUNDS']:
                x1 = float(b['XMIN'])
                x2 = float(b['XMAX'])
                x3 = float(data[i]['FACEBOUNDS']['XMIN'])
                x4 = float(data[i]['FACEBOUNDS']['XMAX'])
                y1 = float(b['YMIN'])
                y2 = float(b['YMAX'])
                y3 = float(data[i]['FACEBOUNDS']['YMIN'])
                y4 = float(data[i]['FACEBOUNDS']['YMAX'])
                for x in range(0,im2.size[0]):
                    for y in range(0,im2.size[1]):
                        xtarg = (((x3-x4)/(x1-x2))*float(x - x1) + (x3))
                        ytarg = (((y3-y4)/(y1-y2))*float(y - y1) + (y3))
                        if xtarg > 0 and ytarg > 0 and xtarg < im1.size[1] and ytarg < im1.size[0]:
                            if data[i]["MASK"]:
                                if px3[(x,y)] != (0, 0, 0, 255):
                                    if px4[(x,y)] != (0, 0, 0, 255):
                                        if px1[(xtarg,ytarg)][3] == 255:
                                            px = get_pixel(px1,xtarg,ytarg)
                                            alpha = float(px[3])/255.0
                                            pxb = px2[(x,y)]
                                            if pxb[3] <= px[3]:
                                                px2[(x, y)] = px
                                            else:
                                                px2[(x, y)] = (int(float(pxb[0]) + alpha*(float(px[0] - pxb[0]))),
                                                               int(float(pxb[1]) + alpha*(float(px[1] - pxb[1]))),
                                                               int(float(pxb[2]) + alpha*(float(px[2] - pxb[2]))),
                                                               int(max(px[3], pxb[3])))
                            else:
                                if px4[(x,y)] != (0, 0, 0, 255):
                                    if px1[(xtarg,ytarg)][3] == 255:
                                        px = get_pixel(px1,xtarg,ytarg)
                                        alpha = float(px[3])/255.0
                                        pxb = px2[(x,y)]
                                        if pxb[3] <= px[3]:
                                            px2[(x, y)] = px
                                        else:
                                            px2[(x, y)] = (int(float(pxb[0]) + alpha * (float(px[0] - pxb[0]))),
                                                           int(float(pxb[1]) + alpha * (float(px[1] - pxb[1]))),
                                                           int(float(pxb[2]) + alpha * (float(px[2] - pxb[2]))),
                                                           int(max(px[3], pxb[3])))
        if "JAWBOUNDS" in data[i].keys():
            for b in data[j]['JAW']:
                x1 = float(b['X1'])
                x2 = float(b['X2'])
                x3 = float(data[i]['JAWBOUNDS']['X1'])
                x4 = float(data[i]['JAWBOUNDS']['X2'])
                y1 = float(b['Y1'])
                y2 = float(b['Y2'])
                y3 = float(data[i]['JAWBOUNDS']['Y1'])
                y4 = float(data[i]['JAWBOUNDS']['Y2'])
                for x in range(0,im2.size[0]):
                    for y in range(0,im2.size[1]):
                        xtarg = (((x3-x4)/(x1-x2))*float(x - x1) + (x3))
                        ytarg = (((y3-y4)/(y1-y2))*float(y - y1) + (y3))
                        if xtarg > 0 and ytarg > 0 and xtarg < im1.size[1] and ytarg < im1.size[0]:
                            if data[i]["MASK"]:
                                if px3[(x,y)] != (0, 0, 0, 255):
                                    if px4[(x,y)] != (0, 0, 0, 255):
                                        if px1[(xtarg,ytarg)][3] == 255:
                                            px = get_pixel(px1,xtarg,ytarg)
                                            alpha = float(px[3])/255.0
                                            pxb = px2[(x,y)]
                                            if pxb[3] <= px[3]:
                                                px2[(x, y)] = px
                                            else:
                                                px2[(x, y)] = (int(float(pxb[0]) + alpha*(float(px[0] - pxb[0]))),
                                                               int(float(pxb[1]) + alpha*(float(px[1] - pxb[1]))),
                                                               int(float(pxb[2]) + alpha*(float(px[2] - pxb[2]))),
                                                               int(max(px[3], pxb[3])))
                            else:
                                if px4[(x,y)] != (0, 0, 0, 255):
                                    if px1[(xtarg,ytarg)][3] == 255:
                                        px = get_pixel(px1,xtarg,ytarg)
                                        alpha = float(px[3])/255.0
                                        pxb = px2[(x,y)]
                                        if pxb[3] <= px[3]:
                                            px2[(x, y)] = px
                                        else:
                                            px2[(x, y)] = (int(float(pxb[0]) + alpha * (float(px[0] - pxb[0]))),
                                                           int(float(pxb[1]) + alpha * (float(px[1] - pxb[1]))),
                                                           int(float(pxb[2]) + alpha * (float(px[2] - pxb[2]))),
                                                           int(max(px[3], pxb[3])))
        if "HEADBOUNDS" in data[i].keys():
            for b in data[j]['SOCKET']:
                x1 = float(b['X'])
                x2 = float(data[i]['HEADBOUNDS']['X'])
                y1 = float(b['Y'])
                y2 = float(data[i]['HEADBOUNDS']['Y'])
                w1 = float(b['WIDTH'])
                w2 = float(data[i]['HEADBOUNDS']['WIDTH'])
                for x in range(0,im2.size[0]):
                    for y in range(0,im2.size[1]):
                        xtarg = ((w2/w1)*float(x - x1) + (x2))
                        ytarg = (abs((w2/w1))*float(y - y1) + (y2))
                        if xtarg > 0 and ytarg > 0 and xtarg < im1.size[1] and ytarg < im1.size[0]:
                            if data[i]["MASK"]:
                                if px3[(x,y)] != (0, 0, 0, 255):
                                    if px4[(x,y)] != (0, 0, 0, 255):
                                        if px1[(xtarg,ytarg)][3] == 255:
                                            px = get_pixel(px1,xtarg,ytarg)
                                            alpha = float(px[3])/255.0
                                            pxb = px2[(x,y)]
                                            if pxb[3] <= px[3]:
                                                px2[(x, y)] = px
                                            else:
                                                px2[(x, y)] = (int(float(pxb[0]) + alpha*(float(px[0] - pxb[0]))),
                                                               int(float(pxb[1]) + alpha*(float(px[1] - pxb[1]))),
                                                               int(float(pxb[2]) + alpha*(float(px[2] - pxb[2]))),
                                                               int(max(px[3], pxb[3])))
                            else:
                                if px4[(x,y)] != (0, 0, 0, 255):
                                    if px1[(xtarg,ytarg)][3] == 255:
                                        px = get_pixel(px1,xtarg,ytarg)
                                        alpha = float(px[3])/255.0
                                        pxb = px2[(x,y)]
                                        if pxb[3] <= px[3]:
                                            px2[(x, y)] = px
                                        else:
                                            px2[(x, y)] = (int(float(pxb[0]) + alpha * (float(px[0] - pxb[0]))),
                                                           int(float(pxb[1]) + alpha * (float(px[1] - pxb[1]))),
                                                           int(float(pxb[2]) + alpha * (float(px[2] - pxb[2]))),
                                                           int(max(px[3], pxb[3])))
	if data[i]["HFLIP"]:
            im2 = im2.transpose(Image.FLIP_LEFT_RIGHT)
        if data[j]["VFLIP"]:
            im2 = im2.transpose(Image.FLIP_TOP_BOTTOM)
        im2.convert("P").save("out/%s-%s.png" % (i, j))

print "Fusing complete after %f seconds." % (time.time() - start_time)
