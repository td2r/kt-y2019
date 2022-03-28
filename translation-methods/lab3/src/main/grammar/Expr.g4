grammar Expr;

@header {
import java.util.HashMap;
import java.util.Map;
}

@parser::members{
Map<String, Integer> memory = new HashMap<>();
StringBuilder sb = new StringBuilder();
}

factor
returns [Integer res]
       : LPAREN expr RPAREN { $res = $expr.res; }
       | MINUS factor { $res = -$factor.res; }
       | VAR { $res = memory.get($VAR.text); }
       | NUM { $res = Integer.parseInt($NUM.text); }
       ;

logexp
returns [Integer res]
: factor SLASH2 logexp { $res = (int) Math.round(Math.log($factor.res) / Math.log($logexp.res)); }
| factor { $res = $factor.res; }
;

term
returns [Integer res]
: logexp term1[$logexp.res] { $res = $term1.res; };

term1
[Integer lhs]
returns [Integer res]
      : op=(ASTER | SLASH) logexp term1[$op.text.endsWith("*") ? $lhs * $logexp.res : $lhs / $logexp.res] { $res = $term1.res; }
      | { $res = $lhs; }
      ;

expr
returns [Integer res]
: term expr1[$term.res] { $res = $expr1.res; };

expr1
[Integer lhs]
returns [Integer res]
     : op=(PLUS | MINUS) term expr1[$op.text.endsWith("+") ? $lhs + $term.res : $lhs - $term.res] { $res = $expr1.res; }
     | { $res = $lhs; }
     ;

assign : VAR EQ expr SEMICOLON {
memory.put($VAR.text, $expr.res);
sb.append($VAR.text + " = " + $expr.res + "\n");
};

parse : assign* WS* EOF;

WS        : [ \r\n\t]+ -> skip;

PLUS      : WS* '+';
MINUS     : WS* '-';
ASTER     : WS* '*';
SLASH     : WS* '/';
SLASH2    : WS* '//';
LPAREN    : WS* '(';
RPAREN    : WS* ')';
EQ        : WS* '=' ;
SEMICOLON : WS* ';';

VAR       : [a-zA-Z]+;
NUM       : [0-9]+;
