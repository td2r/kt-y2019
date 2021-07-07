package info.kgeorgiy.ja.Naumov.walk;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.Writer;
import java.nio.file.FileVisitResult;
import java.nio.file.Path;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;

public class HashFileVisitor extends SimpleFileVisitor<Path> {
    private static final byte[] buffer = new byte[8192];
    private final Writer writer;

    public HashFileVisitor(Writer writer) {
        this.writer = writer;
    }

    private static long hash(final File file) {
        long high;
        long hash = 0;
        int n;
        try (FileInputStream is = new FileInputStream(file)) {
            while ((n = is.read(buffer)) != -1) {
                for (int i = 0; i < n; ++i) {
                    hash = (hash << 8) + (buffer[i] & 0xff);
                    high = hash & 0xff00000000000000L;
                    if (high != 0) {
                        hash ^= high >> 48;
                    }
                    hash &= ~high;
                }
            }
        } catch (IOException ignored) {
            hash = 0;
        }
        return hash;
    }

    @Override
    public FileVisitResult visitFile(Path path, BasicFileAttributes attrs) throws IOException {
        writer.write(String.format("%016x %s%n", HashFileVisitor.hash(path.toFile()), path.toString()));
        return FileVisitResult.CONTINUE;
    }

    @Override
    public FileVisitResult visitFileFailed(Path path, IOException exc) throws IOException {
        writer.write(String.format("%016x %s%n", 0L, path.toString()));
        return FileVisitResult.CONTINUE;
    }
}
