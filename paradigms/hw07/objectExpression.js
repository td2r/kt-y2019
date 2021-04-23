"use strict";

function Const(value) { this.value = value; }

Const.ZERO = new Const(0);
Const.ONE = new Const(1);
Const.TWO = new Const(2);
Const.prototype.evaluate = function() { return this.value; };
Const.prototype.toString = function() { return this.value.toString(); };
Const.prototype.diff = function() { return Const.ZERO; };

function Variable(variable) {
    this.variable = variable;
    this.var_ind = Variable.VAR_IND[variable];
}

Variable.VAR_IND = { "x": 0, "y": 1, "z": 2 };
Variable.VX = new Variable("x");
Variable.VY = new Variable("y");
Variable.VZ = new Variable("z");
Variable.prototype.evaluate = function(...x) { return x[this.var_ind]; };
Variable.prototype.toString = function() { return this.variable; };
Variable.prototype.diff = function(dv) { return this.variable === dv ? Const.ONE : Const.ZERO; };

function Operator(f, symbol, diff_f, ...args) {
    this.f = f;
    this.symbol = symbol;
    this.diff_f = diff_f;
    this.args = args;
}

Operator.prototype.evaluate = function(...x) { return this.f(...this.args.map(currentValue => currentValue.evaluate(...x))); };
Operator.prototype.toString = function() { return this.args.concat([this.symbol]).join(" "); };
Operator.prototype.diff = function(dv) { return this.diff_f(dv, ...this.args); };

function AbstractOperator(...op_data) {
    let op = function(...args) { Operator.call(this, ...op_data, ...args); };
    op.argc = op_data[0].length;
    op.prototype = Object.create(Operator.prototype);
    return op;
}

const Add = AbstractOperator((a, b) => a + b, "+", (dv, a, b) => new Add(a.diff(dv), b.diff(dv)));

const Subtract = AbstractOperator((a, b) => a - b, "-", (dv, a, b) => new Subtract(a.diff(dv), b.diff(dv)));

const Multiply = AbstractOperator((a, b) => a * b, "*", (dv, a, b) => new Add(new Multiply(a.diff(dv), b), new Multiply(a, b.diff(dv))));

const Divide = AbstractOperator((a, b) => a / b, "/",
    (dv, a, b) => new Divide(new Subtract(new Multiply(a.diff(dv), b), new Multiply(a, b.diff(dv))), new Multiply(b, b)));

const Negate = AbstractOperator(a => -a, "negate", (dv, a) => new Negate(a.diff(dv)));

const Gauss = AbstractOperator((a, b, c, x) => a * Math.exp((x - b) * (b - x) / (2 * c * c)), "gauss",
    function (dv, a, b, c, x) {
        let xsb = new Subtract(x, b);
        let exp = new Negate(new Divide(new Multiply(xsb, xsb), new Multiply(Const.TWO, new Multiply(c, c))));
        return new Add(new Multiply(a.diff(dv), new Gauss(Const.ONE, b, c, x)), new Multiply(this, exp.diff(dv)));
    }
);




const OPERATION = {
    "+": Add,
    "-": Subtract,
    "*": Multiply,
    "/": Divide,
    "negate": Negate,
    "gauss": Gauss
};

const VARIABLES = {
    "x": Variable.VX,
    "y": Variable.VY,
    "z": Variable.VZ
};

function parse(s) {
    let stack = [];
    for (let token of s.split(" ").filter(token => token.length > 0)) {
        if (token in OPERATION) {
            let op = OPERATION[token];
            stack.push(new op(...stack.splice(-op.argc, op.argc)));
        } else if (token in VARIABLES) {
            stack.push(VARIABLES[token]);
        } else {
            stack.push(new Const(parseFloat(token)));
        }
    }
    return stack.pop();
}
