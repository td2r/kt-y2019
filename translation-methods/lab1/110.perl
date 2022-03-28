while (<>) {
    print if /\b(.+)\g1\b/;
}
