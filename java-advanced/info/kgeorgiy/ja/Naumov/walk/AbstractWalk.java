package info.kgeorgiy.ja.Naumov.walk;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.InvalidPathException;
import java.util.Set;

public class AbstractWalk {
    public static void run(final String[] args, final int maxDepth) {
        if (args == null || args.length != 2 || args[0] == null || args[1] == null) {
            System.out.println("Usage: java RecursiveWalk <input file> <output file>");
            return;
        }
        File outputParent = new File(args[1]).getParentFile();
        if (outputParent == null || !outputParent.isDirectory() && !outputParent.mkdir()) {
            System.out.println("Unable to create parent directory for output file.");
            return;
        }
        try (BufferedReader reader = new BufferedReader(new FileReader(args[0], StandardCharsets.UTF_8));
             BufferedWriter writer = new BufferedWriter(new FileWriter(args[1], StandardCharsets.UTF_8))) {
            String line = reader.readLine();
            while (line != null) {
                try {
                    Files.walkFileTree(new File(line).toPath(), Set.of(), maxDepth, new HashFileVisitor(writer));
                } catch (InvalidPathException ignored) {
                    writer.write(String.format("%016x %s%n", 0L, line));
                }
                line = reader.readLine();
            }
        } catch (FileNotFoundException ignored) {
            System.out.println("Error: input/output file doesn't exists.");
        } catch (IOException ignored) {
            System.out.println("Error: IOException occurred.");
        }
    }
}
