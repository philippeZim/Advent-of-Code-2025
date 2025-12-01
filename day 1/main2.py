with open("/home/eron/Documents/Programming/C/Advent-of-Code-2025/day 1/input.txt", "r", encoding="utf-8") as f:
    lines = f.read().splitlines()

# Part 1

res_1 = 0
c = 50

for x in lines:
    n = int(x[1:])
    if x[0] == 'L':
        c = (c - n) % 100
    else:
        c = (c + n) % 100
    if c == 0:
        res_1 += 1

print(res_1)


res_2 = 0
c = 50

for i, x in enumerate(lines):

    n = int(x[1:])
    res_2 += n // 100
    n %= 100
    if x[0] == 'L':
        if c != 0 and n > c:
            res_2 += 1
        c = (c - n) % 100
    else:
        if c + n > 100:
            res_2 += 1
        c = (c + n) % 100
    if c == 0:
        res_2 += 1
    

print(res_2)

