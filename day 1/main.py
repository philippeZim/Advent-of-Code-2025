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

# Part 2

def help1(roll, x, res):
    n = int(x[1:])
    if x[0] == 'R':
        for _ in range(n):
            if roll == 99:
                roll = 0
            else:
                roll += 1
            if roll == 0:
                res += 1
    else:
        for _ in range(n):
            if roll == 0:
                roll = 99
            else:
                roll -= 1
            if roll == 0:
                res += 1
                
    return roll, res
        
res = 0
g_roll = 50
for x in lines:
    g_roll, res = help1(g_roll, x, res)

print(res)
    