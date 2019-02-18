// you can also use imports, for example:
// import java.util.*;

// you can write to stdout for debugging purposes, e.g.
// System.out.println("this is a debug message");

import java.lang.Math;
import java.util.stream.Stream;

class Solution {
    
    class Range
    {
        int Left  = Integer.MAX_VALUE;
        int Right = Integer.MIN_VALUE;
    }
   
    private static int getCharIndex(char c)
    {
        return Character.getNumericValue(c) - Character.getNumericValue('a'); 
    }
   
    public int solution(String S, int K)
    {
        final int allLowerCaseLetters = 26;
        int[] letterCounts = new int[allLowerCaseLetters];
        int uniqueLetters = 0;
        Range[] ranges = new Range[allLowerCaseLetters];
        for ( int i = 0 ; i < allLowerCaseLetters ; ++i) ranges[i] = new Range();
        for ( int i = 0 ; i < S.length() ; ++i)
        {
            int charIndex = getCharIndex(S.charAt(i));
            ranges[charIndex].Left = Math.min(ranges[charIndex].Left,i);
            ranges[charIndex].Right = Math.max(ranges[charIndex].Right,i);
            if ( letterCounts[charIndex] == 0) ++uniqueLetters;
            letterCounts[charIndex]++;
        }                 
        final int toRemoveK = uniqueLetters - K;
        if (toRemoveK <  0 ) return -1;
        if (toRemoveK == 0 ) return 0;  
        int minLen = Integer.MAX_VALUE;;
        for ( Range a : ranges) 
        {         
            for ( Range b : ranges) 
            {
                int removed = 0;
                int Left  =  a.Left;
                int Right =  b.Right;               
                for ( Range c : ranges)
                {
                    if (c.Left != Integer.MAX_VALUE && c.Left >= Left && c.Right <= Right) ++removed;   
                }
                if ( removed == toRemoveK)
                {
                    minLen = Math.min(minLen, Right - Left + 1);
                }
            }
        }
        return minLen;
    }
             
    public static void main(String[] args) 
    {    
        Solution sol = new Solution();
        System.out.println(sol.solution("zaaaa",1)); 
        System.out.println(sol.solution("abaacbca",2));
        System.out.println(sol.solution("aabcabc",1)); 
    }
}
