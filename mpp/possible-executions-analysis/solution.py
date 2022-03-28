P = [ None
    , "new_state['a'] = 1\nnew_state['x'] = 2"
    , "if state['b'] == 0: new_state['x'] = 3"
    , "new_state['x'] = 4"
    , "new_state['a'] = 0\nnew_state['x'] = 1" ]

Q = [ None
    , "new_state['b'] = 1\nnew_state['y'] = 2"
    , "new_state['y'] = 4 if state['a'] == 0 else 3"
    , "new_state['b'] = 0\nnew_state['y'] = 1"
    , "new_state = None" ] # doesn't work

edges = set(['[P1,Q1,0,0] -> [P2,Q1,1,0]'])
states = set([])
global new_state

def state_to_str(state):
    return '[P%d,Q%d,%d,%d]' % (state['x'], state['y'], state['a'], state['b'])

def dfs(state):
    states.add(state_to_str(state))

    new_state = state.copy()
    exec(P[state['x']])
    edges.add('%s -> %s' % (state_to_str(state), state_to_str(new_state)))
    if not state_to_str(new_state) in states:
        dfs(new_state)

    if state['y'] != 4: # don't know how to deal with it in other way
        new_state = state.copy()
        exec(Q[state['y']])
        edges.add('%s -> %s' % (state_to_str(state), state_to_str(new_state)))
        if not state_to_str(new_state) in states:
            dfs(new_state)

with open('solution.txt', 'w+') as file:
    file.write('Наумов Иван\n')
    start = { 'a': 0, 'b': 0, 'x': 1, 'y': 1 }
    dfs(start)
    for edge in sorted(edges):
        file.write(edge)
        file.write('\n')
