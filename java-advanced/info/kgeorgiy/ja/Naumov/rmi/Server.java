package info.kgeorgiy.ja.Naumov.rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Server extends Remote {
    void exportNewBank() throws RemoteException;
}
