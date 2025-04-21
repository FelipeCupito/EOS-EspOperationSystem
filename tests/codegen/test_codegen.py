import os
import subprocess
import filecmp
import pytest

DSL_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "scripts"))
EXAMPLE = os.path.abspath(os.path.join(os.path.dirname(__file__), "example.dsl"))
OUT = os.path.abspath(os.path.join(os.path.dirname(__file__), "generated.hpp"))
EXPECTED = os.path.abspath(os.path.join(os.path.dirname(__file__), "expected_output.hpp"))

@pytest.fixture(autouse=True)
def cleanup():
    if os.path.exists(OUT): os.remove(OUT)
    yield
    if os.path.exists(OUT): os.remove(OUT)

def test_codegen_generates_expected(tmp_path):
    # Invoca tu script codegen.py para un único archivo
    subprocess.check_call([
        "python3", os.path.join(DSL_DIR, "codegen.py"),
        EXAMPLE, OUT  # asumiendo que tu script acepta input y output
    ])
    assert os.path.exists(OUT)
    # Compara con el golden file
    assert filecmp.cmp(OUT, EXPECTED)



# para correr:

# pip install pytest
# cd eos-framework
# pytest