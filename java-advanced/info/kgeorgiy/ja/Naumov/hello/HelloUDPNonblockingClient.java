package info.kgeorgiy.ja.Naumov.hello;

import info.kgeorgiy.java.advanced.hello.HelloClient;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.util.HashSet;
import java.util.Set;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Consumer;

import static info.kgeorgiy.ja.Naumov.hello.Utils.*;

public class HelloUDPNonblockingClient implements HelloClient {
    private final AtomicInteger done = new AtomicInteger();

    private final ByteBuffer buffer = ByteBuffer.allocate(BYTES_MAX);

    @Override
    public void run(final String host, final int port, final String prefix, final int threads, final int requests) {
        done.set(0);
        final InetSocketAddress address = new InetSocketAddress(host, port);
        final byte[] prefixBytes = prefix.getBytes(CHARSET);
        final byte[] delimiterBytes = "_".getBytes(CHARSET);
        final Set<SelectionKey> selectorKeys = new HashSet<>();
        final Consumer<SelectionKey> selectAction = key -> {
            if (key.isWritable()) {
                send(key);
            }
            if (key.isReadable()) {
                check(key, requests);
            }
        };
        try (final Selector selector = Selector.open()) {
            for (int i = 0; i < threads; ++i) {
                final DatagramChannel channel = DatagramChannel.open();
                channel.configureBlocking(false);
                channel.connect(address);
                selectorKeys.add(channel.register(selector,
                        SelectionKey.OP_WRITE,
                        new ClientData(prefixBytes, delimiterBytes, i)));
            }
            while (done.get() != threads) {
                if (selector.select(selectAction, DEFAULT_TIMEOUT_MS) == 0) {
                    selectorKeys.forEach(key -> {
                        if (key.isValid())
                            key.interestOps(SelectionKey.OP_WRITE);
                    });
                }
            }
        } catch (IOException e) {
            System.out.println("Client running error: " + e.getMessage());
        }
    }

    private void send(final SelectionKey key) {
        try {
            final DatagramChannel channel = (DatagramChannel) key.channel();
            final ClientData data = (ClientData) key.attachment();
            channel.write(data.getBuffer().rewind());
            key.interestOps(SelectionKey.OP_READ);
        } catch (IOException e) {
            key.interestOps(SelectionKey.OP_WRITE);
        }
    }

    private void check(final SelectionKey key, final int requests) {
        try {
            final DatagramChannel channel = (DatagramChannel) key.channel();
            final ClientData data = (ClientData) key.attachment();
            channel.receive(buffer.clear());
            buffer.flip();
            if (verifyResponse(data.getIndex(), data.getNumber())) {
                data.increaseNumber();
            }
            if (data.getNumber() == requests) {
                done.incrementAndGet();
                key.interestOps(0);
                channel.close();
            } else {
                key.interestOps(SelectionKey.OP_WRITE);
            }
        } catch (IOException e) {
            System.out.println("Check error " + e.getMessage());
            key.interestOps(SelectionKey.OP_WRITE);
        }
    }

    private boolean bufferMeetNumber() {
        byte b;
        while (buffer.position() < buffer.limit()) {
            b = buffer.get();
            if (b > 0 && Character.isDigit(b)) {
                buffer.position(buffer.position() - 1);
                return true;
            }
        }
        return false;
    }

    private boolean bufferAssertNumber(final int n) {
        int p = maxSubPow10(n);
        byte b;
        while (buffer.position() < buffer.limit()) {
            b = buffer.get();
            if (b > 0 && Character.isDigit(b)) {
                if (p == 0 || b != (n / p) % 10 + '0') {
                    return false;
                }
                p /= 10;
            } else {
                return p == 0;
            }
        }
        return p == 0;
    }

    private boolean verifyResponse(int index, int number) {
        buffer.rewind();
        return bufferMeetNumber()
                && bufferAssertNumber(index)
                && bufferMeetNumber()
                && bufferAssertNumber(number)
                && !bufferMeetNumber();
    }

    public static void main(String[] args) {
        launchClient(new HelloUDPNonblockingClient(), args);
    }
}
