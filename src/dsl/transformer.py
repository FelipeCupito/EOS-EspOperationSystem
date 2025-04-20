from lark import Lark, Transformer, v_args
import os

# Carga la gramática y crea el parser
grammar = open(os.path.join(os.path.dirname(__file__), "grammar.lark")).read()
parser = Lark(grammar, start="start", parser="lalr")

@v_args(inline=True)
class DSLTransformer(Transformer):
    def program(self, *peripherals):
        return list(peripherals)

    def peripheral_decl(self, _kw, name, _lbr, body, _rbr, _sc):
        return {"type": "peripheral", "name": name.value, "body": body}

    def body(self, *items):
        return list(items)

    def field_decl(self, *args):
        # args: (qualifier?, type, name, semicolon)
        if len(args) == 4:
            qual, t, name, _sc = args
            readonly = (qual.value == "SharedVarRO")
        else:
            t, name, _sc = args
            readonly = False
        return {"kind": "field", "type": t.value, "name": name.value, "readonly": readonly}

    def event_enum_decl(self, _kw, _l, names, _r, _sc):
        return {"kind": "events", "names": [n.value for n in names.children]}

    def service_decl(self, _kw, ret, name, _lp, params, _rp, _sc):
        param_list = []
        if params:
            toks = params.children
            param_list = [(toks[i].value, toks[i+1].value) for i in range(0, len(toks), 2)]
        return {"kind": "service", "return": ret.value, "name": name.value, "params": param_list}

    def periodic_decl(self, _kw, _lp, ms, _rp, func):
        return {"kind": "periodic", "ms": int(ms.value), "function": func.value}