import org.antlr.v4.runtime.CharStreams;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

/*
Modification:
8 // 2 == 3
81 // 8 // 2 == 81 // (8 // 2) == 81 // 3 == 4
 */

public class Main {
    public static void main(String[] args) throws IOException {
        try (final InputStream inputStream = new FileInputStream("/home/td2r/Desktop/mt/lab3/input.txt")) {
            final ExprLexer exprLexer = new ExprLexer(CharStreams.fromStream(inputStream, StandardCharsets.UTF_8));
            final ExprParser exprParser = new ExprParser(new org.antlr.v4.runtime.CommonTokenStream(exprLexer));
            final ExprParser.ParseContext tree = exprParser.parse();
            System.out.println(exprParser.sb);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
