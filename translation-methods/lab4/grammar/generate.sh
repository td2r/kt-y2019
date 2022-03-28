#!/bin/bash

cd "$(dirname $0)" || exit

#antlr4="java -jar /usr/local/lib/antlr-4.9.2-complete.jar"

java -jar /usr/local/lib/antlr-4.9.2-complete.jar -o ../ -no-listener -Dlanguage=Cpp Grammar.g4
rm ../*.interp ../*.tokens
