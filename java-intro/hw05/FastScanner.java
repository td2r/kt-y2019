import java.io.*;
import java.lang.Integer;
import java.lang.Character;
import java.lang.StringBuilder;

public class FastScanner {
    private static boolean[] buildTokenSeparator() {
        boolean[] tokenSeparator = new boolean[Character.MAX_VALUE];
        for (char c = 0; c < Character.MAX_VALUE; c++) {
            tokenSeparator[c] = Character.isWhitespace(c);
        }
        return tokenSeparator;
    }

    private static boolean[] buildWordSeparator() {
        boolean[] wordSeparator = new boolean[Character.MAX_VALUE];
        for (char c = 0; c < Character.MAX_VALUE; c++) {
            wordSeparator[c] = !Character.isLetter(c) && c != '\'' &&
                                Character.getType(c) != Character.DASH_PUNCTUATION;
        }
        return wordSeparator;
    }

    private static final boolean[] TOKEN_SEPARATOR = buildTokenSeparator();
    private static final boolean[] WORD_SEPARATOR = buildWordSeparator();

    private final Reader reader;

    private int c;
    private boolean used = true;

    private String token = "";


    FastScanner(InputStream in) {
        reader = new BufferedReader(new InputStreamReader(in));
    }

    FastScanner(InputStream in, String charsetName)
            throws UnsupportedEncodingException {
        reader = new BufferedReader(new InputStreamReader(in, charsetName));
    }

    FastScanner(File source, String charsetName)
            throws FileNotFoundException, UnsupportedEncodingException {
        reader = new BufferedReader(
                new InputStreamReader(new FileInputStream(source), charsetName)
                );
    }

    FastScanner(String source) {
        reader = new StringReader(source);
    }

    public void close() throws IOException {
        reader.close();
    }


    private void extractToken(boolean inLine, boolean[] separator)
            throws IOException {
        StringBuilder sb = new StringBuilder();
        if (used) {
            c = reader.read();
            used = false;
        }
        while (c != -1) {
            if (inLine && c == '\n') {
                break;
            }
            if (!separator[c]) {
                sb.append((char) c);
            } else if (sb.length() != 0) {
                break;
            }
            c = reader.read();
        }
        token = sb.toString();
    }


    public boolean hasNext(boolean inLine) throws IOException {
        if (token.isEmpty()) {
            extractToken(inLine, TOKEN_SEPARATOR);
        }
        return !token.isEmpty();
    }

    public boolean hasNextInt(boolean inLine) throws IOException {
        return hasNextInt(inLine, 10);
    }

    public boolean hasNextInt(boolean inLine, int radix) throws IOException {
        if (!hasNext(inLine)) {
            return false;
        }
        try {
            Integer.parseInt(token, radix);
            return true;
        } catch (NumberFormatException e) {
            return false;
        }
    }

    public boolean hasNextLine() throws IOException {
        if (used) {
            c = reader.read();
            used = false;
        }
        return c != -1;
    }

    public boolean hasNextWord(boolean inLine) throws IOException {
        if (token.isEmpty()) {
            extractToken(inLine, WORD_SEPARATOR);
        }
        return !token.isEmpty();
    }

    public String next(boolean inLine) throws IOException {
        if (!hasNext(inLine)) {
            throw new IOException("EOF reached");
        }
        String ret = token;
        token = "";
        return ret;
    }

    public String nextLine() throws IOException {
        if (!hasNextLine()) {
            throw new IOException("EOF reached");
        }
        StringBuilder sb = new StringBuilder();
        if (used) {
            c = reader.read();
            used = false;
        }
        while (c != -1 && c != '\n') {
            sb.append((char)c);
            c = reader.read();
        }
        c = reader.read();
        return sb.toString();
    }

    public void skipLine() throws IOException {
        if (used) {
            c = reader.read();
            used = false;
        }
        while (c != -1 && c != '\n') {
            c = reader.read();
        }
        c = reader.read();
    }


    public int nextInt(boolean inLine) throws IOException {
        return nextInt(inLine, 10);
    }

    public int nextInt(boolean inLine, int radix) throws IOException {
        if (!hasNextInt(inLine, radix)) {
            if (token.isEmpty()) {
                throw new IOException("EO" + (inLine ? "L" : "F") + "reached");
            } else {
                throw new IOException("Next token not number");
            }
        }
        int ret = Integer.parseInt(token, radix);
        token = "";
        return ret;
    }

    public String nextWord(boolean inLine) throws IOException {
        if (!hasNextWord(inLine)) {
            throw new IOException("EO" + (inLine ? "L" : "F") + " reached");
        }
        String ret = token;
        token = "";
        return ret;
    }
}
