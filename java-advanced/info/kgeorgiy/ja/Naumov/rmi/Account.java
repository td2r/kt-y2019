package info.kgeorgiy.ja.Naumov.rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Account extends Remote {
    String getId() throws RemoteException;
    int getAmount() throws RemoteException;
    void setAmount(final int amount) throws RemoteException;
    void changeAmount(final int delta) throws RemoteException;
}
