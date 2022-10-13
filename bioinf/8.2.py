def table_from_input(rows):
    table = {}
    columns = input().split()
    for i in range(len(rows)):
        row = input().split()
        table[rows[i]] = {columns[j - 1]: float(row[j]) for j in range(1, len(row))}
    return table

q = int(input())
input()
Y = input()
T = len(Y)
input()
y_alpha = input().split()
k = len(y_alpha)
input()
x_alpha = input().split()
n = len(x_alpha)
input()
a = table_from_input(x_alpha)
input()
b = table_from_input(x_alpha)

prec = 3

for _ in range(q):
    alpha = {x: [0] * (T + 1) for x in x_alpha}
    beta = {x: [0] * (T + 1) for x in x_alpha}

    for i in x_alpha:
        alpha[i][1] = b[i][Y[0]] / n
    for t in range(2, T + 1):
        for i in x_alpha:
            # Y is 0-index
            alpha[i][t] =  b[i][Y[t - 1]] * \
                    sum(alpha[j][t - 1] * a[j][i] for j in x_alpha)

    for i in x_alpha:
        beta[i][T] = 1
    for t in range(T - 1, 0, -1):
        for i in x_alpha:
            # Y is 0-index
            beta[i][t] = sum(beta[j][t + 1] * a[i][j] * b[j][Y[t]] for j in x_alpha)

    gamma = {i: [0] + [alpha[i][t] * beta[i][t] for t in range(1, T + 1)] \
                                                for i in x_alpha}

    xi = {i : {j : [0] + [alpha[i][t] * a[i][j] * beta[j][t + 1] * b[j][Y[t]] \
            for t in range(1, T)] \
            for j in x_alpha} \
            for i in x_alpha} # Y is 0-index

    a1 = {i : {j: 0 for j in x_alpha} for i in x_alpha}
    for i in x_alpha:
        denom = sum(gamma[i][t] for t in range(1, T))
        for j in x_alpha:
            a1[i][j] = sum(xi[i][j]) / denom
    
    b1 = {i : {j: 0 for j in y_alpha} for i in x_alpha}
    for i in x_alpha:
        denom = sum(gamma[i])
        for j in y_alpha:
            num = 0
            for t in range(1, T + 1):
                if Y[t - 1] == j:
                    num += gamma[i][t]
            b1[i][j] = num / denom

    a = a1
    b = b1

    print(' '.join(x_alpha))
    for i in x_alpha:
        print(i, end='')
        for j in range(len(x_alpha)):
            print(' ' + str(round(a[i][x_alpha[j]], prec)), end='')
        print()
    print("--------")
    print(' '.join(y_alpha))
    for i in x_alpha:
        print(i, end='')
        for j in range(len(y_alpha)):
            print(' ' + str(round(b[i][y_alpha[j]], prec)), end='')
        print()
    print()
    print()

