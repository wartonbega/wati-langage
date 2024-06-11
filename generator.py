from wati_parser import *
from code_analysis import *
import inbuilt_functions as inb
import sys
basic = """

default rel
extern _malloc
extern _free
section .text
    global _start

"""

basic_end = """
  mov rax, 0x2000001
  mov rdi, 0
  syscall
"""

func_end = """
  ret
"""

exit_end = """
  mov rax, 0x2000001
  pop rdi
  syscall
"""

IMPORTED = []

MALLOC_NAME = "malloc_bis"
FREE_NAME = "_free"

arg_register = ["rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rsp", "rbp", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"]

octo_reg = ["rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rsp", "rbp", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"]
tetr_reg = ["eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d"]
bina_reg = ["ax", "bx", "cx", "dx", "si", "di", "sp", "bp", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w"]
unit_reg = ["al", "bl", "cl", "dl", "sil", "dil", "spl", "bpl", "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b"]

INT_SIZE = 8

reg_size = {
    8: octo_reg,
    4: tetr_reg,
    2: bina_reg,
    1: unit_reg
}

word_size = {
    8: "qword",
    4: "dword",
    2: "word",
    1: "byte"
}

class LabelGenerator:
    def __init__(self) -> None:
        self.count = 0
    
    def label(self, modif="", base="Label"):
        lab = f"{base}{modif}{self.count}"
        self.count += 1
        return lab
    
    
class Classes:
    def __init__(self, name: str, attributes_name: list[str], attributes_type: list[str], methodes: dict[str: Tuple["Generator", list[tok.BasicToken], str]]) -> None:
        self.name = name
        self.att_name = attributes_name
        self.att_type = attributes_type
        self.methodes = methodes

labels = LabelGenerator()

class Generator:
    def __init__(self, toks: list[tok.BasicToken], name="_start", end=basic_end, arg_num = 0, ret_type="rien"):
        self.toks = toks
        self.name = name
        self.generation = [f"{name}:"]

        self.used = True
        
        self.sim_stack = []
        self.scopes = [] # index in self.variables
        self.variables = [] # identifiers
        self.variables_info = {} # identifier : (type, stack, allocated)
        self.global_vars = {} # identifiers : type
        
        self.functions: dict[str: Tuple[Generator, list[tok.BasicToken], str]] = {
            "println(chr)": (None, None, "rien"),
            "print(chr)": (None, None, "rien"),
            "exit(ent)": (None, None, "rien"),
            "allom(ent)": (None, None, "*rien"),
            "libere(*rien)": (None, None, "rien"),
        }
        
        self.basic_end = end

        self.declared_string = []
        
        self.arg_num = arg_num
        self.args_type: dict[str: str] = {} # 
        self.ret_type = ret_type
        
        self.classes: dict[str, Classes] = {}
        
    def generate_code(self):
        if not self.used:
            return 
        for i in range(self.arg_num):
            self.push(arg_register[i])
            
        found_ret = False
        for i, tok in enumerate(self.toks):
            if self.name == "_start":
                pass
            self.g_instruction(tok)
            if tok.get_rule() == keyword_return:
                found_ret = True
        
        assert found_ret or self.ret_type == "rien", f"The function {self.ret_type} does not return something for every path" # TODO: replace with code analysis
        
        self.g_end(self.basic_end)

        # Little check to know if we messed up with the pop/push function : 
        assert len(self.sim_stack) == 0, f"The sim stack is not empty : {self.sim_stack} for {self.name}"
        
    def generate_func(self):
        if self.name == "_start":        
            for i in self.functions:
                if self.functions[i][0] != None and self.functions[i][0].used:
                    self.functions[i][0].generate_code() 
                    self.generation += self.functions[i][0].generation

    def pop(self, register):
        self.generation.append(f"  pop {register}")
        self.sim_stack.pop()
    
    def push(self, register, size=INT_SIZE):
        self.generation.append(f"  push {register}")
        self.sim_stack.append(size)
        
    def g_end(self, end):
        for _ in range(len(self.variables)):
            name = self.variables.pop()
            if name not in self.global_vars:
                self.pop("r15 ;; unused values")
            self.variables_info.__delitem__(name)
        self.generation.append(end)    
    
    def g_instruction(self, tok: tok.BasicToken):
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
        elif tok.get_rule() == funcall:
            type_t = type(tok, self.variables_info, self.functions, self.classes)
            self.g_funcall(tok)
            if type_t != "rien":
                self.pop("rax")
        elif tok.get_rule() == methcall:
            type_t = type(tok, self.variables_info, self.functions, self.classes)
            self.g_methcall(tok)
            if type_t != "rien":
                self.pop("rax")
        elif tok.get_rule() == funcdef:
            self.g_funcdef(tok)
        elif tok.get_rule() == keyword_return:
            self.g_return(tok)
        elif tok.get_rule() == keyword_inclue:
            self.g_inclue(tok)
        elif tok.get_rule() == classdef:
            self.g_classdef(tok)
        elif tok.get_rule() == operator or tok.get_rule() == attended_expression:
            self.g_statement(tok)
            self.pop("rax ;; inutile")
        elif tok.get_rule() == colonm:
            pass
        else:
            pass
            #print(tok.get_rule())
    
    def g_func_exit(self, tok: tok.BasicToken):
        self.g_statement(tok)
        self.pop("rax")
        self.g_end(exit_end)
    
    def g_func_malloc(self, arg: tok.BasicToken):
        self.g_statement(arg)
        self.pop("rdx")
        self.generation.append(f"  call {MALLOC_NAME}")
        self.push("rax")
    
    def g_func_free(self, ptr: tok.BasicToken):
        self.g_statement(ptr)
        self.pop("rdi")
        self.generation.append(f"  call {FREE_NAME}")
    
    def g_func_print(self, tok: tok.BasicToken):
        t = type(tok, self.variables_info, self.functions, self.classes)
        if t == "chr":
            self.g_statement(tok)
            self.pop("rax")
            self.generation.append("  mov [chr_buffer], rax")
            self.generation.append("  mov rdx, 1")
            self.generation.append("  mov rsi, qword chr_buffer")
            self.generation.append("  mov rax, 0x2000004")
            self.generation.append("  mov rdi, 1")
            self.generation.append("  syscall")
        elif t == "liste[chr]":
            self.g_statement(tok)
            self.pop("rax")
            self.push("rax")
            self.generation.append("  mov rdi, rax")
            self.generation.append("  call _strlen")
            self.generation.append("  lea rdx, [rax*8]")
            self.pop("rsi")
            self.generation.append("  mov rax, 0x2000004")
            self.generation.append("  mov rdi, 1")
            self.generation.append("  syscall")
    
    def g_func_println(self, tok: tok.BasicToken):
        t = type(tok, self.variables_info, self.functions, self.classes)
        if t == "chr":
            self.g_statement(tok)
            self.pop("rax")
            self.generation.append("  mov [chr_buffer], rax")
            self.generation.append("  mov rdx, 1")
            self.generation.append("  mov rsi, qword chr_buffer")
            self.generation.append("  mov rax, 0x2000004")
            self.generation.append("  mov rdi, 1")
            self.generation.append("  syscall")
            self.generation.append("  mov rax, 0x2000004")
            self.generation.append("  mov rdi, 1")
            self.generation.append("  mov rdx, 1")
            self.generation.append("  mov rsi, qword newline")
            self.generation.append("  syscall")
        elif t == "liste[chr]":
            self.g_statement(tok)
            self.pop("rax")
            self.push("rax")
            self.generation.append("  mov rdi, rax")
            self.generation.append("  call _strlen")
            self.generation.append("  mov rdx, rax")
            self.pop("rsi")
            self.generation.append("  mov rax, 0x2000004")
            self.generation.append("  mov rdi, 1")
            self.generation.append("  syscall")
            self.generation.append("  mov rax, 0x2000004")
            self.generation.append("  mov rdi, 1")
            self.generation.append("  mov rdx, 1")
            self.generation.append("  mov rsi, qword newline")
            self.generation.append("  syscall")
    
    
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
        
    def g_scope(self, toks: tok.BasicToken):
        self.begin_scope()
        for i, tok in enumerate(toks.child):
            self.g_instruction(tok)
        self.end_scope()
            
    def g_identifier(self, tok: tok.BasicToken) -> bool:
        n = tok.content
        if n in self.global_vars:
            self.push(f"qword [{n}]")
            return False
        if not n in self.variables:
            assert False, f"Uknown identifier : {n}"

        type_t, pos, free = self.variables_info[n]
        shift = 0
        for i in self.sim_stack[pos:]:
            shift += i
        size_t_word = word_size[type_size(type_t)]
        self.push(f"{size_t_word} [rsp + {shift}]")
        return False

    def g_operator_add(self):
        self.generation.append("  add rax, rbx")
    
    def free_chaine(self, ptr):
        self.generation.append(f"  ;;free")
        self.push(ptr)
        self.generation.append(f"  mov rdi, {ptr}")
        self.generation.append("  call _strlen")
        self.generation.append("  mov rdx, rax")
        self.pop("rdi")
        self.generation.append("  mov rsi, rdx")
        self.generation.append("  call free")
        #; Arguments: rdi = pointer to allocated memory, rsi = size
    
    def g_operator_add_chaine_chaine(self, lhs_f, rhs_f):
        self.pop("rax    ;; The ptr2")
        self.generation.append("  mov rbx, rax ;; ptr 2 saved")
        self.generation.append("  mov rdi, rax")
        self.generation.append("  call _strlen")
        self.generation.append("  mov rdx, rax ;; the size")
        self.pop("rax   ;; ptr1")
        self.generation.append("  mov rcx, rax ;; ptr 1 saved")
        self.push("rbx")
        self.push("rcx")
        self.generation.append("  mov rdi, rax ")
        self.generation.append("  call _strlen")
        self.generation.append("  add rdx, rax ;; the new size")
        self.generation.append(f"  call {MALLOC_NAME} ;; we allocate some memory, the ptr is in rax")
        # We need to inverse the order of the two last element on the stack to get
        # the operand order right
        self.pop("rcx")
        self.pop("rbx")
        self.push("rcx")
        self.push("rbx")
        self.pop("rcx")
        self.generation.append("  mov rdi, rax")
        self.generation.append("  mov rsi, rcx")
        self.generation.append("  call chr_copy")
        self.pop("rbx")
        self.generation.append("  mov rsi, rbx")
        self.generation.append("  call chr_copy")
        self.generation.append("  mov byte [rdi], 0 ;; mod the last char to br 0")
        self.push("rax ;; the output")
        if lhs_f:
            self.free_chaine("rcx")
        if rhs_f:
            self.free_chaine("rbx")
    
    def g_operator_add_chr_chaine(self, free):
        self.pop("rbx")
        self.pop("rcx ;; the ptr")
        self.push("rbx")
        self.push("rcx")
        self.generation.append("  mov rdi, rcx")
        self.generation.append("  call _strlen")
        self.generation.append("  add rax, 1")
        self.generation.append("  mov rdx, rax")
        self.generation.append(f"  call {MALLOC_NAME}")
        self.pop("rcx")
        self.push("rax")
        self.generation.append("  mov rsi, rcx")
        self.generation.append("  mov rdi, rax")
        self.generation.append("  call chr_copy")
        self.pop("rax")
        self.pop("rbx ;; the chr (number)")
        self.push("rax")
        self.generation.append("  mov [rdi], rbx")
        self.generation.append("  inc rdi")
        self.generation.append("  mov byte [rdi], 0")
        
        if free:
            self.free_chaine("rcx")
    
    def g_operator_add_chr_chr(self):
        self.generation.append("  mov rdx, 2")
        self.generation.append(f"  call {MALLOC_NAME}")
        self.pop("rbx")
        self.pop("rcx")
        self.push("rax")        
        self.generation.append("  mov qword [rax], rcx")
        self.generation.append("  mov qword [rax + 1], rbx")
        self.generation.append("  inc rax")
        self.generation.append("  mov qword [rax + 2], 0")
        
    def g_operator_sub(self):
        self.generation.append("  sub rax, rbx")
    
    def g_operator_mul(self):
        self.generation.append("  mul rbx")
    
    def g_operator_div(self):
        self.generation.append("  xor rdx, rdx")
        self.generation.append("  div rbx")
    
    def g_operator_modulo(self):
        self.generation.append("  xor rdx, rdx")
        self.generation.append("  div rbx")
        self.generation.append("  mov rax, rdx")
        
        
    def g_operator_and(self):
        self.generation.append("  and rax, rbx")
    
    def g_operator_or(self):
        self.generation.append("  or rax, rbx")
        
    def g_operator_less_than(self):
        lab1 = labels.label("Lt")
        lab2 = labels.label("Lt")
        self.generation.append("  cmp rax, rbx")
        self.generation.append(f"  jns {lab2}")
        self.generation.append("  mov rax, 1")
        self.generation.append(f"  jmp {lab1}")
        self.generation.append(f"{lab2}:")
        self.generation.append("  mov rax, 0")
        self.generation.append(f"{lab1}:")
    
    def g_operator_less_eq_than(self):
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
    
    def g_operator_more_than(self):
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
    
    def g_operator_more_eq_than(self):
        lab1 = labels.label("Mt")
        lab2 = labels.label("Mt")
        self.generation.append("  cmp rax, rbx")
        self.generation.append(f"  js {lab2}")
        self.generation.append("  mov rax, 1")
        self.generation.append(f"  jmp {lab1}")
        self.generation.append(f"{lab2}:")
        self.generation.append("  mov rax, 0")
        self.generation.append(f"{lab1}:")
        
    def g_operator_equals(self):
        lab1 = labels.label("Eq")
        lab2 = labels.label("Eq")
        self.generation.append("  cmp rax, rbx")
        self.generation.append(f"  je {lab1}")
        self.generation.append("  mov rax, 0")
        self.generation.append(f"  jmp {lab2}")
        self.generation.append(f"{lab1}:")
        self.generation.append("  mov rax, 1")
        self.generation.append(f"{lab2}:")

    def g_basic_eq(self):
        lab1 = labels.label("Eq")
        lab2 = labels.label("Eq")
        self.generation.append("  cmp rax, rbx")
        self.generation.append(f"  je {lab1}")
        self.generation.append("  mov rax, 0")
        self.generation.append(f"  jmp {lab2}")
        self.generation.append(f"{lab1}:")
        self.generation.append("  mov rax, 1")
        self.generation.append(f"{lab2}:")
    
    def g_operator_not_equals(self):
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
        lhs_type = type(lhs, self.variables_info, self.functions, self.classes)
        lhs_free = self.g_statement(lhs)
        rhs = tok.child[1]
        rhs_type = type(rhs, self.variables_info, self.functions, self.classes)
        rhs_free = self.g_statement(rhs)
        
        ret_type = type(tok, self.variables_info, self.functions, self.classes)
        op = tok.content
        if f"{lhs_type}.{FUNC_OP[op]}({rhs_type})" in self.functions:
            
            self.pop("rbx")
            self.pop("rax")
            self.generation.append(f"  call _{(f"{lhs_type}.{FUNC_OP[op]}_{rhs_type}_").replace("[", "_").replace("]", "_").replace(",", "_").replace("*", "ptr")}")
            self.push("rax")
            return False
        if lhs_type == "ent":
            self.pop("rbx")
            self.pop("rax")
            if op == "+":
                self.g_operator_add()
            elif op == "-":
                self.g_operator_sub()
            elif op == "*":
                self.g_operator_mul()
            elif op == "/":
                self.g_operator_div()
            elif op == "%":
                self.g_operator_modulo()
            elif op == "<":
                self.g_operator_less_than()
            elif op == ">":
                self.g_operator_more_than()
            elif op == "<=":
                self.g_operator_less_eq_than()
            elif op == ">=":
                self.g_operator_more_eq_than()
            elif op == "==":
                self.g_operator_equals()
            elif op == "!=":
                self.g_operator_not_equals()
            self.push("rax")
            return False
        if lhs_type == "bool":
            self.pop("rbx")
            self.pop("rax")
            if op == "&&":
                self.g_operator_and()
            elif op == "||":
                self.g_operator_or()
            self.push("rax")
            return False
        if lhs_type == "liste[chr]":
            if rhs_type == "liste[chr]":
                if op == "+":
                    self.g_operator_add_chaine_chaine(lhs_free, rhs_free)
            if rhs_type == "chr":
                if op == "+":
                    self.g_operator_add_chr_chaine(lhs_free)
            return True
        if lhs_type == "chr":
            if rhs_type == "chr" or rhs_type == "ent":
                self.pop("rbx")
                self.pop("rax")
                if op == "+":
                    self.g_operator_add()
                elif op == "==":
                    self.g_operator_equals()
                elif op == "!=":
                    self.g_operator_not_equals()
                self.push("rax")
            return False
        if op == "==":
            self.pop("rbx")
            self.pop("rax")
            self.g_basic_eq()
            self.push("rax")
            return False
        if op == "!=":
            self.pop("rbx")
            self.pop("rax")
            self.g_basic_neq()
            self.push("rax")
            return False
        assert False, f"Incompatible type for operator '{op}' {lhs_type}, {rhs_type}"

    
    def g_statement(self, tok: tok.BasicToken) -> bool:
        if tok.get_rule() == int_:
            self.generation.append(f"  mov rax, {tok.content}")
            self.push("rax")
            return False
        if tok.get_rule() == t_bool:
            if tok.content == "Vrai":
                self.generation.append(f"  mov rax, 1")
            else:
                self.generation.append(f"  mov rax, 0")
            self.push("rax")
            return False
        if tok.get_rule() == identifier:
           return self.g_identifier(tok) 
        if tok.get_rule() == parenthesis:
            return self.g_statement(tok.child[0])
        if tok.get_rule() == operator:
            type(tok, self.variables_info, self.functions, self.classes)
            return self.g_operator(tok)
        if tok.get_rule() == char:
            content = tok.child[0].content
            assert len(content) <= 1, "Char definition must be 0 or 1 long"
            if len(content) == 0:
                self.generation.append("  mov rax, 0")
            else:
                self.generation.append(f"  mov rax, {ord(content)}")
            self.push("rax")
            return False
        if tok.get_rule() == string:
            content = tok.child[0].content
            l = len(content)
            if not content in self.declared_string:
                self.declared_string.append(content.replace("\n", "\", 10 ,\""))
            self.generation.append(f"  mov rax, qword msg{len(self.declared_string)-1}")
            self.push("rax")
            self.generation.append(f"  mov rdx, {l}")
            self.generation.append(f"  call {MALLOC_NAME}")
            self.push("rax")
            self.generation.append(f"  mov rdi, rax")
            self.pop("rsi")
            self.generation.append(f"  call chr_copy")
            return True
        if tok.get_rule() == funcall:
            self.g_funcall(tok)
            return False # TODO: determine wether the function returns something to free
        if tok.get_rule() == casting:
            type_t_dest = type(tok          , self.variables_info, self.functions, self.classes)
            type_t_orgn = type(tok.child[1] , self.variables_info, self.functions, self.classes)
            self.g_statement(tok.child[1].child[0])
            return True
        if tok.get_rule() == array_accession:
            name = tok.child[0]
            type_t = type(name, self.variables_info, self.functions, self.classes)
            if is_liste(type_t):
                index = tok.child[1].child[0]
                i_type = type(index, self.variables_info, self.functions, self.classes)
                assert i_type == "ent", f"The type of the index should be 'ent', not '{i_type}'"
                self.g_statement(name)
                self.g_statement(index)
                self.pop("rdx")
                self.pop("rax")
                self.generation.append(f"  lea rax, [rax + rdx*{type_size(get_liste_type(type_t))}]")
                self.push("qword [rax]")
                return False
            if is_ptr(type_t):
                index = tok.child[1].child[0]
                i_type = type(index, self.variables_info, self.functions, self.classes)
                assert i_type == "ent", f"The type of the index should be 'ent', not '{i_type}'"
                self.g_statement(name)
                self.g_statement(index)
                self.pop("rdx")
                self.pop("rax")
                self.generation.append(f"  lea rax, [rax + rdx]")
                self.push("qword [rax]")
                return False
            
            assert is_liste(type_t), f"To be accessed, the identifier type must be an 'liste'-like or 'ptr'-like, not '{type_t}'"
        if tok.get_rule() == classcall:
            self.g_classcall(tok)
            return True
        if tok.get_rule() == attribute_identifier:
            name_t = type(tok.child[0], self.variables_info, self.functions, self.classes)
            name_t = name_t[1:] # BUG
            assert name_t in self.classes, f"Unknown classe {name_t}"
            self.g_statement(tok.child[0])
            for c in tok.child[1:]:
                assert name_t in self.classes, f"Unknown classe {name_t}"
                self.pop("rax")
                r = self.classes[name_t]
                att_name = c.child[0].content
                assert att_name in r.att_name, f"Class {name_t} has no attribute : {att_name}"
                i = r.att_name.index(att_name)
                decal_size = sum([type_size(t) for t in r.att_type[:i]])
                if decal_size != 0:
                    self.generation.append(f"  add rax, {decal_size}")
                self.push(f"qword [rax]")
                name_t = r.att_type[i]

            return False
        if tok.get_rule() == methcall:
            type_t = type(tok, self.variables_info, self.functions, self.classes)
            self.g_methcall(tok)
            return False
        if tok.get_rule() == ptr_access:
            self.g_statement(tok.child[0])
            self.pop("rax")
            self.push("qword [rax]")
            return False
        if tok.get_rule() == dereferencement:
            type_t = type(tok, self.variables_info, self.functions, self.classes)
            name = tok.child[0].content
            type_t, pos, free = self.variables_info[name]
            self.generation.append(f"  lea rax, [rsp + {(len(self.sim_stack) - pos)*8}];; modify a variable")
            self.push("rax")
            return False
        assert False, f"Not implemented : {tok.get_rule().name}"
        return False
    
    def g_classdef(self, token: tok.BasicToken):
        assert self.name == "_start", "Can't define classe outside _start"
        type_intern_def = token.child[0]
        name = token.child[1].content
        
        attributes = token.child[2].child[0]
        methodes = token.child[2].child[1]
        
        att_names = [att.child[1].content   for att in attributes.child]
        att_types = [gettype(att.child[0].child[0])  for att in attributes.child]
        
        TYPES.append(name)
        TYPES_SIZE[name] = 8 # Because it is a pointer, it can be 8 bytes long (I think) (yes)
        
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
        
        methodes_def = {}
        for m in methodes.child:
            ret_type = gettype(m.child[0].child[0])
            f_name = f"{name}." + m.child[1].content
            args = m.child[2]
            type_name = "("
            if not isinstance(args.child[0], tok.t_empty):
                for i, v in enumerate(args.child):
                    type_t = gettype(v.child[0].child[0])
                    type_name += f"{type_t}," if i + 1 != len(args.child) else f"{type_t}"

            type_name += ")"
            if type_name[1:-1] in TYPES and m.child[1].content in OP_FUNC:
                modify_operators(m.child[1].content, name, type_name[1:-1], ret_type)
            scope = m.child[3]
            narg = len(args.child) if not isinstance(args.child[0], tok.t_empty) else 0
            func = Generator(scope.child, f"_{(f_name+type_name).replace("(", "_").replace(")", "_").replace(",", "_").replace("[", "_").replace("]", "_").replace("*", "ptr")}", func_end, arg_num=narg + 1, ret_type=ret_type)
            func.functions = self.functions
            func.classes = self.classes
            func.declared_string = self.declared_string
            func.global_vars = self.global_vars
            for i in self.global_vars:
                func.variables.append(i)
                func.variables_info[i] = (self.global_vars[i], 0, False)
            
            func.variables.append("soit")
            func.variables_info["soit"] = (name, len(func.variables), False)
            func.args_type["soit"] = name
            
            func.used = True # TODO: determine wether the method is used
                         
            if not isinstance(args.child[0], tok.t_empty):
                for _, v in enumerate(args.child): # Assignement of the argument
                    type_t = gettype(v.child[0].child[0])
                    func.variables.append(v.child[1].content)
                    func.variables_info[v.child[1].content] = (type_t, len(func.variables), False)
                    func.args_type[v.child[1].content] = type_t
                    
            self.functions[f_name + type_name] = (func, args.child, ret_type)
        clss = Classes(name, att_names, att_types, methodes_def)
        self.classes[name] = clss

    def g_classcall(self, token: tok.BasicToken):
        name = token.child[0].content
        args = token.child[1]
        clss = self.classes[name]
        size = sum([type_size(t) for t in clss.att_type])
    
        footprint_name = f"{name}.constructeur"
        if args.child[0].get_rule() != listed_value:
            args = [args.child[0]]
        elif isinstance(args.child[0], tok.t_empty):
            args = []
        else:
            args = args.child[0].child
            
        args_type_name = "("
        for i, v in enumerate(args):
            type_t = type(v, self.variables_info, self.functions, self.classes)
            args_type_name += f"{type_t}," if i + 1 != len(args) else f"{type_t}"
        args_type_name += ")"
        footprint_name += args_type_name
            
        assert footprint_name in self.functions, f"Function not defined : {footprint_name}"
        func_gen, f_args, ret_type = self.functions[footprint_name]
        func_gen.used = True
        assert func_gen.arg_num - 1 == len(args), f"Wrong number of arguments, {footprint_name} needs {func_gen.arg_num - 1} argument{"s" if func_gen.arg_num > 2 else ""}"
        assert ret_type == "rien", f"'constructeur' should return 'rien'"
        f_args_name = [f_args[i].child[1].content for i in range(func_gen.arg_num - 1)]
        f_args_type = [func_gen.args_type[i] for i in f_args_name]
        
        self.generation.append(f"  mov rdx, {size}")
        self.generation.append(f"  call {MALLOC_NAME}")
        self.push("rax")
        for i, v in enumerate(args):
            type_t = type(v, self.variables_info, self.functions, self.classes)
            assert type_t == f_args_type[i], f"Wrong type ('{type_t}') for the argument '{f_args_name[i]}', it is meant to be '{f_args_type[i]}'"
            self.g_statement(v)
        
        for i in range(func_gen.arg_num):
            self.pop(arg_register[func_gen.arg_num - i - 1])
        self.push("rax")
        self.generation.append(f"  call _{footprint_name.replace("(", "_").replace("[", "_").replace("]", "_").replace(")", "_").replace(",", "_").replace("*", "ptr")}") # Grossier
    
    def g_methcall(self, token: tok.BasicToken):
        footprint_name = get_methcall_name(token, self.variables_info, self.functions, self.classes, self.global_vars)
        soit = token.child[0].child[0]
        args = token.child[1]
        if args.child[0].get_rule() != listed_value:
            args = [args.child[0]]
        elif isinstance(args.child[0], tok.t_empty):
            args = []
        else:
            args = args.child[0].child
        assert footprint_name in self.functions, f"Function not defined : {footprint_name}"
        func_gen, f_args, ret_type = self.functions[footprint_name]
        assert func_gen.arg_num - 1 == len(args), f"Wrong number of arguments, {footprint_name} needs {func_gen.arg_num - 1} argument{"s" if func_gen.arg_num > 2 else ""}"
        f_args_name = [f_args[i].child[1].content for i in range(func_gen.arg_num - 1)]
        f_args_type = [func_gen.args_type[i] for i in f_args_name]
        for i, v in enumerate(args):
            type_t = type(v, self.variables_info, self.functions, self.classes)
            assert type_t == f_args_type[i], f"Wrong type ('{type_t}') for the argument '{f_args_name[i]}', it is meant to be '{f_args_type[i]}'"
            self.g_statement(v)
        
        self.g_statement(soit)
        self.pop(arg_register[0])
        for i in range(len(args)):
            self.pop(arg_register[len(args) - i])
        self.generation.append(f"  call _{footprint_name.replace("(", "_").replace(")", "_").replace("[", "_").replace("]", "_").replace(",", "_").replace("*", "ptr")}") # Grossier
        if ret_type != "rien":
            self.push("rax")
    
    def g_funcall(self, tok: tok.BasicToken):
        name = tok.child[0].content
        args = tok.child[1]
        footprint_name = get_funcall_name(tok, self.variables_info, self.functions, self.classes)
        if footprint_name == "exit(ent)":
            self.g_func_exit(args)
        elif footprint_name == "print(chr)":
            self.g_func_print(args)
        elif footprint_name == "println(chr)":
            self.g_func_println(args)
        elif footprint_name == "allom(ent)":
            self.g_func_malloc(args)
        elif footprint_name == "libere(*rien)":
            self.g_func_free(args.child[0])
        else:
            if args.child[0].get_rule() != listed_value:
                args = [args.child[0]]
            else:
                args = args.child[0].child
            assert footprint_name in self.functions, f"Function not defined : {footprint_name}"
            func_gen, f_args, ret_type = self.functions[footprint_name]
            func_gen.used = True
            assert func_gen.arg_num == len(args), f"Wrong number of arguments, {footprint_name} needs {len(f_args)} argument{"s" if len(f_args) > 1 else ""}"
            f_args_name = [f_args[i].child[1].content for i in range(func_gen.arg_num)]
            f_args_type = [func_gen.args_type[i] for i in f_args_name]
            for i, v in enumerate(args):
                type_t = type(v, self.variables_info, self.functions, self.classes)
                assert type_t == f_args_type[i], f"Wrong type ('{type_t}') for the argument '{f_args_name[i]}', it is meant to be '{f_args_type[i]}'"
                self.g_statement(v)
            for i in range(len(args)):
                self.pop(arg_register[len(args) - i - 1])
            self.generation.append(f"  call _{footprint_name.replace("(", "_").replace(")", "_").replace(",", "_").replace("[", "_").replace("]", "_").replace("*", "ptr")}")
            if ret_type != "rien":
                self.push("rax")
            
    def g_return(self, token: tok.BasicToken):
        ret = token.child[0]
        if not isinstance(ret, tok.t_empty):
            type_t = type(ret, self.variables_info, self.functions, self.classes)
            assert type_t == self.ret_type, f"The return '{type_t}' type is does not match the expected type : '{self.ret_type}'"
            self.g_statement(ret)
            self.pop("rax")
        elif self.ret_type != "rien":
            assert False, f"The return 'rien' type is does not match the expected type : '{self.ret_type}'"
        for i in range(len(self.variables)):
            if self.variables[i] not in self.global_vars:
                self.generation.append("  pop r15 ;; unused values")
        self.generation.append("  ret")
    
    def g_inclue(self, token: tok.BasicToken):
        filename = token.child[0].child[0].content
        if filename not in IMPORTED:
            IMPORTED.append(filename)
            doc = Document(filename=filename)
            toks = tokenise(basic_rulling, doc)
            g = Generator(toks)
            g.functions = self.functions
            g.classes = self.classes
            g.declared_string = self.declared_string
            g.variables = self.variables
            g.global_vars = self.global_vars
            g.variables_info = self.variables_info
            g.generate_code()

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
        self.pop("rax") # The condition
        if_lab = labels.label() # The if stmt label
        if_lab_end = labels.label("End") # The end label
        self.generation.append("  test rax, rax") # The test
        self.generation.append(f"  jz {if_lab}") # is it false ?
        
        self.g_scope(token.child[1])
        self.generation.append(f"  jmp {if_lab_end}")
        self.generation.append(f"{if_lab}:")
        if not isinstance(token.child[2], tok.t_empty): #there are elifs
            self.generation.append(f"  jmp {if_lab_end}")
            self.generation.append(if_lab + ":")

            for t in token.child[2].child:
                self.g_elif(t, if_lab_end)

        if not isinstance(token.child[3], tok.t_empty): # there is an else
            self.g_else(token.child[3], if_lab_end)            

        self.generation.append(if_lab_end + ":")

    def g_while(self, token: tok.BasicToken):
        condition = token.child[0]
        code = token.child[1]
        
        label_begin = labels.label("WhileBegin")
        label_end = labels.label("WhileEnd")
        
        self.generation.append(f"{label_begin}:")
        self.g_statement(condition)
        self.pop("rax") # The condition
        self.generation.append("  test rax, rax") # The test
        self.generation.append(f"  jz {label_end}") # is it false ?
        
        self.g_scope(code)
        
        self.generation.append(f"  jmp {label_begin}")
        
        self.generation.append(f"{label_end}:")
    
    def g_funcdef(self, token: tok.BasicToken):
        assert self.name == "_start", "Can't define functions outside the main file"
        ret_type = gettype(token.child[0].child[0])
        name = token.child[1].content
        args = token.child[2]
        type_name = "("
        if not isinstance(args.child[0], tok.t_empty):
            for i, v in enumerate(args.child):
                type_t = gettype(v.child[0].child[0])
                type_name += f"{type_t}," if i + 1 != len(args.child) else f"{type_t}"
        type_name += ")"
        
        scope = token.child[3]
        arglen = len(args.child) if not isinstance(args.child[0], tok.t_empty) else 0
        func = Generator(scope.child, f"_{(name+type_name).replace("(", "_").replace(")", "_").replace(",", "_").replace("[", "_").replace("]", "_").replace("*", "ptr")}", func_end, arg_num=arglen, ret_type=ret_type)
        func.functions = self.functions
        func.classes = self.classes
        func.declared_string = self.declared_string
        func.global_vars = self.global_vars
        for i in self.global_vars:
            func.variables.append(i)
            func.variables_info[i] = (self.global_vars[i], 0, False)
        
        if not isinstance(args.child[0], tok.t_empty):
            for i, v in enumerate(args.child):
                type_t = gettype(v.child[0].child[0])
                func.variables.append(v.child[1].content)
                func.variables_info[v.child[1].content] = (type_t, len(func.variables), False)
                func.args_type[v.child[1].content] = type_t
        self.functions[name + type_name] = (func, args.child, ret_type)
    
    def g_arraydef(self, token: tok.BasicToken):
        array_def = token.child[0]
        l_type = gettype(array_def)
        length = array_def.child[0].child[0]
        name = token.child[1].content
        type_t_length = type(length, self.variables_info, self.functions, self.classes)
        assert type_t_length == "ent", f"The length of the array muste be of type 'ent', not '{type_t_length}'"
        self.g_statement(length)
        self.pop("rdx")
        self.generation.append(f"  lea rdx, [rdx*{type_size(l_type)}]")
        self.generation.append(f"  call {MALLOC_NAME}")
        if name[:2] == "__":
            self.generation.append(f"  mov qword [{name}], rax")
            self.global_vars[name] = l_type
        else:
            self.push("rax")
        assert name not in self.variables, f"'{name}' is already defined and can't be overriden with an array"
        self.variables.append(name)
        self.variables_info[name] = (f"{l_type}", len(self.sim_stack), True)
    
    def g_attribute_modif(self, ident: tok.BasicToken, token: tok.BasicToken, statement: tok.BasicToken):
        name_t = type(ident, self.variables_info, self.functions, self.classes)
        self.g_statement(ident)
        for j, c in enumerate(token.child):
            self.pop("rax")
            assert name_t in self.classes, f"Unknown classe {name_t}"
            att_name = c.child[0].content
            clss = self.classes[name_t]
            assert att_name in clss.att_name, f"Class {name_t} has no attribute : {att_name}"
            i = clss.att_name.index(att_name)
            sizes = [type_size(t) for t in clss.att_type]
            decal_size = sum(sizes[:i])
            self.generation.append(f"  add rax, {decal_size}")
            if j == len(token.child) - 1:
                self.push(f"rax")
            else:
                self.push(f"qword [rax]")
            name_t = clss.att_type[i]
        
        s_type = type(statement, self.variables_info, self.functions, self.classes)
        assert name_t == s_type, f"Wrong type for the attribute assignement, expected '{name_t}', got '{s_type}'"
        self.g_statement(statement)
        self.pop("rbx")
        self.pop("rax")
        self.generation.append(f"  mov qword [rax], rbx")
        #if type_size(s_type) == 8:
        #if type_size(s_type) == 4: # TODO
        #    self.generation.append(f"  mov dword [rax], ebx")
        
    def g_array_modif(self, token: tok.BasicToken):
        name = token.child[1].content
        n_type = type(token.child[1], self.variables_info, self.functions, self.classes)
        if is_liste(n_type):
            index = token.child[2].child[0]
            value = token.child[-1]
            i_type = type(index, self.variables_info, self.functions, self.classes)
            assert i_type == "ent", f"The index must be of type 'ent', not '{i_type}'"
            assert name in self.variables, f"The variable '{name}' in unknown"
            type_t, pos, free = self.variables_info[name]
            c_type = type(value, self.variables_info, self.functions, self.classes)
            assert f"liste[{c_type}]" == type_t, f"Wrong type for array assignement, expected '{type_t[len("liste["): -1]}', got '{c_type}'"
            self.g_statement(index)
            self.g_statement(value)
            self.pop("rbx")
            self.pop("rdx")
            decal = sum(self.sim_stack[:pos - 1])
            print(c_type)
            print(type_size(c_type))
            if name in self.global_vars:
                self.generation.append(f"  mov rax, qword [{name}]")
            else:
                self.generation.append(f"  mov rax, qword [rsp + {decal}]")
            self.generation.append(f"  lea rax, [rax + rdx*{type_size(c_type)}]")
            self.generation.append(f"  mov {word_size[type_size(c_type)]} [rax], {reg_size[type_size(c_type)][1]}")
            return
        if is_ptr(n_type):
            index = token.child[2].child[0]
            value = token.child[-1]
            i_type = type(index, self.variables_info, self.functions, self.classes)
            assert i_type == "ent", f"The index must be of type 'ent', not '{i_type}'"
            assert name in self.variables, f"The variable '{name}' in unknown"
            type_t, pos, free = self.variables_info[name]
            self.g_statement(index)
            self.g_statement(value)
            self.pop("rbx")
            self.pop("rdx")
            if name in self.global_vars:
                self.generation.append(f"  mov rax, qword [{name}]")
            else:
                self.generation.append(f"  mov rax, qword [rsp + {(len(self.sim_stack) - pos)*8}]")
            self.generation.append(f"  lea rax, [rax + rdx]")
            self.generation.append(f"  mov qword [rax], rbx")
            return
    
    def g_vardef(self, token: tok.BasicToken):
        opt_type_def = token.child[0]
        
        attended_type = ""
        if not isinstance(opt_type_def, tok.t_empty):
            attended_type = gettype(opt_type_def)
        name = token.child[1].content
        array_access = token.child[2]
        attribute_access = token.child[3]
        statement = token.child[-1]
        
        if not isinstance(array_access, tok.t_empty):
            return self.g_array_modif(token)
        if not isinstance(attribute_access, tok.t_empty):
            return self.g_attribute_modif(token.child[1], token.child[3], statement)
        
        
        c_type = type(statement, self.variables_info, self.functions, self.classes)
        assert not (attended_type != "" and c_type != attended_type), f"The attended type '{attended_type}' does not match the type got '{c_type}'"

        alloc = self.g_statement(statement)
        self.pop("rax")
        if name[:2] == "__":
            assert is_ptr(c_type) or type_size(c_type) == 8, "The type of a global variable should be a pointer or should be 8 bytes long"
            if name not in self.global_vars:
                self.global_vars[name] = c_type
            self.generation.append(f"  mov qword [{name}], rax")
            if name not in self.variables:
                self.variables.append(name)
                self.variables_info[name] = (c_type, 0, False)
        elif name in self.variables and attended_type == "":
            type_t, pos, free = self.variables_info[name]
            assert c_type == type_t, f"The type got ({c_type}) does not match the variable type ('{type_t}')"
            self.generation.append(f"  mov [rsp + {(len(self.sim_stack) - pos)*8}], rax ;; modify a variable")
            if free:
                return
                assert False, "Not implemented"
        elif name in self.variables and attended_type != "":
            assert False, f"The variable '{name}' is already defined and can't be redefined"
        else:
            self.push("rax")
            self.variables.append(name)
            self.variables_info[name] = (c_type, len(self.sim_stack), alloc)

    def to_file(self):
        r = ""
        for i in self.generation:
            r += i + "\n"
        return r

    def optimise(self):
        sorted = []
        last = ""
        for i, inst in enumerate(self.generation):
            sorted.append(inst)
            last = inst
        self.generation = sorted

def generate(g :Generator):
    g.used = True
    g.generate_code()
    g.generate_func()
    g.generation.append(inb.print_func)
    g.generation.append("section .data")
    g.generation.append("  chr_buffer:  times 10 db 0")
    g.generation.append("""  initial_break:
    dd 0x00000000
  current_break:
    dd 0x00000000
  new_break:
    dd 0x00000000""")
    for i, msg in enumerate(g.declared_string):
        g.generation.append(f"  msg{i}: db \"{msg}\", 0, 0, 0, 0, 0, 0, 0, 0")
    for i, name in enumerate(g.global_vars):
        g.generation.append(f"  {name}: db 0")
    g.generation.append("  newline: db 10")
    return basic + g.to_file()

doc = Document(filename=sys.argv[1])
toks = tokenise(basic_rulling, doc)

g = Generator(toks)
g_str = generate(g)
with open("test.asm",'w') as file: 
    file.write(g_str)   