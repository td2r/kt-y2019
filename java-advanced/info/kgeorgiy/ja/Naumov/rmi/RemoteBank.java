package info.kgeorgiy.ja.Naumov.rmi;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Predicate;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class RemoteBank implements Bank {
    public static final Predicate<String> PASSPORT_MATCH = Pattern.compile("[^:]+").asMatchPredicate();
    private final Pattern ACCOUNT_PATTERN = Pattern.compile("([^:]+):([^:]+)");

    private final int port;
    private final ConcurrentMap<String, Person> persons = new ConcurrentHashMap<>();
    private final ConcurrentMap<String, ConcurrentMap<String, Account>> personalAccounts = new ConcurrentHashMap<>();
    private final ConcurrentMap<String, Account> accounts = new ConcurrentHashMap<>();

    public RemoteBank(final int port) {
        this.port = port;
    }

    private boolean isValidPassport(final String passport) {
        return PASSPORT_MATCH.test(passport);
    }

    private String[] splitAccountId(final String id) {
        final Matcher matcher = ACCOUNT_PATTERN.matcher(id);
        if (matcher.matches()) {
            final String[] group = new String[]{ matcher.group(1), matcher.group(2) };
            if (persons.containsKey(group[0])) {
                return group;
            }
        }
        throw new IllegalArgumentException("Invalid account id");
    }

    @Override
    public Person createPerson(final String name, final String surname, final String passport) throws RemoteException {
        if (!isValidPassport(passport)) {
            throw new IllegalArgumentException("Illegal password");
        }
        AtomicReference<RemoteException> atomicReference = new AtomicReference<>(null);
        final Person person = persons.computeIfAbsent(passport, s -> {
            final Person created = new RemotePerson(name, surname, passport, this);
            try {
                UnicastRemoteObject.exportObject(created, port);
            } catch (RemoteException e) {
                atomicReference.set(e);
                return null;
            }
            personalAccounts.put(passport, new ConcurrentHashMap<>());
            return created;
        });
        if (person == null) {
            throw atomicReference.get();
        }
        return person;
    }

    @Override
    public Person getLocalPerson(final String passport) throws RemoteException {
        final Person person = persons.get(passport);
        if (person == null) {
            return null;
        }
        final Map<String, Account> localAccounts = new HashMap<>();
        for (ConcurrentMap.Entry<String, Account> entry : personalAccounts.get(passport).entrySet()) {
            localAccounts.put(entry.getKey(), new LocalAccount(entry.getKey(), entry.getValue().getAmount()));
        }
        return new LocalPerson(person.getName(), person.getSurname(), passport, localAccounts);
    }

    @Override
    public Person getRemotePerson(final String passport) throws RemoteException {
        return persons.get(passport);
    }

    @Override
    public Account createAccount(final String id) throws RemoteException {
        final String[] idPart = splitAccountId(id);
        final AtomicReference<RemoteException> atomicReference = new AtomicReference<>(null);
        final Account account = accounts.computeIfAbsent(id, s -> {
            final Account created = new RemoteAccount(id);
            try {
                UnicastRemoteObject.exportObject(created, port);
            } catch (RemoteException e) {
                atomicReference.set(e);
                return null;
            }
            personalAccounts.get(idPart[0]).put(idPart[1], created);
            return created;
        });
        if (account == null) {
            throw atomicReference.get();
        }
        return account;
    }

    @Override
    public Account getAccount(final String id) throws RemoteException {
        splitAccountId(id);
        return accounts.get(id);
    }
}
