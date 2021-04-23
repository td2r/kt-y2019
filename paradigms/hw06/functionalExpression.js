"use strict";

const cnst = a => () => a;

const VAR_INDEX = {"x": 0, "y": 1, "z": 2};
const variable = s => {
    const i = VAR_INDEX[s];
    return (...x) => x[i];
};

const operator = f => (...args) => (...x) => f(...args.map(vars => vars(...x)));

const add = operator((a, b) => a + b);
const subtract = operator((a, b) => a - b);
const multiply = operator((a, b) => a * b);
const divide = operator((a, b) => a / b);
const negate = operator(a => -a);
const abs = operator(a => Math.abs(a));
const iff = operator((a, b, c) => a >= 0 ? b : c);

const OPERATION = {
    "+": [add, 2],
    "-": [subtract, 2],
    "*": [multiply, 2],
    "/": [divide, 2],
    "negate": [negate, 1],
    "abs": [abs, 1],
    "iff": [iff, 3]
};

const one = cnst(1);
const two = cnst(2);

const CONSTANT = {
    "one": one,
    "two": two
};

const VARIABLES = {
    "x": variable("x"),
    "y": variable("y"),
    "z": variable("z")
};

function parse(s) {
    let stack = [];
    for (let token of s.split(" ").filter(token => token.length > 0)) {
        if (token in OPERATION) {
            let op = OPERATION[token];
            stack.push(op[0](...stack.splice(-op[1], op[1])));
        } else if (token in CONSTANT) {
            stack.push(CONSTANT[token]);
        } else if (token in VARIABLES) {
            stack.push(VARIABLES[token]);
        } else {
            stack.push(cnst(parseFloat(token)));
        }
    }
    return stack.pop();
}
