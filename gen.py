import sys
import shutil
import subprocess

import jinja2

TYPES = ["int", "float", "long", "double", "char", "short"]
NUM_TYPES = ["int", "float", "long", "double", "short"]

if len(sys.argv) < 3:
    print(f"Usage: {sys.argv[0]} <template_path> <output_path>",
          file=sys.stderr)
    print("Error: Missing arguments.", file=sys.stderr)
    exit(1)


TEMPLATE_FILE = sys.argv[1]
OUTPUT_FILE = sys.argv[2]

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
