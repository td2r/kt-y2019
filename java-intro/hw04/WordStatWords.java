import java.io.*;
import java.util.Arrays;
import java.util.Scanner;

public class WordStatWords {
    public static boolean isWordCharacter(char c) {
        return Character.isLetter(c) || c == '\'' || Character.getType(c) == Character.DASH_PUNCTUATION;
    }

    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Not enough arguments");
            return;
        }
        if (args[0] == null) {
            System.out.println("Error, input file name is null");
            return;
        }
        if (args[1] == null) {
            System.out.println("Error, output file name is null");
            return;
        }
        
        int n = 0;
        String[] words = new String[1];

        try {
            BufferedReader in = new BufferedReader(
                new InputStreamReader(
                    new FileInputStream(new File(args[0])), "utf8"
                )
            );
            String s;
            try {
                while ((s = in.readLine()) != null) {
                    s = s.toLowerCase();
                    int len = s.length();
                    for (int r = 0; r < len;) {
                        while (r < len && !isWordCharacter(s.charAt(r))) {
                            r++;
                        }
                        int l = r;
                        while (r < len && isWordCharacter(s.charAt(r))) {
                            r++;
                        }
                        if (l != r) {
                            if (n >= words.length) {
                                words = Arrays.copyOf(words, 2 * n);
                            }
                            words[n++] = s.substring(l, r);
                        }
                    }
                }
            } finally {
                in.close();
            }
        } catch (FileNotFoundException e) {
            System.out.println("Input file not found: " + e.getMessage());
            return;
        } catch (SecurityException e) {
            System.out.println("You don't have access to the input file: " + e.getMessage());
            return;
        } catch (IOException e) {
            System.out.println("Input file I/O error occurs: " + e.getMessage());
            return;
        }

        Arrays.sort(words, 0, n);

        try {
            BufferedWriter out = new BufferedWriter(
                    new OutputStreamWriter(
                    new FileOutputStream(new File(args[1])), "utf8"
                )
            );
            try {
                for (int i = 0; i < n;) {
                    int count = 0;
                    String s = words[i];
                    while (i < n && words[i].equals(s)) {
                        count++;
                        i++;
                    }
                    out.write(s + " " + count);
                    out.newLine();
                }
            } finally {
                out.close();
            }
        } catch (FileNotFoundException e) {
            System.out.println("Output file not found: " + e.getMessage());
            return;
        } catch (SecurityException e) {
            System.out.println("You don't have access to the output file: " + e.getMessage());
            return;
        } catch (IOException e) {
            System.out.println("Output file I/O error occurs: " + e.getMessage());
            return;
        }
    }
}
