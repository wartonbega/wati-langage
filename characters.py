# The basic alphabetic letters, both uppercase and lowercase
# The list can be extended to support special characters like é, à, è, ñ...
BASIC_CHARACTERS = [chr(i) for i in (list(range(65, 91)) + list(range(97, 123)))]
WHITESPACE =  [" ", "\n", "\t", "\r"]
NUMERICS = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]

# Operator // not put in because a lot of languages uses it as a comment
OPERATORS = ["++", "**", "<=", ">=", "!=", "==", "&&", "||", "<", ">", "^", "*", "/", "+", "-", "%"]
ONE_SYM_OPERATORS = ["<", ">", "^", "*", "/", "+", "-", "%"]
TWO_SYM_OPERATORS = ["++", "**", "<=", ">=", "!=", "==", "&&", "||", "<<", ">>"]

class Character:
    def __init__(self):
        pass

    def get_len(self) -> int:
        return 0

    def accepted(self, c, index) -> bool:
        return False

class c_any_but(Character):
    def __init__(self, but):
        self.but = but
        super().__init__()

    def get_len(self) -> int:
        return len(self.but)

    def accepted(self, c, index):
        return self.but != c and c != "<EOF>"

class c_setlist(Character):
    def __init__(self, setlist:list[str]):
        self.setlist = setlist
        super().__init__()

    def get_len(self) -> int:
        return 1

    def accepted(self, c, index):
        return c in self.setlist

class c_does_not_start_with(Character):
    def __init__(self, char:str):
        self.char = char
        super().__init__()

    def get_len(self) -> int:
        return len(self.char)

    def accepted(self, c, index):
        if index == 0 and c == self.char:
            return False
        return c != "<EOF>"

class c_does_not_start_with_char(Character):
    def __init__(self, char:Character):
        self.char = char
        super().__init__()

    def get_len(self) -> int:
        return self.char.get_len()

    def accepted(self, c, index):
        if index == 0 and self.char.accepted(c, index):
            return False
        return c != "<EOF>"

class c_mutliple_conditions(Character):
    def __init__(self, *args:Character):
        self.chars = args
        super().__init__()

    def get_len(self) -> int:
        return self.chars[0].get_len()

    def accepted(self, c, index):
        for i in self.chars:
            if not i.accepted(c, index):
                return False
        return True
