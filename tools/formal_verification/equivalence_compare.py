import sys

valuesmap = {}

for line in sys.stdin:
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

eqlist = []
        
for el in valuesmap:
    label = "EQ"
    if (valuesmap[el][0] != valuesmap[el][1]):
        label = "NE"

    if (valuesmap[el][0] is None) or (valuesmap[el][1] is None):
        label = "NN"

        
    print("%s: %s, <1>=%s, <2>=%s"%(label, el, valuesmap[el][0], valuesmap[el][1]))
