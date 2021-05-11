// you can also use imports, for example:
// import java.util.*;

// you can write to stdout for debugging purposes, e.g.
// System.out.println("this is a debug message");

// Use Java 11


import java.util.HashMap;
import java.util.concurrent.atomic.AtomicInteger;

public class Solution {
    public int solution(int[] A, int L, int R) {
        var mapper = new HashMap<Integer, Integer>();
        for (int a : A) {
            mapper.put(a, mapper.getOrDefault(a, 0) + 1);
        }
        AtomicInteger result = new AtomicInteger();
        mapper.forEach((key, value) -> {
            int count = Math.min(value, 2);
            if (count == 2) {
                if (key >= L) {
                    count--;
                }
                if (key <= R) {
                    count--;
                }
            }
            else if (key >= L && key <= R)
            {
                count--;
            }
            if (count > 0) {
                result.addAndGet(count);
            }
        });
        return result.get();
    }
}
