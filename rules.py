from typing import Any, Union
from typing_extensions import Callable, Iterable, Tuple

from document import Document
import characters as ch
import tokens as tok

BASIC_CHARACTERS = ch.BASIC_CHARACTERS
WHITESPACE = ch.WHITESPACE

def blank_character(c):
    return c in WHITESPACE

def ignore_whitespace_consume(doc:Document):
    while blank_character(doc.peek()):
        doc.consume()


def ignore_whitespace_peeking(doc:Document, shift=0):
    whitespace_counter = 0
    while blank_character(doc.peek(shift=whitespace_counter + shift)):
        whitespace_counter += 1
    return whitespace_counter

def basic_rearangement(t: tok.BasicToken) -> tok.BasicToken:
    return t

class RuleError(Exception):
    def __init__(self, message, document: Document, shift = 0) -> None:
        self.message = document.find_reference(shift=shift) + " : " + message
        super().__init__(self.message)

class Rule:
    def __init__(self):
        self.name = ""
        self._ignore_tok = False
        self.rearangement: Callable[[tok.BasicToken], tok.BasicToken] = basic_rearangement
        self._error = ""

    def consume(self, document:Document) -> tok.BasicToken:
        return tok.t_empty(self._ignore_tok, self)

    def ignore_token(self) -> "Rule":
        self._ignore_tok = not self._ignore_tok
        return self

    def set_error(self, err) -> "Rule":
        self._error = err
        return self

    def set_rearangement(self, rearangement: Callable[[tok.BasicToken], tok.BasicToken]):
        self.rearangement = rearangement
        return self

    def try_consume(self, document:Document, shift=0) -> Tuple[bool, int]:
        return (False, 0)

    def has_name(self) -> bool:
        return self.name != ""

    def get_name(self) -> str:
        return self.name

    def set_name(self, name) -> "Rule":
        self.name = name
        return self
    
    def __str__(self):
        return str(self.__class__).split(".")[-1].replace("'>", "")

class r_setlist(Rule):
    def __init__(self, setlist:Union[Iterable[str], ch.Character]):
        super().__init__()
        self.setlist = setlist
        if isinstance(self.setlist, Iterable):
            self.len = len(self.setlist[0])
        else:
            self.len = self.setlist.get_len()

    def consume(self, document:Document) -> tok.BasicToken:
        """Returns the symbols from the setlist available in the document"""
        test, length = self.try_consume(document)
        assert test, f"No symbols found from {self.setlist}"
        return self.rearangement(tok.t_setlist(document.consume(length), self._ignore_tok, self, document.find_reference()))

    def try_consume_character(self, document:Document, shift, setlist:ch.Character):
        count = 0
        while setlist.accepted(document.peek(shift = count + shift, length=self.len), count):
            count += 1
        return (True, count) if count != 0 else (False, 0)

    def try_consume_setlist(self, document:Document, shift, setlist:Iterable):
        count = 0
        while document.peek(shift = count + shift, length=self.len) in setlist:
            count += self.len
        return (True, count) if count != 0 else (False, 0)

    def try_consume(self, document, shift=0):
        """Returns wether the rule matches the document, and the maximum length"""
        if isinstance(self.setlist, ch.Character):
            return self.try_consume_character(document, shift, self.setlist)
        return self.try_consume_setlist(document, shift, self.setlist)

class r_digit(r_setlist):
    def __init__(self):
        super().__init__(ch.NUMERICS)

class r_characters(r_setlist):
    def __init__(self):
        super().__init__(BASIC_CHARACTERS)

class r_character(Rule):
    def __init__(self, char):
        super().__init__()
        assert len(char) == 1, "Char len for r_character must be 1 character long"
        self.char = char

    def consume(self, document:Document):
        return self.rearangement(tok.t_character(document.consume(), self._ignore_tok, self, document.find_reference()))

    def try_consume(self, document:Document, shift = 0):
        cond = document.peek(shift=shift) == self.char
        return (cond, 1 if cond else 0)

class r_char_sequence(Rule):
    def __init__(self, sequence, forbidden_c = ""):
        super().__init__()
        self.lit_seq = sequence
        self.forbidden_c = forbidden_c
        
    def consume(self, document:Document):
        assert self.try_consume(document)[0], "The sequence is incomplete"
        return self.rearangement(tok.t_char_sequence(document.consume(length=len(self.lit_seq)), self._ignore_tok, self, document.find_reference()))

    def try_consume(self, document:Document, shift=0):
        """Check wether the character sequence is complete"""
        if document.peek(length=len(self.lit_seq), shift=shift) == self.lit_seq:
            if document.peek(length=1, shift=shift+1) not in self.forbidden_c:
                return (True, len(self.lit_seq))
        
        if self._error != "": raise RuleError(self._error, document=document, shift=shift)
        return (False, 0)

    def get_len(self):
        return len(self.lit_seq)


class r_sequence(Rule):
    def __init__(self, *args:Rule, ignore_whitespace=True):
        super().__init__()
        self.sequence = list(args) # let as a tuple
        self.ignore_whitespace = ignore_whitespace # The whitespaces between all the components

    def __str__(self):
        seq = "[ "
        for i in self.sequence[:-1]:
            seq += str(i) + ", "
        seq += str(self.sequence[-1]) + " ]"
        return super().__str__() + seq


    def consume(self, document:Document):
        ret = tok.t_sequence(self, self._ignore_tok, self, document.find_reference())
        for i, rule in enumerate(self.sequence):
            if self.ignore_whitespace:
                ignore_whitespace_consume(document)
            ret.push_child(rule.consume(document))
        return self.rearangement(ret)


    def try_consume(self, document:Document, shift=0):
        counter = 0
        for i, rule in enumerate(self.sequence):
            if self.ignore_whitespace:
                counter += ignore_whitespace_peeking(document, shift=counter + shift)
            check, c = rule.try_consume(document, shift=counter + shift)
            counter += c
            if not check:
                return (False, 0)
        return (True, counter)

class r_evaluated_setlist(Rule):
    def __init__(self, setlist:ch.Character):
        super().__init__()
        self.setlist = setlist
        self.rulling: list[Rule]
        self.evaluate: Callable[..., list[tok.BasicToken]]
        self.try_evaluate: Callable[..., Tuple[bool, int]]
        self.can_eval = False

    def set_evalutation(self, rulling:list[Rule], evaluation:Callable[..., list[tok.BasicToken]], try_evaluation:Callable[..., Tuple[bool, int]]):
        self.rulling = rulling
        self.evaluate = evaluation
        self.try_evaluate = try_evaluation
        self.can_eval = True

    def consume(self, document: Document) -> tok.BasicToken:
        assert self.can_eval, "Can not start evaluating"
        test, length = self.try_consume(document)
        assert test, f"No symbols found from {self.setlist}"
        _, refc, refl = document.find_reference().split(":")
        refc, refl = int(refc), int(refl)
        r = document.consume(length)
        toks = self.evaluate(self.rulling, Document(content=r, print_advance=False).set_reference(document.filename, refc, refl))
        t = tok.t_evaluated_setlist(self.setlist, self._ignore_tok, self, document.find_reference())
        for i in toks:
            t.push_child(i)
        return self.rearangement(t)

    def try_consume_character(self, document:Document, shift, setlist:ch.Character):
        count = 0
        condition = True
        count += ignore_whitespace_peeking(document, shift=count+shift)
        while setlist.accepted(document.peek(shift = count + shift, length=setlist.get_len()), count):
            condition, r = self.try_evaluate(self.rulling, document, count + shift)
            count += r
            count += ignore_whitespace_peeking(document, shift=count+shift)
        
        if count == 0:
            if self._error != "": raise RuleError(self._error, document=document, shift=shift)
            return (False, 0)
        return (True, count)


    def try_consume(self, document:Document, shift=0):
        assert self.can_eval, "Can not start evaluating"
        return self.try_consume_character(document, shift, self.setlist)


class r_enclosure(Rule):
    # Perfect for strings definition
    def __init__(self, begin:str, end:str, mid: Rule = None):
        super().__init__()
        self.begin = r_char_sequence(begin)
        self.lit_begin = begin

        if mid == None:
            self.mid = r_optional(r_setlist(ch.c_any_but(end)))
        else:
            self.mid = mid

        self.end = r_char_sequence(end)
        self.lit_end = end

        self.rulling: list[Rule]
        self.evaluate: Callable[..., list[tok.BasicToken]]
        self.try_evaluate: Callable[..., Tuple[bool, int]]
        self.evaluation = False
        self.whitespace_ig = False

    def set_mid_patern(self, mid: Rule):
        self.mid = mid
        self.whitespace_ig = True
        self.evaluation = False

    def set_evaluation(self, rulling:list[Rule], evaluation:Callable[..., list[tok.BasicToken]], try_evaluation:Callable[..., Tuple[bool, int]]):
        self.rulling = rulling
        self.evaluate = evaluation
        self.try_evaluate = try_evaluation
        self.mid = r_evaluated_setlist(ch.c_any_but(self.lit_end))
        self.mid.set_evalutation(self.rulling, self.evaluate, self.try_evaluate)
        self.whitespace_ig = False
        self.evaluation = True
        return self

    def consume(self, document:Document):
        self.begin.consume(document)
        if self.whitespace_ig: ignore_whitespace_consume(document)
        r = self.mid.consume(document)
        if self.whitespace_ig: ignore_whitespace_consume(document)
        self.end.consume(document)
        t = tok.t_enclosure(self, self._ignore_tok, self, document.find_reference())
        if self.evaluation:
            for i in r.child:
                t.push_child(i)
        else:
            t.push_child(r)
        return self.rearangement(t)

    def try_consume(self, document:Document, shift=0):
        counter = 0

        test, r = self.begin.try_consume(document, shift=shift)
        counter += r
        if not test:
            return (False, 0)

        if self.whitespace_ig: counter += ignore_whitespace_peeking(document, shift=shift+counter)
        test, r = self.mid.try_consume(document, shift=shift+counter)
        counter += r
        if not test:
            return (False, 0)
        if self.whitespace_ig: counter += ignore_whitespace_peeking(document, shift=shift+counter)
        test, r = self.end.try_consume(document, shift=shift+counter)
        counter += r
        if not test:
            return (False, 0)

        return (True, counter)

class r_optional(Rule):
    # An optional rule : can and can not exist
    def __init__(self, subrule:Rule):
        super().__init__()
        self.sub = subrule

    def __eq__(self, value: Rule) -> bool:
        return value == self.sub

    def consume(self, document:Document):
        test, r = self.sub.try_consume(document)
        if not test:
            return self.rearangement(tok.t_empty(self._ignore_tok, self))
        return self.rearangement(self.sub.consume(document))

    def try_consume(self, document:Document, shift=0):
        test, r = self.sub.try_consume(document, shift=shift)
        return (True, r)


class r_option(Rule):
    # Allows multiple type of rule and returns the first matching
    def __init__(self, *options:Rule):
        super().__init__()
        self.options = list(options)
        self.parenting = False

    def __eq__(self, value: Rule) -> bool:
        return value in self.options

    def __str__(self):
        seq = "[ "
        for i in self.options[:-1]:
            seq += str(i) + ", "
        seq += str(self.options[-1]) + " ]"
        return super().__str__() + seq

    def be_parent(self) -> Rule:
        self.parenting = True
        return self

    def add_option(self, opt: Rule):
        self.options.append(opt)

    def consume(self, document:Document):
        # Matches the first option 'matchable'
        for opt in self.options:
            test, r = opt.try_consume(document)
            if test:
                p = self.rearangement(opt.consume(document))
                if self.parenting:
                    p.parent = self
                return p
        
        assert False, "Option not found"


    def try_consume(self, document:Document, shift=0):
        for opt in self.options:
            test, r = opt.try_consume(document, shift=shift)
            if test:
                return (True, r)
        if self._error != "": raise RuleError(self._error, document=document, shift=shift)
        return (False, 0)


class r_patern_repetition(Rule):
    def __init__(self, patern:Rule, ignore_whitespace=True):
        super().__init__()
        self.patern = patern
        self.ignore_whitespace = ignore_whitespace

    def __str__(self):
        seq = f"[ {str(self.patern)} ]"
        return super().__str__() + seq

    def consume(self, document:Document):
        r = tok.t_patern_repetition(self, self._ignore_tok, self, document.find_reference())
        if self.ignore_whitespace:
            ignore_whitespace_consume(document)
        condition = self.patern.try_consume(document)[0]
        
        while condition:
            x = self.patern.consume(document)
            if self.ignore_whitespace:
                ignore_whitespace_consume(document)
            r.push_child(x)
            condition = self.patern.try_consume(document)[0]
        return self.rearangement(r)


    def try_consume(self, document:Document, shift=0):
        count = 0
        decal = 0
        if self.ignore_whitespace:
            decal += ignore_whitespace_peeking(document, shift=shift)
        
        condition, r = self.patern.try_consume(document, shift=shift + decal)
        while condition:
            count += 1
            decal += r
            if self.ignore_whitespace:
                decal += ignore_whitespace_peeking(document, shift=shift + decal)
            condition, r = self.patern.try_consume(document, shift=shift + decal)

        return (True, decal) if count != 0 else (False, 0)

class r_identifier(r_setlist):
    def __init__(self):
        # The rule is that an identifier must not start with a number
        # But can contain numbers
        c = ch.c_does_not_start_with_char(ch.c_setlist(ch.NUMERICS))
        d = ch.c_setlist(ch.NUMERICS + ch.BASIC_CHARACTERS)
        a = ch.c_mutliple_conditions(c, d)
        super().__init__(a)

    def consume(self, document: Document):
        r = super().consume(document).content
        return self.rearangement(tok.t_identifier(r, self._ignore_tok, self, document.find_reference()))
