$_ = do { local $/; <> };

s/<.*?>//g;
s/[\ \n]*//;  # Remove empty lines and leading spaces before first text line
s/[\ \n]*$//; # Remove empty lines and trailing spaces after last text line
s/\n\ */\n/g;
s/\ *\n/\n/g;
s/\n{2,}/\n\n/g;
s/\ +/\ /g;

print;
