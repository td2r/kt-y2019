# E' = D
# T' = U
G = {
        "E" : { "TD" },
        "D" : { "+TD", "-TD", "" },
        "T" : { "FU" },
        "U" : { "*FU", "/FU", "" },
        "F" : { "f(A)", "(E)", "-F", "n" },
        "A" : { "EB", "" },
        "B" : { ",EB", "" }
    }

# G = {
#         "E" : { "E+T", "T" },
#         "T" : { "T*F", "F" },
#         "F" : { "(E)", "n" }
#     }

S = "E"

first  = { nt : set({}) for nt in G.keys() }
follow = { nt : set({}) for nt in G.keys() }
follow[S].update("$")

def first_of(a):
    if len(a) == 0:
        return { "" }
    elif not a[0].isupper():
        return { a[0] }
    res = first[a[0]]
    if "" in res:
        res = (res - { "" }).union(first_of(a[1:]))
    return res

def calc_ff():
    changed = True
    while (changed):
        changed = False
        for A, alphas in G.items():
            for a in alphas:
                if not (first_of(a).issubset(first[A])):
                    first[A].update(first_of(a))
                    changed = True
    changed = True
    while (changed):
        changed = False
        for A, alphas in G.items():
            for a in alphas:
                for i in range(len(a)):
                    if not a[i].isupper():
                        continue
                    beta = a[:i]
                    B = a[i]
                    gamma = a[i+1:]
                    upd = first_of(gamma)
                    if "" in upd:
                        upd = (upd - {""}).union(follow[A])
                    if not (upd.issubset(follow[B])):
                        changed = True
                        follow[B].update(upd)

calc_ff()

print("FIRST")
for A, firstA in first.items():
    print(A + ": " + str(sorted(firstA)))
print("FOLLOW")
for A, followA in follow.items():
    print(A + ": " + str(sorted(followA)))
