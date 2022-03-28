$chars = '[x,y,z]';
while (<>) {
    print if /$chars.{5,17}$chars/;
}
