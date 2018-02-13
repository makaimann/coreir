import sys

valuesmap = {}

for line in sys.stdin:
    if "unsat" in line:
        print("SYSTEMS ARE EQUIVALENT")
        quit(0)
    
    if " = " in line:
        if "<1>" in line:
            idx = 0
            line = line.replace("<1>","")
        if "<2>" in line:
            idx = 1
            line = line.replace("<2>","")
            
        line = line.replace("\n","").replace(" ","").split("=")
        varname = line[0]
        value = line[1]

        if varname not in valuesmap:
            valuesmap[varname] = [None,None]

        valuesmap[varname][idx] = value

eqlines = []
nelines = []
nplines = []

for el in valuesmap:
    line = "%s: <1>=%s, <2>=%s"%(el, valuesmap[el][0], valuesmap[el][1])

    if (valuesmap[el][0] is None) or (valuesmap[el][1] is None):
        nplines.append(line)
        continue

    if (valuesmap[el][0] != valuesmap[el][1]):
        nelines.append(line)
        continue
        
    eqlines.append(line)

eqlines.sort()
nelines.sort()
nplines.sort()

eqlines.insert(0, "EQUALS")
eqlines.insert(1, "EQ")

nelines.insert(0, "NOT EQUALS")
nelines.insert(1, "NE")

nplines.insert(0, "NOT PRESENT IN BOTH SYSTEMS")
nplines.insert(1, "NP")

for lines in [nelines, nplines, eqlines]:
    print("\n-- %s (CURRENT) --"%lines[0])
    print("\n".join(["%s - %s"%(lines[1], x) for x in lines[2:] if "_N: " not in x]))
    print("\n-- %s (NEXT) --"%lines[0])
    print("\n".join(["%s - %s"%(lines[1], x) for x in lines[2:] if "_N: " in x]))
