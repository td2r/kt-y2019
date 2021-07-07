package info.kgeorgiy.ja.Naumov.rmi;

import java.io.Externalizable;
import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectOutput;

public class LocalAccount extends AbstractAccount implements Externalizable {
    public LocalAccount() {
        this(null);
    }

    public LocalAccount(final String id) {
        super(id);
    }

    public LocalAccount(final String id, final int amount) {
        super(id);
        this.amount = amount;
    }

    @Override
    public void writeExternal(final ObjectOutput out) throws IOException {
        out.writeObject(id);
        out.writeObject(amount);
    }

    @Override
    public void readExternal(final ObjectInput in) throws IOException, ClassNotFoundException {
        id = (String) in.readObject();
        amount = (int) in.readObject();
    }
}
