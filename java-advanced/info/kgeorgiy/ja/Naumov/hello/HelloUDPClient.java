package info.kgeorgiy.ja.Naumov.hello;

import info.kgeorgiy.java.advanced.hello.HelloClient;

import java.io.IOException;
import java.net.*;
import java.util.Random;
import java.util.regex.Matcher;
import java.util.stream.IntStream;

import static info.kgeorgiy.ja.Naumov.hello.Utils.*;

public class HelloUDPClient implements HelloClient {
    private DatagramPacket newReceivePacket(final int length) {
        return new DatagramPacket(new byte[length], length);
    }

    private DatagramPacket newSendPacket(final String prefix, final int maxSuffix, final int port) {
        final byte[] prefixBytes = prefix.getBytes(CHARSET);
        final DatagramPacket packet =
                new DatagramPacket(new byte[prefixBytes.length + String.valueOf(maxSuffix).getBytes(CHARSET).length],
                        prefixBytes.length);
        System.arraycopy(prefixBytes, 0, packet.getData(), 0, prefixBytes.length);
        packet.setPort(port);
        return packet;
    }

    private void setSuffix(final DatagramPacket packet, final int suffix, final int pos) {
        final byte[] suffixBytes = String.valueOf(suffix).getBytes(CHARSET);
        System.arraycopy(suffixBytes, 0, packet.getData(), pos, suffixBytes.length);
        packet.setLength(pos + suffixBytes.length);
    }

    @Override
    public void run(final String host, final int port, final String prefix, final int threads, final int requests) {
        final InetAddress[] addresses;
        try {
            addresses = InetAddress.getAllByName(host);
            if (addresses.length == 0) {
                System.out.println("Empty addresses list");
                return;
            }
        } catch (UnknownHostException e) {
            System.out.println("Can't get host addresses");
            return;
        }
        final Random rand = new Random(System.currentTimeMillis());
        IntStream.range(0, threads).mapToObj(i -> new Thread(() -> {
            try (final DatagramSocket socket = new DatagramSocket()) {
                socket.setSoTimeout(DEFAULT_TIMEOUT_MS);

                final DatagramPacket receivePacket = newReceivePacket(socket.getReceiveBufferSize());
                final DatagramPacket sendPacket = newSendPacket(prefix + i + '_', requests - 1, port);
                final int startLength = sendPacket.getLength();

                IntStream.range(0, requests).forEach(j -> {
                    setSuffix(sendPacket, j, startLength);
                    for (int k = rand.nextInt(addresses.length); !socket.isClosed(); k = (k + 1) % addresses.length) {
                        sendPacket.setAddress(addresses[k]);
                        try {
                            socket.send(sendPacket);
                            socket.receive(receivePacket);
                            final String response = new String(receivePacket.getData(),
                                    receivePacket.getOffset(),
                                    receivePacket.getLength(),
                                    CHARSET);
                            final Matcher matcher = PATTERN.matcher(response);
                            if (matcher.matches()
                                    && String.valueOf(i).equals(matcher.group(2))
                                    && String.valueOf(j).equals(matcher.group(4))) {
                                System.out.println(response);
                                break;
                            }
                        } catch (IOException ignored) {
                        }
                    }
                });
            } catch (SocketException e) {
                System.out.println("Thread" + i + " unable to open datagram socket\n" + e.getMessage());
            }
        }))
                .peek(Thread::start)
                .forEach(thread -> {
                    try {
                        thread.join();
                    } catch (InterruptedException e) {
                        System.out.println("One of threads was interrupted\n" + e.getMessage());
                    }
                });
    }

    public static void main(String[] args) {
        launchClient(new HelloUDPClient(), args);
    }
}
