#!/bin/bash

cd "$(dirname $0)/src/main/grammar" || exit

#antlr4="java -jar /usr/local/lib/antlr-4.9.2-complete.jar"

java -jar /usr/local/lib/antlr-4.9.2-complete.jar -o ../java -no-listener Expr.g4
rm ../java/*.interp
rm ../java/*.tokens
