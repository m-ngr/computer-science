
/**
 * Week 2: StringsFirstAssignments - Part 3
 * 
 * @author (Mahmoud Elnagar) 
 * @version (18/07/2022)
 */
public class Part3 {
    public boolean twoOccurrences(String stringa, String stringb){
        //returns true if stringa appears at least twice in stringb, otherwise it returns false.
        int index = stringb.indexOf(stringa);
        if (index == -1) return false;
        index = stringb.indexOf(stringa, index + stringa.length());
        if (index == -1) return false;
        return true;
    }
    
    public String lastPart(String stringa, String stringb){
        int index = stringb.indexOf(stringa);
        if (index == -1) return stringb;
        return stringb.substring(index + stringa.length(), stringb.length());
    }
    
    void testTwoOccurrences(String a, String b){
        System.out.println("twoOccurrences(“" + a + "”, “" + b + "”) returns " 
        + (twoOccurrences(a, b)? "True":"False"));
    }
    
    void testLastPart(String a, String b){
        System.out.println("lastPart(“" + a + "”, “" + b + "”) returns " + lastPart(a, b));
    }
    
    public void testing(){
        testTwoOccurrences("by", "A story by Abby Long");
        testTwoOccurrences("a", "banana");
        testTwoOccurrences("atg", "ctgtatgta");
        
        testLastPart("an", "banana");
        testLastPart("zoo", "forest");
    }
    
    public static void main(String[] args){
        Part3 p3 = new Part3();
        p3.testing();
    }
}
