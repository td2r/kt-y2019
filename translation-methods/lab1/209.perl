while (<>) {
    s/\([^\)]*\)/\(\)/g;
    print;
}