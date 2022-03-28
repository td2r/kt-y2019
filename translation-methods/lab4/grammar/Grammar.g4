grammar Grammar;

grule : WS* NTNAME WS* ARROW (WS* NAME)* {printf("penis\n");};

WS     : [ \t\r\n] -> skip;
ARROW  : '->';
TNAME  : [a-z] [a-zA-Z0-9_]*;
NTNAME : [A-Z] [a-zA-Z0-9_]*;
NAME   : [a-zA-Z] [a-zA-Z0-9_]*;
