package info.kgeorgiy.ja.Naumov.rmi;

import java.rmi.RemoteException;

public abstract class AbstractPerson implements Person {
    protected String name;
    protected String surname;
    protected String passport;

    public AbstractPerson(final String name, final String surname, final String passport) {
        this.name = name;
        this.surname = surname;
        this.passport = passport;
    }

    /** Return person's name. */
    public String getName() throws RemoteException {
        return name;
    }

    /** Return person's surname. */
    public String getSurname() throws RemoteException {
        return surname;
    }

    /** Return person's passport. */
    public String getPassport() throws RemoteException {
        return passport;
    }
}
