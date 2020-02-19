import java.util.ArrayList;
import java.util.Scanner;

public class CodeChallenge {
    public static Scanner scanner = new Scanner(System.in);

    public static void main(String[] args) {
        String str = scanner.next();
        int start = 0;
        int biggestCount = 0;
        ArrayList<Character> chars = new ArrayList<>();
        for (int i = 0; i < str.length(); i++) {
            if (!chars.contains(str.charAt(i))) {
                chars.add(str.charAt(i));
            } else {
                if (biggestCount < chars.size()) {
                    biggestCount = chars.size();
                }
                chars = new ArrayList<>();
                i = start;
                start++;
            }
        }
        if (biggestCount < chars.size()) {
            biggestCount = chars.size();
        }
        System.out.println(biggestCount);
    }
}
