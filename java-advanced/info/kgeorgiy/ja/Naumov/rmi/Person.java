package info.kgeorgiy.ja.Naumov.rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Person extends Remote {
    String getName() throws RemoteException;
    String getSurname() throws RemoteException;
    String getPassport() throws RemoteException;
    Account createAccount(final String subId) throws RemoteException;
    Account getAccount(final String subId) throws RemoteException;
}
