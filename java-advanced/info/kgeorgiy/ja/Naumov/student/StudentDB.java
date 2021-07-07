package info.kgeorgiy.ja.Naumov.student;

import info.kgeorgiy.java.advanced.student.*;

import java.util.*;
import java.util.function.BinaryOperator;
import java.util.function.Function;
import java.util.stream.Collector;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class StudentDB implements AdvancedQuery {
    private static final Comparator<Student> COMPARE_STUDENTS_BY_NAME =
            Comparator.comparing(Student::getLastName).reversed()
                    .thenComparing(Comparator.comparing(Student::getFirstName).reversed())
                    .thenComparing(Student::compareTo);

    private static final Comparator<Map.Entry<String, Set<GroupName>>> COMPARE_SET_SIZE_THEN_STRING =
            Comparator.comparingInt((Map.Entry<String, Set<GroupName>> e) -> e.getValue().size())
                    .thenComparing(Map.Entry::getKey);

    private static final Comparator<Map.Entry<GroupName, Set<String>>> COMPARE_SET_SIZE_THEN_GROUP =
            Comparator.comparingInt((Map.Entry<GroupName, Set<String>> e) -> e.getValue().size())
            .thenComparing(Comparator.comparing((Function<Map.Entry<GroupName, Set<String>>, GroupName>) Map.Entry::getKey).reversed());

    private static final Comparator<Map.Entry<GroupName, List<Student>>> COMPARE_LIST_SIZE_THEN_GROUP =
            Comparator.comparingInt((Map.Entry<GroupName, List<Student>> e) -> e.getValue().size())
                    .thenComparing(Map.Entry::getKey);

    static String getFullName(Student student) {
        return student.getFirstName() + " " + student.getLastName();
    }

    private <T, C> C mapAndCollect(List<Student> students, Function<Student, T> mapper, Collector<T, ?, C> collector) {
        return students.stream().map(mapper).collect(collector);
    }

    @Override
    public List<String> getFirstNames(List<Student> students) {
        return mapAndCollect(students, Student::getFirstName, Collectors.toList());
    }

    @Override
    public List<String> getLastNames(List<Student> students) {
        return mapAndCollect(students, Student::getLastName, Collectors.toList());
    }

    @Override
    public List<GroupName> getGroups(List<Student> students) {
        return mapAndCollect(students, Student::getGroup, Collectors.toList());
    }

    @Override
    public List<String> getFullNames(List<Student> students) {
        return mapAndCollect(students, StudentDB::getFullName, Collectors.toList());
    }

    @Override
    public Set<String> getDistinctFirstNames(List<Student> students) {
        return students.stream().map(Student::getFirstName).sorted().collect(Collectors.toCollection(LinkedHashSet::new));
    }

    @Override
    public String getMaxStudentFirstName(List<Student> students) {
        return students.stream().max(Student::compareTo).map(Student::getFirstName).orElse("");
    }

    private List<Student> sortStudentsBy(Collection<Student> students, Comparator<Student> comparator) {
        return students.stream().sorted(comparator).collect(Collectors.toList());
    }

    @Override
    public List<Student> sortStudentsById(Collection<Student> students) {
        return sortStudentsBy(students, Student::compareTo);
    }

    @Override
    public List<Student> sortStudentsByName(Collection<Student> students) {
        return sortStudentsBy(students, COMPARE_STUDENTS_BY_NAME);
    }

    private <T> Stream<Student> findStudents(Collection<Student> students, Function<Student, T> getter, T value) {
        return students.stream().filter(student -> getter.apply(student).equals(value));
    }

    private <T> List<Student> findStudentsAndSort(Collection<Student> students, Function<Student, T> getter, T value) {
        return findStudents(students, getter, value).sorted(COMPARE_STUDENTS_BY_NAME).collect(Collectors.toList());
    }

    @Override
    public List<Student> findStudentsByFirstName(Collection<Student> students, String name) {
        return findStudentsAndSort(students, Student::getFirstName, name);
    }

    @Override
    public List<Student> findStudentsByLastName(Collection<Student> students, String name) {
        return findStudentsAndSort(students, Student::getLastName, name);
    }

    @Override
    public List<Student> findStudentsByGroup(Collection<Student> students, GroupName group) {
        return findStudentsAndSort(students, Student::getGroup, group);
    }

    @Override
    public Map<String, String> findStudentNamesByGroup(Collection<Student> students, GroupName group) {
        return findStudents(students, Student::getGroup, group)
                .collect(Collectors.toMap(
                    Student::getLastName,
                    Student::getFirstName,
                    BinaryOperator.minBy(String::compareTo)));
    }

    private List<Group> getGroups(Collection<Student> students, Function<Collection<Student>, List<Student>> converter) {
        return students.stream()
                .collect(Collectors.groupingBy(Student::getGroup, Collectors.toList()))
                .entrySet().stream()
                .sorted(Map.Entry.comparingByKey())
                .map(entry -> new Group(entry.getKey(), converter.apply(entry.getValue())))
                .collect(Collectors.toList());
    }

    @Override
    public List<Group> getGroupsByName(Collection<Student> students) {
        return getGroups(students, this::sortStudentsByName);
    }

    @Override
    public List<Group> getGroupsById(Collection<Student> students) {
        return getGroups(students, this::sortStudentsById);
    }

    private <T extends Comparable<T>, C> T groupAndGetMax(Collection<Student> students,
                                 Collector<Student, ?, Map<T, C>> collector,
                                 Comparator<Map.Entry<T, C>> comparator,
                                 T defaultValue) {
        return students.stream()
                .collect(collector)
                .entrySet().stream()
                .max(comparator)
                .map(Map.Entry::getKey).orElse(defaultValue);
    }

    @Override
    public GroupName getLargestGroup(Collection<Student> students) {
        return groupAndGetMax(students,
                Collectors.groupingBy(Student::getGroup, Collectors.toList()),
                COMPARE_LIST_SIZE_THEN_GROUP,
                null);
    }

    @Override
    public GroupName getLargestGroupFirstName(Collection<Student> students) {
        return groupAndGetMax(students,
                Collectors.groupingBy(Student::getGroup, Collectors.mapping(Student::getFirstName, Collectors.toSet())),
                COMPARE_SET_SIZE_THEN_GROUP,
                null);
    }

    @Override
    public String getMostPopularName(Collection<Student> students) {
        return groupAndGetMax(students,
                Collectors.groupingBy(Student::getFirstName, Collectors.mapping(Student::getGroup, Collectors.toSet())),
                COMPARE_SET_SIZE_THEN_STRING,
                "");
    }

    private <T> List<T> listOfValuesByIndices(Collection<Student> students, int[] indices, Function<Student, T> mapper) {
        return Arrays.stream(indices).mapToObj(List.copyOf(students)::get).map(mapper).collect(Collectors.toList());
    }

    @Override
    public List<String> getFirstNames(Collection<Student> students, int[] indices) {
        return listOfValuesByIndices(students, indices, Student::getFirstName);
    }

    @Override
    public List<String> getLastNames(Collection<Student> students, int[] indices) {
        return listOfValuesByIndices(students, indices, Student::getLastName);
    }

    @Override
    public List<GroupName> getGroups(Collection<Student> students, int[] indices) {
        return listOfValuesByIndices(students, indices, Student::getGroup);
    }

    @Override
    public List<String> getFullNames(Collection<Student> students, int[] indices) {
        return listOfValuesByIndices(students, indices, StudentDB::getFullName);
    }
}
