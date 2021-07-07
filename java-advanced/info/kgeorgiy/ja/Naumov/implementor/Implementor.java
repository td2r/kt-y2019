package info.kgeorgiy.ja.Naumov.implementor;

import info.kgeorgiy.java.advanced.implementor.ImplerException;
import info.kgeorgiy.java.advanced.implementor.JarImpler;

import javax.tools.JavaCompiler;
import javax.tools.ToolProvider;
import java.io.*;
import java.lang.reflect.*;
import java.net.URISyntaxException;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.*;
import java.util.function.Function;
import java.util.jar.Attributes;
import java.util.jar.JarEntry;
import java.util.jar.JarOutputStream;
import java.util.jar.Manifest;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

/**
 * This class provides a functions to create default interface or abstract classes implementations.
 * To create a file with implementation in given directory use function {@link Implementor#implement}.
 * To create a jar file with compiled implementation, use function {@link Implementor#implementJar}
 * It also have a {@link Implementor#main} function to pass your arguments via stdin.
 *
 * @author Naumov Ivan
 */
public class Implementor implements JarImpler {
    /**
     * Holds suffix for name of implemented class.
     */
    private static final String IMPL_SUFFIX = "Impl";
    /**
     * Holds a prefix name for all generated method's parameters.
     */
    private static final String PARAM_PREFIX = "x";

    /**
     * Default constructor of class {@link Implementor}.
     */
    public Implementor() {}

    /**
     * Boxing class for a {@link Method} class. Its main purpose to contain method's substitution map and make methods
     * equivalent by their signatures and return types to use a hash set of it in {@link #implementMethods}.
     */
    private static class WrappedMethod {
        /**
         * Special class to hold method's signature. It's main purpose to group {@link WrappedMethod WrappedMethod}
         * instances, ignoring method's return type to infer most common covariant type in {@link #inferCovariantType}.
         */
        public static class MethodSignature {
            /**
             * Method's name.
             */
            String name;
            /**
             * Method's raw parameters array.
             */
            Class<?>[] parameters;

            /**
             * Creates a {@link MethodSignature MethodSignature} instance from a {@link Method} instance
             *
             * @param method {@link Method} instance to take signature of.
             */
            MethodSignature(Method method) {
                name = method.getName();
                parameters = method.getParameterTypes();
            }

            /**
             * Overridden {@link Object#equals} method.
             *
             * @param o {@link Object} instance to compare with.
             * @return {@code true} if {@code o} represents an {@link MethodSignature MethodSignature} instance of the
             * same function; {@code false} otherwise.
             */
            @Override
            public boolean equals(Object o) {
                if (o instanceof MethodSignature) {
                    MethodSignature other = (MethodSignature) o;
                    return name.equals(other.name) && Arrays.equals(parameters, other.parameters);
                }
                return false;
            }

            /**
             * Returns a hash code for this signature. The hash code for a {@code MethodSignature} object is computed as
             * <blockquote>
             *     name.{@link String#hashCode hashCode}() ^ {@link Arrays#hashCode}(parameters)
             * </blockquote>
             *
             * @return hash code of this signature.
             */
            @Override
            public int hashCode() {
                return name.hashCode() ^ Arrays.hashCode(parameters);
            }
        }

        /**
         * Wrapped {@link Method} instance.
         */
        private final Method method;
        /**
         * Substitution map for generic variables used in return type or parameters of this  method.
         */
        private final Map<String, String> subst;

        /**
         * Creates a {@link WrappedMethod WrappedMethod} instance from a {@link Method} instance and substitution map
         * for generic variables used in parameters or return value of this method.
         *
         * @param method method to hold.
         * @param subst substitution map for this method.
         */
        public WrappedMethod(Method method, Map<String, String> subst) {
            this.method = method;
            this.subst = subst;
        }

        /**
         * Returns holding {@link Method} instance.
         *
         * @return {@link #method} field.
         */
        public Method getMethod() {
            return method;
        }

        /**
         * Returns substitution map for holden method.
         *
         * @return {@link #subst} field.
         */
        public Map<String, String> getSubst() {
            return subst;
        }

        /**
         * Returns a {@link MethodSignature MethodSignature} for a containing method.
         *
         * @return signature of field {@link #method}.
         */
        public MethodSignature getSignature() {
            return new MethodSignature(method);
        }

        /**
         * Checks if given object is the same wrapped method. This method compares two methods according to its
         * signatures, return types and modifiers.
         *
         * @param o object to compare with.
         * @return {@code true} if {@code o} is a wrapped method with the same signature and return type; {@code false}
         * otherwise.
         */
        @Override
        public boolean equals(Object o) {
            if (o instanceof WrappedMethod) {
                WrappedMethod other = (WrappedMethod) o;
                return method.getGenericReturnType().equals(other.method.getGenericReturnType())
                        && method.getName().equals(other.method.getName())
                        && Arrays.equals(method.getParameterTypes(), other.method.getParameterTypes())
                        && method.getModifiers() == other.method.getModifiers();
            }
            return false;
        }

        /**
         * Returns hash code of holden method according to its signature and return type.
         *
         * @return hash code for contained method.
         */
        @Override
        public int hashCode() {
            return 31 * (31 * Arrays.hashCode(method.getParameterTypes()) + method.getName().hashCode())
                    + method.getGenericReturnType().hashCode();
        }
    }

    /**
     * File visitor used in {@link Implementor#implementJar} for recursive deleting temporary directories.
     */
    private static class RmrfFileVisitor extends SimpleFileVisitor<Path> {
        /**
         * Default constructor.
         */
        RmrfFileVisitor() {
            super();
        }

        /**
         * Deletes file when visit it.
         *
         * @param file {@link Path} of file file to visit.
         * @param attrs {@link BasicFileAttributes} {@code file} attributes.
         * @return {@link FileVisitResult#CONTINUE}
         * @throws IOException if deletion fails for some reasons.
         */
        @Override
        public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
            Files.delete(file);
            return FileVisitResult.CONTINUE;
        }

        /**
         * Deletes directory after recursive visiting it via {@link #visitFile}.
         * @param dir {@link Path} of directory to visit.
         * @param e {@link IOException} instance if error occurred during directory visiting.
         * @return {@link FileVisitResult#CONTINUE}
         * @throws IOException if deletion fails for some reasons.
         */
        @Override
        public FileVisitResult postVisitDirectory(Path dir, IOException e) throws IOException {
            Files.delete(dir);
            return FileVisitResult.CONTINUE;
        }
    }

    @Override
    public void implementJar(Class<?> token, Path jarFile) throws ImplerException {
        Path tmpDir;
        try {
            jarFile = jarFile.toAbsolutePath();
            createParentDir(jarFile);
            tmpDir = Files.createTempDirectory(jarFile.getParent(), "tmpDir");
        } catch (IOException e) {
            throw new ImplerException(e);
        }

        try {
            implement(token, tmpDir);
            compile(token, tmpDir);
            packToJar(jarFile, tmpDir,
                    getImplPackageName(token).replace('.', '/') + '/'
                            + token.getSimpleName() + IMPL_SUFFIX + ".class");
        } catch (Exception e) {
            throw new ImplerException(e);
        } finally {
            try {
                Files.walkFileTree(tmpDir, new RmrfFileVisitor());
            } catch (IOException e) {
                System.out.println("Error during deleting template directory " + tmpDir);
                e.printStackTrace();
            }
        }
    }

    /**
     * Complies implemented class in given directory. Method obtains default java compiler via {@link ToolProvider}
     * and use it to compile it with optional {@code -cp} argument pointing to classpath of {@code token} if it's
     * possible to get.
     *
     * @param token implemented class.
     * @param tmpDir package directory of implementation.
     * @throws ImplerException if compiler is missing or implementation contains compilation errors.
     */
    private void compile(Class<?> token, Path tmpDir) throws ImplerException {
        JavaCompiler compiler = ToolProvider.getSystemJavaCompiler();
        if (compiler == null) {
            throw new ImplerException("Could not find java compiler.");
        }
        List<String> args = new ArrayList<>(List.of(getImplementPath(token, tmpDir).toString()));
        String cp = getClassPath(token);
        if (cp != null) {
            args.addAll(List.of("-cp", tmpDir + File.pathSeparator + cp));
        }
        int compileCode = compiler.run(null, null, null, args.toArray(String[]::new));
        if (compileCode != 0) {
            throw new ImplerException("Compiler exit code " + compileCode);
        }
    }

    /**
     * Returns class path of {@code token}.
     *
     * @param token class which classpath to get.
     * @return classpath of {@code token} or {@code null} if this class doesn't have it.
     * @throws ImplerException if exception occurs while getting classpath.
     */
    private String getClassPath(Class<?> token) throws ImplerException {
        try {
            return Path.of(token.getProtectionDomain().getCodeSource().getLocation().toURI()).toString();
        } catch (URISyntaxException e) {
            throw new ImplerException("Could not get class path", e);
        } catch (NullPointerException e) {
            return null;
        }
    }


    /**
     * Packs directory to <var>.jar</var> file with a single entry.
     *
     * @param pathToJar path where to put <var>.jar</var> file. It's parent directory should exists.
     * @param fileDir directory to pack.
     * @param entryName name of single entry in result <var>.jar</var> file.
     * @throws ImplerException if an {@link IOException} occurs during work with file system.
     */
    private void packToJar(Path pathToJar, Path fileDir, String entryName) throws ImplerException {
        Manifest manifest = new Manifest();
        manifest.getMainAttributes().put(Attributes.Name.MANIFEST_VERSION, "1.0");
        try (JarOutputStream out = new JarOutputStream(Files.newOutputStream(pathToJar), manifest)) {
            out.putNextEntry(new JarEntry(entryName));
            Files.copy(Paths.get(fileDir.toString(), entryName), out);
            out.closeEntry();
        } catch (IOException e) {
            throw new ImplerException(e);
        }
    }

    /**
     * Returns path where to locate <var>.java</var> file with implementation source. It calculates by resolving to
     * {@code root}'s absolute path class package directory and implemented class name. Class package directory is it's
     * implementation package name with dots replaces by {@link File#separatorChar}. Implemented class name is a simple
     * name of given class concatenated with {@link #IMPL_SUFFIX} and <var>.java</var> extension.
     *
     * @param token implemented class.
     * @param root  root directory for locating source files.
     * @return path where to locate implementation of {@code token} source code.
     * @throws ImplerException if an error occurs during obtaining {@code token}'s implementation package name.
     */
    private Path getImplementPath(Class<?> token, Path root) throws ImplerException {
        return root.toAbsolutePath()
                .resolve(getImplPackageName(token).replace('.', File.separatorChar))
                .resolve(token.getSimpleName() + IMPL_SUFFIX + ".java");
    }

    /**
     * Creates parent directory if it's not exists.
     *
     * @param path child directory.
     * @throws IOException if an error occurs during creating non-existent directories.
     */
    private void createParentDir(Path path) throws IOException {
        Path parent = path.getParent();
        if (parent != null) {
            Files.createDirectories(parent);
        }
    }

    /**
     * Converts given string to unicode formatted string.
     *
     * @param s unformatted string.
     * @return unicode formatted string equals given.
     */
    private String toUnicodeString(String s) {
        StringBuilder result = new StringBuilder();
        for (char c : s.toCharArray()) {
            if (c < 128) {
                result.append(c);
            } else {
                result.append(String.format("\\u%04X", (int) c));
            }
        }
        return result.toString();
    }

    /**
     * Returns package name for implementation of given class.
     *
     * @param token class to implement.
     * @return empty package name if this class has no classpath or {@code token}'s package name otherwise.
     * @throws ImplerException if an error occurs during getting {@code token}'s classpath.
     */
    private String getImplPackageName(Class<?> token) throws ImplerException {
        return getClassPath(token) == null ? "" : token.getPackageName();
    }

    @Override
    public void implement(Class<?> token, Path root) throws ImplerException {
        root = getImplementPath(token, root);

        final int modifiers = token.getModifiers();

        if (token == Enum.class) {
            throw new ImplerException("Unable to directly implement java.lang.Enum class.");
        } else if (token.isPrimitive()) {
            throw new ImplerException("Unable to implement " + token.getSimpleName() + ".");
        } else if (Modifier.isPrivate(modifiers)) {
            throw new ImplerException("Unable to implement private class.");
        } else if (Modifier.isFinal(modifiers)) {
            throw new ImplerException("Unable to extend final class.");
        }

        try {
            createParentDir(root);
            BufferedWriter writer = Files.newBufferedWriter(root);
            implementClass(token, writer);
            writer.close();
        } catch (Exception e) {
            throw new ImplerException(e);
        }
    }

    /**
     * Writes implementation of given class to given writer.
     *
     * @param token class to implement.
     * @param writer output writer for source code.
     * @throws IOException if error occurs during writing.
     * @throws ImplerException if given class cannot be implemented.
     */
    private void implementClass(Class<?> token, Writer writer) throws IOException, ImplerException {
        writer.write(toUnicodeString(implementationOfPackage(token)));
        writer.write(toUnicodeString(implementationOfDeclaration(token)));
        implementConstructors(token, writer);
        implementMethods(token, writer);
        writer.write(toUnicodeString(String.format("}%n")));
    }

    /**
     * Returns string containing implementation of package for given class. The result of this method would be an empty
     * string, if given class has empty implementation package name, or string
     * <blockquote>
     *     {@code "package "} + implementation package name + {@code ";"} + two break line characters
     * </blockquote>
     * @param token class to implement.
     * @return implemented package declaration string.
     * @throws ImplerException if an error occurs while getting {@code token}'s implement package name.
     */
    private String implementationOfPackage(Class<?> token) throws ImplerException {
        String packageName = getImplPackageName(token);
        if (packageName.isEmpty())
            return "";
        return String.format("package %s;%n%n", packageName);
    }

    /**
     * Returns string containing implementation of {@link Type} instance according to given substitution map for
     * generic parameters.
     *
     * @param type type to implement.
     * @param subst substitution map for generic parameter names which <b>must</b> contain all type parameter names
     *              used in {@code type}, or {@code null} to substitute for each parameter name itself.
     * @return implemented type string.
     */
    private String implementationOfType(Type type, Map<String, String> subst) {
        if (type instanceof GenericArrayType) {
            return implementationOfType(((GenericArrayType) type).getGenericComponentType(), subst) + "[]";
        } else if (type instanceof ParameterizedType) {
            ParameterizedType parameterizedType = (ParameterizedType) type;
            return implementationOfType(parameterizedType.getRawType(), subst) +
                    "<" + Arrays.stream(parameterizedType.getActualTypeArguments())
                            .map(t -> implementationOfType(t, subst))
                            .collect(Collectors.joining(", ")) + ">";
        } else if (type instanceof TypeVariable) {
            String name = ((TypeVariable<?>) type).getName();
            return subst == null ? name : subst.get(name);
        } else if (type instanceof WildcardType) {
            WildcardType wildcardType = (WildcardType) type;
            String result = "?";
            Type[] lowerBounds = wildcardType.getLowerBounds();
            Type[] upperBounds = wildcardType.getUpperBounds();
            if (lowerBounds.length > 0) {
                result += " super " + implementationOfType(lowerBounds[0], subst);
            } else if (!upperBounds[0].equals(Object.class)) {
                result += " extends " + implementationOfType(upperBounds[0], subst);
            }
            return result;
        } else {
            return ((Class<?>) type).getCanonicalName();
        }
    }

    /**
     * Makes an attempt to infer most common covariant type from not empty list of types. This method assumes that all
     * types are instances of the same type interface (have the same structure) and tries to recursively infer minimal
     * type (minimal according to comparator cmp1, which is used to compare raw type instances; cmp2 is reversed cmp1
     * comparator used in inferring wildcard types). This method also doesn't know about type parameters conversion
     * (to another type parameter or just type) and in case of all types in list are generic parameters, it returns
     * implementation of a random parameter.
     *
     * @param types <b>not empty</b> list of types, which represents list of return types of some function.
     * @param subst generic parameters substitution map, which <b>must contain</b> all parameters used in at least one
     *              type in list.
     * @param cmp1 comparator of raw types, according to which method would infer <i>minimal</i> type.
     * @param cmp2 reversed {@code cmp1} comparator, which would be used to infer <i>maximal</i> lower bound of a
     *             several {@link WildcardType wildcard types}.
     * @return implemented minimal type according to comparator {@code cmp1} and substitution map.
     *
     * @see Type
     * @see GenericArrayType
     * @see ParameterizedType
     * @see TypeVariable
     * @see WildcardType
     * @see Class
     */
    private String inferCovariantType(List<Type> types,
                                      Map<String, String> subst,
                                      Comparator<Class<?>> cmp1,
                                      Comparator<Class<?>> cmp2) {
        Type type = types.get(0);
        if (type instanceof GenericArrayType) {
            return inferCovariantType(types.stream()
                            .map(t -> ((GenericArrayType) t).getGenericComponentType())
                            .collect(Collectors.toList()), subst, cmp1, cmp2) + "[]";
        } else if (type instanceof ParameterizedType) {
            Class<?> minClass = types.stream()
                    .map(t -> ((Class<?>) ((ParameterizedType) t).getRawType()))
                    .min(cmp1).get();
            List<ParameterizedType> minTypes = types.stream()
                    .map(t -> (ParameterizedType) t)
                    .filter(t -> t.getRawType().equals(minClass))
                    .collect(Collectors.toList());
            return minClass.getCanonicalName() + "<" +
                    IntStream.range(0, ((ParameterizedType) type).getActualTypeArguments().length)
                            .mapToObj(i -> minTypes.stream()
                                    .map(t -> t.getActualTypeArguments()[i])
                                    .collect(Collectors.toList()))
                            .map(list -> inferCovariantType(list, subst, cmp1, cmp2))
                            .collect(Collectors.joining(", ")) + ">";
        } else if (type instanceof TypeVariable) {
            return implementationOfType(type, subst);
        } else if (type instanceof WildcardType) {
            String result = "?";
            List<WildcardType> wildcardTypes = types.stream().map(t -> (WildcardType) t).collect(Collectors.toList());
            if (wildcardTypes.stream().anyMatch(t -> t.getLowerBounds().length > 0)) {
                result += " super " + inferCovariantType(wildcardTypes.stream()
                                .filter(t -> t.getLowerBounds().length > 0)
                                .map(t -> t.getLowerBounds()[0])
                                .collect(Collectors.toList()),
                        subst, cmp2, cmp1);
            } else if (wildcardTypes.stream().anyMatch(t -> !t.getUpperBounds()[0].equals(Object.class))) {
                result += " extends " + inferCovariantType(wildcardTypes.stream()
                                .map(t -> t.getUpperBounds()[0])
                                .collect(Collectors.toList()),
                        subst, cmp1, cmp2);
            }
            return result;
        } else {
            return types.stream().map(t -> (Class<?>)t).min(cmp1).get().getCanonicalName();
        }
    }

    /**
     * Return implementation of a {@link TypeVariable type variable} as it was declared.
     *
     * @param typeVariable variable to declare.
     * @param subst substitution map for variable name which it <b>must contain</b>.
     * @return string containing real variable name and bounds separated by {@code &}.
     */
    private String getTypeVariableDeclaration(TypeVariable<?> typeVariable, Map<String, String> subst) {
        String result = implementationOfType(typeVariable, subst);
        Type[] bounds = typeVariable.getBounds();
        if (!bounds[0].equals(Object.class)) {
            result += " extends " + Arrays.stream(bounds)
                    .map(t -> implementationOfType(t, subst))
                    .collect(Collectors.joining(" & "));
        }
        return result;
    }

    /**
     * Return generic declaration string of all {@link TypeVariable type variables} in {@code typeVariables} array as
     * <blockquote>
     *     {@code "<declarationOfVariable1, ..., declarationOfVariableN>"}
     * </blockquote>
     * @param typeVariables type variables to declare.
     * @param subst substitution map for type variable names which <b>must contain</b> all of them.
     * @return generic declaration of {@code typeVariables}.
     */
    private String getGenericDeclaration(TypeVariable<?>[] typeVariables, Map<String, String> subst) {
        return getGenericMapped(typeVariables, t -> getTypeVariableDeclaration(t, subst));
    }

    /**
     * Does the same as {@link #getGenericDeclaration} but without variable's bounds and substitution map.
     *
     * @param typeVariables type variables to list.
     * @return string of variable names separated by comma and enclosed with triangle brackets.
     */
    private String getGenericSubstitution(TypeVariable<?>[] typeVariables) {
        return getGenericMapped(typeVariables, TypeVariable::getName);
    }

    /**
     * Maps type variables to strings via {@code mapper}, join them by comma and enclose by triangle brackets if result
     * non-empty.
     *
     * @param typeVariables array of type variables.
     * @param mapper function from {@link TypeVariable} to {@link String}
     * @return generic-type string contains result of mapping function to each type variable.
     */
    private String getGenericMapped(TypeVariable<?>[] typeVariables,
                                    Function<? super TypeVariable<?>, ? extends String> mapper) {
        String result = Arrays.stream(typeVariables).map(mapper).collect(Collectors.joining(", "));
        if (!result.isEmpty()) {
            result = "<" + result + ">";
        }
        return result;
    }

    /**
     * Return string containing declaration of implemented class ended with open bracket and break line character.
     *
     * @param token class to implement.
     * @return string containing declaration of implemented class.
     */
    private String implementationOfDeclaration(Class<?> token) {
        TypeVariable<? extends Class<?>>[] typeParameters = token.getTypeParameters();
        return String.format("public class %s%s%s %s %s%s {%n",
                token.getSimpleName(),
                IMPL_SUFFIX,
                getGenericDeclaration(typeParameters, null),
                token.isInterface() ? "implements" : "extends",
                token.getCanonicalName(),
                getGenericSubstitution(typeParameters));
    }

    /**
     * Wraps all non-private methods in {@link WrappedMethod WrappedMethod} with {@code subst} and add all of
     * them to {@code set}.
     *
     * @param methods methods to wrap.
     * @param subst substitution map to wrap each method with.
     * @param set set to add all wrapped methods.
     */
    private void wrapForSet(Method[] methods, Map<String, String> subst, Set<WrappedMethod> set) {
        set.addAll(Arrays.stream(methods)
                .filter(method -> !Modifier.isPrivate(method.getModifiers()))
                .map(method -> new WrappedMethod(method, subst))
                .collect(Collectors.toSet()));
    }

    /**
     * Constructs new substitution map according to extended class actual and generic type parameters and substitution
     * map of inherited class.
     *
     * @param type actual {@link Type} of extended class.
     * @param prevSubst inherited class substitution map.
     * @return substitution map for extended class.
     */
    private Map<String, String> getNextSubst(Type type, Map<String, String> prevSubst) {
        Map<String, String> subst = new HashMap<>();
        if (type instanceof ParameterizedType) {
            ParameterizedType parameterizedType = (ParameterizedType) type;
            TypeVariable<? extends Class<?>>[] typeParameters =
                    ((Class<?>) parameterizedType.getRawType()).getTypeParameters();
            Type[] actualTypeArguments = parameterizedType.getActualTypeArguments();
            for (int i = 0; i < typeParameters.length; ++i) {
                subst.put(typeParameters[i].getName(), implementationOfType(actualTypeArguments[i], prevSubst));
            }
        }
        return subst;
    }

    /**
     * Calls {@link #wrapForSet} from {@code token}'s declared methods and {@code set}, and then recursively iterates
     * over implemented by {@code token} interfaces.
     *
     * @param token class to iterate from.
     * @param subst substitution map for {@code token}.
     * @param set set collecting wrapped declared methods.
     */
    private void iterateOverInterfaces(Class<?> token, Map<String, String> subst, Set<WrappedMethod> set) {
        wrapForSet(token.getDeclaredMethods(), subst, set);
        Class<?>[] interfaces = token.getInterfaces();
        Type[] genericInterfaces = token.getGenericInterfaces();
        for (int i = 0; i < interfaces.length; ++i) {
            iterateOverInterfaces(interfaces[i], getNextSubst(genericInterfaces[i], subst), set);
        }
    }

    /**
     * Return nth parameter of some implemented class.
     *
     * @param n parameter index.
     * @return {@link #PARAM_PREFIX} + n.
     */
    private String nthParameter(int n) {
        return PARAM_PREFIX + n;
    }

    /**
     * Checks if given class has non-private constructor without arguments.
     *
     * @param token class which constructor need to check.
     * @return {@code true} if sought constructor exists, {@code false} otherwise.
     */
    private boolean hasDefaultConstructor(Class<?> token) {
        try {
            return !Modifier.isPrivate(token.getDeclaredConstructor().getModifiers());
        } catch (NoSuchMethodException e) {
            return false;
        }
    }

    /**
     * Implements all required non-private constructors of {@code token} and writes it to {@code writer}.
     *
     * @param token class to implement.
     * @param writer output writer for implementation.
     * @throws IOException if an error occurs during writing.
     * @throws ImplerException if {@code token} doesn't have non-private constructors.
     */
    private void implementConstructors(Class<?> token, Writer writer) throws IOException, ImplerException {
        if (token.isInterface() || hasDefaultConstructor(token))
            return;
        List<Constructor<?>> constructors = Arrays.stream(token.getDeclaredConstructors())
                .filter(constructor -> !Modifier.isPrivate(constructor.getModifiers()))
                .collect(Collectors.toList());
        if (constructors.isEmpty()) {
            throw new ImplerException("Empty non-private constructors list.");
        }
        for (Constructor<?> constructor : constructors) {
            writer.write(toUnicodeString(implementationOfConstructor(constructor)));
        }
    }

    /**
     * Returns implementation of given constructor which calls {@code super} from constructor's arguments.
     *
     * @param constructor constructor to implement.
     * @return string implementation of {@code constructor}.
     */
    private String implementationOfConstructor(Constructor<?> constructor) {
        StringBuilder result = new StringBuilder();
        int modifiers = constructor.getModifiers();
        Type[] genericParameterTypes = constructor.getGenericParameterTypes();
        result.append(String.format("    %s %s %s(%s) ",
                Modifier.isPublic(modifiers) ? "public" : Modifier.isProtected(modifiers) ? "protected" : "",
                getGenericDeclaration(constructor.getTypeParameters(), null),
                constructor.getDeclaringClass().getSimpleName() + IMPL_SUFFIX,
                implementationOfParameterTypes(genericParameterTypes, null, constructor.isVarArgs())));

        result.append(implementationOfThrows(constructor.getGenericExceptionTypes()));

        result.append(String.format("{ super(%s); }%n",
                IntStream.range(0, genericParameterTypes.length)
                        .mapToObj(this::nthParameter)
                        .collect(Collectors.joining(", "))));

        return result.toString();
    }

    /**
     * Implement all not implemented abstract methods of class {@code token} and its superclasses and writes it to
     * {@code writer}.
     *
     * @param token class to implement.
     * @param writer output writer for implementation.
     * @throws IOException if an error occurs while writing implementation.
     */
    private void implementMethods(Class<?> token, Writer writer) throws IOException {
        Set<WrappedMethod> signatures = new HashSet<>();
        TypeVariable<? extends Class<?>>[] typeParameters = token.getTypeParameters();
        Set<String> genericVariables =
                Arrays.stream(typeParameters).map(TypeVariable::getName).collect(Collectors.toSet());
        Map<String, String> subst =
                Arrays.stream(typeParameters).map(TypeVariable::getName).collect(Collectors.toMap(s -> s, s -> s));
        while (token != null) {
            iterateOverInterfaces(token, subst, signatures);
            subst = getNextSubst(token.getGenericSuperclass(), subst);
            token = token.getSuperclass();
        }
        List<List<WrappedMethod>> methodLists = signatures.stream()
                .collect(Collectors.groupingBy(WrappedMethod::getSignature, Collectors.toList()))
                .values().stream()
                .filter(list -> {
                    List<Integer> mods = list.stream()
                            .map(WrappedMethod::getMethod)
                            .map(Method::getModifiers)
                            .collect(Collectors.toList());
                    return mods.stream().anyMatch(Modifier::isAbstract)
                            && mods.stream().noneMatch(Modifier::isFinal);
                })
                .collect(Collectors.toList());
        for (List<WrappedMethod> methodList : methodLists) {
            writer.write(toUnicodeString(implementationOfMethod(methodList, genericVariables)));
        }
    }

    /**
     * Completes method's substitution map by adding to it all type variables from method's generic declaration. For
     * each variable this method creates name which didn't used before and which {@code classGenericVariables} doesn't
     * contain.
     *
     * @param wrappedMethod {@link WrappedMethod wrapped method} which substitution map to complete.
     * @param classGenericVariables generic variable names to avoid.
     * @return completed substitution map.
     */
    private Map<String, String> completeMethodSubst(WrappedMethod wrappedMethod, Set<String> classGenericVariables) {
        Set<String> methodGenericVariables = new HashSet<>();
        Map<String, String> subst = new HashMap<>(Map.copyOf(wrappedMethod.getSubst()));
        TypeVariable<?>[] typeParameters = wrappedMethod.getMethod().getTypeParameters();
        for (TypeVariable<?> typeVariable : typeParameters) {
            String name = typeVariable.getName();
            if (classGenericVariables.contains(name) || methodGenericVariables.contains(name)) {
                int suffix = 1;
                while (classGenericVariables.contains(name + suffix) || methodGenericVariables.contains(name + suffix)) {
                    ++suffix;
                }
                name += suffix;
            }
            subst.put(typeVariable.getName(), name);
            methodGenericVariables.add(name);
        }
        return subst;
    }

    /**
     * Compare two {@link Class} instances according to relation <i>is {@code class1} descendant of {@code class2}</i>.
     * Returns {@code -1} if {@code class1} is descendant of {@code class2}; {@code 1} if {@code class1} is ancestor of
     * {@code class2}; 0 if {@code class1 == class2}.
     *
     * @param class1 first compare argument.
     * @param class2 second compare argument.
     * @return result of comparison.
     */
    private static int compareClasses(Class<?> class1, Class<?> class2) {
        return class1.equals(class2) ? 0 : class2.isAssignableFrom(class1) ? -1 : 1;
    }

    /**
     * Compare two {@link Class} instances according to relation <i>is {@code class1} ancestor of {@code class2}</i>.
     * Returns {@code -1} if {@code class1} is ancestor of {@code class2}; {@code 1} if {@code class1} is descendant of
     * {@code class2}; 0 if {@code class1 == class2}.
     *
     * @param class1 class1 first compare argument.
     * @param class2 second compare argument.
     * @return result of comparison.
     */
    private static int reversedCompareClasses(Class<?> class1, Class<?> class2) {
        return compareClasses(class2, class1);
    }

    /**
     * Creates implementation of a method from list of a methods with the same names and signatures and set of already
     * used type variable names. Implemented method return type inferred by method {@link #inferCovariantType} and has
     * the same name and signature as a random method from a {@code methodList}.
     *
     * @param methodList <b>not empty</b> list of {@link WrappedMethod wrapped methods} with the same names and
     *                   signatures.
     * @param classGenericVariables set of already used type variable names to avoid in use in generic declaration of
     *                              implemented method.
     * @return implementation of described method.
     */
    private String implementationOfMethod(List<WrappedMethod> methodList, Set<String> classGenericVariables) {
        WrappedMethod wrappedMethod = methodList.get(0);
        Map<String, String> methodSubst = completeMethodSubst(wrappedMethod, classGenericVariables);
        return implementationOfMethod(wrappedMethod.getMethod(),
                methodSubst,
                inferCovariantType(methodList.stream()
                                .map(WrappedMethod::getMethod)
                                .map(Method::getGenericReturnType)
                                .collect(Collectors.toList()),
                        methodSubst,
                        Implementor::compareClasses, Implementor::reversedCompareClasses));
    }

    /**
     * Creates implementation of a given method according to given substitution map with a given return value.
     *
     * @param method method to implement.
     * @param subst substitution map for generic variable names which <b>must contain</b> all variables used in method.
     * @param returnValue string representative of {@code method}'s return value.
     * @return {@code method}'s implementation with given string return value.
     */
    private String implementationOfMethod(Method method, Map<String, String> subst, String returnValue) {
        StringBuilder result = new StringBuilder();

        int modifiers = method.getModifiers();
        TypeVariable<Method>[] typeParameters = method.getTypeParameters();

        result.append(String.format("    %s %s %s %s(%s) ",
                Modifier.isPublic(modifiers) ? "public" : Modifier.isProtected(modifiers) ? "protected" : "",
                getGenericDeclaration(typeParameters, subst),
                returnValue,
                method.getName(),
                implementationOfParameterTypes(method.getGenericParameterTypes(), subst, method.isVarArgs())));

        Class<?> returnType = method.getReturnType();
        result.append(String.format("{%s}%n",
                returnType.equals(void.class) ? ""
                        : String.format(" return %s; ",
                        returnType.isPrimitive() ? (returnType.equals(boolean.class) ? "false" : "0")
                                : "null")));

        return result.toString();
    }

    /**
     * Creates implementation of some function or constructor's arguments list. For example this method's implementation
     * of parameter types is {@code "Type[] types, Map<String, String> subst, boolean isVarArg"}. This method creates
     * parameter's names according to function {@link #nthParameter} and allows to make last parameter variable argument
     * parameter.
     *
     * @param types types of parameters in declaration order.
     * @param subst substitution map for generic variable names which <b>must contain</b> all variables used
     *              in {@code types}.
     * @param isVarArg is last parameter variable argument; if its value is {@code true} then <i>last parameter
     *                 must be type of an array</i>.
     * @return implementation of given parameter types.
     */
    private String implementationOfParameterTypes(Type[] types, Map<String, String> subst, boolean isVarArg) {
        String result = IntStream.range(0, types.length)
                .mapToObj(i -> implementationOfType(types[i], subst) + " " + nthParameter(i))
                .collect(Collectors.joining(", "));
        if (isVarArg) {
            int x = result.lastIndexOf("[]");
            result = result.substring(0, x) + "..." + result.substring(x + 2);
        }
        return result;
    }

    /**
     * Constructs implementation of list of exceptions to throw. If {@code genericExceptionTypes} is empty then empty
     * string is returned. Otherwise returns string
     * <blockquote>
     *     {@code "throws Exception1, Exception2, ..., ExceptionN "}
     * </blockquote>
     * @param genericExceptionTypes exception types to throw.
     * @return string implementation of throw signature.
     */
    private String implementationOfThrows(Type[] genericExceptionTypes) {
        String result = "";
        if (genericExceptionTypes.length > 0) {
            result = String.format("throws %s ",
                    Arrays.stream(genericExceptionTypes)
                            .map(t -> implementationOfType(t, null))
                            .collect(Collectors.joining(", ")));
        }
        return result;
    }

    /**
     * Application entry point. Result depends on passed arguments. If there is 2 arguments: first is a full name of a
     * class and second is an absolute path, then this function calls {@link #implement} method from 1st and 2nd
     * arguments. Else if there is 3 arguments: 1st must be {@code -jar} string, 2nd full name of a class and 3rd -
     * absolute path to jar file, then function {@link #implementJar} would be called.
     *
     * @param args method's arguments list.
     */
    public static void main(String[] args) {
        if (args == null) {
            System.out.println("null args");
            return;
        }
        try {
            if (args.length == 2) {
                new Implementor().implement(Class.forName(args[0]), Path.of(args[1]));
            } else if (args.length == 3 && args[0].equals("-jar")) {
                new Implementor().implementJar(Class.forName(args[1]), Path.of(args[2]));
            } else {
                System.out.println("Usage: java Implementor full.class.name path.for.implementation");
                System.out.println("Usage: java Implementor -jar full.class.name path.for.implementation.jar");
                return;
            }
            System.out.println("Done.");
        } catch (ImplerException e) {
            System.out.println("Unable to implement this class.");
            System.out.println(e.getMessage());
        } catch (ClassNotFoundException e) {
            System.out.println("No such class found.");
        }
    }
}
