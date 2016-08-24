import time
start = time.time()
print "Loading types."
f = open("types.dat")
s = f.readline().replace("\n", "")
types = {}
status = {}
moves = {}
mon = {}
special_list = []
special_list.append("ENEMY_LAST")
special_list.append("FIRST")
special_list.append("POST_CONFUSE")
special_list.append("RANDOM")
special_list.append("UNAVOIDABLE")
while s != "":
    t = s.split(":")[0]
    types[t] = {}
    s = s.split(" ")
    x = 1
    while x < len(s):
        types[t][s[x].split("x")[0]] = float(s[x].split("x")[1])
        x = x + 1
    s = f.readline().replace("\n", "")

for s in types.keys():
    for t in types[s].keys():
        if not t in types.keys():
            print "Error in %s: %s" % (s, t)

f.close()
print "Loading status types."
f = open("status.dat")
s = f.readline().replace("\n", "")
while s != "":
    status[s] = True
    s = f.readline().replace("\n", "")

f.close()
print "Loading move database."
f = open("moves.dat")
s = f.readline().replace("\n", "")
t = ""
while s != "":
    s = s.replace("\n", "")
    if s.startswith("MOVE:"):
        t = s[6:]
        if moves != {} and t in moves.keys():
            print "Error: %s defined more than once." % t
        moves[t] = {}
    elif s.startswith("TYPE:"):
        moves[t]["TYPE"] = s.split(" ")[1]
    elif s.startswith("DEFENSE:"):
        moves[t]["DEFENSE"] = s.split(" ")[1]
    elif s.startswith("POWER"):
        moves[t]["POWER"] = s.split(" ")[1]
    elif s.startswith("ACC"):
        moves[t]["ACC"] = s.split(" ")[1]
    elif s.startswith("PP"):
        moves[t]["PP"] = s.split(" ")[1]
    elif s.startswith("SELF"):
        moves[t]["SELF"] = s[6:]
    elif s.startswith("TARGET"):
        moves[t]["TARGET"] = s[8:]
    elif s.startswith("QUEUE:"):
        moves[t]["QUEUE"] = s[7:]
    elif s.startswith("QUEUE_ONLY"):
        moves[t]["QUEUE_ONLY"] = s[12:]
    elif s.startswith("DESC"):
        moves[t]["DESC"] = s[6:]
    elif s.startswith("CRITICAL"):
        moves[t]["CRITICAL"] = s[10:]
    elif s.startswith("SPECIAL"):
        moves[t]["SPECIAL"] = s[9:]
    else:
        print "Error:  Unknown symbol %s" % s
    s = f.readline().replace("\n", "")
    if s == "":
        s = f.readline().replace("\n", "")
f.close()

for s in moves.keys():
    if "PP" not in moves[s].keys() and "QUEUE_ONLY" not in moves[s].keys():
        print "Error:  %s has no PP." % s
    if "PP" in moves[s].keys() and "QUEUE_ONLY" in moves[s].keys():
        print "Error:  %s has PP and is a queue-only move." % s
    if "QUEUE" in moves[s].keys():
        t = moves[s]["QUEUE"].split(" ")
        x = 0
        while x < len(t):
            if t[x] not in moves.keys():
                print "Error:  %s queues move %s that is not defined" % (s, t[x])
            x = x + 1
    if "TYPE" not in moves[s].keys():
        print "Error:  %s has no type." % s
    if "TYPE" in moves[s].keys():
        if moves[s]["TYPE"] not in types.keys():
            print "Error:  %s uses type %s that does not exist." % (s, moves[s]["TYPE"])
    if "ACC" not in moves[s].keys() and "POWER" in moves[s].keys():
        print "Error:  %s has power, but no accuracy."
    if "ACC" not in moves[s].keys() and "TARGET" in moves[s].keys():
        print "Error:  %s has target, but no accuracy."
    if "POWER" in moves[s].keys() and not "DEFENSE" in moves[s].keys():
        print "Error:  %s has power, but not defense." % s
    if "SELF" in moves[s].keys():
        t = moves[s]["SELF"].split(" ")
        x = 0
        while x < len(t):
            if t[x].split(":")[0] not in status.keys():
                print "Error:  %s grants status %s which does not exist." % (s, t[x])
            x = x + 1
    if "TARGET" in moves[s].keys():
        t = moves[s]["TARGET"].split(" ")
        x = 0
        while x < len(t):
            if t[x].split(":")[0] not in status.keys():
                print "Error:  %s inflicts status %s which does not exist." % (s, t[x])
            x = x + 1
    if "CRITICAL" in moves[s].keys():
        if moves[s]["CRITICAL"] != "HIGH" and moves[s]["CRITICAL"] != "NORMAL":
            print "Error:  %s has not defined critical hit chance %s." % (s, moves[s]["CRITICAL"])
    if "SPECIAL" in moves[s].keys():
        t = moves[s]["SPECIAL"].split(" ")
        x = 0
        while x < len(t):
            if t[x] not in special_list:
                print "Error:  %s has not defined special modifier %s." % (s, t[x])
            x = x + 1

print "Loading creature database."
f = open("mon.dat")
s = f.readline().replace("\n", "")
t = ""
while s != "":
    if s.startswith("NUMBER"):
        t = s[8:]
        if mon != {} and t in mon.keys():
            print "Error:  %s already defined." % s
        mon[t] = {}
    elif s.startswith("NAME"):
        mon[t]["NAME"] = s[6:]
    elif s.startswith("TYPE1"):
        mon[t]["TYPE1"] = s[7:]    
    elif s.startswith("TYPE2"):
        mon[t]["TYPE2"] = s[7:]
        if mon[t]["TYPE1"] == mon[t]["TYPE2"]:
            print "Error:  %s is set for both types of %s" % (mon[t]["TYPE1"], mon[t]["NAME"])
    elif s.startswith("HP"):
        mon[t]["HP"] = s[4:]
    elif s.startswith("ATTACK"):
        mon[t]["ATTACK"] = s[8:]
    elif s.startswith("DEFENSE"):
        mon[t]["DEFENSE"] = s[9:]
    elif s.startswith("SPECIAL"):
        mon[t]["SPECIAL"] = s[9:]
    elif s.startswith("SPEED"):
        mon[t]["SPEED"] = s[7:]
    elif s.startswith("MOVES"):
        mon[t]["MOVES"] = []
        s = f.readline().replace("\n", "")
        while not s.startswith("TM") and not s.startswith("EVOLUTION"):
            mon[t]["MOVES"].append(s[s.find(" ") + 1:])
            s = f.readline().replace("\n", "")
    if s.startswith("EVOLUTION"):
        mon[t]["EVOLUTION"] = []
        s = f.readline().replace("\n", "")
        ev = {}
        ev["NONE"] = True
        while not s.startswith("TM"):
            u = s.split(":")[1]
            if "LEVEL" in u:
                u = u.split(" ")[0]
            if u in ev.keys():
                print "Error:  %s contains multiple evolutions with the same trigger %s." % (mon[t]["NAME"], u)
            ev[u] = True
            mon[t]["EVOLUTION"].append(s.split(":")[0])
            s = f.readline().replace("\n", "")
    if s.startswith("TM"):
        mon[t]["TM"] = s[4:]
    elif s.startswith("HM"):
        mon[t]["HM"] = s[4:]
    s = f.readline().replace("\n", "")
    if s == "":
        s = f.readline().replace("\n", "")
f.close()

for s in mon.keys():
    for t in mon.keys():
        if t != s:
            if mon[s]["NAME"] == mon[t]["NAME"]:
                print "Error:  %s and %s have the same name (%s)" % (s, t, mon[t]["NAME"])
    x = 0
    while x < len(mon[s]["MOVES"]):
        if mon[s]["MOVES"][x] not in moves.keys():
            print "Error:  %s references move %s that is not defined." % (mon[s]["NAME"], mon[s]["MOVES"][x])
        if "QUEUE_ONLY" in moves[mon[s]["MOVES"][x]].keys():
            print "Error:  %s references queue-only move %s." % (mon[s]["NAME"], mon[s]["MOVES"][x])
        x = x + 1
    x = 0
    if "EVOLUTION" in mon[s].keys():
        while x < len(mon[s]["EVOLUTION"]):
            if mon[s]["EVOLUTION"][x] not in mon.keys():
                print "Error:  %s evolves into %s that is not defined." % (mon[s]["NAME"], mon[s]["EVOLUTION"][x])
            x = x + 1

print "Validation checks completed in %f seconds." % (time.time() - start)
