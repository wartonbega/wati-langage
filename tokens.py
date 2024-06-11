from typing_extensions import Iterable
from copy import deepcopy

PRINT_PREFIX = "|   "


class PrintSettings:
    patern_repetition_extended_printing = False
    sequence_extended_printing = False
    setlist_extended_printing = True
    evaluated_setlist_extended_printing = True
    collapse = float("inf")
    # collapse = 2


SETTINGS = PrintSettings()


def get_rule_name(r):
    return str(r.__class__).split(".")[-1].replace("'>", "")


class BasicToken:
    def __init__(self, ignore: bool, parent_rule: "Rule", reference: str):
        self.child: list[BasicToken] = []
        self.content = ""
        self.ignored_token = ignore
        self.parent = parent_rule
        self.reference = reference

    def print_self(self, rec, end):
        print(PRINT_PREFIX * rec + self.content, end=end)

    def print(self, rec = 0):
        if rec < SETTINGS.collapse or len(self.child) == 0:
            self.print_self(rec, end="\n")
            for i in self.child:
                i.print(rec + 1)
        else:
            self.print_self(rec, end="[...]\n")

    def push_child(self, child: "BasicToken"):
        if child.ignored_token:
            for i in child.child:
                self.child.append(i)
        else:
            self.child.append(child)

    def get_rule(self) -> "Rule":
        return self.parent
    
    def __eq__(self, value: object) -> bool:
        return self.get_rule() == value
    
    def modify(self, src: list[str], dest: list) -> "BasicToken":
        n: BasicToken = deepcopy(self)
        n.parent = self.parent
        if n.content in src:
            return dest[src.index(n.content)]
        n.child = []
        for i in self.child:
            n.push_child(i.modify(src, dest))
        return n

class t_identifier(BasicToken):
    def __init__(self, iden: str, ignore: bool, parent, ref):
        super().__init__(ignore, parent, ref)
        self.content = iden

    def print_self(self, rec, end):
        print(PRINT_PREFIX * rec + f"identifier : {self.content}", end=end)


class t_enclosure(BasicToken):
    def __init__(self, rule: "r_enclosure", ignore: bool, parent: "Rule", ref):
        super().__init__(ignore, parent, ref)
        self.content = rule.lit_begin + " " + rule.lit_end
        self.rule = rule
        self.child = []

    def push_child(self, child: BasicToken):
        if self.ignored_token:
            return
        return super().push_child(child)

    def print_self(self, rec, end):
        if self.rule.has_name():
            print(PRINT_PREFIX * rec + self.rule.get_name(), end=end)
        else:
            print(PRINT_PREFIX * rec + f"{self.content}", end=end)


class t_patern_repetition(BasicToken):
    def __init__(self, pat: "Rule", ignore: bool, parent, ref):
        super().__init__(ignore, parent, ref)
        self.content = ""
        self.pat = pat
        self.child = []

    def print_self(self, rec, end):
        if self.pat.has_name():
            print(PRINT_PREFIX * rec + self.pat.get_name(), end=end)
        elif SETTINGS.patern_repetition_extended_printing:
            print(PRINT_PREFIX * rec + f"{self.pat}", end=end)
        else:
            print(PRINT_PREFIX * rec + f"{get_rule_name(self.pat)}", end=end)


class t_sequence(BasicToken):
    def __init__(self, sequence: "Rule", ignore: bool, parent, ref):
        super().__init__(ignore, parent, ref)
        self.content = "" # = sequence
        self.sequence = sequence
        self.child = []

    def print_self(self, rec, end):
        if self.sequence.has_name():
            print(PRINT_PREFIX * rec + self.sequence.get_name())
        elif SETTINGS.sequence_extended_printing:
            print(PRINT_PREFIX * rec + f"{self.sequence}", end=end)
        else:
            print(PRINT_PREFIX * rec + f"{get_rule_name(self.sequence)}", end=end)


class t_char_sequence(BasicToken):
    def __init__(self, sequence, ignore: bool, parent, ref):
        super().__init__(ignore, parent, ref)

        self.content =  "" # = sequence
        self.sequence = sequence
        self.child = []


class t_character(BasicToken):
    def __init__(self, char, ignore: bool, parent, ref):
        super().__init__(ignore, parent, ref)
        self.content = char
        self.child = []


class t_setlist(BasicToken):
    def __init__(self, char, ignore: bool, parent, ref):
        super().__init__(ignore, parent, ref)
        self.content = char
        self.child = []

    def print_self(self, rec, end):
        if SETTINGS.setlist_extended_printing:
            print(PRINT_PREFIX * rec + f"setlist : {self.content}", end=end)
        else:
            print(PRINT_PREFIX * rec + f"setlist", end=end)


class t_evaluated_setlist(BasicToken):
    def __init__(self, char, ignore: bool, parent, ref):
        super().__init__(ignore, parent, ref)
        self.content = char
        self.child = []

    def print_self(self, rec, end):
        if SETTINGS.evaluated_setlist_extended_printing:
            print(PRINT_PREFIX * rec + f"setlist : {self.content}", end=end)
        else:
            print(PRINT_PREFIX * rec + f"setlist", end=end)


class t_empty(BasicToken):
    def __init__(self, ignore: bool, parent):
        super().__init__(ignore, parent, "")
        self.content = ""
        self.child = []

    def print_self(self, rec, end):
        print(PRINT_PREFIX * rec + f"EMPTY", end=end)


class t_main(BasicToken):
    def __init__(self, ignore: bool, parent):
        super().__init__(ignore, parent, "")
        self.content = ""
        self.child = []

    def print_self(self, rec, end):
        print(PRINT_PREFIX * rec + f"code block", end=end)
