from parser_imp import *
from rules import ignore_whitespace_consume, r_digit, r_optional, r_patern_repetition

doc = Document(filename="test.wati")

ch.BASIC_CHARACTERS.append("_")
identifier = rls.r_identifier()

class_type_opt = rls.r_sequence(
    rls.r_character("<").ignore_token(),
    rls.r_identifier(),
    rls.r_patern_repetition(
        rls.r_sequence(
            rls.r_character(",").ignore_token(),
            rls.r_identifier()
        )
    ),
    rls.r_character(">").ignore_token(),
    ignore_whitespace=True
).set_name("class-type-opt")

rulling = [class_type_opt]
toks = tokenise(rulling, doc)
for i in toks:
    i.print(0)
