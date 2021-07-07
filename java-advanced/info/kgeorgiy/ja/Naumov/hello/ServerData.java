package info.kgeorgiy.ja.Naumov.hello;

import java.net.SocketAddress;
import java.nio.ByteBuffer;

import static info.kgeorgiy.ja.Naumov.hello.Utils.BYTES_MAX;
import static info.kgeorgiy.ja.Naumov.hello.Utils.SERVER_PREFIX;

public class ServerData extends Node {
    private final ByteBuffer buffer;
    private SocketAddress address;

    public ServerData() {
        buffer = ByteBuffer.allocate(BYTES_MAX);
        buffer.put(SERVER_PREFIX);
    }

    public ByteBuffer getBuffer() {
        return buffer;
    }

    public SocketAddress getAddress() {
        return address;
    }

    public void setAddress(final SocketAddress address) {
        this.address = address;
    }
}
