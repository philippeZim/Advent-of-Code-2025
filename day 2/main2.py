with open("save.txt", "r", encoding="utf-8") as f:
    text = f.read().splitlines()
res = []
for s in text:
    s_fixed = s.replace("{", "{'").replace(": ", "': ").replace(", ", ", '")
    d = eval(s_fixed)
    new_str = str([x for x in d.values()]).replace("[", "{").replace("]", "}") + ","
    res.append(new_str)


with open("save2.txt", "w", encoding="utf-8") as f:
    f.write("\n".join(res))