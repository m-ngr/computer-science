
/**
 * Week 2: StringsFirstAssignments - Part 1
 * 
 * @author (Mahmoud Elnagar) 
 * @version (18/07/2022)
 */
public class Part1 {
    public String findSimpleGene(String dna){
        // Finds the index position of the start codon “ATG”.
        int startIndex = dna.indexOf("ATG");
        // If there is no “ATG”, return the empty string.
        if (startIndex == -1) return "";
        // Finds the index position of the first stop codon “TAA” appearing after the “ATG” that was found.
        int endIndex = dna.indexOf("TAA", startIndex + 3);
        // If there is no such “TAA”, return the empty string.
        if (endIndex == -1) return "";
        // If the length of the substring between the “ATG” and “TAA” is a multiple of 3
        // then return the substring that starts with that “ATG” and ends with that “TAA”.
        if ((endIndex - startIndex) % 3 != 0) return "";
        
        return dna.substring(startIndex, endIndex + 3);
    }
    
    public void testSimpleGene(){
        String[] dnas = {"ATTGGAATTTGG",
                         "AATGGAATTTGG",
                         "ATTGTAATTTGG",
                         "ATGGGAATTAAG",
                         "ATGGGAATTTAA",
                         "AAATGCCCTAACTAGATTAAGAAACC"};
        for (String dna: dnas){
            System.out.println("Dna: "+ dna);
            System.out.println("Gene: "+ findSimpleGene(dna));
        }
    }
    
    public static void main(String[] args){
        Part1 p1 = new Part1();
        p1.testSimpleGene();
    }
}
