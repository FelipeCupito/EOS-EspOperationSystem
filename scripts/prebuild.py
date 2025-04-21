Import("env")
import os, subprocess, sys

# Determina rutas
project_dir = env['PROJECT_DIR']
lib_dir     = os.path.join(project_dir, "lib", "eos-framework", "plugin")
build_dir   = os.path.join(lib_dir, "build")
so_name     = "EOSPlugin.dll" if sys.platform == "win32" else "libEOSPlugin.so"
so_path     = os.path.join(build_dir, so_name)

# 1) Compila el plugin si es necesario
if not os.path.exists(so_path):
    os.makedirs(build_dir, exist_ok=True)
    subprocess.check_call(["cmake", "-B", build_dir, "-S", lib_dir])
    subprocess.check_call(["cmake", "--build", build_dir])

# 2) Genera los headers con clang++ + plugin
src_dir = os.path.join(project_dir, "src")
out_dir = os.path.join(project_dir, "generated")
os.makedirs(out_dir, exist_ok=True)

for fn in os.listdir(src_dir):
    if fn.endswith(".hpp"):
        src = os.path.join(src_dir, fn)
        subprocess.check_call([
            "clang++", "-fsyntax-only",
            "-Xclang", "-load", so_path,
            "-Xclang", "-plugin=eos-plugin",
            "-I" + os.path.join(project_dir, "lib", "eos-framework", "include"),
            "-I" + src_dir,
            src
        ])
