package info.kgeorgiy.ja.Naumov.rmi;

import java.rmi.*;

public abstract class AbstractAccount implements Account {
    protected String id;
    protected int amount;

    public AbstractAccount(final String id) {
        this.id = id;
        amount = 0;
    }

    /** Returns account identifier. */
    public String getId() throws RemoteException {
        return id;
    }

    /** Returns amount of money at the account. */
    public synchronized int getAmount() throws RemoteException {
        System.out.println("Getting amount of money for account " + id);
        return amount;
    }

    /** Sets amount of money at the account. */
    public synchronized void setAmount(final int amount) throws RemoteException {
        System.out.println("Setting amount of money for account " + id);
        this.amount = amount;
    }

    public synchronized void changeAmount(final int delta) throws RemoteException {
        System.out.println("Changing amount of money for account " + id);
        this.amount += delta;
    }
}