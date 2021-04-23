package md2html;

import java.io.*;
import java.lang.*;
import java.util.*;

public class Md2Html {
    private static String openTag(String s) {
        if (s.isEmpty()) {
            return "";
        }
        return "<" + s + ">";
    }

    private static String closeTag(String s) {
        if (s.isEmpty()) {
            return "";
        }
        return "</" + s + ">";
    }

    private static String linkTag(String s) {
        return "<a href='" + s + "'>";
    }

    private static boolean isSpecialChar(char c) {
        return c == '*' || c == '_' || c == '-' || c == '`' || c == '\\';
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

    private final static List<String> markers =
        List.of("**", "__", "--", "*", "_", "`", "[");

    private static int parse(String s, int i, String t, MultiText multiText) {
        if (t.equals("[")) {
            t = "]";
        }
        int n = s.length();
        int m = t.length();
        StringBuilder sb = new StringBuilder();
        MultiText mt = new MultiText();
        for (; i < n; ++i) {
            char c = s.charAt(i);
            if (c == '<') {
                sb.append("&lt;");
            } else if (c == '>') {
                sb.append("&gt;");
            } else if (c == '&') {
                sb.append("&amp;");
            } else if (i + 1 < n && c == '\\' &&
                    isSpecialChar(s.charAt(i + 1))) {
                sb.append(s.charAt(++i));
            } else if (!t.isEmpty() && s.startsWith(t, i)) {
                if (t.equals("]")) {
                    StringBuilder linkBuilder = new StringBuilder();
                    while (i < n && s.charAt(++i) != '(');
                    while (i < n && s.charAt(++i) != ')') {
                        linkBuilder.append(s.charAt(i));
                    }
                    mt.addFirst(new Text(linkTag(linkBuilder.toString())));
                    mt.add(new Text(sb.toString()));
                    mt.add(new Text("</a>"));
                    multiText.add(mt);
                    return i + 1;
                } else {
                    String tag = markerToTag(t);
                    mt.addFirst(new Text(openTag(tag)));
                    mt.add(new Text(sb.toString()));
                    mt.add(new Text(closeTag(tag)));
                    multiText.add(mt);
                    return i + m;
                }
            } else {
                boolean matched = false;
                for (String str : markers) {
                    if (s.startsWith(str, i)) {
                        mt.add(new Text(sb.toString()));
                        sb = new StringBuilder();
                        i = parse(s, i + str.length(), str, mt) - 1;
                        matched = true;
                        break;
                    }
                }
                if (!matched) {
                    sb.append(c);
                }
            }
        }
        if (sb.length() > 0) {
            mt.add(new Text(sb.toString()));
        }
        mt.addFirst(new Text(t));
        multiText.add(mt);
        return n;
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
                    MultiText multiText = new MultiText();
                    parse(paragraphs.get(i + 1), 0, "", multiText);
                    StringBuilder sb = new StringBuilder();
                    multiText.toText(sb);

                    out.write(paragraphs.get(i));
                    out.write(sb.toString());
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
