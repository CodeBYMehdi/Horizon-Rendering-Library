import sys, os

input_file = sys.argv[1]
symbol = sys.argv[2]

with open(input_file, "rb") as f:
    data = f.read()

out = "#include <stddef.h>\n"
out += f"const unsigned char {symbol}[] = {{\n"
out += ','.join(f"0x{b:02x}" for b in data)
if(not len(data) <= 0):
    #on null-termine la chaine
    out += ",0x00"
out += "\n};\n"
#on rajoute 1 car on rajoute un 0x00 pour la null-terminaison
out += f"size_t {symbol}_len = {len(data) + 1};\n"

print(out)