import os

class settings:
    last_perc = 0

def reset_bar():
    settings.last_perc = 0

def print_bar(perc, filename):
    large, _ = os.get_terminal_size()
    print("\r", end="")
    if int(perc * 100) == settings.last_perc:
        return
    else:
        settings.last_perc = int(perc * 100)
    p = str(int(perc*100)) + "%"
    l = large - len(filename) - 7 - len(str(p))
    print(filename, f" {p}: [ ", "#" * int(perc * l), " " * int((1 - perc)*l), f" ]", sep="", end="")

class Document:
    def __init__(self, filename="", content="", print_advance=True):
        self.filename = filename
        self.counter = 0
        assert not (filename != "" and content != ""), "(intern) : Both name and content cant be assigned"

        if filename != "":
            with open(filename, "r") as f:
                self.doc = f.read()
        else:
             self.doc = content
        
        self.total_len = len(self.doc)
             
        self.doc = self.doc.replace("\\n", "\n") # To replace
        self.begin_c = 0
        self.begin_l = 0
        self.ref_set = False
        
        self.print_advance = print_advance
    
    def percentage(self):
        if self.total_len == 0:
            return 0
        return (self.counter) / self.total_len

    def set_reference(self, filename, beginc: int, beginl: int) -> "Document":
        self.filename = filename
        self.begin_c = beginc
        self.begin_l = beginl
        self.ref_set = True
        return self

    def find_reference(self, shift = 0) -> str:
        l = 1
        c = 1
        for i in self.doc[:self.counter+1]:
            if i == "\n":
                l += 1
                c = 1
            else:
                c += 1
        if not self.ref_set and self.filename != "":
            return f"{self.filename}:{l}:{c}"
        return f"{self.filename}:{l+self.begin_c}:{c+self.begin_l}"

    def peek(self, shift = 0, length = 1):
        """Return the symbols in the document shifted by the amount demanded, and the length demanded"""
        if self.counter + shift < len(self.doc):
            return self.doc[self.counter + shift : self.counter + shift + length]
        return "<EOF>"


    def consume(self, length = 1) -> str:
        """Consumes the symbols from the document, with the length desired"""
        if self.counter + length < 1 + len(self.doc):
            r = self.doc[self.counter : self.counter + length]
            self.counter += length
            if self.print_advance:
                print_bar(self.percentage(), self.filename)
            return r
        self.counter += length
        return "<EOF>"

    def __str__(self):
        return self.doc
