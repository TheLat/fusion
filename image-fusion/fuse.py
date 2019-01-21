from PIL import Image

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
    elif line.startswith("FACE_USES:"):
        data[key]["BODY"] = line.split(":")[1] + ".png"
    elif line == "USE_MASK:YES":
        data[key]["MASK"] = True
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
print data


for i in range(1, len(data) + 1):
    for j in range(1, len(data) + 1):
        if i == j:
            Image.open("front/%s.png" % i).convert("RGBA").convert("P").save("out/%s-%s.png" % (i, j))
            continue
        im1 = Image.open("front/%s-face.png" % i).convert("RGBA")
        im2 = Image.open("front/%s-%s" % (j, data[i]['BODY'])).convert("RGBA")
        im3 = Image.open("front/%s-mask.png" % j).convert("RGBA")
        px1 = im1.load()
        px2 = im2.load()
        px3 = im3.load()
        for x in range(0,im2.size[0]):
            for y in range(0,im2.size[1]):
		for index in range(0, len(data[j]['PRIMARY'])):
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
                        xtarg = int(((x3-x4)/(x1-x2))*float(x - x1) + (x3))
                        ytarg = int(((y3-y4)/(y1-y2))*float(y - y1) + (y3))
                        if xtarg > 0 and ytarg > 0 and xtarg < im1.size[1] and ytarg < im1.size[0]:
                            if data[i]["MASK"]:
                                if px3[(x,y)] != (0, 0, 0, 255):
                                    if px1[(xtarg,ytarg)][3] == 255:
                                        px2[(x,y)] = px1[(xtarg,ytarg)]
                            else:
                                if px1[(xtarg,ytarg)][3] == 255:
                                    px2[(x,y)] = px1[(xtarg,ytarg)]
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
                        xtarg = int(((x3-x4)/(x1-x2))*float(x - x1) + (x3))
                        ytarg = int(((y3-y4)/(y1-y2))*float(y - y1) + (y3))
                        if xtarg > 0 and ytarg > 0 and xtarg < im1.size[1] and ytarg < im1.size[0]:
                            if data[i]["MASK"]:
                                if px3[(x,y)] != (0, 0, 0, 255):
                                    if px1[(xtarg,ytarg)][3] == 255:
                                        px2[(x,y)] = px1[(xtarg,ytarg)]
                            else:
                                if px1[(xtarg,ytarg)][3] == 255:
                                    px2[(x,y)] = px1[(xtarg,ytarg)]
	if data[i]["HFLIP"]:
            im2 = im2.transpose(Image.FLIP_LEFT_RIGHT)
        im2.convert("P").save("out/%s-%s.png" % (i, j))
