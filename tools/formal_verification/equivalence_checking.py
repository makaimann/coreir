import sys
import argparse

class Config(object):
    input_file1 = None
    input_file2 = None

    inputs_list1 = None
    inputs_list2 = None
    outputs_list1 = None
    outputs_list2 = None

    imap_file = None
    
    def __init__(self):
        self.input_file1 = None
        self.input_file2 = None
        self.inputs_list1 = None
        self.inputs_list2 = None
        self.outputs_list1 = None
        self.outputs_list2 = None

        self.imap_file = None

def generate_equivalence_check(config):

    model_1 = ""
    with open(config.input_file1) as f:
        model_1 = f.read()

    model_2 = ""
    with open(config.input_file2) as f:
        model_2 = f.read()

    if not config.imap_file:
        
        inps1 = []
        inps2 = []

        if config.inputs_list1 != "":
            inps1 = config.inputs_list1.replace(" ", "").split(",")

        if config.inputs_list2 != "":
            inps2 = config.inputs_list2.replace(" ", "").split(",")

        assert(len(inps1) == len(inps2))

        inps = [(x1, inps2[inps1.index(x1)]) for x1 in inps1]

        outs1 = []
        outs2 = []

        if config.outputs_list1 != "":
            outs1 = config.outputs_list1.replace(" ", "").split(",")

        if config.outputs_list2 != "":
            outs2 = config.outputs_list2.replace(" ", "").split(",")

        assert(len(outs1) == len(outs2))

        outs = [(x1, outs2[outs1.index(x1)]) for x1 in outs1]

    else:

        inps = []
        outs = []
        stas = []
        
        with open(config.imap_file) as f:

            phase = None
            ST = "STATES:"
            IN = "INPUTS:"
            OU = "OUTPUTS:"
            
            for line in f.readlines():
                line = line.replace(" ", "").replace("\n", "")
                
                if line == "":
                    continue
                if IN in line:
                    phase = IN
                    continue
                if OU in line:
                    phase = OU
                    continue
                if ST in line:
                    phase = ST
                    continue

                
                if "#" == line[0]:
                    continue
                
                line = line.split("=")
                value = None
                
                if len(line) == 1:
                    value = (line[0],line[0])
                else:
                    value = (line[0],line[1])

                if phase == IN:
                    inps.append(value)
                    continue
                if phase == OU:
                    outs.append(value)
                    continue
                if phase == ST:
                    stas.append(value)
                    continue
                    

    set_vals = []

    init_vars_1 = []
    curr_vars_1 = []
    next_vars_1 = []
    init_1 = []
    trans_1 = []
    variables_1 = []

    init_vars_2 = []
    curr_vars_2 = []
    next_vars_2 = []
    init_2 = []
    trans_2 = []
    variables_2 = []

    (set_vals, variables_1, init_vars_1, curr_vars_1, next_vars_1, init_1, trans_1) = parse_model(model_1)
    (set_vals, variables_2, init_vars_2, curr_vars_2, next_vars_2, init_2, trans_2) = parse_model(model_2)

    print("\n".join(set_vals))

    print("\n".join([x.replace(CURR, m_1("")+CURR) for x in curr_vars_1]))
    print("\n".join([x.replace(NEXT, m_1("")+NEXT) for x in next_vars_1]))
    print("\n".join([x.replace(CURR, m_1("")+CURR).replace(NEXT, m_1("")+NEXT) for x in trans_1]))

    print("\n".join([x.replace(CURR, m_2("")+CURR) for x in curr_vars_2]))
    print("\n".join([x.replace(NEXT, m_2("")+NEXT) for x in next_vars_2]))
    print("\n".join([x.replace(CURR, m_2("")+CURR).replace(NEXT, m_2("")+NEXT) for x in trans_2]))

    pre = []

    for inp in inps:
        pre.append("(= %s %s)"%(curr(m_1(inp[0])), curr(m_2(inp[1]))))

    for ous in outs:
        pre.append("(= %s %s)"%(curr(m_1(ous[0])), curr(m_2(ous[1]))))

    for sta in stas:
        pre.append("(= %s %s)"%(curr(m_1(sta[0])), curr(m_2(sta[1]))))
        
    for inp in inps:
        pre.append("(= %s %s)"%(next(m_1(inp[0])), next(m_2(inp[1]))))
            
    pos = []

    for ous in outs:
        pos.append("(= %s %s)"%(next(m_1(ous[0])), next(m_2(ous[1]))))


    precond = to_and(pre)
    poscond = to_and(pos)
    cond = "(and %s (not %s))"%(precond, poscond)

    print("(assert %s)"%cond)

    print("(check-sat)")

    return 0


DFUN = "declare-fun"
CURR = "__CURR__"
NEXT = "__NEXT__"
INIT = "__AT0"
COMM = ";;"

def parse_model(model):
    setvals = []
    init_vars = []
    curr_vars = []
    next_vars = []
    init = []
    trans = []
    variables = []

    for line in model.split("\n"):
        if COMM in line:
            continue
        if line == "":
            continue
        if ("declare-fun" in line):
            if (CURR in line):
                curr_vars.append(line)
                var = line[line.find(DFUN)+len(DFUN)+1:line.find(")")-2]
                variables.append(var)
            if (NEXT in line):
                next_vars.append(line)
            if (INIT in line):
                init_vars.append(line)
        elif ("(set" in line):
            setvals.append(line)
        else:
            if INIT in line:
                init.append(line)
            else:
                trans.append(line)

    return (setvals, variables, init_vars, curr_vars, next_vars, init, trans)

def to_and(lst):
    if len(lst) == 1:
        return lst[0]
    
    ret = "(and %s %s)"%(lst[0], lst[1])
    if len(lst) == 2:
        return ret

    for i in range(2,len(lst),1):
        ret = "(and %s %s)"%(ret, lst[i])

    return ret

def at(time):
    return "__AT%s"%time

def curr(var):
    return "%s%s"%(var, CURR)

def next(var):
    return "%s%s"%(var, NEXT)

def m_1(prefix):
    return "%s<1>"%prefix

def m_2(prefix):
    return "%s<2>"%prefix


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Equivalence check of two smtlib2 formulas generated by CoreIR.')

    parser.set_defaults(input_file1=None)
    parser.add_argument('-if1', '--input-file1', metavar='input_file1', type=str, required=True,
                       help='input file 1')

    parser.set_defaults(input_file2=None)
    parser.add_argument('-if2', '--input-file2', metavar='input_file2', type=str, required=True,
                       help='input file 2')

    parser.set_defaults(imap_file=None)
    parser.add_argument('-m', '--imap-file', metavar='imap_file', type=str, required=False,
                        help='input map file')    
    
    parser.set_defaults(inputs_1=None)
    parser.add_argument('-i1', '--inputs-1', metavar='inputs_1', type=str, required=False,
                        help='list of input variables for file 1')

    parser.set_defaults(inputs_2=None)
    parser.add_argument('-i2', '--inputs-2', metavar='inputs_2', type=str, required=False,
                        help='list of input variables for file 1')

    parser.set_defaults(outputs_1=None)
    parser.add_argument('-o1', '--outputs-1', metavar='outputs_1', type=str, required=False,
                        help='list of output variables for file 1')

    parser.set_defaults(outputs_2=None)
    parser.add_argument('-o2', '--outputs-2', metavar='outputs_2', type=str, required=False,
                        help='list of output variables for file 1')
    
    args = parser.parse_args()

    config = Config()

    if not args.imap_file:
        if not (args.inputs_1 and args.inputs_2 and args.outputs_1 and args.outputs_2):
            print("Please provide input/output maps")
            sys.exit(0)
    
    config.input_file1 = args.input_file1
    config.input_file2 = args.input_file2
    config.inputs_list1 = args.inputs_1
    config.inputs_list2 = args.inputs_2
    config.outputs_list1 = args.outputs_1
    config.outputs_list2 = args.outputs_2

    config.imap_file = args.imap_file

    sys.exit(generate_equivalence_check(config))
