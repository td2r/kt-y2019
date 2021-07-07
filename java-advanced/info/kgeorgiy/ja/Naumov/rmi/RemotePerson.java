package info.kgeorgiy.ja.Naumov.rmi;

import java.rmi.RemoteException;

public class RemotePerson extends AbstractPerson {
    private final Bank bank;

    public RemotePerson(final String name, final String surname, final String passport, final Bank bank) {
        super(name, surname, passport);
        this.bank = bank;
    }

    @Override
    public Account createAccount(final String subId) throws RemoteException {
        return bank.createAccount(passport + ":" + subId);
    }

    @Override
    public Account getAccount(final String subId) throws RemoteException {
        return bank.getAccount(passport + ":" + subId);
    }

    @Override
    public String toString() {
        return "RemotePerson{" +
                "name='" + name + '\'' +
                ", surname='" + surname + '\'' +
                ", passport='" + passport + '\'' +
                '}';
    }
}
