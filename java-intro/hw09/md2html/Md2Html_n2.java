package md2html;

import java.io.*;
import java.lang.*;
import java.util.*;

public class Md2Html_n2 {
    private static String openTag(String s) {
        if (s.equals("")) {
            return "";
        }
        return "<" + s + ">";
    }

    private static String closeTag(String s) {
        if (s.equals("")) {
            return "";
        }
        return "</" + s + ">";
    }

    private static boolean isSpecialChar(char c) {
        return c == '*' || c == '_' || c == '-' || c == '`' || c == '/';
    }

    private static String markerToTag(String marker) {
        switch (marker) {
            case "*":
            case "_":
                return "em";
            case "**":
            case "__":
                return "strong";
            case "--":
                return "s";
            case "`":
                return "code";
            default:
                return "";
        }
    }

    private static int indexOf(String s, String tmp, int i, int endIndex) {
        int m = tmp.length();
        for (; i < endIndex; ++i) {
            char c = s.charAt(i);
            if (c == '\\') {
                ++i;
            } else if (i + m <= endIndex && s.startsWith(tmp, i)) {
                return i;
            }
        }
        return -1;
    }

    private static String parse(String s, int i, int endIndex) {
        StringBuilder sb = new StringBuilder();
        for (; i < endIndex; ++i) {
            char c = s.charAt(i);
            if (c == '<') {
                sb.append("&lt;");
            } else if (c == '>') {
                sb.append("&gt;");
            } else if (c == '&') {
                sb.append("&amp;");
            } else if (i + 1 < endIndex && c == '\\' &&
                    isSpecialChar(s.charAt(i + 1))) {
                sb.append(s.charAt(++i));
            } else if (s.startsWith("**", i)) {
                int j = indexOf(s, "**", i + 2, endIndex);
                if (j == -1) {
                    sb.append(c);
                } else {
                    sb.append("<strong>");
                    sb.append(parse(s, i + 2, j));
                    sb.append("</strong>");
                    i = j + 1;
                }
            } else if (s.startsWith("__", i)) {
                int j = indexOf(s, "__", i + 2, endIndex);
                if (j == -1) {
                    sb.append(c);
                } else {
                    sb.append("<strong>");
                    sb.append(parse(s, i + 2, j));
                    sb.append("</strong>");
                    i = j + 1;
                }
            } else if (s.startsWith("--", i)) {
                int j = indexOf(s, "--", i + 2, endIndex);
                if (j == -1) {
                    sb.append(c);
                } else {
                    sb.append("<s>");
                    sb.append(parse(s, i + 2, j));
                    sb.append("</s>");
                    i = j + 1;
                }
            } else if (s.startsWith("*", i)) {
                int j = indexOf(s, "*", i + 1, endIndex);
                if (j == -1) {
                    sb.append(c);
                } else {
                    sb.append("<em>");
                    sb.append(parse(s, i + 1, j));
                    sb.append("</em>");
                    i = j;
                }
            } else if (s.startsWith("_", i)) {
                int j = indexOf(s, "_", i + 1, endIndex);
                if (j == -1) {
                    sb.append(c);
                } else {
                    sb.append("<em>");
                    sb.append(parse(s, i + 1, j));
                    sb.append("</em>");
                    i = j;
                }
            } else if (s.startsWith("`", i)) {
                int j = indexOf(s, "`", i + 1, endIndex);
                if (j == -1) {
                    sb.append(c);
                } else {
                    sb.append("<code>");
                    sb.append(parse(s, i + 1, j));
                    sb.append("</code>");
                    i = j;
                }
            } else {
                sb.append(c);
            }
        }
        return sb.toString();
    }

    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Not enough arguments");
            return;
        }

        List<String> paragraphs = new ArrayList<>();
        try {
            BufferedReader in = new BufferedReader(new InputStreamReader(
                        new FileInputStream(new File(args[0])), "utf8"));
            try {
                StringBuilder sb = new StringBuilder();
                String line = "";
                while (line != null) {
                    String paragraphTag = "";
                    while ((line = in.readLine()) != null && !line.isEmpty()) {
                        if (sb.length() == 0) {
                            int cnt = 0;
                            while (cnt < line.length() && line.charAt(cnt) == '#') {
                                ++cnt;
                            }
                            if (cnt > 0 && cnt < Math.min(line.length(), 7) &&
                                    line.charAt(cnt) == ' ') {
                                paragraphTag = "h" + cnt;
                                ++cnt;
                            } else {
                                paragraphTag = "p";
                                cnt = 0;
                            }
                            line = line.substring(cnt);
                        }
                        sb.append(line);
                        sb.append("\n");
                    }
                    if (sb.length() > 0) {
                        sb.deleteCharAt(sb.length() - 1);
                        paragraphs.add(openTag(paragraphTag));
                        paragraphs.add(sb.toString());
                        paragraphs.add(closeTag(paragraphTag));
                        sb = new StringBuilder();
                    }
                }
            } finally {
                in.close();
            }
        } catch (IOException e) {
            System.out.println("Input file error: " + e.getMessage());
            return;
        }

        try {
            BufferedWriter out = new BufferedWriter(new OutputStreamWriter(
                        new FileOutputStream(new File(args[1])), "utf8"));
            try {
                int n = paragraphs.size();
                for (int i = 0; i < n; i += 3) {
                    int m = paragraphs.get(i + 1).length();
                    String s = parse(paragraphs.get(i + 1), 0, m);

                    out.write(paragraphs.get(i));
                    out.write(s);
                    out.write(paragraphs.get(i + 2));
                    out.newLine();
                }
            } finally {
                out.close();
            }
        } catch (IOException e) {
            System.out.println("Output file error: " + e.getMessage());
            return;
        }
    }
}
