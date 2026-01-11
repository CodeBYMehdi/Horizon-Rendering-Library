import sys, os

input_file = sys.argv[1]
symbol = sys.argv[2]

with open(input_file, "rb") as f:
    data = f.read()

out = f"const unsigned char {symbol}[] = {{\n"
out += ','.join(f"0x{b:02x}" for b in data)
out += "\n};\n"
out += f"const unsigned int {symbol}_len = {len(data)};\n"

print(out)