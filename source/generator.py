from source.wati_parser import *
from source.code_analysis import *
import source.inbuilt_functions as inb
import source.tokens as tok
import copy
from source.document import Document
from typing import Tuple
from source.parser_imp import *
import source.asm_optimiser as asm_optimiser
import os, sys

plateform = sys.platform
macos = sys.platform == "darwin"
print(f"Compilation pour '{plateform}'")


starting_label = "_start" if sys.platform == "darwin" else "_main"

basic = f"""
default rel

section .text
    global {starting_label}
"""

basic_end = f"""
  mov rax, {0x2000001 if macos else 1} ;; exit
  mov rdi, 0 ;; exit success
  syscall
"""

func_end = """
  ret
"""

exit_end = f"""
  mov rax, {0x2000001 if macos else 1} ;; exit
  pop rdi
  syscall
"""

IMPORTED = []

MALLOC_NAME = "malloc"
# FREE_NAME = "_free"

arg_register = [
    "rax",
    "rbx",
    "rcx",
    "rdx",
    "rsi",
    "rdi",
    "rsp",
    "rbp",
    "r8",
    "r9",
    "r10",
    "r11",
    "r12",
    "r13",
    "r14",
    "r15",
]

octo_reg = [
    "rax",
    "rbx",
    "rcx",
    "rdx",
    "rsi",
    "rdi",
    "rsp",
    "rbp",
    "r8",
    "r9",
    "r10",
    "r11",
    "r12",
    "r13",
    "r14",
    "r15",
]
tetr_reg = [
    "eax",
    "ebx",
    "ecx",
    "edx",
    "esi",
    "edi",
    "esp",
    "ebp",
    "r8d",
    "r9d",
    "r10d",
    "r11d",
    "r12d",
    "r13d",
    "r14d",
    "r15d",
]
bina_reg = [
    "ax",
    "bx",
    "cx",
    "dx",
    "si",
    "di",
    "sp",
    "bp",
    "r8w",
    "r9w",
    "r10w",
    "r11w",
    "r12w",
    "r13w",
    "r14w",
    "r15w",
]
unit_reg = [
    "al",
    "bl",
    "cl",
    "dl",
    "sil",
    "dil",
    "spl",
    "bpl",
    "r8b",
    "r9b",
    "r10b",
    "r11b",
    "r12b",
    "r13b",
    "r14b",
    "r15b",
]

arg_register = [
    "rax",
    "rbx",
    "rcx",
    "rdx",
    "rsi",
    "rdi",
    "r8",
    "r9",
    "r10",
    "r11",
    "r12",
    "r13",
    "r14",
    "r15",
]  # on enlève rbp et rsp (au cas où ...)
octo_reg = [
    "rax",
    "rbx",
    "rcx",
    "rdx",
    "rsi",
    "rdi",
    "r8",
    "r9",
    "r10",
    "r11",
    "r12",
    "r13",
    "r14",
    "r15",
]
tetr_reg = [
    "eax",
    "ebx",
    "ecx",
    "edx",
    "esi",
    "edi",
    "r8d",
    "r9d",
    "r10d",
    "r11d",
    "r12d",
    "r13d",
    "r14d",
    "r15d",
]
bina_reg = [
    "ax",
    "bx",
    "cx",
    "dx",
    "si",
    "di",
    "r8w",
    "r9w",
    "r10w",
    "r11w",
    "r12w",
    "r13w",
    "r14w",
    "r15w",
]
unit_reg = [
    "al",
    "bl",
    "cl",
    "dl",
    "sil",
    "dil",
    "r8b",
    "r9b",
    "r10b",
    "r11b",
    "r12b",
    "r13b",
    "r14b",
    "r15b",
]

INT_SIZE = 8

reg_size = {8: octo_reg, 4: tetr_reg, 2: bina_reg, 1: unit_reg}

word_size = {8: "qword", 4: "dword", 2: "word", 1: "byte"}

PATH = []


class LabelGenerator:
    def __init__(self) -> None:
        self.count = 0

    def label(self, modif="", base="Label"):
        lab = f"{base}{modif}{self.count}"
        self.count += 1
        return lab


class Bool:
    def __init__(self) -> None:
        self.bool = False

    def set_true(self):
        self.bool = True


class Classes:
    def __init__(
        self,
        name: str,
        attributes_name: list[str],
        attributes_type: list[str],
        methodes: dict[str : Tuple["Generator", list[tok.BasicToken], str]],
        iterator: bool,
    ) -> None:
        self.name = name
        self.att_name = attributes_name
        self.att_type = attributes_type
        self.methodes = methodes
        self.iterator = iterator


labels = LabelGenerator()


class Generator:
    def __init__(
        self,
        toks: list[tok.BasicToken],
        name=starting_label,
        end=basic_end,
        arg_num=0,
        ret_type="rien",
        ref="",
        shared_lib=False,
    ):
        self.toks = toks
        self.name = name
        self.real_name = ""
        self.generation = []
        self.reference = ref

        self.used = True
        self.inde = False
        self.shared_lib = shared_lib

        self.sim_stack = []
        self.scopes = []  # index in self.variables
        self.variables = []  # identifiers
        self.variables_info = {}  # identifier : (type, stack, extern)
        self.global_vars = {}  # identifiers : type
        self.sharded_library_imported = []  # name
        self.extern_functions = {}
        self.defined_types = {}  # typename : size
        self.undetermined = {}  # name : token
        self.undetermined_nomenclature = {}  # name : token
        self.defined = []
        self.functions: dict[str : Tuple[Generator, list[tok.BasicToken], str, bool]] = {}

        self.basic_end = end

        self.declared_string = []

        self.arg_num = arg_num
        self.args_type: dict[str:str] = {}  #
        self.ret_type = ret_type

        self.classes: dict[str, Classes] = {}

        if shared_lib:
            self.generation.append(f"global {name}")
        self.generation.append(f"{name}: ")
        
    def gen(self, stm):
        self.generation.append(stm)

    def generate_code(self, without_decoration=False):
        # information(f"Compilation de {self.name}", self.reference)
        if not without_decoration:
            if not self.used:
                return
            for i in range(self.arg_num):
                self.push_reg(arg_register[i])
            if self.name == starting_label:
                self.gen("  mov rax, plateforme_name")
                self.gen("  mov qword [__plateforme], rax")

        found_ret = False
        for i, tok in enumerate(self.toks):
            self.g_instruction(tok)
            if tok.get_rule() == keyword_return:
                found_ret = True

        if not without_decoration:
            if not (found_ret or self.ret_type == "rien"):
                error(
                    f"La fonction '{self.real_name}' ne renvoie pas quelque chose pour chaque possibilité",
                    self.reference,
                )

            self.g_end(self.basic_end)

            # Little check to know if we messed up with the pop/push function :
            assert (
                len(self.sim_stack) == 0
            ), f"The sim stack is not empty : {self.sim_stack} for {self.name}, {self.reference}"

    def add_function(self, name, value):
        self.functions[name] = value

    def generate_func(
        self,
    ):  # Quand des nouvelles fonctions sont définies dans une fonction pendant
        # la génération, ça merde. Donc il faut faire tampon
        function_stack = list(self.functions.keys())
        if self.name == starting_label:
            while len(function_stack) != 0:
                i = function_stack.pop(0)
                if self.functions[i][0] != None:  # and self.functions[i][0].used:
                    c = copy.copy(self.functions)
                    self.functions[i][0].functions = c
                    # print(list(c.keys()))
                    # if not self.functions[i][-1]:
                    #    error(f"La fonction '{i}' a été uniquement déclarée, pas implémenté, et ne peut pas conséquent pas être liée", self.functions[i][0].reference)

                    if self.functions[i][-1] != "func":
                        self.extern_functions[i] = self.nasm_footprint_name(i)
                        continue
                    self.functions[i][0].generate_code()
                    potent = []
                    for j in c:
                        if j not in self.functions:
                            potent.append(j)
                            self.functions[j] = self.functions[i][0].functions[j]
                    function_stack = potent + function_stack
                    self.generation += self.functions[i][0].generation

    def get_reg_size(self, reg):
        if reg in octo_reg:
            return 8
        if reg in tetr_reg:
            return 4
        if reg in bina_reg:
            return 2
        if reg in unit_reg:
            return 1
        assert False, f"Undefined register {reg}"

    def cores_reg(self, reg: str, size: int):
        register_ind = reg_size[size].index(reg)
        return reg_size[size][register_ind]

    def pop(self, register: str = ""):
        if register == "":
            register = reg_size[8][-1]
        self.gen(f"  xor {register}, {register}")  # Optionel mais plus safe
        size = self.get_reg_size(register)
        if size != 8:
            register_ind = reg_size[size].index(register)
            register = reg_size[8][register_ind]
        self.gen(f"  pop {register}")
        assert (
            len(self.sim_stack) != 0
        ), f"Le stack est vide {self.reference}, {self.name}, {self.sim_stack}"
        self.sim_stack.pop()

    def push_reg(self, register: str):
        size = self.get_reg_size(register)
        if size != 8:
            register_ind = reg_size[size].index(register)
            register = reg_size[8][register_ind]
        self.gen(f"  push {register}")
        self.sim_stack.append(8)

    def push_gen(self, register: str):
        self.gen(f"  push {register}")
        self.sim_stack.append(8)
        return 8

    def g_empty_vars(self):
        #if self.name == starting_label:
        #    print(self.variables)
        for _ in range(len(self.variables)):
            name = self.variables.pop()
            if name not in self.global_vars:
                #if self.name == starting_label:
                #    print(name)
                self.pop()
                self.variables_info.__delitem__(name)

    def g_end(self, end):
        self.g_empty_vars()
        if not self.shared_lib:
            self.gen(end)
        else:
            self.gen("  ret")

    def begin_scope(self):
        self.scopes.append(len(self.variables))

    def end_scope(self):
        n = self.scopes.pop()
        size = 0
        for i in range(n, len(self.variables)):
            name = self.variables.pop()
            if not name in self.global_vars:
                size += self.sim_stack.pop()
                self.variables_info.__delitem__(name)
        if size != 0:
            self.generation.append(f"  add rsp, {size}")

    def g_instruction(self, tok: tok.BasicToken):
        #if self.name == starting_label:
        #   tok.print(0)
        if tok.get_rule() == vardef:
            self.g_vardef(tok)
        elif tok.get_rule() == array_def:
            self.g_arraydef(tok)
        elif tok.get_rule() == scope:
            self.g_scope(tok)
        elif tok.get_rule() == ifstmt:
            self.g_ifstmt(tok)
        elif tok.get_rule() == whileloop:
            self.g_while(tok)
        elif tok.get_rule() == forloop:
            self.g_for(tok)
        elif tok.get_rule() == funcall:
            type_t = type(
                tok, self.variables_info, self.functions, self.classes, self.global_vars
            )
            self.g_funcall(tok)
            if type_t != "rien":
                self.pop("rax")
        elif tok.get_rule() == methcall:
            type_t = type(
                tok, self.variables_info, self.functions, self.classes, self.global_vars
            )
            self.g_methcall(tok)
            if type_t != "rien":
                self.pop("rax")
        elif tok.get_rule() == funcdef:
            self.g_funcdef(tok)
        elif tok.get_rule() == funcdef_dec:
            self.g_func_declaration(tok)
        elif tok.get_rule() == keyword_return:
            self.g_return(tok)
        elif tok.get_rule() == keyword_syscall:
            self.g_syscall(tok)
        elif tok.get_rule() == keyword_inclue:
            self.g_inclue(tok)
        elif tok.get_rule() == keyword_typedef:
            self.g_typdef(tok)
        elif tok.get_rule() == keyword_type_convert:
            self.g_convertype(tok)
        elif tok.get_rule() == keyword_extern:
            self.g_extern(tok)
        elif tok.get_rule() == utilise_k:
            self.g_utilise(tok)
        elif tok.get_rule() == definis_dans:
            self.g_nomenclature(tok)
        elif tok.get_rule() == classdef:
            self.g_classdef(tok)
        elif tok.get_rule() == outside_expected_value:
            t = type(
                tok.child[0],
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
            self.g_statement(tok.child[0])
            if t != "rien":
                self.gen(";; pop dans rax car innutile")
                self.pop("rax")
        elif tok.get_rule() == operator or tok.get_rule() == attended_expression:
            t = type(
                tok, self.variables_info, self.functions, self.classes, self.global_vars
            )
            self.g_statement(tok)
            if t != "rien":
                self.gen(";; pop dans rax car innutile")
                self.pop("rax")
        elif tok.get_rule() == define_k:
            self.g_define(tok)
        elif tok.get_rule() == ifdefscope:
            self.g_ifdef(tok)
        elif tok.get_rule() == colonm:
            pass
        else:
            pass

    def g_malloc(self, size: str):
        self.gen(f"  mov rdx, {size}")
        self.gen(f"  call {MALLOC_NAME}")

    def g_define(self, token: tok.BasicToken):
        name = token.child[0].content
        self.defined.append(name)

    def g_ifdef(self, token: tok.BasicToken):
        name = token.child[0].content
        scope = token.child[1]
        if name in self.defined:
            self.g_scope(scope)

    def g_return(self, token: tok.BasicToken):
        ret = token.child[0]
        if not isinstance(ret, tok.t_empty):
            type_t = type(
                ret, self.variables_info, self.functions, self.classes, self.global_vars
            )
            if type_t != self.ret_type:
                error(
                    f"Le type de retour '{type_t}' ne correspond pas au type attendu : '{self.ret_type}'",
                    token.reference,
                )
            self.g_statement(ret)
            self.pop("rax")
        elif self.ret_type != "rien":
            error(
                f"Le type de retour 'rien' ne correspond pas au type attendu : '{self.ret_type}'",
                token.reference,
            )
        for i in range(len(self.variables)):
            if self.variables[i] not in self.global_vars:
                self.gen("  pop r15 ;; unused values")
        self.gen("  ret")

    def g_inclue(self, token: tok.BasicToken):
        filename = token.child[0].child[0].content
        filename = filename.split("/")
        base = filename[:-1]
        head = filename[-1]
        for i in base:
            PATH.append(i)
            PATH.append("/")
        filename = ""
        for i in PATH:
            filename += i
        filename += head
        header_file = False
        if (
            head.split(".")[-1].lower() == "watih" and self.name == starting_label
        ):  # Meaning a header
            header_file = True
            _lab = self.nasm_footprint_name(filename)
            self.sharded_library_imported.append(filename.removesuffix(".watiH"))
            if not self.inde:
                self.gen(f"  call {_lab[:-1]}.shared_lib")
                self.extern_functions[f"{_lab[:-1]}.shared_lib"] = (
                    f"{_lab[:-1]}.shared_lib"
                )

        if filename not in IMPORTED:
            IMPORTED.append(filename)
            try:
                doc = Document(filename=filename)
            except FileNotFoundError:
                error(f"Fichier non trouvé : {filename}", token.reference)
            toks = tokenise(basic_rulling, doc, True)
            g = Generator(toks, name=filename, shared_lib=self.shared_lib)
            g.sim_stack = self.sim_stack
            g.functions = self.functions
            g.classes = self.classes
            g.declared_string = self.declared_string
            g.variables = self.variables
            g.global_vars = self.global_vars
            g.sharded_library_imported = self.sharded_library_imported
            g.variables_info = self.variables_info
            g.extern_functions = self.extern_functions
            g.undetermined = self.undetermined
            g.defined = self.defined

            self.gen(f";; debut de {filename}")
            g.generate_code(True)
            information(f"Importé '{filename}'", token.reference)
            
            # La règle : 
            # Si c'est un header et que le fichier est déclaré indépendant, alors on n'execute pas le code qui y est produit
            # dans tous les autres cas, on execute
            if not self.inde or not header_file:
                if g.shared_lib:
                    for i in g.generation[2:]:
                        self.generation.append(i)
                else:
                    for i in g.generation[1:]:
                        self.generation.append(i)
                self.gen(f";; fin de {filename}")
        for i in base:
            PATH.pop()
            PATH.pop()

    def g_syscall(self, token: tok.BasicToken):
        # RDI, RSI, RDX, R10, R8, and R9, in order. System call number goes into RAX.
        val = token.child[0]
        if val.get_rule() == listed_value:
            val = val.child
        else:
            val = [val]
        if len(val) > 7:
            error("Un syscall ne peut pas avoir plus de 7 arguments", token.reference)
        t0 = type(
            val[0], self.variables_info, self.functions, self.classes, self.global_vars
        )
        if t0 != "ent":
            error(
                f"Le premier arguement d'un syscall doit être un entier, pas '{t0}'",
                val[0].reference,
            )
        for arg in val[1:]:
            self.g_statement(arg)
        order = ["rdi", "rsi", "rdx", "r10", "r8", "r9"][: len(val) - 1]
        order.reverse()
        for i, arg in enumerate(val[1:]):
            self.pop(f"{order[i]}")
        num = 0x2000000 if sys.platform == "darwin" else 0
        self.g_statement(val[0])
        self.pop("rax")
        self.gen(f"  lea rax, [rax + {str(hex(num))}]")
        self.gen("  syscall")
        self.push_reg("rax")

        if "sys_ret_" in self.variables:
            error(
                f"La variabe 'sys_ret_' pour le retour du syscall a déjà été définie et ne peut pas être redéfinie",
                token,
            )
        size_t = 8  # Un ent
        self.variables.append("sys_ret_")
        self.variables_info["sys_ret_"] = ("ent", len(self.sim_stack), True, False)

    def g_typdef(self, token: tok.BasicToken):
        name = token.child[0].content
        size = int(token.child[1].content)
        # On ajoute la référence au type que on est entrain de définir
        TYPES.append(name)
        TYPES_SIZE[name] = size

        for op in operators_type:
            for i in operators_type[op]:
                i.append(False)
            operators_type[op].append([False] * len(operators_type[op][0]))

        for i in range(len(operators_type["=="])):
            operators_type["=="][type_int(name)][i] = "bool"
            operators_type["=="][i][type_int(name)] = "bool"
        for i in range(len(operators_type["!="])):
            operators_type["!="][type_int(name)][i] = "bool"
            operators_type["!="][i][type_int(name)] = "bool"

    def g_convertype(self, token: tok.BasicToken):
        name1 = token.child[0].content
        name2 = token.child[1].content
        if name1 not in TYPES:
            error(f"Type inconnu : '{name1}'", token.child[0].reference)
        if name2 not in TYPES:
            error(f"Type inconnu : '{name2}'", token.child[1].reference)

    def g_extern(self, token: tok.BasicToken):
        # token.print(0)
        ret_type = token.child[0]
        type_t = gettype(ret_type)
        name = token.child[1].content
        og_name = name
        args = token.child[2]
        name = self.footprint_name_def(name, args)

        self.add_function(name, (None, args, type_t, "externe"))
        self.extern_functions[name] = og_name

    def g_utilise(self, token: tok.BasicToken):
        # token.print()
        name = token.child[1].content
        type_usage_opt = token.child[0]
        if (
            name in self.undetermined
            and name + get_class_type(type_usage_opt) not in self.classes
        ):
            self.generate_undetermined(name, type_usage_opt)
        elif name in self.undetermined_nomenclature:
            self.generate_undetermined_nomenclature(name, type_usage_opt)
        elif name + get_class_type(type_usage_opt) in self.classes:
            print(
                f"Attention : déjà utilisé : '{name + get_class_type(type_usage_opt)}' : {token.reference}"
            )
        else:
            error(f"Nom inconnu de classe ou de nomenclature : {name}", token.reference)

    def g_nomenclature(self, token: tok.BasicToken):
        name = token.child[1].content
        self.undetermined_nomenclature[name] = token

    def g_func_print(self, token: tok.BasicToken):
        # Le chr sont de longueur 1
        self.g_statement(token)
        self.pop("rax")
        self.gen("  mov byte [chr_buffer], al")
        self.gen("  mov rdx, 1")
        self.gen("  mov rsi, qword chr_buffer")
        self.gen(f"  mov rax, {4 + 0x2000000 if macos else 0}")
        self.gen("  mov rdi, 1")
        self.gen("  syscall")

    def footprint_name_def(self, basename, args: tok.BasicToken):
        type_name = basename + "("
        if not isinstance(args.child[0], tok.t_empty):
            for i, v in enumerate(args.child):
                type_t = gettype(v.child[0])
                type_name += f"{type_t}," if i + 1 != len(args.child) else f"{type_t}"
        type_name += ")"
        return type_name

    def footprint_name_call(self, basename, args: list):
        type_name = basename + "("
        for i, v in enumerate(args):
            type_t = type(
                v, self.variables_info, self.functions, self.classes, self.global_vars
            )
            type_name += f"{type_t}," if i + 1 != len(args) else f"{type_t}"
        type_name += ")"
        return type_name

    def footprint_name_methode(self, self_arg, basename, args: list):
        type_name = basename + "(" + self_arg + ("," if len(args) != 0 else "")
        for i, v in enumerate(args):
            type_t = type(
                v, self.variables_info, self.functions, self.classes, self.global_vars
            )
            type_name += f"{type_t}," if i + 1 != len(args) else f"{type_t}"
        type_name += ")"
        return type_name

    def g_classdef(self, token: tok.BasicToken):
        # if self.name == starting_label :
        #    token.print(0)
        # if self.name != starting_label:
        #    error("Ne peut pas définir de classe en dehors de '_start'", token.reference)
        type_intern_def = token.child[0]
        name = token.child[1].content
        information(f"Définition de la classe {name}", token.reference)

        if not isinstance(
            type_intern_def, tok.t_empty
        ):  # type pas encore déterminé, la classe sera compilé à postériori
            self.undetermined[name] = token
            return

        iterator: Bool = not isinstance(token.child[2], tok.t_empty)

        attributes = token.child[3].child[0]
        methodes = token.child[3].child[1]

        att_names = [att.child[1].content for att in attributes.child]
        att_types = [gettype(att.child[0]) for att in attributes.child]

        # On ajoute la référence au type que on est entrain de définir
        TYPES.append(name)
        TYPES_SIZE[name] = sum([type_size(t) for t in att_types])

        # On rajoute la définition des opérateurs
        for op in operators_type:
            for i in operators_type[op]:
                i.append(False)
            operators_type[op].append([False] * len(operators_type[op][0]))

        # Deux opérateurs de base qui sont nécessaire au langage
        for i in range(len(operators_type["=="])):
            operators_type["=="][type_int(name)][i] = "bool"
            operators_type["=="][i][type_int(name)] = "bool"
        for i in range(len(operators_type["!="])):
            operators_type["!="][type_int(name)][i] = "bool"
            operators_type["!="][i][type_int(name)] = "bool"

        methodes_def = {}
        for m in methodes.child:
            ret_type = gettype(m.child[0])
            f_name = f"*{name}." + m.child[1].content
            args = m.child[2]
            footprint_name = self.footprint_name_def(f_name, args)
            if footprint_name[1:-1] in TYPES and m.child[1].content in OP_FUNC:
                modify_operators(
                    m.child[1].content, name, footprint_name[1:-1], ret_type
                )
            narg = len(args.child) if not isinstance(args.child[0], tok.t_empty) else 0
            if m.get_rule() == methodedef:
                scope = m.child[3]
                func = Generator(
                    scope.child,
                    f"{self.nasm_footprint_name(footprint_name)}",
                    func_end,
                    arg_num=narg,
                    ret_type=ret_type,
                    shared_lib=self.shared_lib,
                )
            else:
                if (
                    name in self.functions
                ):  # On a déjà le corp de la fonction, on ne va pas l'écraser avec "rien"
                    continue
                func = Generator(
                    tok.t_empty(True, None),
                    f"{self.nasm_footprint_name(footprint_name)}",
                    func_end,
                    arg_num=narg,
                    ret_type=ret_type,
                    shared_lib=self.shared_lib,
                )

            func.real_name = footprint_name
            func.functions = self.functions
            func.classes = self.classes
            func.declared_string = self.declared_string
            func.global_vars = self.global_vars
            func.sharded_library_imported = self.sharded_library_imported
            func.extern_functions = self.extern_functions
            func.undetermined = self.undetermined
            func.defined = self.defined
            func.used = True  # TODO: determine wether the method is used

            if not isinstance(args.child[0], tok.t_empty):
                for _, v in enumerate(args.child):  # Assignement of the argument
                    type_t = gettype(v.child[0])
                    func.variables.append(v.child[1].content)
                    func.variables_info[v.child[1].content] = (
                        type_t,
                        len(func.variables),
                        False,
                        False,
                    )
                    func.args_type[v.child[1].content] = type_t
            for i in self.global_vars:
                func.variables.append(i)
                func.variables_info[i] = (self.global_vars[i], 0, False, False)
            self.add_function(
                footprint_name, (func, args.child, ret_type, "func" if m.get_rule() == methodedef else "dec")
            )

        clss = Classes(name, att_names, att_types, methodes_def, iterator)
        self.classes[name] = clss

    def g_func_declaration(self, token: tok.BasicToken):
        ret_type = token.child[0]
        type_t = gettype(ret_type)
        name = token.child[1]
        name = token.child[1]
        if name.get_rule() == string:
            name = name.child[0].content
        elif isinstance(name.get_rule(), rls.r_sequence):
            name = name.child[0].content + "." + name.child[1].content
        else:
            name = name.content
        og_name = name
        args = token.child[2]
        name = self.footprint_name_def(name, args)

        arglen = len(args.child) if not isinstance(args.child[0], tok.t_empty) else 0

        if (
            name in self.functions
        ):  # On a déjà le corp de la fonction, on ne va pas l'écraser avec "rien"
            return

        func = Generator(
            tok.t_empty(True, None),
            f"{self.nasm_footprint_name(name)}",
            func_end,
            arg_num=arglen,
            ret_type=ret_type,
            ref=token.reference,
            shared_lib=self.shared_lib,
        )
        func.real_name = name
        func.functions = self.functions
        func.classes = self.classes
        func.declared_string = self.declared_string
        func.global_vars = self.global_vars
        func.sharded_library_imported = self.sharded_library_imported
        func.extern_functions = self.extern_functions
        func.undetermined = self.undetermined
        func.defined = self.defined
        # On assigne les arguments
        if not isinstance(args.child[0], tok.t_empty):
            for i, v in enumerate(args.child):
                type_t = gettype(v.child[0])
                func.variables.append(v.child[1].content)
                func.variables_info[v.child[1].content] = (
                    type_t,
                    len(func.variables),
                    False,
                    False,
                )
                func.args_type[v.child[1].content] = type_t

        # On rajoute bien sûr les variables globales à la liste de variables connues
        for i in self.global_vars:
            func.variables.append(i)
            func.variables_info[i] = (self.global_vars[i], 0, False, False)

        self.add_function(name, (func, args.child, gettype(ret_type), "dec"))

    def g_funcdef(self, token: tok.BasicToken):
        # if self.name != starting_label:
        #    error("Ne peut pas définir de fonction en dehors de '_start'", token.reference)

        name = token.child[1]
        if name.get_rule() == string:
            name = name.child[0].content
        elif isinstance(name.get_rule(), rls.r_sequence):
            name = name.child[0].content + "." + name.child[1].content
        else:
            name = name.content
        args = token.child[2]
        scope = token.child[3]

        ret_type = gettype(token.child[0])
        footprint_name = self.footprint_name_def(name, args)
        arglen = len(args.child) if not isinstance(args.child[0], tok.t_empty) else 0

        # On copie les informations utiles au code
        # global_vars est passé en tant que tel pour que la liste puisse être modifiée
        func = Generator(
            scope.child,
            f"{self.nasm_footprint_name(footprint_name)}",
            func_end,
            arg_num=arglen,
            ret_type=ret_type,
            ref=token.reference,
            shared_lib=self.shared_lib,
        )
        func.real_name = footprint_name
        func.functions = self.functions
        func.classes = self.classes
        func.declared_string = self.declared_string
        func.global_vars = self.global_vars
        func.sharded_library_imported = self.sharded_library_imported
        func.extern_functions = self.extern_functions
        func.undetermined = self.undetermined
        func.defined = self.defined
        # On assigne les arguments
        if not isinstance(args.child[0], tok.t_empty):
            for i, v in enumerate(args.child):
                type_t = gettype(v.child[0])
                func.variables.append(v.child[1].content)
                func.variables_info[v.child[1].content] = (
                    type_t,
                    len(func.variables),
                    False,
                    False,
                )
                func.args_type[v.child[1].content] = type_t

        # On rajoute bien sûr les variables globales à la liste de variables connues
        for i in self.global_vars:
            func.variables.append(i)
            func.variables_info[i] = (self.global_vars[i], 0, False, False)

        if footprint_name in self.functions:
            print(
                f"Redéfinition de {footprint_name} : {token.reference} , définis ici : {self.functions[footprint_name][0].reference}"
            )
        self.add_function(footprint_name, (func, args.child, ret_type, "func"))

    def nasm_footprint_name(self, name):
        # TODO: Trouver le moyen d'avoir un caractère unique pour le remplacement
        return "_" + name.replace("(", "_").replace(")", "_").replace(",", "_").replace(
            "[", "_"
        ).replace("]", "_").replace("*", "ptr").replace("<", "..").replace(
            ">", ".."
        ).replace(
            "/", "_"
        )

    def g_funcall(self, token: tok.BasicToken):
        name = token.child[0].content
        args = token.child[1]
        footprint_name = get_funcall_name(
            token, self.variables_info, self.functions, self.classes, self.global_vars
        )
        if args.child[0].get_rule() != listed_value:
            args = [args.child[0]]
        else:
            args = args.child[0].child

        if footprint_name in self.extern_functions:
            for arg in args:
                self.g_statement(arg)
            if footprint_name in self.functions and self.functions[footprint_name][-1] in ["func", "dec"] :
                order = arg_register[: len(args)]
            else:
                print(footprint_name)
                order = ["rdi", "rsi", "rdx", "r10", "r8", "r9"][: len(args)]
            order.reverse()
            if len(args) > len(order):
                error(
                    f"Trop d'arguments passés, maximum : {len(order)}", token.reference
                )
            for i, arg in enumerate(args):
                self.pop(f"{order[i]}")
            self.gen(f"  call {self.extern_functions[footprint_name]}")
            _, _, ret_type, _ = self.functions[footprint_name]
            if ret_type != "rien":
                self.push_reg("rax")
            return type_size(ret_type)

        if footprint_name not in self.functions:
            error(
                f"La fonction '{footprint_name}' n'est pas déclarée",
                token.child[0].reference,
            )
        func_gen, f_args, ret_type, _ = self.functions[footprint_name]
        func_gen.used = True
        if func_gen.arg_num != len(args):
            error(
                f"Mauvais nombre d'arguments indiqué : {footprint_name} requiers {len(f_args)} argument{'s' if len(f_args) > 1 else ''}"
            )
        f_args_name = [f_args[i].child[1].content for i in range(func_gen.arg_num)]
        f_args_type = [func_gen.args_type[i] for i in f_args_name]
        for i, v in enumerate(args):
            type_t = type(
                v, self.variables_info, self.functions, self.classes, self.global_vars
            )
            if type_t != f_args_type[i]:
                error(
                    f"Mauvais type ('{type_t}') pour l'arguement '{f_args_name[i]}', il est sensé être du type '{f_args_type[i]}'"
                )
            self.g_statement(v)
        for i in range(len(args)):
            self.pop(arg_register[len(args) - i - 1])
        self.gen(f"  call {self.nasm_footprint_name(footprint_name)}")
        if ret_type != "rien":
            self.push_gen("rax")  # TODO: modify this
        return type_size(ret_type)

    def g_methcall(self, token: tok.BasicToken):
        footprint_name = get_methcall_name(
            token, self.variables_info, self.functions, self.classes, self.global_vars
        )
        args = token.child[1]
        if args.child[0].get_rule() != listed_value:
            args = [args.child[0]]
        elif isinstance(args.child[0], tok.t_empty):
            args = []
        else:
            args = args.child[0].child
        if footprint_name not in self.functions:
            error(f"Fonction inconnue : '{footprint_name}'", token.reference)

        func_gen, f_args, ret_type, _ = self.functions[footprint_name]
        if func_gen.arg_num - 1 != len(args):
            error(
                f"Mauvais nombre d'arguments, {footprint_name} requiers {func_gen.arg_num - 1} argument{'s' if func_gen.arg_num > 2 else ''}",
                token.reference,
            )
        f_args_name = [f_args[i].child[1].content for i in range(func_gen.arg_num)]
        f_args_type = [func_gen.args_type[i] for i in f_args_name]
        for i, v in enumerate(args):
            type_t = type(
                v, self.variables_info, self.functions, self.classes, self.global_vars
            )
            if type_t != f_args_type[i + 1]:
                error(
                    f"Mauvais type ('{type_t}') pour le {i + 1} ème argument '{f_args_name[i + 1]}', il devraît être '{f_args_type[i + 1]}', pas '{type_t}'",
                    token.reference,
                )
            self.g_statement(v)

        soit = token.child[0]
        soit_bis = copy.copy(soit)
        soit_bis.child = copy.copy(soit.child)
        soit_bis.child.pop()
        
        self.g_statement(soit_bis)
        self.pop(arg_register[0])
        for i in range(len(args)):
            self.pop(arg_register[len(args) - i])
        self.generation.append(
            f"  call {self.nasm_footprint_name(footprint_name)}"
        )
        if ret_type != "rien":
            self.push_reg("rax")

    def generate_undetermined_nomenclature(
        self, name: str, class_type_opt: tok.BasicToken
    ):
        following = get_class_type(class_type_opt)
        token: tok.BasicToken = self.undetermined_nomenclature[name]
        names = token.child[0]
        if isinstance(names.child[0], tok.t_empty):
            names = names.child[1:]
        else:
            names = names.child
        if isinstance(class_type_opt.child[0], tok.t_empty):
            class_type_opt = class_type_opt.child[1:]
        else:
            class_type_opt = class_type_opt.child
        if len(class_type_opt) != len(names):
            error(
                f"Mauvais nombre de types indiqué pour l'utilisation",
                class_type_opt.reference,
            )
        type_names = [t.content for t in names]
        token = token.child[2].modify(type_names, class_type_opt)
        information(f"Généré : {name + following}")
        self.g_scope(token)

    def generate_undetermined(self, name: str, class_type_opt: tok.BasicToken):
        following = get_class_type(class_type_opt)
        token: tok.BasicToken = self.undetermined[name]
        names = token.child[0]
        if isinstance(names.child[0], tok.t_empty):
            names = names.child[1:]
        else:
            names = names.child
        if isinstance(class_type_opt.child[0], tok.t_empty):
            class_type_opt = class_type_opt.child[1:]
        else:
            class_type_opt = class_type_opt.child
        if len(class_type_opt) != len(names):
            error(
                f"Mauvais nombre de types indiqué pour la définition de la classe",
                class_type_opt.reference,
            )
        type_names = [t.content for t in names]
        token = token.modify(type_names, class_type_opt)
        token.child[0] = tok.t_empty(False, rls.r_character("X"))
        token.child[1].content = name + following
        self.g_classdef(token)

    def g_classcall(self, token: tok.BasicToken):
        # if self.name == starting_label:
        #    token.print(0)
        class_type_opt = token.child[0]
        name = token.child[1].content
        if (
            name in self.undetermined
            and name + get_class_type(class_type_opt) not in self.classes
        ):
            self.generate_undetermined(name, class_type_opt)
        if (
            not isinstance(class_type_opt, tok.t_empty)
            and name + get_class_type(class_type_opt) in self.classes
        ):
            name = name + get_class_type(class_type_opt)

        # stack_alloced = not isinstance(token.child[2], tok.t_empty)
        if False:
            assert False, "Pas implémenté"
        else:
            args = token.child[2]
            if name not in self.classes:
                error(f"Nom de classe inconnue : '{name}'", token.child[1].reference)
            clss = self.classes[name]
            size = sum([type_size(t) for t in clss.att_type])

            footprint_name = f"*{name}.constructeur"
            if args.child[0].get_rule() != listed_value:
                args = [args.child[0]]
            elif isinstance(args.child[0], tok.t_empty):
                args = []
            else:
                args = args.child[0].child

            footprint_name = self.footprint_name_methode(
                "*" + name, footprint_name, args
            )

            if footprint_name not in self.functions:
                error(f"Fonction non définie : {footprint_name}", token.reference)
            func_gen, f_args, ret_type, _ = self.functions[footprint_name]
            func_gen.used = True
            if func_gen.arg_num - 1 != len(args):
                error(
                    f"Mauvais nombre d'arguments, {footprint_name} requiers {func_gen.arg_num - 1} argument{'s' if func_gen.arg_num > 2 else ''}",
                    token.reference,
                )
            if ret_type != f"*{name}":
                error(
                    f"Le constructeur devrait renvoyer '*{name}', pas '{ret_type}'",
                    func_gen.reference,
                )
            f_args_name = [f_args[i].child[1].content for i in range(func_gen.arg_num)]
            f_args_type = [func_gen.args_type[i] for i in f_args_name]

            for i, v in enumerate(args):
                type_t = type(
                    v,
                    self.variables_info,
                    self.functions,
                    self.classes,
                    self.global_vars,
                )
                if type_t != f_args_type[i + 1]:
                    error(
                        f"Mauvais type '{type_t}' pour l'arguement '{f_args_name[i + 1]}', il devraît être de type '{f_args_type[i]}'",
                        token.reference,
                    )
                self.g_statement(v)
            for i in range(len(args)):
                self.pop(arg_register[func_gen.arg_num - i - 1])
            self.gen(
                "  mov rax, 0"
            )  # Le pointeur doit être initialisé dans constructeur
            self.gen(f"  call {self.nasm_footprint_name(footprint_name)}")  # Grossier
            self.push_reg("rax")
            return 8  # La taille d'un pointeur

    def g_scope(self, toks: tok.BasicToken):
        self.begin_scope()
        for i, tok in enumerate(toks.child):
            self.g_instruction(tok)
        self.end_scope()

    def g_elif(self, tok: tok.BasicToken, end_label: str):
        condition = tok.child[0]
        self.g_statement(condition)
        self.pop("rax")
        lab = labels.label()
        self.generation.append("  test rax, rax")
        self.generation.append(f"  jz {lab}")
        self.g_scope(tok.child[1])
        self.generation.append(f"  jmp {end_label}")
        self.generation.append(lab + ":")

    def g_else(self, tok: tok.BasicToken, end_label: str):
        self.g_scope(tok.child[0])
        self.generation.append(f"  jmp {end_label}")

    def g_ifstmt(self, token: tok.BasicToken):
        condition = token.child[0]
        self.g_statement(condition)
        self.pop("rax")  # The condition
        if_lab = labels.label()  # The if stmt label
        if_lab_end = labels.label("End")  # The end label
        self.generation.append("  test rax, rax")  # The test
        self.generation.append(f"  jz {if_lab}")  # is it false ?

        self.g_scope(token.child[1])
        self.generation.append(f"  jmp {if_lab_end}")
        self.generation.append(f"{if_lab}:")
        if not isinstance(token.child[2], tok.t_empty):  # there are elifs
            for t in token.child[2].child:
                self.g_elif(t, if_lab_end)

        if not isinstance(token.child[3], tok.t_empty):  # there is an else
            self.g_else(token.child[3], if_lab_end)
        self.generation.append(f"{if_lab_end}:")

    def g_while(self, token: tok.BasicToken):
        condition = token.child[0]
        code = token.child[1]
        label_begin = labels.label("WhileBegin")
        label_end = labels.label("WhileEnd")
        self.generation.append(f"{label_begin}:")
        self.g_statement(condition)
        self.pop("rax")  # The condition
        self.generation.append("  test rax, rax")  # The test
        self.generation.append(f"  jz {label_end}")  # is it false ?
        self.g_scope(code)
        self.generation.append(f"  jmp {label_begin}")
        self.generation.append(f"{label_end}:")

    def g_for(self, token: tok.BasicToken):
        var_name = token.child[0].content
        value = token.child[1]
        scope = token.child[2]
        if value == attended_expression:
            type_t = type(
                value,
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
            if not is_ptr(type_t):
                error(
                    f"La valeur dans la boucle 'pour' doît être un objet",
                    value.reference,
                )
            type_t = get_ptr_type(type_t)
            if type_t not in self.classes:
                error(f"La classe '{type_t}' n'existe pas", value.reference)
            clss: Classes = self.classes[type_t]
            if not clss.iterator:
                error(f"L'objet doit être un itérateur", value.reference)
            fname_deb = f"*{type_t}.debut(*{type_t})"
            fname_svt = f"*{type_t}.suivant(*{type_t})"
            fname_fin = f"*{type_t}.fin(*{type_t})"
            if (
                fname_deb not in self.functions
                or fname_fin not in self.functions
                or fname_svt not in self.functions
            ):
                error(
                    f"Un itérateur doit posséder une méthode '.debut()', une méthode '.suivant()' et une méthode '.fin()'",
                    value.reference,
                )

            f_debG: Generator
            f_deb_rettype: str
            f_svtG: Generator
            f_svt_rettype: str
            f_finG: Generator
            f_fin_rettype: str
            f_debG, _, f_deb_rettype, _ = self.functions[fname_deb]
            f_svtG, _, f_svt_rettype, _ = self.functions[fname_svt]
            f_finG, _, f_fin_rettype, _ = self.functions[fname_fin]
            if f_fin_rettype != "bool":
                error(
                    f"Le type de retour de la méthode '{fname_fin}' doit être 'bool', pas '{f_fin_rettype}'",
                    value.reference,
                )
            if f_deb_rettype != "rien":
                error(f"La méthode '{fname_deb}' doit être 'rien'", value.reference)
            if f_svt_rettype == "rien":
                error(
                    f"La méthode '{fname_svt}' ne doit pas être 'rien'", value.reference
                )

            footprint_n_deb = self.nasm_footprint_name(fname_deb)
            footprint_n_svt = self.nasm_footprint_name(fname_svt)
            footprint_n_fin = self.nasm_footprint_name(fname_fin)
            self.gen(f"  mov rax, 0")
            self.push_reg("rax")
            if var_name in self.variables:
                error(
                    f"La variable '{var_name}' est déjà définie",
                    token.child[0].reference,
                )
            self.variables.append(var_name)
            self.variables_info[var_name] = (f_svt_rettype, len(self.sim_stack), False, False)
            pos = len(self.sim_stack)

            self.g_statement(value)

            self.gen("mov rax, [rsp]")
            self.gen(f"  call {footprint_n_deb}")

            labdeb = labels.label("debut", "forloop")
            labfin = labels.label("fin", "forloop")

            self.gen(f"{labdeb}:")

            self.gen("mov rax, [rsp]")
            self.gen(f"  call {footprint_n_svt}")
            decal = sum(self.sim_stack[pos:])
            self.gen(f"  mov qword [rsp + {decal}], rax")

            self.g_scope(scope)

            self.gen("mov rax, [rsp]")
            self.gen(f"  call {footprint_n_fin}")
            self.gen(f"  cmp rax, 0")
            self.gen(f"  jpe {labdeb}")
            self.pop("rax")  # L'itérateur
            self.pop("rax")  # L'indexateur
            self.variables.pop(self.variables.index(var_name))
            self.variables_info.__delitem__(var_name)
        elif value == forloop_brackets:
            value = value.child[0]
            val1 = value.child[0]
            val2 = value.child[1]
            t1 = type(
                val1,
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
            t2 = type(
                val2,
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
            if t1 != "ent" or t2 != "ent":
                error(
                    f"Les deux bornes de la boucle doivent être de type 'ent', pas '{t1}' et '{t2}",
                    value.reference,
                )
            self.g_statement(val2)
            self.g_statement(val1)
            self.gen("  mov rax, qword [rsp]")
            self.push_reg("rax")
            if var_name in self.variables:
                error(
                    f"La variable '{var_name}' est déjà définie",
                    token.child[0].reference,
                )
            self.variables.append(var_name)
            self.variables_info[var_name] = ("ent", len(self.sim_stack), False, False)
            pos = len(self.sim_stack)
            labdeb = labels.label("debut", "forloop")
            labfin = labels.label("fin", "forloop")
            # Début de la boucle
            # Sur le stack on a [index, val1, val2, ...]
            self.gen(f"{labdeb}:")
            self.gen("  mov rax, qword [rsp]")
            self.gen("  mov rbx, qword [rsp + 16]")
            self.gen(f"  cmp rax, rbx")
            self.gen(f"  jns {labfin}")
            self.g_scope(scope)
            self.pop("rax")
            self.gen("inc rax")
            self.push_reg("rax")
            self.gen(f"  jmp {labdeb}")
            self.gen(f"{labfin}:")
            self.pop("rax")  # L'itérateur
            self.pop("rax")  # La borne haute/basse (jsp)
            self.pop("rax")  # La borne haute/basse
            self.variables.pop(self.variables.index(var_name))
            self.variables_info.__delitem__(var_name)

    def g_identifier(self, token: tok.BasicToken):
        n = token.content
        if n in self.global_vars:
            self.push_gen(f"qword [{n}]")
            return 8
        if not n in self.variables:
            error(f"Identifiant inconnu '{n}'", token.reference)

        type_t, pos, free, _ = self.variables_info[n]
        shift = 0
        for i in self.sim_stack[pos:]:
            shift += i
        self.push_gen(f"qword [rsp + {shift}]")
        return type_size(type_t)

    def g_operator_add(self, float=False):
        self.generation.append(f"  {'f' if float else ''}add rax, rbx")

    def g_operator_sub(self, float=False):
        self.generation.append(f"  {'f' if float else ''}sub rax, rbx")

    def g_operator_mul(self, float=False):
        self.generation.append(f"  {'f' if float else ''}mul rbx")

    def g_operator_div(self, float=False):
        self.generation.append("  xor rdx, rdx")
        self.generation.append(f"  {'f' if float else ''}div rbx")

    def g_operator_modulo(self, float=False):
        self.generation.append("  xor rdx, rdx")
        self.generation.append("  div rbx")
        self.generation.append("  mov rax, rdx")

    def g_operator_and(self, float=False):
        self.generation.append("  and rax, rbx")
    
    def g_operator_left_shift(self, float=False):
        self.generation.append("  shl rax, rbx")
        
    def g_operator_right_shift(self, float=False):
        self.generation.append("  shr rax, rbx")

    def g_operator_or(self, float=False):
        self.generation.append("  or rax, rbx")

    def g_operator_less_than(self, float=False):
        lab1 = labels.label("Lt")
        lab2 = labels.label("Lt")
        self.generation.append("  cmp rax, rbx")
        self.generation.append(f"  jns {lab2}")
        self.generation.append("  mov rax, 1")
        self.generation.append(f"  jmp {lab1}")
        self.generation.append(f"{lab2}:")
        self.generation.append("  mov rax, 0")
        self.generation.append(f"{lab1}:")

    def g_operator_less_eq_than(self, float=False):
        lab1 = labels.label("Lt")
        lab2 = labels.label("Lt")
        self.generation.append("  inc rbx")
        self.generation.append("  cmp rax, rbx")
        self.generation.append(f"  jns {lab2}")
        self.generation.append("  mov rax, 1")
        self.generation.append(f"  jmp {lab1}")
        self.generation.append(f"{lab2}:")
        self.generation.append("  mov rax, 0")
        self.generation.append(f"{lab1}:")

    def g_operator_more_than(self, float=False):
        lab1 = labels.label("Mt")
        lab2 = labels.label("Mt")
        self.generation.append("  inc rbx")
        self.generation.append("  cmp rax, rbx")
        self.generation.append(f"  js {lab2}")
        self.generation.append("  mov rax, 1")
        self.generation.append(f"  jmp {lab1}")
        self.generation.append(f"{lab2}:")
        self.generation.append("  mov rax, 0")
        self.generation.append(f"{lab1}:")

    def g_operator_more_eq_than(self, float=False):
        lab1 = labels.label("Mt")
        lab2 = labels.label("Mt")
        self.generation.append("  cmp rax, rbx")
        self.generation.append(f"  js {lab2}")
        self.generation.append("  mov rax, 1")
        self.generation.append(f"  jmp {lab1}")
        self.generation.append(f"{lab2}:")
        self.generation.append("  mov rax, 0")
        self.generation.append(f"{lab1}:")

    def g_operator_equals(self, float=False):
        lab1 = labels.label("Eq")
        lab2 = labels.label("Eq")
        self.generation.append("  cmp rax, rbx")
        self.generation.append(f"  je {lab1}")
        self.generation.append("  mov rax, 0")
        self.generation.append(f"  jmp {lab2}")
        self.generation.append(f"{lab1}:")
        self.generation.append("  mov rax, 1")
        self.generation.append(f"{lab2}:")

    def g_basic_eq(self, float=False):
        lab1 = labels.label("Eq")
        lab2 = labels.label("Eq")
        self.generation.append("  cmp rax, rbx")
        self.generation.append(f"  je {lab1}")
        self.generation.append("  mov rax, 0")
        self.generation.append(f"  jmp {lab2}")
        self.generation.append(f"{lab1}:")
        self.generation.append("  mov rax, 1")
        self.generation.append(f"{lab2}:")

    def g_operator_not_equals(self, float=False):
        lab1 = labels.label("Neq")
        lab2 = labels.label("Neq")
        self.generation.append("  cmp rax, rbx")
        self.generation.append(f"  jne {lab1}")
        self.generation.append("  mov rax, 0")
        self.generation.append(f"  jmp {lab2}")
        self.generation.append(f"{lab1}:")
        self.generation.append("  mov rax, 1")
        self.generation.append(f"{lab2}:")

    def g_basic_neq(self):
        lab1 = labels.label("Neq")
        lab2 = labels.label("Neq")
        self.generation.append("  cmp rax, rbx")
        self.generation.append(f"  jne {lab1}")
        self.generation.append("  mov rax, 0")
        self.generation.append(f"  jmp {lab2}")
        self.generation.append(f"{lab1}:")
        self.generation.append("  mov rax, 1")
        self.generation.append(f"{lab2}:")

    def g_operator(self, tok: tok.BasicToken) -> bool:
        lhs = tok.child[0]
        lhs_type = type(
            lhs, self.variables_info, self.functions, self.classes, self.global_vars
        )
        lhs_size = self.g_statement(lhs)
        rhs = tok.child[1]
        rhs_type = type(
            rhs, self.variables_info, self.functions, self.classes, self.global_vars
        )
        rhs_size = self.g_statement(rhs)

        ret_type = type(
            tok, self.variables_info, self.functions, self.classes, self.global_vars
        )
        op = tok.content
        
        #print(f"{lhs_type}.{FUNC_OP[op]}({lhs_type},{rhs_type})")
        #print(list(self.functions.keys()))
        if f"{lhs_type}.{FUNC_OP[op]}({lhs_type},{rhs_type})" in self.functions:
            self.pop("rbx")
            self.pop("rax")
            self.generation.append(
                f"  call {self.nasm_footprint_name(f'{lhs_type}.{FUNC_OP[op]}({lhs_type},{rhs_type})')}"
            )
            self.push_reg("rax")
            _, _, ret_type, _ = self.functions[
                f"{lhs_type}.{FUNC_OP[op]}({lhs_type},{rhs_type})"
            ]
            return type_size(ret_type)
        if lhs_type == "ent":
            self.pop("rbx")
            self.pop("rax")
            if op == "+":
                self.g_operator_add(float=False)
            elif op == "-":
                self.g_operator_sub(float=False)
            elif op == "*":
                self.g_operator_mul(float=False)
            elif op == "/":
                self.g_operator_div(float=False)
            elif op == "%":
                self.g_operator_modulo(float=False)
            elif op == "<":
                self.g_operator_less_than(float=False)
            elif op == ">":
                self.g_operator_more_than(float=False)
            elif op == "<=":
                self.g_operator_less_eq_than(float=False)
            elif op == ">=":
                self.g_operator_more_eq_than(float=False)
            elif op == "==":
                self.g_operator_equals(float=False)
            elif op == "!=":
                self.g_operator_not_equals(float=False)
            elif op == "||":
                self.g_operator_or(float=False)
            elif op == "&&":
                self.g_operator_and(float=False)
            elif op == "<<":
                self.g_operator_left_shift(float=False)
            elif op == ">>":
                self.g_operator_right_shift(float=False)
            # elif op == "^":
            #    self.g_operator_pow()
            self.push_reg("rax")
            return False
        if lhs_type == "bool":
            self.pop("rbx")
            self.pop("rax")
            if op == "&&":
                self.g_operator_and()
            elif op == "||":
                self.g_operator_or()
            self.push_reg("rax")
            return False
        if op == "==":
            self.pop("rbx")
            self.pop("rax")
            self.g_basic_eq()
            self.push_reg("rax")
            return False
        if op == "!=":
            self.pop("rbx")
            self.pop("rax")
            self.g_basic_neq()
            self.push_reg("rax")
            return False
        error(
            f"Opérateur '{op}' incompatible entre {lhs_type} et {rhs_type}",
            tok.reference,
        )

    def g_array_access(self, token: tok.BasicToken):
        name = token.child[0]
        type_t = type(
            name, self.variables_info, self.functions, self.classes, self.global_vars
        )
        index = token.child[1].child[0]
        i_type = type(
            index, self.variables_info, self.functions, self.classes, self.global_vars
        )
        if i_type != "ent":
            error(
                f"Le type de l'index devrait être 'ent', pas '{i_type}'",
                token.reference,
            )
        if is_ptr(type_t):
            type_t_bis = get_ptr_type(type_t)
            if type_t_bis not in self.classes:
                error(
                    f"Nom de classe ou d'objet inconnu : '{type_t_bis}'",
                    token.reference,
                )
            func_name = f"{type_t}.index({type_t},ent)"
            if func_name not in self.functions:
                error(
                    f"Méthode inconnue : '{func_name}'. La classe doit contenir une méthode '.index({type_t},ent)' pour supporter l'indexation",
                    token.reference,
                )
            funcgen, args, rettype, _ = self.functions[func_name]
            if rettype == "rien":
                error(
                    f"Le type de retour de la fonction '{func_name}' ne devrait pas être 'rien'",
                    token.reference,
                )
            self.g_statement(index)
            self.g_statement(name)
            # order : ["rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rsp", "rbp", "r8", ...]
            self.pop(arg_register[0])
            self.pop(arg_register[1])
            self.gen(f"  call {self.nasm_footprint_name(func_name)}")
            self.push_reg("rax")
            return rettype
        elif not is_liste(type_t):
            error(f"Le type doit être liste[]", token.reference)

        self.g_statement(name)
        self.g_statement(index)
        self.pop("rdx")
        self.pop("rax")
        type_t_list = get_liste_type(type_t)
        self.generation.append(f"  lea rax, [rax + rdx*{type_size(type_t_list)}]")
        self.gen("  xor rbx, rbx")
        self.gen(
            f"  mov {reg_size[type_size(type_t_list)][1]}, {word_size[type_size(type_t_list)]} [rax]"
        )
        self.push_reg("rbx")
        return get_liste_type(type_t)

    def g_statement(self, token: tok.BasicToken) -> int:
        if token.get_rule() == operator:
            t = type(
                token,
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
            self.g_operator(token)
            return type_size(t)
        if token.get_rule() == int_:
            # ints are 8 bytes long
            self.gen(f"  mov {reg_size[8][0]}, {token.content}")
            self.push_reg(reg_size[8][0])
            return type_size("ent")
        if token.get_rule() == hex_int:
            self.gen(f"  mov {reg_size[8][0]}, 0x{token.child[0].content.lower()}")
            self.push_reg(reg_size[8][0])
            return type_size("ent")
        if token.get_rule() == char:
            content = token.child[0].content
            if len(content) > 1:
                error(
                    f"La définition de 'chr' doit être de longeure 1 ou 0",
                    token.reference,
                )
            if len(content) == 0:
                self.gen("  mov rax, 0")
            else:
                self.gen(f"  mov rax, {ord(content)}")
            self.push_reg("rax")
            return type_size("chr")
        if token.get_rule() == string:
            content = token.child[0].content
            l = len(content)
            # content = bytes(content, "utf-8").decode("unicode_escape")
            if not content.replace("\n", '", 10 ,"') in self.declared_string:
                self.declared_string.append(content.replace("\n", '", 10 ,"'))
            g = self.declared_string.index(content.replace("\n", '", 10 ,"'))
            self.gen(f"  mov rax, qword msg{g}")
            self.push_reg("rax")
            # self.gen(f"  mov rdx, {l + 2}") # +2 encore une fois, jsp pourquoi
            # self.g_malloc("rdx")
            # self.gen(f"  mov rdi, rax")
            # self.pop(f"rsi")
            # self.push_reg(f"rdi")
            # self.gen(f"  call chr_copy")
            return 8  # C'est un pointeur
        if token.get_rule() == t_bool:
            self.gen("  xor rax, rax")
            if token.content != "Vrai" and token.content != "Faux":
                error(f"Pas un vrai booléen : {token.content}", token.reference)
            if token.content == "Vrai":
                self.gen(f"  mov rax, 1")
            else:
                self.gen(f"  mov rax, 0")
            self.push_reg("rax")
            return type_size("bool")
        if token.get_rule() == identifier:
            return self.g_identifier(token)
        if token.get_rule() == casting:
            type_t_dest = type(
                token,
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
            type_t_orgn = type(
                token.child[1],
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
            if not type_exists(type_t_dest):
                error(f"Type inconnu : '{type_t_dest}'", token.reference)
            size = type_size(type_t_dest)
            self.g_statement(token.child[1])
            self.pop("rax")
            self.gen("  xor rbx, rbx")
            self.gen(f"  mov {reg_size[size][1]}, {reg_size[size][0]}")
            self.push_reg("rbx")
            return size
        if token.get_rule() == parenthesis or token.get_rule() == casting_args:
            return self.g_statement(token.child[0])
        if token.get_rule() == funcall:
            return self.g_funcall(token)
        if token.get_rule() == sizeof_funcall:
            t = token.child[0].child[0]
            type_t = gettype(t)
            size = TYPES_SIZE[type_t]
            self.gen(f"  mov rax, {size}")
            self.push_reg("rax")
            return 8
        if token.get_rule() == typeof_funcall:
            t = token.child[0].child[0]
            type_t = type(
                t, self.variables_info, self.functions, self.classes, self.global_vars
            )
            l = len(type_t)
            if not type_t.replace("\n", '", 10 ,"') in self.declared_string:
                self.declared_string.append(type_t.replace("\n", '", 10 ,"'))
            g = self.declared_string.index(type_t.replace("\n", '", 10 ,"'))
            self.gen(f"  mov rax, qword msg{g}")
            self.push_reg("rax")
            return 8
        if token.get_rule() == attr_exist:
            value = token.child[0].child[0].child[0]
            t = token.child[0].child[0].child[1].child[0].content
            type_t = type(
                value,
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
            if not is_ptr(type_t):
                error(
                    f"!attribut_existe : Le premier argument n'est pas un objet ('{type_t}')"
                )
            type_t = get_ptr_type(type_t)
            if type_t not in self.classes:
                error(
                    f"!attribut_existe : Le premier argument n'est pas un objet ('{type_t}')",
                    token.reference,
                )
            clss = self.classes[type_t]
            if t in clss.att_name:
                self.gen("  mov rax, 1")
            else:
                self.gen("  mov rax, 0")
            self.push_reg("rax")
            return type_size("bool")
        if token.get_rule() == meth_exist:
            value = token.child[0].child[0].child[0]
            t = token.child[0].child[0].child[1].child[0].content
            type_t = type(
                value,
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
            if f"{type_t}.{t}" in self.functions:
                self.gen("  mov rax, 1")
            else:
                self.gen("  mov rax, 0")
            self.push_reg("rax")
            return type_size("bool")
        if token.get_rule() == func_exist:
            t = token.child[0].child[0].child[0].content
            if t in self.functions:
                self.gen("  mov rax, 1")
            else:
                self.gen("  mov rax, 0")
            self.push_reg("rax")
            return type_size("bool")
        if token.get_rule() == methcall:
            return self.g_methcall(token)
        if token.get_rule() == classcall:
            return self.g_classcall(token)
        if token.get_rule() == array_accession:
            return type_size(self.g_array_access(token))
        if token.get_rule() == attribute_identifier:
            type_t = self.g_attribute_identifier(token.child[0], token.child[1:])
            
            return type_size(type_t)
        if token.get_rule() == not_op:
            type_t = type(
                token,
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
            self.g_statement(token.child[0])
            if f"{type_t}._pas({type_t})" in self.functions:
                self.pop("rax")
                self.generation.append(
                    f"  call {self.nasm_footprint_name(f'{type_t}._pas({type_t})')}"
                )
                self.push_reg("rax")
                _, _, ret_type, _ = self.functions[f"{type_t}._pas({type_t})"]
                return type_size(ret_type)
            self.pop(f"{reg_size[type_size(type_t)][0]}")
            self.gen(f"  not {reg_size[type_size(type_t)][0]}")
            self.push_reg("rax")
            return type_size(type_t)
        if token.get_rule() == ptr_access:
            type_t = type(
                token,
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
            size = TYPES_SIZE[type_t] if type_t != "rien" else 1
            self.g_statement(token.child[0])
            self.pop(f"{reg_size[size][0]}")
            self.gen("  xor rbx, rbx")
            self.gen(f"  mov {reg_size[size][1]}, {word_size[size]} [rax]")
            self.push_reg("rbx")
            return size
        if token.get_rule() == dereferencement:
            size = 8  # un pointeur
            if token.child[0] != identifier and token.child[0] != attribute_identifier:
                error(
                    f"La valeur passé à l'opérateur '&' doit être une variable",
                    token.reference,
                )
            if token.child[0] == identifier:
                name = token.child[0].content
                if name not in self.variables:
                    error(
                        f"La variable '{name}' n'est pas définie",
                        token.child[0].reference,
                    )
                type_t, pos, free, _ = self.variables_info[name]
                shift = 0
                for i in self.sim_stack[pos:]:
                    shift += i
                self.gen(f"  lea rax, [rsp + {shift}]")
                self.push_reg(f"rax")
                return 8
            if token.child[0] == attribute_identifier:
                self.g_attribute_identifier(
                    token.child[0].child[0], token.child[0].child[1:]
                )
                return 8  # un pointeur
        if token.get_rule() == negativ:
            type_t = type(
                token,
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
            self.g_statement(token.child[0])
            if f"{type_t}._negatif({type_t})" in self.functions:
                self.pop("rax")
                self.generation.append(
                    f"  call {self.nasm_footprint_name(f'{type_t}._negatif({type_t})')}"
                )
                self.push_reg("rax")
                _, _, ret_type, _ = self.functions[f"{type_t}._negatif({type_t})"]
                return type_size(ret_type)
            if type_t != "ent":
                error(
                    f"Le type '{type_t}' ne peut pas être négatif, ou ne possède pas de méthode '{f'{type_t}._negatif({type_t})'}'",
                    token.reference,
                )
            self.pop("rax")
            self.gen("  mov rbx, 0")
            self.gen(f"  sub rbx, rax")
            self.push_reg("rbx")
            return type_size(type_t)
        if token.get_rule() == conditional_value:
            type1 = type(
                token.child[0],
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
            condition_type = type(
                token.child[1],
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
            type2 = type(
                token.child[2],
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
            if type1 != type2:
                error(
                    f"Les types des deux valeurs possibles devraient être égaux, pas '{type1}' et '{type2}'",
                    token.reference,
                )
            if condition_type != "bool":
                error(
                    f"Le type de la condition devrait être de type 'bool', pas '{condition_type}'",
                    token.reference,
                )
            lab1 = labels.label(modif="cond_val")
            lab_end = labels.label(modif="cond_val_end")
            self.g_statement(token.child[1])  # La condition
            self.pop("rax")
            self.gen("  test rax, rax")
            self.gen(f"  jnz {lab1}")
            self.g_statement(token.child[2])
            self.gen(f"  jmp {lab_end}")
            self.gen(f"{lab1}:")
            self.g_statement(token.child[0])
            self.gen(f"{lab_end}:")
            # On triche :
            self.sim_stack.pop()
            return type_size(type1)
        assert False, "Pas implémenté"

    def g_attribute_identifier(self, name: tok.BasicToken, attributes: list[tok.BasicToken]):
        name_t = type(
            name, self.variables_info, self.functions, self.classes, self.global_vars
        )
        self.g_statement(name)
        for j, c in enumerate(attributes):
            if not is_ptr(name_t):
                error(f"Le type '{name_t}' n'a pas d'attribut", c.reference)
            name_t = get_ptr_type(name_t)
            if name_t not in self.classes:
                error(f"Nom de classe inconnue : '{name_t}'", c.reference)
            self.pop("rax")
            r = self.classes[name_t]
            att_name = c.child[0].content
            if att_name not in r.att_name:
                error(
                    f"La classe '{name_t}' n'a pas d'attribut '{att_name}'", 
                    c.reference
                )
            i = r.att_name.index(att_name)
            decal_size = sum([type_size(t) for t in r.att_type[:i]])
            if decal_size != 0:
                self.generation.append(f"  add rax, {decal_size}")
            size = type_size(r.att_type[i])
            if j == len(attributes) - 1:
                self.gen("  xor rbx, rbx")
                self.gen(f"  mov {reg_size[size][1]}, {word_size[size]} [rax]")
                self.push_reg("rbx")
            else:
                self.push_gen("qword [rax]")
            name_t = r.att_type[i]
        return name_t

    def g_arraydef(self, token: tok.BasicToken):
        # assert False, f"Déprécié {token.reference}"
        array_def = token.child[0]
        l_type = gettype(array_def)
        length = array_def.child[0].child[0]
        # stack_alloc = not isinstance(token.child[1], tok.t_empty)
        name = token.child[1].content
        type_t_length = type(
            length, self.variables_info, self.functions, self.classes, self.global_vars
        )
        if type_t_length != "ent":
            error(
                f"La longueur de la liste doit être indiquée par un 'ent', pas '{type_t_length}'",
                token.reference,
            )
        self.g_statement(length)
        self.pop("rdx")
        self.gen(
            f"  lea rdx, [(rdx + 2) * {type_size(l_type)}]"
        )  # Le +2 résoud un bug d'affichage pour les chaines de caractère ...
        self.g_malloc("rdx")
        if name in self.variables:
            error(
                f"'{name}' a déjà été défini et ne peut pas être réécris par une liste",
                token.reference,
            )
        if name[:2] == "__":
            self.gen(f"  mov qword [{name}], rax")
            self.global_vars[name] = l_type
            self.variables.append(name)
            self.variables_info[name] = (f"{l_type}", len(self.sim_stack), False, True)
        else:
            self.push_reg("rax")
            self.variables.append(name)
            self.variables_info[name] = (f"{l_type}", len(self.sim_stack), False, False)

    def g_array_modif(self, token: tok.BasicToken):
        # token.print()
        array_value = token.child[2]
        attributs_modif = token.child[3]
        index = token.child[4].child[0]
        value = token.child[-1]
        i_type = type(
            index, self.variables_info, self.functions, self.classes, self.global_vars
        )
        if i_type != "ent":
            error(f"L'index doit être de type 'ent', pas '{i_type}'", index.reference)
        c_type = type(
            value, self.variables_info, self.functions, self.classes, self.global_vars
        )
        if not isinstance(attributs_modif, tok.t_empty):
            type_t = self.g_attribute_identifier(array_value, attributs_modif.child)
        else:
            self.g_statement(array_value)
            type_t = type(
                array_value,
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )

        if is_ptr(type_t):
            type_t_bis = get_ptr_type(type_t)
            if type_t_bis not in self.classes:
                error(
                    f"Nom de classe ou d'objet inconnu : '{type_t_bis}'",
                    token.reference,
                )
            func_name = f"{type_t}.assigne_index({type_t},ent,{c_type})"
            if func_name not in self.functions:
                error(
                    f"Méthode inconnue : '{func_name}'. La classe doit contenir une méthode '.assigne_index(<{type_t}>,<ent>,<{c_type}>)' pour supporter l'assignement à un index",
                    token.reference,
                )
            funcgen, args, rettype, _ = self.functions[func_name]
            if rettype != "rien":
                error(
                    f"Le type de retour de la fonction '{func_name}' devrait être 'rien'",
                    token.reference,
                )
            self.g_statement(index)
            self.g_statement(value)
            # order : ["rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rsp", "rbp", "r8", ...]
            self.pop("rcx")
            self.pop("rbx")
            self.pop("rax")
            self.gen(f"  call {self.nasm_footprint_name(func_name)}")
            return
        elif not is_liste(type_t):
            error(
                f"Mauvais type pour l'assignation à un index, on s'attendais à une 'liste[...]', on a eut '{type_t}'",
                array_value.reference,
            )
        if get_liste_type(type_t) != c_type:
            error(
                f"Mauvais type pour l'assignement, on attendais '{get_liste_type(type_t)}', et on a eut '{c_type}'",
                token.reference,
            )
        self.g_statement(index)
        self.g_statement(value)
        self.pop("rbx")
        self.pop("rdx")
        self.pop("rax")
        self.generation.append(f"  lea rax, [rax + rdx*{type_size(c_type)}]")
        self.generation.append(
            f"  mov {word_size[type_size(c_type)]} [rax], {reg_size[type_size(c_type)][1]} ;; test"
        )

    def g_attribute_modif(
        self, ident: tok.BasicToken, token: tok.BasicToken, statement: tok.BasicToken
    ):
        name_t = type(
            ident, self.variables_info, self.functions, self.classes, self.global_vars
        )
        self.g_statement(ident)
        for j, c in enumerate(token.child):
            self.pop("rax")
            if not is_ptr(name_t):
                error(
                    f"La variable doit être un objet, pas être de type '{name_t}'",
                    c.reference,
                )
            name_t = get_ptr_type(name_t)
            if name_t not in self.classes:
                error(f"Nom de classe inconnue : '{name_t}'", c.reference)
            att_name = c.child[0].content
            clss = self.classes[name_t]
            if att_name not in clss.att_name:
                error(
                    f"La classe '{name_t}' n'a pas d'attribut '{att_name}'", c.reference
                )
            i = clss.att_name.index(att_name)
            sizes = [type_size(t) for t in clss.att_type]
            decal_size = sum(sizes[:i])
            self.generation.append(f"  add rax, {decal_size}")
            if j == len(token.child) - 1:
                self.push_reg(f"rax")
            else:
                self.push_gen(f"qword [rax]")
            name_t = clss.att_type[i]

        s_type = type(
            statement,
            self.variables_info,
            self.functions,
            self.classes,
            self.global_vars,
        )
        if name_t != s_type:
            error(
                f"Mauvais type pour l'attribute, on s'attendais à '{name_t}', on a eut '{s_type}'",
                statement.reference,
            )
        self.g_statement(statement)
        self.pop("rbx")
        self.pop("rax")
        size = type_size(s_type)
        self.gen(f"  mov {word_size[size]} [rax], {reg_size[size][1]}")

    def g_write2ptr(self, name_og: tok.BasicToken, statement: tok.BasicToken):
        name = name_og.content
        if not name in self.variables:
            error(f"La variable '{name}' n'existe pas", name_og.reference)

        type_t, pos, free, _ = self.variables_info[name]
        if not is_ptr(type_t):
            error(f"La variable doît être de type 'ptr'", name_og.reference)
        c_type = type(
            statement,
            self.variables_info,
            self.functions,
            self.classes,
            self.global_vars,
        )
        if c_type != get_ptr_type(type_t):
            error(
                f"Mauvais type pour l'assignement, on attendais '{get_ptr_type(type_t)}', et on a eut '{c_type}'"
            )
        self.g_statement(statement)
        self.pop("rbx")
        decal = sum(self.sim_stack[pos:])

        size_t = TYPES_SIZE[c_type] if c_type != "rien" else 1

        if name in self.global_vars:
            self.gen(f"  mov rax, qword [{name}]")
        else:
            self.gen(f"  mov rax, qword [rsp + {decal}]")
        self.gen(f"  mov {word_size[size_t]} [rax], {reg_size[size_t][1]}")

    def global_vardef(self, name, type):
        self.global_vars[name] = type
        self.variables.append(name)
        self.variables_info[name] = (type, 0, False, False)

    def g_vardef(self, token: tok.BasicToken):
        # if self.name == starting_label:
        #    token.print(0)
        opt_type_usage = token.child[0]
        opt_ptr_modif = token.child[1]
        name = token.child[2].content
        name_og = token.child[2]
        opt_attributes = token.child[3]
        opt_indexation = token.child[4]
        content = token.child[5]

        if not isinstance(opt_ptr_modif, tok.t_empty):
            return self.g_write2ptr(name_og, content)
        if not isinstance(opt_indexation, tok.t_empty):
            return self.g_array_modif(token)
        if not isinstance(opt_attributes, tok.t_empty):
            return self.g_attribute_modif(name_og, opt_attributes, content)

        desired_type = (
            gettype(opt_type_usage)
            if not isinstance(opt_type_usage, tok.t_empty)
            else ""
        )
        if not isinstance(content, tok.t_empty):
            reg_size_t = self.g_statement(content)
            got_type = type(
                content,
                self.variables_info,
                self.functions,
                self.classes,
                self.global_vars,
            )
        elif not isinstance(opt_type_usage, tok.t_empty) and name not in self.variables:
            got_type = desired_type
            self.gen("  mov rax, 0")
            self.push_reg("rax")
        else:
            if isinstance(opt_type_usage, tok.t_empty):
                error(
                    f"Ne peut pas déclarer une variable de type implicite si le contenu n'est pas déclaré",
                    token.reference,
                )
            elif name in self.variables:
                error(
                    f"Ne peut pas déclarer une variable déjà existante", token.reference
                )
        if len(name) >= 3 and name[:2] == "__":
            if not is_ptr(got_type) and type_size(got_type) > 8:
                error(
                    f"La taille maximale d'une variable est 8 octets, pas {type_size(got_type)} ({got_type})",
                    token.reference,
                )
            self.pop("rax")
            if name not in self.global_vars:
                self.global_vars[name] = got_type
            if not isinstance(content, tok.t_empty):
                self.generation.append(f"  mov qword [{name}], rax")
                if name not in self.variables:
                    self.variables.append(name)
                    self.variables_info[name] = (got_type, 0, False, True)
            else:
                if name not in self.variables:
                    self.variables.append(name)
                    self.variables_info[name] = (got_type, 0, False, False)
        elif name not in self.variables:
            if desired_type != "" and desired_type != got_type:
                error(
                    f"Le type désiré '{desired_type}' et le type obtenu '{got_type}' ne correspondent pas",
                    token.reference,
                )
            self.variables.append(name)
            self.variables_info[name] = (got_type, len(self.sim_stack), False, False)
        else:
            if not isinstance(opt_type_usage, tok.t_empty):
                error(
                    f"La variabe '{name}' a déjà été définie et ne peut pas être redéfinie",
                    name_og.reference,
                )
            if got_type != self.variables_info[name][0]:
                error(
                    f"Le type attendu ('{self.variables_info[name][0]}') pour la variable '{name}' n'est pas celui obtenu '{got_type}'",
                    token.reference,
                )

            reg = reg_size[reg_size_t][0]
            self.pop(reg)

            size_t = type_size(got_type)
            stack_pos = self.variables_info[name][1]
            decal = sum(self.sim_stack[stack_pos:])
            self.gen(f"  mov {word_size[size_t]} [rsp + {decal}], {reg}")

    def to_file(self):
        r = ""
        for i in self.generation:
            r += i + "\n"
        return r


def generate(
    g: Generator, optimise: bool, defined: list[str], shared: bool, independant: bool
):
    clear_str = "`\\033[2J\\033[H`"
    g.used = True
    g.inde = independant
    g.global_vars["__plateforme"] = "liste[chr]"
    g.variables.append("__plateforme")
    g.variables_info["__plateforme"] = ("liste[chr]", 0, True, True)
    g.defined.append(f"PLATEFORME_{sys.platform.upper()}")
    g.declared_string.append(clear_str)
    information("Définis : ", f"PLATEFORME_{sys.platform.upper()}")
    for d in defined:
        g.defined.append(d.upper())
        information("Définis : ", d.upper())
    g.generate_code()
    g.generate_func()
    if optimise:
        g.generation = asm_optimiser.optimise(g.generation)
    g.generation.append(inb.print_func)
    g.generation.append("section .data")
    g.generation.append("  chr_buffer:  times 10 db 0")
    for i, msg in enumerate(g.declared_string):
        if msg == clear_str:
            g.generation.append(f'  msg{i}: db {clear_str}, 0, 0, 0, 0, 0, 0, 0, 0')
        else:
            g.generation.append(f'  msg{i}: db "{msg}", 0, 0, 0, 0, 0, 0, 0, 0')
    g.gen(f'  plateforme_name: db "{sys.platform}", 0, 0, 0, 0, 0, 0, 0, 0')
    for i, name in enumerate(g.global_vars):
        if shared:
            if not g.variables_info[name][-1]:
                g.gen(f"  extern {name}")
            else:
                if not g.variables_info[name][2]:
                    g.gen(f"  global {name}")
                g.generation.append(f"  {name}: dq 0")
        else:
            if not g.variables_info[name][-1]:
                g.gen(f"  extern {name}")
            else:
                g.generation.append(f"  {name}: dq 0")
    g.generation.append("  newline: db 10")

    for e in g.extern_functions:
        g.generation.append(f"extern {g.extern_functions[e]}")
    return basic + g.to_file(), g.sharded_library_imported


def run_code(
    filename,
    output_asm_name,
    optimise_asm=True,
    defined=[],
    shared_lib=False,
    independant=False,
):
    global starting_label, basic
    doc = Document(filename=filename)
    filename = filename.split("/")
    for f in filename[:-1]:
        PATH.append(f)
        PATH.append("/")
    toks = tokenise(basic_rulling, doc, True)
    full = ""
    for p in PATH:
        full += p
    full += filename[-1]
    if shared_lib:
        full = Generator.nasm_footprint_name(None, full)
        starting_label = full + ".shared_lib"
        basic = f"""
default rel

section .text
"""
    g = Generator(toks, name=starting_label, shared_lib=shared_lib)
    g_str, shared = generate(g, optimise_asm, defined, shared_lib, independant)
    with open(output_asm_name, "w") as file:
        file.write(g_str)
    return shared
