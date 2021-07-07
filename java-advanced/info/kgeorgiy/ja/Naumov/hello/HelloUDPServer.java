package info.kgeorgiy.ja.Naumov.hello;

import info.kgeorgiy.java.advanced.hello.HelloServer;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class HelloUDPServer implements HelloServer {
    private static final int TIMEOUT_MS = 500;
    private static final byte[] prefix = "Hello, ".getBytes(StandardCharsets.UTF_8);

    private List<Thread> workers;
    private DatagramSocket socket;
    private volatile boolean closed = false;

    @Override
    public void start(final int port, final int threads) {
        final int receiveBufferSize;
        try {
            socket = new DatagramSocket(port);
            socket.setSoTimeout(TIMEOUT_MS);
            receiveBufferSize = socket.getReceiveBufferSize();
        } catch (SocketException e) {
            System.out.println("Unable to create socket on port " + port + "\n" + e.getMessage());
            return;
        }
        final Runnable action = () -> {
            final byte[] buffer = new byte[receiveBufferSize];
            final DatagramPacket packet =
                    new DatagramPacket(buffer, receiveBufferSize);
            while (!closed && !socket.isClosed()) {
                try {
                    socket.receive(packet);

                    final int len = packet.getLength();
                    final byte[] response = new byte[prefix.length + len];

                    System.arraycopy(prefix, 0, response, 0, prefix.length);
                    System.arraycopy(buffer, 0, response, prefix.length, len);
                    packet.setData(response);

                    socket.send(packet);
                } catch (IOException ignored) {
                } finally {
                    packet.setData(buffer);
                }
            }
        };
        workers = Stream.generate(() -> new Thread(action))
                .limit(threads)
                .peek(Thread::start)
                .collect(Collectors.toList());
    }

    @Override
    public void close() {
        synchronized (this) {
            if (closed)
                return;
            closed = true;
            socket.close();
        }
        for (int i = 0; i < workers.size(); ++i) {
            try {
                workers.get(i).join();
            } catch (InterruptedException e) {
                --i;
            }
        }
    }

    public static void main(String[] args) {
        Utils.launchServer(new HelloUDPServer(), args);
    }
}
