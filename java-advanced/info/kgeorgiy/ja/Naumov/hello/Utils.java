package info.kgeorgiy.ja.Naumov.hello;

import info.kgeorgiy.java.advanced.hello.HelloClient;
import info.kgeorgiy.java.advanced.hello.HelloServer;

import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.Objects;
import java.util.regex.Pattern;

public class Utils {
    static final int BYTES_MAX = 1024;
    static final int DEFAULT_TIMEOUT_MS = 500;
    static final Charset CHARSET = StandardCharsets.UTF_8;
    static final byte[] SERVER_PREFIX = "Hello, ".getBytes(StandardCharsets.UTF_8);
    static final Pattern PATTERN = Pattern.compile("(\\D*)(\\d+)(\\D+)(\\d+)(\\D*)");

    static int maxSubPow10(final int n) {
        int m = 1;
        while (10L * m <= n) {
            m *= 10;
        }
        return m;
    }

    static void putNumber(final ByteBuffer buffer, final int n) {
        for (int p = maxSubPow10(n); p != 0; p /= 10) {
            buffer.put((byte) ((n / p) % 10 + '0'));
        }
    }

    public static void launchClient(HelloClient client, String[] args) {
        if (args == null || args.length != 5 || Arrays.stream(args).anyMatch(Objects::isNull)) {
            System.out.println("Usage: " + client.getClass().getCanonicalName() + " host port prefix threads requests");
            return;
        }
        final String host = args[0];
        final String prefix = args[2];
        final int port;
        final int threads;
        final int requests;
        try {
            port = Integer.parseInt(args[1]);
            threads = Integer.parseInt(args[3]);
            requests = Integer.parseInt(args[4]);
        } catch (NumberFormatException e) {
            System.err.println("Port, threads and requests arguments must be integer");
            return;
        }
        client.run(host, port, prefix, threads, requests);
    }

    public static void launchServer(HelloServer server, String[] args) {
        if (args == null || args.length != 2 || Arrays.stream(args).anyMatch(Objects::isNull)) {
            System.out.println("Usage: " + server.getClass().getCanonicalName() + " port threads");
            return;
        }
        final int port;
        final int threads;
        try {
            port = Integer.parseInt(args[0]);
            threads = Integer.parseInt(args[1]);
        } catch (NumberFormatException e) {
            System.err.println("Port and threads arguments must be integer");
            return;
        }
        server.start(port, threads);
    }
}
