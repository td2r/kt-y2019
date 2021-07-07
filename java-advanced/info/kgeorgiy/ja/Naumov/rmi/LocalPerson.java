package info.kgeorgiy.ja.Naumov.rmi;

import java.io.Externalizable;
import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectOutput;
import java.util.Map;

public class LocalPerson extends AbstractPerson implements Externalizable {
    private Map<String, Account> accounts;

    public LocalPerson() {
        super(null, null, null);
    }

    public LocalPerson(final String name,
                       final String surname,
                       final String passport,
                       final Map<String, Account> accounts) {
        super(name, surname, passport);
        this.accounts = accounts;
    }

    @Override
    public void writeExternal(final ObjectOutput out) throws IOException {
        out.writeObject(name);
        out.writeObject(surname);
        out.writeObject(passport);
        out.writeObject(accounts);
    }

    @Override
    @SuppressWarnings("unchecked")
    public void readExternal(final ObjectInput in) throws IOException, ClassNotFoundException {
        name = (String) in.readObject();
        surname = (String) in.readObject();
        passport = (String) in.readObject();
        accounts = (Map<String, Account>) in.readObject();
    }

    @Override
    public Account createAccount(final String subId) {
        return accounts.computeIfAbsent(subId, s -> new LocalAccount(subId));
    }

    @Override
    public Account getAccount(final String subId) {
        return accounts.get(subId);
    }

    @Override
    public String toString() {
        return "LocalPerson{" +
                "name='" + name + '\'' +
                ", surname='" + surname + '\'' +
                ", passport='" + passport + '\'' +
                '}';
    }
}
