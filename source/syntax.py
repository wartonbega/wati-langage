from source.parser_imp import *

ch.BASIC_CHARACTERS.append("_")

def get_prec(operator: str) -> int:
    if operator in ["||", "&&"]:
        return 7
    if operator in ["<", ">", "<=", ">=", "==", "!="]:
        return 6
    if operator in ["+", "-"]:
        return 5
    if operator in ["%"]:
        return 4
    if operator in ["*", "/", "%"]:
        return 3
    if operator in ["**", "^"]:
        return 2
    if operator in [">>", "<<"]:
        return 1
    assert False, operator

def calcul_rearangement(t:tok.BasicToken) -> tok.BasicToken:
    # should look like 'value1 operator value2 operator ... operator valueN
    # Precedence : get_prec(...)
    if len(t.child) <= 1:
        return t
    max, index = get_prec(t.child[1].content), 1
    for i, c in enumerate(t.child):
        if c.content in ch.OPERATORS:
            prec = get_prec(c.content)
            if max <= prec:
                max = prec
                index = i

    lhs = tok.t_setlist("lhs", True, operator, "")
    for i in range(0, index):
        lhs.push_child(t.child[i])

    rhs = tok.t_setlist("rhs", True, operator, "")
    for i in range(index + 1, len(t.child)):
        rhs.push_child(t.child[i])

    t.child[index].push_child(calcul_rearangement(lhs))
    t.child[index].push_child(calcul_rearangement(rhs))
    return t.child[index]

comment = rls.r_sequence(
    rls.r_char_sequence("//").ignore_token(),
    rls.r_setlist(ch.c_any_but("\n"))
).set_name("comment")

colonm = rls.r_character(";").ignore_token()
k_classe = rls.r_char_sequence("classe").ignore_token()
k_contient = rls.r_char_sequence("contient").ignore_token()
k_func = rls.r_char_sequence("fonction").ignore_token()
k_methode = rls.r_char_sequence("methode").ignore_token()
k_pour = rls.r_char_sequence("pour").ignore_token()
k_dans = rls.r_char_sequence("dans").ignore_token()
k_tant = rls.r_char_sequence("tant").ignore_token()
k_que  = rls.r_char_sequence("que") .ignore_token()
k_si   = rls.r_char_sequence("si")  .ignore_token()
k_sinonsi = rls.r_char_sequence("sinonsi").ignore_token()
k_sinon = rls.r_char_sequence("sinon").ignore_token()

k_return = rls.r_char_sequence("renvoie").ignore_token()
k_break = rls.r_char_sequence("casse").ignore_token()
k_include = rls.r_char_sequence("inclue").ignore_token()
k_typedef = rls.r_char_sequence("deftype").ignore_token()
k_type_convert = rls.r_char_sequence("convertype").ignore_token()
k_syscall = rls.r_char_sequence("syscall").ignore_token()
k_extern = rls.r_char_sequence("externe").ignore_token()
k_utilise = rls.r_char_sequence("utilise").ignore_token()
k_definis = rls.r_char_sequence("definis").ignore_token()
k_nomenclature = rls.r_char_sequence("nomenclature").ignore_token()

operator = rls.r_option(
    rls.r_setlist(ch.TWO_SYM_OPERATORS),
    rls.r_setlist(ch.ONE_SYM_OPERATORS)
)

type_names = rls.r_option(
    rls.r_char_sequence("ent" , ch.BASIC_CHARACTERS + ch.NUMERICS),     # for integers
    rls.r_char_sequence("flot", ch.BASIC_CHARACTERS + ch.NUMERICS),    # for floats
    rls.r_char_sequence("chr" , ch.BASIC_CHARACTERS + ch.NUMERICS),     # For characters (single ones)
    rls.r_char_sequence("bool", ch.BASIC_CHARACTERS + ch.NUMERICS),    # For booleans
    rls.r_char_sequence("rien", ch.BASIC_CHARACTERS + ch.NUMERICS),     # For void
    rls.r_identifier()
).set_name("TypeN")
array_dec_brackets = rls.r_enclosure("[", "]").set_name("brackets")
array_dec_brackets_explicit = rls.r_enclosure("[", "]").set_name("brackets")

type_array_declaration: rls.r_sequence = rls.r_sequence(
    array_dec_brackets,
    rls.r_optional(rls.r_character("&")),
    type_names
).set_name("array-dec")

type_array_declaration_explicit: rls.r_sequence = rls.r_sequence(
    array_dec_brackets_explicit,
    rls.r_optional(rls.r_character("&")),
    type_names
).set_name("array-dec-explicit")

ptr_type = rls.r_sequence(
    rls.r_char_sequence("*").ignore_token(),
    type_names
).set_name("ptr-dec")

type_t_name_dec: rls.r_option = rls.r_option(
    type_names,
    type_array_declaration,
    ptr_type
).set_name("Type_t_name_dec")

class_type_opt = rls.r_sequence(
    rls.r_character("<").ignore_token(),
    rls.r_optional(
        rls.r_patern_repetition(
            rls.r_sequence(
                type_t_name_dec,
                rls.r_character(",").ignore_token(),
            ).ignore_token()
        ).ignore_token()
    ),
    type_t_name_dec,
    rls.r_character(">").ignore_token()
).set_name("class-type-opt")

type_usage = rls.r_sequence(
    rls.r_character("<").ignore_token(),
    type_t_name_dec,
    rls.r_optional(
        class_type_opt
    ),
    rls.r_character(">").ignore_token()
).set_name("Type-Usage")

#class_type_opt.sequence[1].sub.patern.sequence[0] = type_usage
type_names.add_option(type_usage)


# enclosures
scope = rls.r_enclosure("fait", "fin").set_name("scope{}")
class_scope = rls.r_enclosure("contient", "fin").set_name("classe-scope{}")
parenthesis = rls.r_enclosure("(", ")")
func_arguments = rls.r_enclosure("(", ")")
casting_args = rls.r_enclosure("(", ")").set_name("casting-arg()")
brackets = rls.r_enclosure("[", "]")
string = rls.r_enclosure("\"", "\"", mid=rls.r_optional(rls.r_setlist(ch.c_any_but("\"")))).set_name("string-def")
cstring = rls.r_sequence(
    rls.r_character("c").ignore_token(),
    rls.r_enclosure("\"", "\"", mid=rls.r_optional(rls.r_setlist(ch.c_any_but("\"")))).set_name("string-def")
)
char = rls.r_enclosure("'", "'", mid=rls.r_optional(rls.r_setlist(ch.c_any_but("'")))).set_name("char-def")

identifier = rls.r_identifier()
int_ = rls.r_digit()

hex_int = rls.r_sequence(
    rls.r_character("0").ignore_token(),
    rls.r_character("x").ignore_token(),
    rls.r_setlist(ch.NUMERICS + ["A", "B", "C", "D", "E", "F"] + ["a", "b", "c", "d", "e", "f"]).set_error("Expected hexadecimal number"),
    ignore_whitespace=False
).set_name("Hex-number")

t_bool = rls.r_option(
    rls.r_char_sequence("Vrai"),
    rls.r_char_sequence("Faux")
).set_name("bool")

float = rls.r_sequence(
    int_,
    rls.r_character("."),
    int_,
    ignore_whitespace=False
).set_name("float")

proto_attribute = rls.r_sequence(
    rls.r_character(".").ignore_token(),
    identifier
).set_name("proto-atribute")

attribute_identifier = rls.r_sequence(
    identifier,
    rls.r_patern_repetition(
        proto_attribute
    ).ignore_token()
).set_name("atribute_identifer")

# casts
casting = rls.r_sequence(
    type_usage,
    rls.r_optional(rls.r_character("!")),
    rls.r_option(casting_args)
).set_name("cast")

# Function call
funcall = rls.r_sequence(
    rls.r_character("!").ignore_token(),
    identifier,
    parenthesis
).set_name("funcall")

sizeof_funcall = rls.r_sequence(
    rls.r_character("!").ignore_token(),
    rls.r_char_sequence("taillede").ignore_token(),
    rls.r_enclosure("(", ")").set_mid_patern(rls.r_option(
        type_usage
    ))
).set_name("taillede")

stack_alloced_funcall = rls.r_sequence(
    rls.r_character("!").ignore_token(),
    rls.r_char_sequence("alloue_stack").ignore_token(),
    rls.r_enclosure("(", ")").set_mid_patern(rls.r_option(
        type_usage,
        string
    ))
).set_name("alloue_stack")

typeof_funcall = rls.r_sequence(
    rls.r_character("!").ignore_token(),
    rls.r_char_sequence("typede").ignore_token(),
    rls.r_enclosure("(", ")")
).set_name("typede")

attr_exist = rls.r_sequence(
    rls.r_character("!").ignore_token(),
    rls.r_char_sequence("attribut_existe").ignore_token(),
    rls.r_enclosure("(", ")")
).set_name("attr_exist")

meth_exist = rls.r_sequence(
    rls.r_character("!").ignore_token(),
    rls.r_char_sequence("methode_existe").ignore_token(),
    rls.r_enclosure("(", ")")
).set_name("attr_exist")

func_exist = rls.r_sequence(
    rls.r_character("!").ignore_token(),
    rls.r_char_sequence("fonction_existe").ignore_token(),
    rls.r_enclosure("(", ")").set_mid_patern(string)
).set_name("func_exist")

methcall = rls.r_sequence(
    rls.r_character("!").ignore_token(),
    attribute_identifier,
    parenthesis
).set_name("methcall")

classcall = rls.r_sequence(
    rls.r_optional(
        class_type_opt
    ),
    identifier,
    rls.r_optional(
        rls.r_character("&")  
    ),
    parenthesis
).set_name("calsscall")

func_value = rls.r_sequence(
    rls.r_character("!"),
    identifier
).set_name("funcvalue")

attribute = rls.r_sequence(
    rls.r_character("."),
    identifier,
    ignore_whitespace=False
).set_name("attribute")

value = rls.r_option(
    sizeof_funcall,
    stack_alloced_funcall,
    typeof_funcall,
    attr_exist,
    func_exist,
    meth_exist,
    funcall,
    classcall,
    methcall,
    func_value, # Maybe not keep it here (?)
    attribute_identifier,
    float,
    t_bool,
    string,
    cstring,
    char,
    hex_int,
    int_,
    identifier,
    casting,
    parenthesis
)

negativ = rls.r_sequence(
    rls.r_character("-").ignore_token(),
    value
).set_name("negativ")

not_op = rls.r_sequence(
    rls.r_character("~").ignore_token(),
    value
).set_name("not-op~")

ptr_access = rls.r_sequence(
    rls.r_character("*").ignore_token(),
    value
).set_name("ptr-access")

dereferencement = rls.r_sequence(
    rls.r_character("&").ignore_token(),
    value
).set_name("dereferencement")

array_accession = rls.r_sequence(
    value,
    brackets
).set_name("array-access")

proto_calcul = rls.r_sequence(
    rls.r_option(
        negativ,
        not_op,
        ptr_access,
        dereferencement,
        array_accession,
        value,
        identifier
    ),
    operator
).set_name("proto-calcul").ignore_token()

calcul = rls.r_sequence(
    rls.r_patern_repetition(proto_calcul).ignore_token(),
    rls.r_option(
        negativ,
        not_op,
        ptr_access,
        dereferencement,
        array_accession,
        value,
        identifier
    )
).set_name("calcul").set_rearangement(calcul_rearangement)

attended_expression = rls.r_option(
    calcul,
    negativ,
    not_op,
    ptr_access,
    dereferencement,
    array_accession,
    value,
    identifier
)

typeof_funcall.sequence[2].set_mid_patern(attended_expression)

attr_exist.sequence[2].set_mid_patern(
    rls.r_sequence(
        attended_expression,
        rls.r_character(",").ignore_token(),
        string
    )
)

meth_exist.sequence[2].set_mid_patern(
    rls.r_sequence(
        attended_expression,
        rls.r_character(",").ignore_token(),
        string
    )
)
#array_accession.sequence[0] = attended_expression

conditional_value = rls.r_sequence(
    rls.r_character("?").ignore_token(),
    attended_expression,
    k_si,
    attended_expression,
    k_sinon,
    attended_expression
).set_name("conditional-value")

attended_expression.add_option(conditional_value)

array_dec_brackets.set_mid_patern(attended_expression)
array_dec_brackets_explicit.set_mid_patern(int_)

keyword_return = rls.r_sequence(
    k_return,
    rls.r_optional(attended_expression),
    rls.r_character(";").ignore_token().set_error("';' attendu")
).set_name("keyword-renvoie")

keyword_casse = rls.r_sequence(
    k_break,
    rls.r_character(";").ignore_token().set_error("';' attendu")
).set_name("keyword-renvoie")

keyword_inclue = rls.r_sequence(
    k_include,
    string,
    rls.r_character(";").ignore_token().set_error("';' attendu")
).set_name("keyword-inclue")

keyword_typedef = rls.r_sequence(
    k_typedef,
    rls.r_identifier().set_error("Expected identifier"),
    int_,
    rls.r_character(";").ignore_token().set_error("';' attendu")
).set_name("keyword-typedef")

keyword_type_convert = rls.r_sequence(
    k_type_convert,
    rls.r_identifier().set_error("Expected identifier"),
    rls.r_identifier().set_error("Expected identifier"),
    rls.r_character(";").ignore_token().set_error("';' attendu")
).set_name("keyword-convertype")

proto_list = rls.r_sequence(
    attended_expression,
    rls.r_character(",").ignore_token()
).set_name("proto-list").ignore_token()

listed_value = rls.r_sequence(
    rls.r_patern_repetition(proto_list).ignore_token(),
    attended_expression
).set_name("listed-value")

argument_def = rls.r_sequence(
    type_usage,
    identifier
).set_name("argument-def")

proto_argument = rls.r_sequence(
    argument_def,
    rls.r_character(",").ignore_token()
).ignore_token()

arguments = rls.r_sequence(
    rls.r_patern_repetition(proto_argument).ignore_token(),
    argument_def
).ignore_token()

keyword_syscall = rls.r_sequence(
    k_syscall,
    rls.r_option(listed_value, attended_expression).set_error("Une valeur est attendue"),
    rls.r_character(";").ignore_token().set_error("';' attendu")
).set_name("keyword-inclue")

extern_func_parenth = rls.r_enclosure("(", ")")

extern_argument_def = rls.r_sequence(
    type_usage,
).set_name("extern-arg-def")

proto_extern_argument = rls.r_sequence(
    extern_argument_def,
    rls.r_character(",").ignore_token()
).ignore_token()

extern_arguments = rls.r_sequence(
    rls.r_patern_repetition(proto_extern_argument).ignore_token(),
    extern_argument_def
).ignore_token()

keyword_extern = rls.r_sequence(
    k_extern,
    type_usage,
    rls.r_identifier().set_error("Une valeur est attendue"),
    extern_func_parenth,
    rls.r_character(";").ignore_token().set_error("';' attendu")
).set_name("keyword-extern")


array_def = rls.r_sequence(
    rls.r_sequence(
        rls.r_character("<").ignore_token(),
        type_array_declaration_explicit,
        rls.r_character(">").ignore_token()
    ).ignore_token(),
    
    identifier,
    rls.r_character(";").ignore_token().set_error("';' attendu")
).set_name("array-def")

vardef = rls.r_sequence(
    rls.r_optional(type_usage),
    rls.r_optional(rls.r_character("*")).set_name("ptr-modif"),
    identifier,
    rls.r_optional(rls.r_patern_repetition(proto_attribute).set_name("attributes-asgmt")),
    rls.r_optional(brackets),
    rls.r_optional(
        rls.r_sequence(
            rls.r_character("=").ignore_token(),
            attended_expression
        ).ignore_token()
    ),
    rls.r_character(";").set_error("';' attendu").ignore_token()
).set_name("vardef")

funcdef = rls.r_sequence(
    k_func,
    type_usage,
    rls.r_option(
        rls.r_sequence(
            identifier,
            rls.r_character(".").ignore_token(),
            identifier
        ).set_name("funcdef-methode"),
        identifier,
        string
    ),
    func_arguments,
    scope
).set_name("funcdef")

funcdef_dec = rls.r_sequence(
    k_func,
    type_usage,
    rls.r_option(
        rls.r_sequence(
            identifier,
            rls.r_character(".").ignore_token(),
            identifier
        ).set_name("funcdef-methode"),
        identifier,
        string
    ),
    func_arguments,
    rls.r_character(";").ignore_token()
).set_name("function-declaration")

methodedef = rls.r_sequence(
    k_methode,
    type_usage,
    identifier,
    func_arguments,
    scope
).set_name("methdef")

methodedef_dec = rls.r_sequence(
    k_methode,
    type_usage,
    identifier,
    func_arguments,
    rls.r_character(";").ignore_token()
).set_name("methdef-declaration")

attributedef = rls.r_sequence(
    type_usage,
    identifier,
    rls.r_character(";").ignore_token().set_error("';' attendu")
).set_name("attributedef")

class_scope.set_mid_patern(
    rls.r_sequence(
        rls.r_optional(rls.r_patern_repetition(attributedef).set_name("attributes")),
        rls.r_optional(rls.r_patern_repetition(
            rls.r_option(
                methodedef,
                methodedef_dec
            )
        ).set_name("methodes")),
    ).ignore_token()
)

classdef = rls.r_sequence(
    k_classe,
    rls.r_optional(
        class_type_opt
    ),
    identifier,
    rls.r_optional(
        rls.r_char_sequence("IT")
    ).set_name("iterateur"),
    class_scope.set_error("Expected class scope")
).set_name("classdef")

proto_elif = rls.r_sequence(
    k_sinonsi,
    attended_expression,
    scope
).set_name("elif")

proto_else = rls.r_sequence(
    k_sinon,
    scope
).set_name("else")

ifstmt = rls.r_sequence(
    k_si,
    attended_expression,
    scope,
    rls.r_optional(rls.r_patern_repetition(proto_elif).set_name("elifs")),
    rls.r_optional(proto_else)
).set_name("ifstmt")

forloop_brackets = rls.r_enclosure("[", "]").set_mid_patern(
    rls.r_sequence(
    attended_expression,
    rls.r_character(",").ignore_token(),
    attended_expression)
)

forloop = rls.r_sequence(
    k_pour,
    rls.r_identifier(),
    k_dans,
    rls.r_option(
        forloop_brackets,
        attended_expression
    ),
    scope
).set_name("forloop")

whileloop = rls.r_sequence(
    k_tant,
    k_que.set_error("expected 'que'"),
    attended_expression,
    scope
).set_name("whileloop")

# ifdef
ifdefscope = rls.r_sequence(
    rls.r_character("%").ignore_token(),
    rls.r_char_sequence("sidef").ignore_token(),
    rls.r_identifier(),
    scope
)

define_k = rls.r_sequence(
    rls.r_character("%").ignore_token(),
    k_definis,
    rls.r_identifier()
)

definis_dans = rls.r_sequence(
    k_nomenclature,
    class_type_opt,
    identifier,
    k_dans,
    scope
).set_name("nomenclature")

utilise_k = rls.r_sequence(
    k_utilise,
    class_type_opt,
    identifier,
    rls.r_character(";").ignore_token()
).set_name("utilise-keyword")

# outside-of-smthg value
outside_expected_value = rls.r_sequence(
    attended_expression,
    rls.r_character(";").ignore_token().set_error("Expected ';'")
).set_name("outside-expected-value")

basic_rulling = [
    comment,
    ifdefscope,
    definis_dans,
    define_k,
    utilise_k,
    array_def,
    funcdef,
    funcdef_dec,
    classdef,
    ifstmt,
    forloop,
    whileloop,
    keyword_return,
    keyword_casse,
    keyword_syscall,
    keyword_extern,
    keyword_inclue,
    keyword_typedef,
    keyword_type_convert,
    scope,
    vardef,
    listed_value,
    parenthesis,
    outside_expected_value,
    colonm
]

func_arguments.set_mid_patern(
    rls.r_optional(
        rls.r_option(
            arguments,
            argument_def
        )   
    )
)

casting.sequence[2].add_option(value)
casting_args.set_mid_patern(
    attended_expression
)

parenthesis.set_mid_patern(
    rls.r_optional(
        rls.r_option(
            listed_value,
            attended_expression
        )   
    )
)

extern_func_parenth.set_mid_patern(
    rls.r_optional(
        rls.r_option(
            extern_arguments,
            extern_argument_def
        )   
    )
)

brackets.set_mid_patern(
    attended_expression
)

scope.set_evaluation(basic_rulling, tokenise, try_tokenise)