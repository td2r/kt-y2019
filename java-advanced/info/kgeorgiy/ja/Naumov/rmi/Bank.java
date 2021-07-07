package info.kgeorgiy.ja.Naumov.rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Bank extends Remote {
    Person createPerson(String name, String surname, String passport) throws RemoteException;
    Person getLocalPerson(String passport) throws RemoteException;
    Person getRemotePerson(String passport) throws RemoteException;
    Account createAccount(String id) throws RemoteException;
    Account getAccount(String id) throws RemoteException;
}
