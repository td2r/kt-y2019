public class SumHex {
    public static void main(String[] args) {
        int sum = 0;
        for (String str : args) {
            String s = str.stripTrailing().toUpperCase();
            int n = s.length();
            for (int r = 0; r < n; ) {
                while (Character.isWhitespace(s.charAt(r))) {
                    r++;
                }
                int l = r;
                while (r < n && !Character.isWhitespace(s.charAt(r))) {
                    r++;
                }
                if (s.startsWith("0X", l)) {
                    sum += Integer.parseUnsignedInt(s.substring(l + 2, r), 16);
                    //sum += (int)Long.parseLong(s.substring(l + 2, r), 16);
                    /*
                    if (l + 2 == r - 1) {
                        sum += Integer.parseInt(s.substring(l + 2, r), 16);
                    } else {
                        sum += Integer.parseInt(s.substring(l + 2, r - 1), 16) * 16 +
                        Integer.parseInt(s.substring(r - 1, r), 16);
                    }
                    */
                } else {
                    sum += Integer.parseInt(s.substring(l, r));
                }
            }
        }
        System.out.println(sum);
    }
}
