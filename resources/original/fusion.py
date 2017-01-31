import time
start = time.time()
i = 0
all = {}
f = open("pokemon.dat", "rt")
s = f.readline()
while s != "":
    s = s.replace("\n", "")
    if s.startswith("NUMBER: "):
        i = s.split(" ")[1]
        if i not in all.keys():
            all[i] = {}
    if s.startswith("SWITCH: "):
        all[i]["SWITCH"] = s.split(" ")[1]
    if s.startswith("NAME: "):
        all[i]["NAME"] = s.split(" ")[1]
    if s.startswith("SURNAME: "):
        all[i]["SURNAME"] = s.split(" ")[1]
    if s.startswith("TYPE1: "):
        all[i]["TYPE1"] = s.split(" ")[1]
    if s.startswith("TYPE2: "):
        all[i]["TYPE2"] = s.split(" ")[1]
    if s.startswith("DEX1: "):
        all[i]["DEX1"] = s[6:]
    if s.startswith("DEX2: "):
        all[i]["DEX2"] = s[6:]
    if s.startswith("EXP_YIELD: "):
        all[i]["EXP_YIELD"] = int(s.split(" ")[1])
    if s.startswith("CATCHRATE: "):
        all[i]["CATCHRATE"] = int(s.split(" ")[1])
    if s.startswith("HP: "):
        all[i]["HP"] = s.split(" ")[1]
    if s.startswith("ATTACK: "):
        all[i]["ATTACK"] = s.split(" ")[1]
    if s.startswith("DEFENSE: "):
        all[i]["DEFENSE"] = s.split(" ")[1]
    if s.startswith("SPECIAL: "):
        all[i]["SPECIAL"] = s.split(" ")[1]
    if s.startswith("SPEED: "):
        all[i]["SPEED"] = s.split(" ")[1]
    if s.startswith("MOVES:"):
        all[i]["MOVES"] = []
        s = f.readline().replace("\n", "")
        while ":" not in s:
            all[i]["MOVES"].append(s)
            s = f.readline().replace("\n", "")
    if s.startswith("EVOLUTION:"):
        all[i]["EVOLUTION"] = []
        s = f.readline().replace("\n", "")
        while not s.startswith("TM:"):
            all[i]["EVOLUTION"].append(s)
            s = f.readline().replace("\n", "")
    if s.startswith("TM: "):
        all[i]["TM"] = {}
        s = s.split(" ")
        x = 1
        while x < len(s):
            all[i]["TM"][int(s[x])] = True
            x = x + 1
    elif s.startswith("HM: "):
        all[i]["HM"] = {}
        s = s.split(" ")
        x = 1
        while x < len(s):
            all[i]["HM"][int(s[x])] = True
            x = x + 1
    s = f.readline()

f.close()

f = open("../data/mon.dat", "wt")
x = 1
y = 1
while x <= len(all):
    y = 1
    while y <= len(all):
        n = (all[str(x)]["NAME"] + all[str(y)]["SURNAME"]).replace("EEEE", "EE")
        if n == "MUUK":
            n = "MUK"
        if n == "EEVVEE":
            n = "EEVEE"
        if n == "MEEW":
            n = "MEW"
        n = n.replace("RAPE", "RILLA")
        print "Fusing " + (all[str(x)]["NAME"] + all[str(x)]["SURNAME"]).replace("EEEE", "EE") + " and " + (all[str(y)]["NAME"] + all[str(y)]["SURNAME"]).replace("EEEE", "EE") + " into " + n
        f.write("NUMBER: %s-%s" % (x, y))
        f.write("\nNAME: %s" % n)
        f.write("\nTYPE1: %s" % all[str(x)]["TYPE1"])
        if all[str(x)]["TYPE1"] != all[str(y)]["TYPE1"]:
            if "SWITCH" in all[str(y)].keys():
                if all[str(x)]["TYPE1"] != all[str(y)]["TYPE2"]:
                    f.write("\nTYPE2: %s" % all[str(y)]["TYPE2"])
                else:
                    f.write("\nTYPE2: %s" % all[str(y)]["TYPE1"])
            else:
                f.write("\nTYPE2: %s" % all[str(y)]["TYPE1"])
        elif "TYPE2" in all[str(y)].keys():
            f.write("\nTYPE2: %s" % all[str(y)]["TYPE2"])
        f.write("\nEXP_YIELD: %s" % ((all[str(x)]["EXP_YIELD"] + all[str(y)]["EXP_YIELD"])/2))
        f.write("\nCATCHRATE: %s" % ((all[str(x)]["CATCHRATE"] + all[str(y)]["CATCHRATE"])/2))
        f.write("\nDEX: %s" % (all[str(x)]["DEX1"] + " " + all[str(y)]["DEX2"]))
        HP = str((int(all[str(x)]["HP"]) + int(all[str(y)]["HP"]))/2)
        f.write("\nHP: %s" % HP)
        ATTACK = str((int(all[str(x)]["ATTACK"]) + int(all[str(y)]["ATTACK"]))/2)
        f.write("\nATTACK: %s" % ATTACK)
        DEFENSE = str((int(all[str(x)]["DEFENSE"]) + int(all[str(y)]["DEFENSE"]))/2)
        f.write("\nDEFENSE: %s" % DEFENSE)
        SPECIAL = str((int(all[str(x)]["SPECIAL"]) + int(all[str(y)]["SPECIAL"]))/2)
        f.write("\nSPECIAL: %s" % SPECIAL)
        SPEED = str((int(all[str(x)]["SPEED"]) + int(all[str(y)]["SPEED"]))/2)
        f.write("\nSPEED: %s" % SPEED)
        f.write("\nMOVES:")
        z = 0
        m = []
        while z < len(all[str(x)]["MOVES"]) or z < len(all[str(y)]["MOVES"]):
            if z < len(all[str(x)]["MOVES"]):
                if z % 2 == 1:
                    m.append((int(all[str(x)]["MOVES"][z].split(" ")[0]), all[str(x)]["MOVES"][z]))
            if z < len(all[str(y)]["MOVES"]):
                if z % 2 == 0:
                    m.append((int(all[str(y)]["MOVES"][z].split(" ")[0]), all[str(y)]["MOVES"][z]))
            z = z + 1
        m.sort(key=lambda tup: tup[0])
        for move in m:
            f.write("\n%s" % move[1])
        ev = []
        if x != y:
            z = 0
            while "EVOLUTION" in all[str(x)].keys() and z < len(all[str(x)]["EVOLUTION"]):
                s = all[str(x)]["EVOLUTION"][z]
                s = s.split(":")
                ev.append("\n%s-%s:%s" % (s[0], y, s[1]))
                z = z + 1
            z = 0
            while "EVOLUTION" in all[str(y)].keys() and z < len(all[str(y)]["EVOLUTION"]):
                s = all[str(y)]["EVOLUTION"][z]
                s = s.split(":")
                ev.append("\n%s-%s:%s" % (x, s[0], s[1]))
                z = z + 1
        else:
            z = 0
            while "EVOLUTION" in all[str(x)].keys() and z < len(all[str(x)]["EVOLUTION"]):
                s = all[str(x)]["EVOLUTION"][z]
                s = s.split(":")
                ev.append("\n%s-%s:%s" % (s[0], s[0], s[1]))
                z = z + 1
        ll = 100
        ev2 = []
        con = {}
        con["NOTHING"] = True
        for e in ev:
            if "LEVEL" in e:
                ll = min(int(e.split(" ")[1]), ll)
        run = False
        for e in ev:
            if not run:
                f.write("\nEVOLUTION:")
                run = True
            if "LEVEL" in e:
                if int(e.split(" ")[1]) == ll:
                    if e.split(":")[1] not in con.keys():
                        f.write(e)
                        con[e.split(":")[1]] = True
            else:
                if "133" in e.split(":")[0] and ("WATER" in e.split(":")[1] or "THUNDER" in e.split(":")[1] or "FIRE" in e.split(":")[1]):
                    continue
                if e.split(":")[1] not in con.keys():
                    f.write(e)
                    con[e.split(":")[1]] = True
        f.write("\nTM:")
        z = 1
        while z <= 50:
            if z in all[str(x)]["TM"].keys() or z in all[str(y)]["TM"].keys():
                f.write(" %s" % z)
            z = z + 1
        f.write("\nHM:")
        z = 1
        while z <= 50:
            if z in all[str(x)]["HM"].keys() or z in all[str(y)]["HM"].keys():
                f.write(" %s" % z)
            z = z + 1

        f.write("\n\n")
        y = y + 1
    x = x + 1

f.close()
print "Fusions completed in %f seconds." % (time.time() - start)
