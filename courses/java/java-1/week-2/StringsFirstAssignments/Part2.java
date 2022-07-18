
/**
 * Week 2: StringsFirstAssignments - Part 2
 * 
 * @author (Mahmoud Elnagar) 
 * @version (18/07/2022)
 */
public class Part2 {
    public String findSimpleGene(String dna, String startCodon, String stopCodon){
        // Finds the index position of the start codon.
        int startIndex = dna.toLowerCase().indexOf(startCodon.toLowerCase());
        // If there is no start codon, return the empty string.
        if (startIndex == -1) return "";
        // Finds the index position of the first stop codon appearing after the start codon that was found.
        int endIndex = dna.toLowerCase().indexOf(stopCodon.toLowerCase(), startIndex + startCodon.length());
        // If there is no such stop codon, return the empty string.
        if (endIndex == -1) return "";
        // If the length of the substring between the start codon and stop codon is a multiple of 3
        // then return the substring that starts with startCodon and ends with stopCodon.
        if ((endIndex - startIndex) % 3 != 0) return "";
        
        return dna.substring(startIndex, endIndex + stopCodon.length());
    }
    
    public void testSimpleGene(){
        String[] dnas = {"ATTGGAATTTGG",
                         "AATGGAATTTGG",
                         "ATTGTAATTTGG",
                         "ATGGGAATTAAG",
                         "ATGGGAATTTAA",
                         "atgggaatttaa"};
        for (String dna: dnas){
            System.out.println("Dna: "+ dna);
            System.out.println("Gene: "+ findSimpleGene(dna, "ATG", "TAA"));
        }
    }
    
    public static void main(String[] args){
        Part2 p2 = new Part2();
        p2.testSimpleGene();
    }
}
