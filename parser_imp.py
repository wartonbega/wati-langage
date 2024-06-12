
from typing import Tuple
import rules as rls
import characters as ch
from document import *
import tokens as tok

def tokenise(rules:list[rls.Rule], document:Document, print_bar=False) -> list[tok.BasicToken]:
    toks: list[tok.BasicToken]
    toks = []
    while document.peek() != "<EOF>":
        rls.ignore_whitespace_consume(document)
        ruled = False
        for rule in rules:
            if rule.try_consume(document)[0]:
                toks.append(rule.consume(document))
                #toks[-1].print(0)
                ruled = True
                break
        if not ruled and document.peek() != "<EOF>":
            print(document.peek(), document.counter)
            raise rls.RuleError("Not eof and not rulled", document)
            assert False, "Not eof and not rulled"
    if print_bar:print(); reset_bar()
    return toks

def try_tokenise(rules:list[rls.Rule], document:Document, shift:int) -> Tuple[bool, int]:
    if document.peek(shift=shift) == "<EOF>":
        return (False, -2)

    decal = 0
    decal += rls.ignore_whitespace_peeking(document, shift=decal+shift)
    ruled = False
    for rule in rules:
        test, r = rule.try_consume(document, shift=decal+shift)
        if test:
            decal += r
            ruled = True
            break
    if not ruled :
        return (False, -1)
    return (True, decal)
