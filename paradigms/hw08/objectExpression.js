"use strict";

function Const(value) {
    this.getValue = () => value;
}

Const.ZERO = new Const(0);
Const.ONE = new Const(1);
Const.TWO = new Const(2);
Const.prototype.evaluate = function() { return this.getValue(); };
Const.prototype.toString = function() { return this.getValue().toString(); };
Const.prototype.diff = function() { return Const.ZERO; };
Const.prototype.prefix = Const.prototype.toString;
Const.prototype.postfix = Const.prototype.toString;

function Variable(variable) {
    this.getVariable = () => variable;
    this.getId = function() { const id = Variable.VAR_IND[variable]; return id; };
}

Variable.VAR_IND = { "x": 0, "y": 1, "z": 2 };
Variable.VX = new Variable("x");
Variable.VY = new Variable("y");
Variable.VZ = new Variable("z");
Variable.prototype.evaluate = function(...x) { return x[this.getId()]; };
Variable.prototype.toString = function() { return this.getVariable(); };
Variable.prototype.diff = function(dv) { return this.getVariable() === dv ? Const.ONE : Const.ZERO; };
Variable.prototype.prefix = Variable.prototype.toString;
Variable.prototype.postfix = Variable.prototype.toString;

function Operator(f, symbol, diff, ...args) {
    this.getFunc = () => f;
    this.getSymbol = () => symbol;
    this.getDiff = () => diff;
    this.getArgs = () => args;
}

Operator.prototype.evaluate = function(...x) { return this.getFunc()(...this.getArgs().map(it => it.evaluate(...x))); };
Operator.prototype.toString = function() { return this.getArgs().concat([this.getSymbol()]).join(" "); };
Operator.prototype.diff = function(dv) { return this.getDiff()(dv, ...this.getArgs()); };
Operator.prototype.prefix = function() { return "(" + [this.getSymbol()].concat(this.getArgs().map(it => it.prefix())).join(" ") + ")"; };
Operator.prototype.postfix = function() { return "(" + this.getArgs().map(it => it.postfix()).join(" ") + " " + this.getSymbol() + ")"; };

function createOperation(...opData) { // op_data is { f, symbol, diff_f }
    let op = function(...args) { Operator.call(this, ...opData, ...args); };
    op.getArgc = () => opData[0].length;
    op.prototype = Object.create(Operator.prototype);
    return op;
}

const Add = createOperation((a, b) => a + b, "+", (dv, a, b) => new Add(a.diff(dv), b.diff(dv)));

const Subtract = createOperation((a, b) => a - b, "-", (dv, a, b) => new Subtract(a.diff(dv), b.diff(dv)));

const Multiply = createOperation((a, b) => a * b, "*", (dv, a, b) => new Add(new Multiply(a.diff(dv), b), new Multiply(a, b.diff(dv))));

const Divide = createOperation((a, b) => a / b, "/",
    (dv, a, b) => new Divide(new Subtract(new Multiply(a.diff(dv), b), new Multiply(a, b.diff(dv))), new Multiply(b, b)));

const Negate = createOperation(a => -a, "negate", (dv, a) => new Negate(a.diff(dv)));

const Gauss = createOperation((a, b, c, x) => a * Math.exp((x - b) * (b - x) / (2 * c * c)), "gauss",
    function (dv, a, b, c, x) {
        let xsb = new Subtract(x, b);
        let exp = new Negate(new Divide(new Multiply(xsb, xsb), new Multiply(Const.TWO, new Multiply(c, c))));
        return new Add(new Multiply(a.diff(dv), new Gauss(Const.ONE, b, c, x)), new Multiply(new Gauss(a, b, c, x), exp.diff(dv)));
    }
);

const Mean = createOperation((...args) => args.reduce((a, it) => a + it, 0) / args.length, "mean",
    (dv, ...args) => new Divide(args.reduce((a, it) => new Add(a, it.diff(dv)), Const.ZERO), new Const(args.length))
);

const Var = createOperation(
    function (...args) {
        let e = args.reduce((a, it) => a + it, 0) / args.length;
        return args.reduce((a, it) => a + (it - e) * (it - e), 0) / args.length;
    }, "var",
    function (dv, ...args) {
        let mean = new Mean(...args);
        return new Subtract(new Divide(args.reduce((a, it) => new Add(a, new Multiply(Const.TWO, new Multiply(it, it.diff(dv)))), Const.ZERO),
            new Const(args.length)),
            new Multiply(Const.TWO, new Multiply(mean, mean.diff(dv))));
    });




const OPERATION = {
    "+": Add,
    "-": Subtract,
    "*": Multiply,
    "/": Divide,
    "negate": Negate,
    "gauss": Gauss,
    "mean": Mean,
    "var": Var
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
            stack.push(new op(...stack.splice(-op.getArgc(), op.getArgc())));
        } else if (token in VARIABLES) {
            stack.push(VARIABLES[token]);
        } else {
            stack.push(new Const(parseFloat(token)));
        }
    }
    return stack.pop();
}

function createError(name, message) {
    const error = function(...args) {
        this.name = name;
        this.message = message(...args);
    };
    error.prototype = Object.create(Error.prototype);
    return error;
}

const UnknownTokenError = createError(
    "UnknownTokenError",
    (token, index) => "Unknown token: " + token + " at " + index
);
const ExpectationError = createError(
    "ExpectationError",
    (expected, found, index) => "Expectation error at " + index + ": expected " + expected + ", found " + found
);
const ArgcMismatchError = createError(
    "ArgcMismatchError",
    (op, argcFound, index) =>
    "Mismatch arguments number of operator " + op.getSymbol() + " at " + index + ": expected " + op.getArgc() + ", found " + argcFound
);

function isDigit(s) {
    return '0' <= s && s <= '9';
}

function isLowerCaseLetter(s) {
    return 'a' <= s && s <= 'z';
}

function isUpperCaseLetter(s) {
    return 'A' <= s && s <= 'Z';
}

function isLetter(s) {
    return isLowerCaseLetter(s) || isUpperCaseLetter(s);
}

function isLetterOrDigit(s) {
    return isLetter(s) || isDigit(s);
}

function isNumber(s) {
    if (s.length === 0) return false;
    let i = 0;
    if (s[i] === '-') {
        if (s.length === 1) {
            return false;
        }
        i++;
    }
    while (i < s.length && isDigit(s.charAt(i))) {
        ++i;
    }
    return i === s.length;
}

function TokenSource(s) {
    let i = 0;
    this.getToken = function() {
        while (i < s.length && s.charAt(i) === " ") {
            ++i;
        }
        if (i === s.length) {
            return "";
        }
        if (s.charAt(i) === '-' || isLetterOrDigit(s.charAt(i))) {
            let result = [];
            if (s.charAt(i) === '-') {
                result.push('-');
                ++i;
            }
            while (i < s.length && isLetterOrDigit(s.charAt(i))) {
                result.push(s.charAt(i++));
            }
            return result.join("");
        } else {
            return s.charAt(i++);
        }
    };
    this.getIndex = () => i;
}


function parsePrefix(s) {
    if (typeof s !== "string") {
        throw new TypeError("Parsing value is not a string");
    }
    let source = new TokenSource(s);
    let token = source.getToken();
    let parsePrefixImpl = function() {
        if (token === "(") {
            token = source.getToken();
            if (!(token in OPERATION)) {
                throw new ExpectationError("operator", token, source.getIndex());
            }
            let op = OPERATION[token];
            let args = [];
            token = source.getToken();
            while (token !== ")" && token !== "") {
                args.push(parsePrefixImpl());
                token = source.getToken();
            }
            if (op.getArgc() !== 0 && args.length !== op.getArgc()) {
                throw new ArgcMismatchError(op.getSymbol(), op.getArgc(), args.length, source.getIndex());
            }
            if (token !== ")") {
                throw new ExpectationError(")", token, source.getIndex());
            }
            return new op(...args);
        } else if (token in VARIABLES) {
            return VARIABLES[token];
        } else if (isNumber(token)) {
            return new Const(Number.parseInt(token));
        } else {
            throw new UnknownTokenError(token, source.getIndex());
        }
    };
    let result = parsePrefixImpl();
    if ((token = source.getToken()) !== "") {
        throw new ExpectationError("end of expression", token, source.getIndex());
    }
    return result;
}

function parsePostfix(s) {
    if (typeof s !== "string") {
        throw new TypeError("Parsing value is not a string");
    }
    let source = new TokenSource(s);
    let token = source.getToken();
    let parsePostfixImpl = function() {
        if (token === "(") {
            let args = [];
            token = source.getToken();
            while (token !== ")" && token !== "" && !(token in OPERATION)) {
                args.push(parsePostfixImpl());
                token = source.getToken();
            }
            if (!(token in OPERATION)) {
                throw new ExpectationError("operator", token, source.getIndex());
            }
            let op = OPERATION[token];
            if ((token = source.getToken()) !== ")" ) {
                throw new ExpectationError(")", token, source.getIndex());
            }
            if (op.getArgc() !== 0 && args.length !== op.getArgc()) {
                throw new ArgcMismatchError(op, args.length, source.getIndex());
            }
            return new op(...args);
        } else if (token in VARIABLES) {
            return VARIABLES[token];
        } else if (isNumber(token)) {
            return new Const(Number.parseInt(token));
        } else {
            throw new UnknownTokenError(token, source.getIndex());
        }
    };
    let result = parsePostfixImpl();
    if ((token = source.getToken()) !== "") {
        throw new ExpectationError("end of expression", token, source.getIndex());
    }
    return result;
}