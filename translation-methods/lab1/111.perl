$old = '(0|1(01*0)*1)+';
while (<>) {
    print if /^$old$/;
}
