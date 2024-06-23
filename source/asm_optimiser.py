
def remove_comments(inst: str):
    if ";" in inst:
        inst = inst[:inst.index(";")] # On enlève les commentaires
    return inst

def pop_reg(inst: str):
    inst = remove_comments(inst)
    inst = inst.split()
    r = []
    for i in inst: 
        if i != "": r.append(i)
    inst = r
    if "pop" in inst and len(r) > 1:
        return inst[1] # le register
    elif "pop" in inst and len(r) == 1:
        return ""
    return ""
    
def push_reg(inst: str):
    inst = remove_comments(inst)
    inst = inst.split()
    r = []
    for i in inst: 
        if i != "": r.append(i)
    inst = r
    if "push" in inst:
        return inst[1] # le register
    return ""

def is_inst(inst: str):
    inst = remove_comments(inst)
    inst = inst.split()
    r = []
    for i in inst: 
        if i != "": r.append(i)
    return len(r) != 0

def optimise(asm : list[str]):
    optimised = []
    for inst in asm:
        if not inst:
            optimised.append(inst)
            continue
        if len(optimised) >= 1 and (reg := pop_reg(inst)) and (oreg:= push_reg(optimised[-1])) and reg == oreg:
            optimised[-1] = f"  mov {reg}, qword [rsp]"
        elif len(optimised) >= 1 and (reg := push_reg(inst)) and (oreg:= pop_reg(optimised[-1])) and reg == oreg:
            optimised.pop()
        elif is_inst(inst):
            optimised.append(inst)

    return optimised