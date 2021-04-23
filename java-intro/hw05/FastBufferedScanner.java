import java.io.*;
import java.lang.Integer;
import java.lang.Character;
import java.lang.StringBuilder;

/*

TODO:
> close() throws IOException
> no var names starts with _
> s.length() == 0 is equal to s.isEmpty()
> precalc token and word separators

*/

public class FastBufferedScanner {
    private final int LENGTH = 1024;
    private final Reader reader;

    private char[] buff = new char[LENGTH];
    private int bpos;
    private int bsize;
    private String token = "";


    FastBufferedScanner(Reader reader) {
        this.reader = reader;
    }

    FastBufferedScanner(InputStream in) {
        reader = new InputStreamReader(in);
    }
    FastBufferedScanner(InputStream in, String charsetName) throws UnsupportedEncodingException {
        reader = new InputStreamReader(in, charsetName);
    }

    FastBufferedScanner(File source, String charsetName) throws FileNotFoundException, UnsupportedEncodingException {
        reader = new InputStreamReader(new FileInputStream(source), charsetName);
    }

    FastBufferedScanner(String source) {
        reader = new StringReader(source);
    }

    public void close() {
        try {
            reader.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }


    private void _readBuffer() throws IOException {
        bpos = 0;
        bsize = reader.read(buff);
    }

    private void _extractToken(boolean inLine, boolean word) throws IOException {
        StringBuilder sb = new StringBuilder();
        if (bpos == bsize) {
            _readBuffer();
        }
        while (bpos < bsize) {
            if (inLine && buff[bpos] == '\n') {
                break;
            }
            if (!word && !Character.isWhitespace(buff[bpos]) ||
                word && isWordChar(buff[bpos])) {
                sb.append(buff[bpos]);
            } else if (sb.length() != 0) {
                break;
            }
            bpos++;
            if (bpos == bsize) {
                _readBuffer();
            }
        }
        token = sb.toString();
    }

    private boolean isWordChar(char c) {
        return Character.isLetter(c) || c == '\'' ||
               Character.getType(c) == Character.DASH_PUNCTUATION;
    }


    public void skipLine() throws IOException {
        if (bpos == bsize) {
            _readBuffer();
        }
        while (bpos < bsize) {
            if (buff[bpos] == '\n') {
                bpos++;
                if (bpos == bsize) {
                    _readBuffer();
                }
                break;
            }
            bpos++;
        }
    }


    public boolean hasNext() throws IOException {
        if (token.length() == 0) {
            _extractToken(false, false);
        }
        return token.length() != 0;
    }

    public boolean hasNextInLine() throws IOException {
        if (token.length() == 0) {
            _extractToken(true, false);
        }
        return token.length() != 0;
    }

    public boolean hasNextInt() throws IOException {
        return hasNextInt(10);
    }

    public boolean hasNextInt(int radix) throws IOException {
        if (!hasNext()) {
            return false;
        }
        try {
            Integer.parseInt(token, radix);
        } catch (NumberFormatException e) {
            return false;
        }
        return true;
    }

    public boolean hasNextIntInLine() throws IOException {
        return hasNextIntInLine(10);
    }

    public boolean hasNextIntInLine(int radix) throws IOException {
        if (!hasNextInLine()) {
            return false;
        }
        try {
            Integer.parseInt(token, radix);
        } catch (NumberFormatException e) {
            return false;
        }
        return true;
    }

    public boolean hasNextLine() {
        return bsize != -1;
    }

    public boolean hasNextWord() throws IOException {
        if (token.length() == 0) {
            _extractToken(false, true);
        }
        return token.length() != 0;
    }

    public boolean hasNextWordInLine() throws IOException {
        if (token.length() == 0) {
            _extractToken(true, true);
        }
        return token.length() != 0;
    }


    public String next() throws IOException {
        if (!hasNext()) {
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
        if (bpos == bsize) {
            _readBuffer();
        }
        while (bpos < bsize) {
            if (buff[bpos] == '\n') {
                bpos++;
                if (bpos == bsize) {
                    _readBuffer();
                }
                break;
            } else {
                sb.append(buff[bpos]);
            }
            bpos++;
            if (bpos == bsize) {
                _readBuffer();
            }
        }
        return sb.toString();
    }

    public int nextInt() throws IOException {
        return nextInt(10);
    }

    public int nextInt(int radix) throws IOException {
        if (!hasNextInt()) {
            throw new IOException("EOF reached or next token not number");
        }
        int ret = Integer.parseInt(token, radix);
        token = "";
        return ret;
    }

    public int nextIntInLine() throws IOException {
        return nextIntInLine(10);
    }

    public int nextIntInLine(int radix) throws IOException {
        if (!hasNextIntInLine()) {
            throw new IOException("EOL reached or next token not number");
        }
        int ret = Integer.parseInt(token, radix);
        token = "";
        return ret;
    }

    public String nextWord() throws IOException {
        if (!hasNextWord()) {
            throw new IOException("EOF reached");
        }
        String ret = token;
        token = "";
        return ret;
    }

    public String nextWordInLine() throws IOException {
        if (!hasNextWordInLine()) {
            throw new IOException("EOL reached");
        }
        String ret = token;
        token = "";
        return ret;
    }
}
