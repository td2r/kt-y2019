package info.kgeorgiy.ja.Naumov.hello;

import java.nio.ByteBuffer;

import static info.kgeorgiy.ja.Naumov.hello.Utils.*;

public class ClientData {
    private final ByteBuffer buffer;
    private final int mark;
    private int number = 0;
    private final int index;

    public ClientData(final byte[] prefixBytes, final byte[] delimiterBytes, final int index) {
        buffer = ByteBuffer.allocate(BYTES_MAX);
        buffer.put(prefixBytes);
        putNumber(buffer, index);
        buffer.put(delimiterBytes);
        mark = buffer.position();
        putNumber(buffer, 0);
        buffer.flip();
        this.index = index;
    }

    public void increaseNumber() {
        putNumber(buffer.clear().position(mark), ++number);
        buffer.flip();
    }

    public ByteBuffer getBuffer() {
        return buffer;
    }

    public int getNumber() {
        return number;
    }

    public int getIndex() {
        return index;
    }

}
