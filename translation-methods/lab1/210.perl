$good = 'a.*?a';
while (<>) {
    s/$good$good$good/bad/g;
    print;
}
