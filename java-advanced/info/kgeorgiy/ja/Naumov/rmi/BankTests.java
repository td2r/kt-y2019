package info.kgeorgiy.ja.Naumov.rmi;

import org.junit.*;
import org.junit.runner.JUnitCore;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.nio.file.Path;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.List;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.Function;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class BankTests {
    private final static String CLASSPATH = getClassPath();
    private static final String SAMPLE_NAME = "Ivan";
    private static final String SAMPLE_SURNAME = "Ivanov";
    private static final String SAMPLE_PASSPORT = "passport";
    private static final String SAMPLE_SUB_ID = "subId";

    private static Process process;
    private static Server server;
    private Bank bank;

    private static String getClassPath() {
        try {
            return Path.of(BankTests.class.getProtectionDomain().getCodeSource().getLocation().toURI()).toString();
        } catch (final URISyntaxException e) {
            throw new AssertionError(e);
        }
    }

    private static void handleInput(final InputStream stream) {
        final StringBuilder builder = new StringBuilder();
        try (final BufferedReader reader = new BufferedReader(new InputStreamReader(stream))) {
            // Using default encoding
            while (true) {
                final String line = reader.readLine();
                if (line == null) {
                    throw new AssertionError("Bad input");
                }
                builder.append(line);
                if ("Server exported".contentEquals(builder)) {
                    return;
                }
            }
        } catch (IOException e) {
            System.err.println(e.getMessage());
        }
    }

    private static void handleErrorStream(final InputStream stream) {
        new Thread(() -> {
            // Using default encoding
            try (final BufferedReader reader = new BufferedReader(new InputStreamReader(stream))) {
                while (true) {
                    final String line = reader.readLine();
                    if (line == null) {
                        return;
                    }
                    System.err.println(line);
                }
            } catch (final IOException e) {
                System.err.println(e.getMessage());
            }
        }).start();
    }

    @BeforeClass
    public static void beforeClass() throws IOException, NotBoundException {
        LocateRegistry.createRegistry(Registry.REGISTRY_PORT);

        ProcessBuilder processBuilder =
                new ProcessBuilder("java", "-cp", CLASSPATH, "info.kgeorgiy.ja.Naumov.rmi.BankServer");
        process = processBuilder.start();
        handleErrorStream(process.getErrorStream());
        handleInput(process.getInputStream());
        server = (Server) Naming.lookup("//localhost/server");
    }

    @AfterClass
    public static void afterClass() {
        process.destroy();
    }

    @Before
    public void before() throws RemoteException, MalformedURLException, NotBoundException {
        server.exportNewBank();
        bank = (Bank) Naming.lookup("//localhost/bank");
    }

    @Test
    public void test01_createGetPerson() throws RemoteException {
        final Person person = bank.createPerson(SAMPLE_NAME, SAMPLE_SURNAME, SAMPLE_PASSPORT);
        Assert.assertEquals("Different person's name", SAMPLE_NAME, person.getName());
        Assert.assertEquals("Different person's surname", SAMPLE_SURNAME, person.getSurname());
        Assert.assertEquals("Different person's passport", SAMPLE_PASSPORT, person.getPassport());

        final Person got = bank.getRemotePerson(SAMPLE_PASSPORT);
        Assert.assertEquals("Different gotten name", SAMPLE_NAME, got.getName());
        Assert.assertEquals("Different gotten surname", SAMPLE_SURNAME, got.getSurname());
        Assert.assertEquals("Different gotten passport", SAMPLE_PASSPORT, got.getPassport());
    }

    @Test
    public void test02_createExistedPerson() throws RemoteException {
        final Person created = bank.createPerson(SAMPLE_NAME, SAMPLE_SURNAME, SAMPLE_PASSPORT);
        final Person existed = bank.createPerson(SAMPLE_NAME, SAMPLE_SURNAME, SAMPLE_PASSPORT);
        Assert.assertEquals("Different person's name", existed.getName(), created.getName());
        Assert.assertEquals("Different person's surname", existed.getSurname(), created.getSurname());
        Assert.assertEquals("Different person's passport", existed.getPassport(), created.getPassport());
    }

    @Test
    public void test03_checkAccountId() throws RemoteException {
        final int amount = 10000;

        final Person person = bank.createPerson(SAMPLE_NAME, SAMPLE_SURNAME, SAMPLE_PASSPORT);
        person.createAccount(SAMPLE_SUB_ID);
        person.getAccount(SAMPLE_SUB_ID).setAmount(amount);

        final Account account = bank.getAccount(SAMPLE_PASSPORT + ':' + SAMPLE_SUB_ID);
        Assert.assertNotNull("Null returned for created account", account);
        Assert.assertEquals(account.getAmount(), amount);

        account.setAmount(2 * amount);
        Assert.assertEquals("Wrong amount on account",
                bank.getAccount(SAMPLE_PASSPORT + ':' + SAMPLE_SUB_ID).getAmount(),
                2 * amount);
    }

    @Test(expected = Exception.class)
    public void test04_invalidPassport() throws RemoteException {
        final String passport = "pass:port";
        bank.createPerson(SAMPLE_NAME, SAMPLE_SURNAME, passport);
    }

    @Test(expected = Exception.class)
    public void test05_invalidSubId() throws RemoteException {
        final String subId = "sub:id";
        final Person person = bank.createPerson(SAMPLE_NAME, SAMPLE_SURNAME, SAMPLE_PASSPORT);
        person.createAccount(subId);
    }

    @Test(expected = Exception.class)
    public void test06_createNoPersonAccount() throws RemoteException {
        bank.createAccount(SAMPLE_SUB_ID);
    }

    @Test
    public void test07_twoPersonsOnePassport() throws RemoteException {
        bank.createPerson(SAMPLE_NAME,
                SAMPLE_SURNAME,
                SAMPLE_PASSPORT);
        final Person person = bank.createPerson(SAMPLE_NAME + SAMPLE_NAME,
                SAMPLE_SURNAME + SAMPLE_SURNAME,
                SAMPLE_PASSPORT);
        Assert.assertEquals("Wrong name of created person", SAMPLE_NAME, person.getName());
        Assert.assertEquals("Wrong surname of created person", SAMPLE_SURNAME, person.getSurname());
    }

    @Test
    public void test08_localPerson() throws RemoteException {
        final String subId1 = "subId1";
        final String subId2 = "subId2";

        bank.createPerson(SAMPLE_NAME, SAMPLE_SURNAME, SAMPLE_PASSPORT);
        final Person remotePerson = bank.getRemotePerson(SAMPLE_PASSPORT);
        remotePerson.createAccount(subId1);

        final Person localPerson1 = bank.getLocalPerson(SAMPLE_PASSPORT);
        Assert.assertNotNull("No account in local person", localPerson1.getAccount(subId1));
        Assert.assertNull("Not existing account exists in local person", localPerson1.getAccount(subId2));

        localPerson1.createAccount(subId2);
        Assert.assertNull("Account created in local person exists in remote person",
                remotePerson.getAccount(subId2));
        remotePerson.createAccount(subId2);

        final Person localPerson2 = bank.getLocalPerson(SAMPLE_PASSPORT);
        Assert.assertNotNull("No account in local person", localPerson2.getAccount(subId1));
        Assert.assertNotNull("No account in local person", localPerson2.getAccount(subId2));
    }

    @Test
    public void test09_localPersonAccount() throws RemoteException {
        final int start = 420;
        final int delta = 69;

        final Person person = bank.createPerson(SAMPLE_NAME, SAMPLE_SURNAME, SAMPLE_PASSPORT);
        person.createAccount(SAMPLE_SUB_ID).setAmount(start);

        final Person remotePerson = bank.getRemotePerson(SAMPLE_PASSPORT);
        final Person localPerson = bank.getLocalPerson(SAMPLE_PASSPORT);

        remotePerson.getAccount(SAMPLE_SUB_ID).changeAmount(delta);
        localPerson.getAccount(SAMPLE_SUB_ID).changeAmount(2 * delta);

        Assert.assertEquals("Wrong remote account amount",
                start + delta,
                remotePerson.getAccount(SAMPLE_SUB_ID).getAmount());
        Assert.assertEquals("Wrong local account amount",
                start + 2 * delta,
                localPerson.getAccount(SAMPLE_SUB_ID).getAmount());
        Assert.assertEquals("Wrong new local account amount",
                start + delta,
                bank.getLocalPerson(SAMPLE_PASSPORT).getAccount(SAMPLE_SUB_ID).getAmount());
    }

    private void generateExecuteThreads(final int n, final Function<Integer, Runnable> supplier) {
        List<Thread> threads = IntStream.range(0, n)
                .mapToObj(supplier::apply)
                .map(Thread::new)
                .collect(Collectors.toList());
        threads.forEach(Thread::start);
        for (final Thread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    @Test
    public void test10_parallelSinglePersonSingleAccount() throws RemoteException {
        final int threads = 15;
        final int changes = 1000;
        final int delta = 3;
        final Person person = bank.createPerson(SAMPLE_NAME, SAMPLE_SURNAME, SAMPLE_PASSPORT);
        final Account account = person.createAccount(SAMPLE_SUB_ID);
        generateExecuteThreads(threads,
                i -> () -> {
            for (int j = 0; j < changes; ++j) {
                try {
                    account.changeAmount(delta);
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
            }
        });
        Assert.assertEquals("Wrong amount", account.getAmount(), threads * changes * delta);
    }

    @Test
    public void test11_parallelSinglePersonManyAccounts() throws RemoteException {
        final int accounts = 15;
        final int changes = 1000;
        final int delta = 3;
        final Person person = bank.createPerson(SAMPLE_NAME, SAMPLE_SURNAME, SAMPLE_PASSPORT);
        generateExecuteThreads(accounts,
                i -> () -> {
            try {
                final Account account = person.createAccount(SAMPLE_SUB_ID + i);
                for (int j = 0; j < changes; ++j) {
                    account.changeAmount(delta);
                }
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        });
        for (int i = 0; i < accounts; ++i) {
            Assert.assertEquals("Wrong amount",
                    changes * delta,
                    person.getAccount(SAMPLE_SUB_ID + i).getAmount());
        }
    }

    @Test
    public void test12_parallelManyPersonsSingleAccount() throws RemoteException {
        final int persons = 15;
        final int changes = 1000;
        final int delta = 3;
        generateExecuteThreads(persons,
                i -> () -> {
            try {
                final Person person = bank.createPerson(
                        SAMPLE_NAME + i,
                        SAMPLE_SURNAME + i,
                        SAMPLE_PASSPORT + i);
                final Account account = person.createAccount(SAMPLE_SUB_ID);
                for (int j = 0; j < changes; ++j) {
                    account.changeAmount(delta);
                }
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        });
        for (int i = 0; i < persons; ++i) {
            Assert.assertEquals("Wrong amount",
                    delta * changes,
                    bank.getRemotePerson(SAMPLE_PASSPORT + i).getAccount(SAMPLE_SUB_ID).getAmount());
        }
    }

    @Test
    public void test13_parallelManyPersonsManyAccounts() throws RemoteException {
        final int threads = 10;
        final int changes = 50;
        final int delta = 3;
        final Person[] persons = new Person[7];
        final Account[][] accounts = new Account[persons.length][10];
        for (int i = 0; i < persons.length; ++i) {
            persons[i] = bank.createPerson(
                    SAMPLE_NAME + i,
                    SAMPLE_SURNAME + i,
                    SAMPLE_PASSPORT + i);
            for (int j = 0; j < accounts[i].length; ++j) {
                accounts[i][j] = persons[i].createAccount(SAMPLE_SUB_ID + i + '_' + j);
            }
        }
        generateExecuteThreads(threads,
                n -> () -> IntStream.range(0, changes).forEach(t -> {
                    for (int i = 0; i < persons.length; ++i) {
                        for (int j = 0; j < accounts[i].length; ++j) {
                            try {
                                accounts[i][j].changeAmount(delta);
                            } catch (RemoteException e) {
                                e.printStackTrace();
                            }
                        }
                    }
                }));
        for (int i = 0; i < persons.length; ++i) {
            for (int j = 0; j < accounts[i].length; ++j) {
                Assert.assertEquals("Wrong amount",
                        changes * threads * delta,
                        accounts[i][j].getAmount());
            }
        }
    }

    @Test
    public void test14_parallelCreatePerson() {
        final int threads = 10;
        final int times = 100;
        final AtomicBoolean flag = new AtomicBoolean(true);
        final Runnable action = () -> {
            try {
                for (int i = 0; i < times; ++i) {
                    final Person person = bank.createPerson(SAMPLE_NAME, SAMPLE_SURNAME, SAMPLE_PASSPORT);
                    if (person == null
                            || !SAMPLE_NAME.equals(person.getName())
                            || !SAMPLE_SURNAME.equals(person.getSurname())
                            || !SAMPLE_PASSPORT.equals(person.getPassport())) {
                        flag.set(false);
                        return;
                    }
                }
            } catch (RemoteException e) {
                flag.set(false);
                e.printStackTrace();
            }
        };
        generateExecuteThreads(threads, i -> action);
        if (!flag.get()) {
            Assert.fail("Parallel creating person fail");
        }
    }

    @Test
    public void test15_parallelCreateAccount() throws RemoteException {
        final int threads = 10;
        final int times = 100;
        final int delta = 5;
        final AtomicBoolean flag = new AtomicBoolean(true);
        final Person person = bank.createPerson(SAMPLE_NAME, SAMPLE_SURNAME, SAMPLE_PASSPORT);
        final Runnable action = () -> {
            try {
                for (int i = 0; i < times; ++i) {
                    final Account account = person.createAccount(SAMPLE_SUB_ID);
                    account.changeAmount(delta);
                }
            } catch (RemoteException e) {
                flag.set(false);
                e.printStackTrace();
            }
        };
        generateExecuteThreads(threads, i -> action);
        if (!flag.get()) {
            Assert.fail("Parallel creating account fail");
        }
        Assert.assertEquals("Parallel creating account wrong amount",
                threads * times * delta,
                person.getAccount(SAMPLE_SUB_ID).getAmount());
    }

    public static void main(String[] args) {
        JUnitCore.main(BankTests.class.getCanonicalName());
    }
}
