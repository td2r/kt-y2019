import java.io.*;
import java.util.*;

public class WordStatFirstIndex {
    private static class Item {
        public int count;
        public int lastLine;
        public IntList entries = new IntList();

        public void addIfFirstInLine(final int line, final int entry) {
            if (lastLine < line) {
                entries.append(entry);
                lastLine = line;
            }
            count++;
        }
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

        Map<String, Item> wordStat = new LinkedHashMap<>();

        try {
            FastScanner in = new FastScanner(new File(args[0]), "utf8");
            try {
                int i = 1;
                while (in.hasNextLine()) {
                    int j = 1;
                    while (in.hasNextWord(true)) {
                        String s = in.nextWord(true).toLowerCase();

                        Item item = wordStat.get(s);
                        if (item == null) {
                            item = new Item();
                            wordStat.put(s, item);
                        }
                        item.addIfFirstInLine(i, j++);
                    }
                    in.skipLine();
                    i++;
                }
            } finally {
                in.close();
            }
        } catch (IOException e) {
            System.out.println("Input file error : " + e.getMessage());
            return;
        }

        try {
            BufferedWriter out = new BufferedWriter(
                new OutputStreamWriter(
                    new FileOutputStream(new File(args[1])), "utf8"
                )
            );
            try {
                for (Map.Entry<String, Item> entry : wordStat.entrySet()) {
                    out.write(entry.getKey() + " " + entry.getValue().count + " " +
                            entry.getValue().entries);
                    out.newLine();
                }
            } finally {
                out.close();
            }
        } catch (IOException e) {
            System.out.println("Output file error : " + e.getMessage());
            return;
        }
    }
}
