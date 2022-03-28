import org.jetbrains.annotations.NotNull;

/**
 * В теле класса решения разрешено использовать только финальные переменные типа RegularInt.
 * Нельзя volatile, нельзя другие типы, нельзя блокировки, нельзя лазить в глобальные переменные.
 *
 * @author Naumov Ivan
 */
public class Solution implements MonotonicClock {
    private final RegularInt c1_1 = new RegularInt(0);
    private final RegularInt c1_2 = new RegularInt(0);
    private final RegularInt c1_3 = new RegularInt(0);
    private final RegularInt c2_1 = new RegularInt(0);
    private final RegularInt c2_2 = new RegularInt(0);
    private final RegularInt c2_3 = new RegularInt(0);

    @Override
    public void write(@NotNull Time time) {
        c2_1.setValue(time.getD1());
        c2_2.setValue(time.getD2());
        c2_3.setValue(time.getD3());

        c1_3.setValue(time.getD3());
        c1_2.setValue(time.getD2());
        c1_1.setValue(time.getD1());
    }

    @NotNull
    @Override
    public Time read() {
        int r1_1 = c1_1.getValue();
        int r1_2 = c1_2.getValue();
        int r1_3 = c1_3.getValue();
        int r2_3 = c2_3.getValue();
        int r2_2 = c2_2.getValue();
        int r2_1 = c2_1.getValue();
        if (r1_1 != r2_1)
            return new Time(r2_1, 0, 0);
        else if (r1_2 != r2_2)
            return new Time(r2_1, r2_2, 0);
        else
            return new Time(r2_1, r2_2, r2_3);
    }
}
