import jinja2
import shutil
import subprocess

TYPES = ["int", "float", "long", "double", "char", "short"]
NUM_TYPES = ["int", "float", "long", "double", "short"]

TEMPLATE_FILE = "clomy.h.tmpl"
OUTPUT_FILE = "clomy.h"

with open(TEMPLATE_FILE) as f:
    template_src = f.read()

template = jinja2.Template(template_src)
output = template.render(types=TYPES, num_types=NUM_TYPES)

with open(OUTPUT_FILE, "w") as f:
        f.write(output)

print(f"Generated {OUTPUT_FILE}")

clang_format = shutil.which("clang-format")
if clang_format:
    try:
        subprocess.run(
            [clang_format, "-i", OUTPUT_FILE],
            check=True
        )
        print(f"Formatted {OUTPUT_FILE} with clang-format")
    except subprocess.CalledProcessError as e:
        print(f"clang-format failed: {e}")
else:
    print("clang-format not found, skipping formatting")
