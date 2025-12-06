"""
def is_valid(x):
    x_string = str(x)
    if len(x_string) % 2 != 0:
        return False
    l = 0
    r = len(x_string) // 2
    for i in range(r):
        if x_string[i] != x_string[r + i]:
            return False
    return True


class RangeSum:
    def __init__(self, low, high, invalid_sum):
        self.low = low
        self.high = high
        self.invalid_sum = invalid_sum
    
    def __repr__(self):
        return f"{{low: {self.low}, high: {self.high}, invalid_sum: {self.invalid_sum}}}"

ranges = []
idx = 0
g_invalid_sum = 0
for x in range(10_000_000_000):
    if is_valid(x):
        ranges.append(RangeSum(idx, x-1, g_invalid_sum))
        idx = x
        print(x)
        g_invalid_sum += x

print(len(ranges))
ranges = [str(x) for x in ranges]
with open("save.txt", "w", encoding="utf-8") as f:
    f.write("\n".join(ranges))
"""
class RangeSum:
    def __init__(self, low, high, invalid_sum):
        self.low = low
        self.high = high
        self.invalid_sum = invalid_sum
    
    def __repr__(self):
        return f"{{low: {self.low}, high: {self.high}, invalid_sum: {self.invalid_sum}}}"
    

ranges = []
idx = 0
g_invalid_sum = 0

for i in range(1, 100_000):
    
    num_str = f"{i}{i}"
    num = int(num_str)
    ranges.append(RangeSum(idx, num-1, g_invalid_sum))
    idx = num
    g_invalid_sum += num
    print(num_str)


def index_of_el(el):
    l, r = 0, len(ranges)
    while l < r:
        m = (l + r) // 2
        if el < ranges[m].low:
            r = m
        elif el > ranges[m].high:
            l = m + 1
        else:
            return m
    return -1

# 11-22
def get_val_for_range(low, high):
    return ranges[index_of_el(high)].invalid_sum - ranges[index_of_el(low-1)].invalid_sum

test1 = """9100-11052,895949-1034027,4408053-4520964,530773-628469,4677-6133,2204535-2244247,55-75,77-96,6855-8537,55102372-55256189,282-399,228723-269241,5874512-6044824,288158-371813,719-924,1-13,496-645,8989806846-8989985017,39376-48796,1581-1964,699387-735189,85832568-85919290,6758902779-6759025318,198-254,1357490-1400527,93895907-94024162,21-34,81399-109054,110780-153182,1452135-1601808,422024-470134,374195-402045,58702-79922,1002-1437,742477-817193,879818128-879948512,407-480,168586-222531,116-152,35-54"""

a = test1.split(",")
g_res = 0
for x in a:
    b = [int(y) for y in x.split("-")]
    g_res += get_val_for_range(*b)
print(g_res)
print(len(ranges))


print(len(ranges))
ranges = [str(x) for x in ranges]
with open("save.txt", "w", encoding="utf-8") as f:
    f.write("\n".join(ranges))