import edu.duke.URLResource;

/**
 * Week 2: StringsFirstAssignments - Part 4
 * 
 * @author (Mahmoud Elnagar) 
 * @version (18/07/2022)
 */
public class Part4 {
    
    public void printYoutubeLinks(String link){
        URLResource res = new URLResource(link);
        for (String word : res.words()) {
            int wIndex = word.toLowerCase().indexOf("youtube.com");
            if (wIndex == -1) continue;
            int startIndex = word.lastIndexOf("\"", wIndex);
            if (startIndex == -1) continue;
            int endIndex = word.indexOf("\"", wIndex);
            if (endIndex == -1) continue;
            System.out.println(word.substring(startIndex + 1, endIndex));
        }
    }
    
    public static void main(String[] args){
        Part4 p4 = new Part4();
        p4.printYoutubeLinks("https://www.dukelearntoprogram.com//course2/data/manylinks.html");
    }
}
