$nobr = '[^\(\)]';
while (<>) {
    print if /\($nobr*\w$nobr*\)/;
}
