import os, re
from jinja2 import Environment, FileSystemLoader

# ---------- regex auxiliares ----------
periph_open   = re.compile(r'^\s*peripheral\s+(\w+)\s*{\s*$')
periph_close  = re.compile(r'^\s*}\s*;?\s*$')

field_rx      = re.compile(r'^\s*(SharedVarRO|SharedVar)?\s*([\w:<>]+)\s+(\w+)\s*;\s*$')
svc_decl_rx   = re.compile(r'^\s*Service\s+([\w:<>]+)\s+(\w+)\s*\(([^)]*)\)\s*;\s*$')
svc_def_rx    = re.compile(r'^\s*Service\s+([\w:<>]+)\s+(\w+)\s*\(([^)]*)\)\s*{\s*$')

def split_params(param_str: str):
    out = []
    if not param_str.strip():
        return out
    for chunk in param_str.split(','):
        t, n = chunk.strip().split()
        out.append((t, n))
    return out

# ---------- lee .dsl -> IR ----------
ir = []
for fn in os.listdir("example"):
    if not fn.endswith(".dsl"): continue
    with open(os.path.join("example", fn)) as f:
        cur = None
        brace_depth = 0
        body_lines  = []
        pending_svc = None   # (ret_type, name, params)
        for raw in f:
            line = raw.rstrip('\n')

            if cur is None:
                m = periph_open.match(line)
                if m:
                    cur = {"name": m.group(1), "fields": [], "services": []}
                continue

            # dentro de un peripheral .............................
            if brace_depth > 0:
                # estamos capturando el cuerpo de un Service definido
                if '{' in line: brace_depth += line.count('{')
                if '}' in line: brace_depth -= line.count('}')
                body_lines.append(raw)
                if brace_depth == 0:
                    # bloque terminado
                    cur["services"].append({
                        "return": pending_svc[0],
                        "name":   pending_svc[1],
                        "params": pending_svc[2],
                        "body":   ''.join(body_lines[:-1])  # sin la llave de cierre
                    })
                    body_lines.clear()
                    pending_svc = None
                continue

            # peripheral cierre
            if periph_close.match(line):
                ir.append(cur)
                cur = None
                continue

            # campo
            if m := field_rx.match(line):
                qual, typ, name = m.groups()
                cur["fields"].append({
                    "type": typ,
                    "name": name,
                    "readonly": qual == "SharedVarRO"
                })
                continue

            # service declarado
            if m := svc_decl_rx.match(line):
                ret, name, params = m.groups()
                cur["services"].append({
                    "return": ret,
                    "name":   name,
                    "params": split_params(params),
                    "body":   None
                })
                continue

            # service definido
            if m := svc_def_rx.match(line):
                ret, name, params = m.groups()
                pending_svc = (ret, name, split_params(params))
                brace_depth = 1
                body_lines.clear()
                continue

# ---------- Jinja2 ----------
env = Environment(
    loader=FileSystemLoader("templates"),
    trim_blocks=True, lstrip_blocks=True
)
tmpl = env.get_template("peripheral.hpp.j2")

os.makedirs("generated", exist_ok=True)
for p in ir:
    ctx = {
        "name": p["name"],
        "body": [],
        "ctor_params": [],
        "init_list":   [],
        "setup_stmts": []
    }
    for f in p["fields"]:
        ctx["body"].append({**f, "kind": "field"})
        ctx["ctor_params"].append(f'{f["type"]} {f["name"]}')
        ctx["init_list"].append(f'{f["name"]}({f["name"]})')
    for s in p["services"]:
        ctx["body"].append({**s, "kind": "service"})
    code = tmpl.render(ctx)
    out  = f"generated/{p['name']}_gen.hpp"
    with open(out, 'w') as fp:
        fp.write(code)
    print("✓ generado", out)
