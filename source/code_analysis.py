from source.syntax import *

python_type = type

INFORMATIONS = False

def levenshtein_dist(a: str, b: str):
    a = a.lower()
    b = b.lower()
    m = len(a)
    n = len(b)
    matrice = [[0 for _ in range(n + 1)] for _ in range(m + 1)]
    
    for i in range(0, m + 1):
        matrice[i][0] = i

    for j in range(0, n + 1):
        matrice[0][j] = j

    for j in range(1, n + 1): 
        for i in range(1, m + 1):
            cout = 1
            if a[i - 1] == b[j - 1]:
                cout = 0

            matrice[i][j] = min(
                matrice[i - 1][j] + 1,
                matrice[i][j - 1] + 1,
                matrice[i - 1][j - 1] + cout
            )
    return matrice[m][n]

def find_best_match(word, l_words):
    d = levenshtein_dist(word, l_words[0])
    last_word = l_words[0]
    for i in l_words[1:]:
        m = levenshtein_dist(word, i)
        if m < d:
            d = m
            last_word = i
    if d > 6 or d == len(last_word):
        return None
    return last_word

def error(message, reference):
    print('\033[91m', '\033[1m', "Erreur", '\033[0m', '\033[1m', ' : ', reference, '\033[0m', sep="")
    print("\t\033[1m", message, "\033[0m")
    exit(1)

def warning(message, reference):
    # 92
    print('\033[95m', '\033[1m', "Attention", '\033[0m', '\033[1m', ' : ', reference, '\033[0m', sep="")
    print("\t\033[1m", message, "\033[0m")

def information(message, reference:str|None=None):
    #print("\033[95m", "Info", "\033[0m : ",'\033[1m', sep="", end="")
    #if reference:
    #    print(reference, '\033[0m', sep="")
    #print("\033[1m", message, "\033[0m")
    ...

# "ent", "bool", "liste", "chr", "ptr", "chaine"

less_equal = [
    ["bool", False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False]
]

more_equal = [
    ["bool", False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False]
]

not_equal = [
    ["bool", "bool", "bool", "bool", "bool", "bool"],
    ["bool", "bool", "bool", "bool", "bool", "bool"],
    ["bool", "bool", "bool", "bool", "bool", "bool"],
    ["bool", "bool", "bool", "bool", "bool", "bool"],
    ["bool", "bool", "bool", "bool", "bool", "bool"],
    ["bool", "bool", "bool", "bool", "bool", "bool"],
]

equal_equal = [
    ["bool", "bool", "bool", "bool", "bool", "bool"],
    ["bool", "bool", "bool", "bool", "bool", "bool"],
    ["bool", "bool", "bool", "bool", "bool", "bool"],
    ["bool", "bool", "bool", "bool", "bool", "bool"],
    ["bool", "bool", "bool", "bool", "bool", "bool"],
    ["bool", "bool", "bool", "bool", "bool", "bool"],
]

op_or = [
    ["ent", False , False, False, False, False],
    [False, "bool", False, False, False, False],
    [False, False , False, False, False, False],
    [False, False , False, False, False, False],
    [False, False , False, False, False, False],
    [False, False , False, False, False, False],
]

op_and = [
    ["ent", False , False, False, False, False],
    [False, "bool", False, False, False, False],
    [False, False , False, False, False, False],
    [False, False , False, False, False, False],
    [False, False , False, False, False, False],
    [False, False , False, False, False, False],
]

op_shift_left = [
    ["ent", False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
]

op_shift_right = [
    ["ent", False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
]

less = [
    ["bool", False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False],
]

more = [
    ["bool", False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False],
    [False , False, False, False, False, False],
]

power = [
    ["ent", False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
]

times = [
    ["ent", False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
]

div = [
    ["ent", False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
]

plus = [
    ["ent", False, False, "chr", "ptr", False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    ["chr", False, False, "chr", False, False],
    ["ptr", False, False, False, False, False],
    [False, False, False, False, False, False],
]

op_minus = [
    ["ent", False, False, False, "ptr", False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    ["ptr", False, False, False, False, False],
    [False, False, False, False, False, False],
]

modulo = [
    ["ent", False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
    [False, False, False, False, False, False],
]

operators_type = {
    "<=": less_equal,
    ">=": more_equal,
    "!=": not_equal,
    "==": equal_equal,
    "||": op_or,
    "&&": op_and,
    ">>": op_shift_right,
    "<<": op_shift_left,
    "<": less,
    ">": more,
    "^": power,
    "*": times,
    "/": div,
    "+": plus,
    "-": op_minus,
    "%": modulo
}

def is_liste(l):
    q = len("liste[")
    if len(l) < q:
        return False
    if l[:len("liste[")] == "liste[":
        return True
    return False

def is_ptr(l):
    if len(l) < 1:
        return False
    if l[:1] == "*":
        return True
    return False

TYPES = ["ent", "bool", "liste", "chr", "ptr", "chaine"]

TYPES_INFO = { # Information de création sur les types
              # à savoir si ils sont considérés comme des structures
              # et doivent être mis dans un mutisize-register
    "ent"    : False,
    "bool"   : False, 
    "liste"  : False, # TODO: modifier
    "chaine" : True,
    "chr"    : False, 
    "ptr"    : False,
    "rien"   : False
}

TYPES_SIZE = {"ent":8, "bool":1, "liste":8, "chaine":128, "chr":1, "ptr": 8, "rien": 0}

OP_FUNC = {
    "_plus_petit_eg": "<=",
    "_plus_grand_eg": ">=",
    "_pas_eg": "!=",
    "_egal": "==",
    "_ou": "||",
    "_et": "&&",
    "_shift_droit" : ">>",
    "_shift_gauche" : "<<",
    "_plus_petit": "<",
    "_plus_grand": ">",
    "_puissance": "^",
    "_fois": "*",
    "_div": "/",
    "_plus": "+",
    "_moins": "-",
    "_modulo": "%",
}

FUNC_OP = {
    "<=" : "_plus_petit_eg",
    ">=" : "_plus_grand_eg",
    "!=" : "_pas_eg",
    "==" : "_egal",
    "||" : "_ou",
    "&&" : "_et",
    ">>" : "_shift_droit",
    "<<" : "_shift_gauche",
    "<"  : "_plus_petit",
    ">"  : "_plus_grand",
    "^"  : "_puissance",
    "*"  : "_fois",
    "/"  : "_div",
    "+"  : "_plus",
    "-"  : "_moins",
    "%"  : "_modulo",
}

def modify_operators(name, type1, type2, ret):
    op = ""
    if name == "_plus_petit_eg": op = "<="
    if name == "_plus_grand_eg": op = ">="
    if name == "_pas_eg": op = "!="
    if name == "_egal": op = "=="
    if name == "_ou": op = "||"
    if name == "_et": op = "&&"
    if name == "_plus_petit": op = "<"
    if name == "_plus_grand": op = ">"
    if name == "_puissance": op = "^"
    if name == "_fois": op = "*"
    if name == "_div": op = "/"
    if name == "_plus": op = "+"
    if name == "_moins": op = "-"
    if name == "_modulo": op = "%"
    
    t1, t2 = type_int(type1), type_int(type2)
    operators_type[op][t1][t2] = ret

def type_int(type: str):
    if is_liste(type):
        type = "liste"
    if is_ptr(type):
        type = "ptr"
    return TYPES.index(type)

def operator_type(op, type_1, type_2):
    t1, t2 = type_int(type_1), type_int(type_2)
    return operators_type[op][t1][t2]

def authorize_casting(orgn_type, dest_type, ref):
    if orgn_type == "chaine" and dest_type == "liste[chr]":
        warning("La conversion de type entre 'chaine' et 'liste[chr]' est déconseillée, préférez la conversion entre une référence vers une chaine et 'liste[chr]' : *chaine -> liste[chr]", ref)
    if get_type_info_stack(dest_type):
        warning("La conversion de type avec un objet alloué sur le stack est déconseillée. On admettra la création de l'objet, rempli par la valeur du casting.", ref)
    if get_type_info_stack(orgn_type):
        warning("La conversion de type avec un objet alloué sur le stack est déconseillée. La valeur prise par l'objet correspond à un pointeur vers la valeur de l'objet (attention encore, la valeur est volatile et peut avoir toute les raisons d'être modifiée).", ref)

def get_funcall_name(tok: tok.BasicToken, variables, functions, classes, global_vars) -> str:
    name = tok.child[0].content
    args = tok.child[1]
    if args.child[0].get_rule() != listed_value:
        args = [args.child[0]]
    else:
        args = args.child[0].child
    args_type_name = "("
    for i, v in enumerate(args):
        type_t = type(v, variables, functions, classes, global_vars)
        args_type_name += f"{type_t}," if i + 1 != len(args) else f"{type_t}"
    args_type_name += ")"
    name += args_type_name
    return name

def get_methcall_name_caca(tok: tok.BasicToken, variables, functions, classes, global_vars) -> str:
    name = type(tok.child[0].child[0], variables, functions, classes, global_vars)
    meth_name = tok.child[0].child[1].child[0].content # BUG GENERATOR
    name += "." + meth_name
    
    args = tok.child[1]
    if args.child[0].get_rule() != listed_value:
        args = [args.child[0]]
    else:
        args = args.child[0].child
    args_type_name = "("
    for i, v in enumerate(args):
        type_t = type(v, variables, functions, classes, global_vars)
        args_type_name += f"{type_t}," if i + 1 != len(args) else f"{type_t}"
    args_type_name += ")"
    name += args_type_name
    return name

def get_methcall_name(tok: tok.BasicToken, variables, functions, classes, global_vars) -> str:
    name_t = type(tok.child[0].child[0], variables, functions, classes, global_vars)
    
    if is_ptr(name_t):
        name_t = get_ptr_type(name_t)
    if name_t not in classes:
        error(f"Classe inconnue : {name_t} (ou n'a jamais été initialisé) (soit, methcall)", tok.reference)
    
    for c in tok.child[0].child[1:-1]:
        att_name = c.child[0].content
        r = classes[name_t]
        if att_name not in r.att_name:
            proposition = find_best_match(att_name, r.att_name)
            if not proposition:
                error(f"Attribut inconnu : {att_name}", c.reference)
            else:
                error(f"Attribut inconnu : {att_name}. Vouliez-vous dire '{proposition}' ?", c.reference)
        i = r.att_name.index(att_name)
        name_t = r.att_type[i]
        ret = (i, r)
        if is_ptr(name_t):
            name_t = get_ptr_type(name_t)
        if name_t not in classes:
            error(f"Classe inconnue : {name_t} (ou n'a jamais été initialisé) (arg, methcall)", c.reference)
    
    final = tok.child[0].child[-1].child[0].content
    name = name_t + "." + final
    args = tok.child[1]
    if args.child[0].get_rule() != listed_value:
        args = [args.child[0]]
    else:
        args = args.child[0].child
    if get_type_info_stack(name_t):
        args_type_name = "(" + "*"+name_t + ("," if len(args) != 0 else "")
    else:
        args_type_name = "(" + name_t + ("," if len(args) != 0 else "")
    for i, v in enumerate(args):
        type_t = type(v, variables, functions, classes, global_vars)
        args_type_name += f"{type_t}," if i + 1 != len(args) else f"{type_t}"
    args_type_name += ")"
    name += args_type_name
    return name

def get_type_info_stack(type: str):
    if not type_exists(type):
        print(TYPES)
        error(f"Type inconnu '{type}'", "Global:0:0")
    if is_ptr(type):
        return TYPES_INFO["ptr"]
    if is_liste(type):
        return TYPES_INFO["liste"]
    else:
        return TYPES_INFO[type]

def get_liste_type(type: str):
    t = type[len("liste["): -1]
    return t

def get_ptr_type(type: str):
    t = type[1:]
    return t
    
def type_exists(type: str) -> bool:
    if is_ptr(type):
        return type_exists(get_ptr_type(type))
    if is_liste(type):
        return type_exists(get_liste_type(type))
    else:
        return type in TYPES or type == "rien"
    
def type_size(type: str):
    if is_liste(type):
        return TYPES_SIZE["liste"]
    if is_ptr(type):
        return TYPES_SIZE["ptr"]
    return TYPES_SIZE[type]

def get_class_type(token: tok.BasicToken):
    type_t = "<"
    debut = 1
    if not isinstance(token.child[0], tok.t_empty):
        debut = 0
    for i, t in enumerate(token.child[debut:]):
        loc_type = gettype(t)
        type_t += loc_type + ("," if i != len(token.child[debut:]) - 1 else "")
    return type_t + ">"


def type(expression: tok.BasicToken, variables:dict, functions:dict, classes:dict[str:"Classes"], global_vars):
    if expression.get_rule() == operator:
        type0 = type(expression.child[0], variables, functions, classes, global_vars)
        type1 = type(expression.child[1], variables, functions, classes, global_vars)
        if type0 == "rien" or type1 == "rien":
            error(f"Ne peut pas appliquer l'opérateur '{expression.content}' avec 'rien'", expression.reference)
        r = operator_type(expression.content, type0, type1)
        
        f = f = f"{type0}."
            
        
        f += f"{FUNC_OP[expression.content]}({type0},{type1})"
        if not r and f not in functions:
            print(list(functions.keys()))
            error(f"Types incompatibles ({type0}, {type1}) avec l'opérateur '{expression.content}', la fonction '{f}' n'existe pas", expression.reference)
        if f in functions:
            return functions[f][2]
        return r
    if expression.get_rule() == funcall:
        name = get_funcall_name(expression, variables, functions, classes, global_vars)
        if name not in functions:
            #for i in functions:
            #    print(i)
            proposition = find_best_match(name, list(functions.keys()))
            if not proposition:
                error(f"Fonction inconnue {name}", expression.reference)
            else:
                error(f"Fonction inconnue {name}. Vouliez-vous dire '{proposition}' ? ", expression.reference)
        return functions[name][2]
    if expression.get_rule() == sizeof_funcall:
        return "ent"
    if expression.get_rule() == stack_alloced_funcall:
        return "bool"
    if expression.get_rule() == typeof_funcall:
        return "liste[chr]"
    if expression.get_rule() == attr_exist:
        return "bool"
    if expression.get_rule() == func_exist:
        return "bool"
    if expression.get_rule() == meth_exist:
        return "bool"
    if expression.get_rule() == methcall:
        name = get_methcall_name(expression, variables, functions, classes, global_vars)
        if name not in functions:
            prop = find_best_match(name, list(functions.keys()))
            if not prop:
                error(f"Methode inconnue {name}", expression.reference)
            else:
                error(f"Methode inconnue {name}. Vouliez-vous dire {prop} ?", expression.reference)
        return functions[name][2]
    if expression.get_rule() == identifier:
        if expression.content in variables:
            return variables[expression.content][0]
        if expression.content in global_vars:
            return global_vars[expression.content]
        prop = find_best_match(expression.content, list(variables.keys()))
        if not prop:
            error(f"Identifiant inconnu : '{expression.content}'", expression.reference)
        else:
            error(f"Identifiant inconnu : '{expression.content}'. Vouliez-vous dire '{prop}' ?", expression.reference)
    if expression.get_rule() == int_ or expression.get_rule() == hex_int:
        return "ent"
    if expression.get_rule() == float:
        return "flot"
    if expression.get_rule() == cstring:
        return "chaine"
    if expression.get_rule() == string:
        return "liste[chr]"
    if expression.get_rule() == char:
        return "chr" 
    if expression.get_rule() == t_bool:
        return "bool"
    if expression.get_rule() == parenthesis or expression.get_rule() == casting_args:
        return type(expression.child[0], variables, functions, classes, global_vars)
    if expression.get_rule() == negativ:
        return type(expression.child[0], variables, functions, classes, global_vars)
    if expression.get_rule() == casting:
        return gettype(expression.child[0])
    if expression.get_rule() == array_accession: # expected to be a list
        r = type(expression.child[0], variables, functions, classes, global_vars)
        if is_liste(r):
            return r[len("liste["):-1]
        if is_ptr(r):
            type_t_bis = get_ptr_type(r)
            if type_t_bis not in classes:
                error(f"Nom de classe ou d'objet inconnu : '{type_t_bis}'", expression.reference)
            func_name = f"{type_t_bis}.index({r},ent)"
            if func_name not in functions:
                error(f"Méthode inconnue : '{func_name}'. La classe doit contenir une méthode '.index({r},ent)' pour supporter l'indexation", expression.reference)
            _, _, rettype, _ = functions[func_name]
            return rettype
        if get_type_info_stack(r):
            fname = f"{r}.index(*{r},ent)"
            if not fname in functions:
                error(f"Type imcompatible avec une indexation, '{r}'. Manque peut être une fonction {fname}", expression.reference)
            _, _, rettype, _ = functions[fname]
            return rettype
        else:
            fname = f"{r}.index({r},ent)"
            if not fname in functions:
                error(f"Type imcompatible avec une indexation, '{r}'. Manque peut être une fonction {fname}", expression.reference)
            _, _, rettype, _ = functions[fname]
            return rettype
    if expression.get_rule() == classcall:
        following = ""
        stack_alloced = not isinstance(expression.child[2], tok.t_empty)
        if not isinstance(expression.child[0], tok.t_empty):
            following = get_class_type(expression.child[0])
        if stack_alloced:
            return f"{expression.child[1].content}" + following
        return f"*{expression.child[1].content}" + following
    if expression.get_rule() == attribute_identifier:
        name_t = type(expression.child[0], variables, functions, classes, global_vars)
        if is_ptr(name_t):
            name_t = get_ptr_type(name_t)
        if name_t not in classes:
            error(f"Classe inconnue : {name_t} (ou n'a jamais été initialisé) (attr ident soit)", c.reference)
        ret = (0, classes[name_t])
        for c in expression.child[1:]:
            att_name = c.child[0].content
            if name_t not in classes:
                error(f"Classe inconnue : {name_t} (ou n'a jamais été initialisé) (attr ident arg)", c.reference)
            r = classes[name_t]
            if att_name not in r.att_name:
                prop = find_best_match(att_name, r.att_name)
                if not prop:
                    error(f"Attribut inconnu à la classe '{name_t}' : {att_name}", c.reference)
                else:
                    error(f"Attribut inconnu à la classe '{name_t}' : {att_name}. Vouliez vous dire '.{prop}' ? ", c.reference)
            i = r.att_name.index(att_name)
            if is_ptr(r.att_type[i]):
                name_t = get_ptr_type(r.att_type[i])
            else:
                name_t = r.att_type[i]
            ret = (i, r)
        return ret[1].att_type[ret[0]]
    if expression.get_rule() == ptr_access:
        access = expression.child[0]
        t = type(access, variables, functions, classes, global_vars)
        if not is_ptr(t):
            error(f"Le type doit être *<type>, pas {t}", expression.reference)
        return get_ptr_type(t)
    if expression.get_rule() == dereferencement:
        #if expression.child[0].content not in variables:
        #    error("Le déréférencement prend en argument seulement une variables définie", expression.reference)
        return f"*{type(expression.child[0], variables, functions, classes, global_vars)}"
    if expression.get_rule() == not_op:
        return type(expression.child[0], variables, functions, classes, global_vars)
    if expression.get_rule() == conditional_value:
        type1 = type(expression.child[0], variables, functions, classes, global_vars)
        condition_type = type(expression.child[1], variables, functions, classes, global_vars)
        type2 = type(expression.child[2], variables, functions, classes, global_vars)
        if type1 != type2:
            error(f"Les types des deux valeurs possibles devraient être égaux, pas '{type1}' et '{type2}'", expression.reference)
        if condition_type != "bool":
            error(f"Le type de la condition devrait être de type 'bool', pas '{condition_type}'", expression.reference)
        return type1
    if expression.get_rule() == fpconversion_funcall:
        type_dest = expression.child[0].child[0].child[0]
        return gettype(type_dest)
    assert False, f"Unimplemented rule : {expression.get_rule().name if expression.get_rule().name != '' else expression.get_rule()}. Reference : {expression.reference}"
    
def gettype(token: tok.BasicToken) -> str:
    following = ""
    if len(token.child) > 1 and not isinstance(token.child[1], tok.t_empty) and token.child[1].get_rule() == class_type_opt:
        following += get_class_type(token.child[1])
    if token == type_array_declaration_explicit:
        r = f"liste[{gettype(token.child[2])}]"
        return r + following
    if token == type_array_declaration:
        r = f"liste[{gettype(token.child[2])}]"
        return r + following
    elif token == ptr_type:
        return f"*{gettype(token.child[0])}" + following
    elif token == type_usage:
        return gettype(token.child[0]) + following
    elif isinstance(token.get_rule(), rls.r_identifier):
        #if token.content not in TYPES and token.content != "rien":
        #    error(f"Type inconnu '{token.content}'", token.reference)
        return token.content + following
    else:
        print(f"What : {token.content} {token.get_rule().name} {python_type(token.get_rule()) == python_type(ptr_type)}")
        return token.content + following