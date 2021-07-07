package info.kgeorgiy.ja.Naumov.rmi;

import java.rmi.*;
import java.rmi.server.*;
import java.net.*;

public final class BankServer implements Server {
    private final static int DEFAULT_PORT = 8888;
    private final int port;

    public BankServer(int port) {
        this.port = port;
    }

    public void exportNewBank() {
        final Bank bank = new RemoteBank(port);
        try {
            UnicastRemoteObject.exportObject(bank, port);
            Naming.rebind("//localhost/bank", bank);
            System.out.println("Bank exported");
        } catch (final RemoteException e) {
            System.out.println("Cannot export object: " + e.getMessage());
            e.printStackTrace();
            System.exit(1);
        } catch (final MalformedURLException e) {
            System.out.println("Malformed URL");
        }
    }

    public static void main(final String... args) {
        final int port = args.length > 0 ? Integer.parseInt(args[0]) : DEFAULT_PORT;
        final Server server = new BankServer(port);
        try {
            UnicastRemoteObject.exportObject(server, port);
            Naming.rebind("//localhost/server", server);
            System.out.println("Server exported");
        } catch (RemoteException e) {
            System.out.println("Cannot export object: " + e.getMessage());
            e.printStackTrace();
            System.exit(1);
        } catch (MalformedURLException e) {
            System.out.println("Malformed URL");
        }
        System.out.println("Server main exit");
    }
}
