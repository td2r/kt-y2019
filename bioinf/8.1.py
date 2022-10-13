def parse_table(table, n1):
    keys2 = input().split()
    n2 = len(keys2)
    for _ in range(n1):
        spl = input().split()
        table[spl[0]] = {}
        for i in range(n2):
            table[spl[0]][keys2[i]] = float(spl[i + 1])

x = input()
input()
obss = input().split() # observations
input()
states = input().split()
input()
a = {}
parse_table(a, len(states))
input()
b = {}
parse_table(b, len(states))

parent = [{} for _ in range(len(x))]

p = {}
for state in states:
    p[state] = b[state][x[0]]

for i in range(1, len(x)):
    p_next = {}
    for j in states:
        p_next[j] = -1
        for k in states:
            upd_value = p[k] * a[k][j] * b[j][x[i]]
            if upd_value > p_next[j]:
                p_next[j] = upd_value
                parent[i][j] = k
    (p, p_next) = (p_next, p)

s = states[0]
for state in states:
    if p[state] > p[s]:
        s = state

path = s
i = len(x) - 1
while i != 0:
    s = parent[i][s]
    path += s
    i = i - 1

print(path[::-1])
