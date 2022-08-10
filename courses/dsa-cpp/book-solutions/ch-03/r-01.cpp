/**
* Mahmoud Elnagar - 10/08/2022
*/
#include<string>
#include<stdexcept>

class GameEntry {
public:
    GameEntry(const std::string& n = "", int s = 0) : name(n), score(s) { };

    std::string getName() const { return name; }
    int getScore() const { return score; };
private:
    std::string name;				
    int score;					
};

class Scores {				
public:
    Scores(int maxEnt = 10) {
        maxEntries = maxEnt;			
        entries = new GameEntry[maxEntries];	
        numEntries = 0;				
    }

    ~Scores() { delete[] entries; }

    unsigned count(const std::string& name) {
        unsigned num = 0;
        for (int i = 0; i < numEntries; ++i) {
            if (entries[i].getName() == name) num++;
        }
        return num;
    }

    void add(const GameEntry& e) {
        int newScore = e.getScore();

        if ((numEntries == maxEntries) && (newScore <= entries[maxEntries - 1].getScore())) {
            return;
        }

        if (count(e.getName()) == (maxEntries / 2)) return;
        
        if (numEntries < maxEntries) numEntries++;

        int i = numEntries - 2;
        while (i >= 0 && newScore > entries[i].getScore()) {
            entries[i + 1] = entries[i];
            i--;
        }
        entries[i + 1] = e;
    }

    GameEntry remove(int i) throw(std::out_of_range) {
        if ((i < 0) || (i >= numEntries))		
            throw std::out_of_range("Invalid index");
        GameEntry e = entries[i];			
        for (int j = i + 1; j < numEntries; j++)
            entries[j - 1] = entries[j];		
        numEntries--;				
        return e;
    }

private:
    int maxEntries;				
    int numEntries;				
    GameEntry* entries;			
};