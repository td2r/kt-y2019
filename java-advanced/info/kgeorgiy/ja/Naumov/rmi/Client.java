package info.kgeorgiy.ja.Naumov.rmi;

import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;

public final class Client {
    /** Utility class. */
    private Client() {}

    public static void main(final String... args) throws RemoteException {
        final Bank bank;
        try {
            bank = (Bank) Naming.lookup("//localhost/bank");
        } catch (final NotBoundException e) {
            System.out.println("Bank is not bound");
            return;
        } catch (final MalformedURLException e) {
            System.out.println("Bank URL is invalid");
            return;
        }

        final String personName = args.length >= 1 ? args[0] : "Ivan";
        final String personSurname = args.length >= 2 ? args[1] : "Naumov";
        final String personPassport = args.length >= 3 ? args[2] : "qwerty12345";

        final String accountId = args.length >= 4 ? args[3] : "geo";

        final int amountChange = args.length >= 5 ? Integer.parseInt(args[4]) : 100;

        Person person = bank.getRemotePerson(personPassport);
        if (person == null) {
            System.out.println("Creating person");
            person = bank.createPerson(personName, personSurname, personPassport);
        } else {
            if (personName.equals(person.getName()) && personSurname.equals(person.getSurname())) {
                System.out.println("Person already exists");
            } else {
                System.out.println("Passport is already in use");
                return;
            }
        }

        Account account = person.getAccount(accountId);
        if (account == null) {
            System.out.println("Creating account");
            account = person.createAccount(accountId);
        } else {
            System.out.println("Account already exists");
        }
        System.out.println("Account id: " + account.getId());
        System.out.println("Money: " + account.getAmount());
        System.out.println("Adding money");
        account.setAmount(account.getAmount() + amountChange);
        System.out.println("Money: " + account.getAmount());
    }
}
