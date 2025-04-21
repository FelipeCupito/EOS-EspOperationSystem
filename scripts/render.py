#!/usr/bin/env python3
"""
render.py  <in.json>  <out.hpp>
Transforma el JSON emitido por el plugin en un header con Jinja2.
"""
import json, pathlib, sys
from jinja2 import Environment, FileSystemLoader

JSON = pathlib.Path(sys.argv[1])
OUT  = pathlib.Path(sys.argv[2])
TPL_DIR = pathlib.Path(__file__).parent.parent / "templates"

env = Environment(loader=FileSystemLoader(str(TPL_DIR)),
                  autoescape=False, trim_blocks=True, lstrip_blocks=True)
tpl = env.get_template("peripheral.hpp.j2")

data = json.loads(JSON.read_text())
OUT.write_text(tpl.render(**data))
print(f"[render.py] Generado {OUT}")
