#Name: Mahmoud Elnagar
#Date: 09/09/2021
#Pset3:  Hangman game
# -----------------------------------
import random

WORDLIST_FILENAME = "words.txt"

def loadWords():
    """
    Returns a list of valid words. Words are strings of lowercase letters.
    
    Depending on the size of the word list, this function may
    take a while to finish.
    """
    print("Loading word list from file...")
    # inFile: file
    inFile = open(WORDLIST_FILENAME, 'r')
    # line: string
    line = inFile.readline()
    # wordlist: list of strings
    wordlist = line.split()
    print("  ", len(wordlist), "words loaded.")
    return wordlist

def chooseWord(wordlist):
    """
    wordlist (list): list of words (strings)

    Returns a word from wordlist at random
    """
    return random.choice(wordlist)
# -----------------------------------
def isWordGuessed(secretWord, lettersGuessed):
    '''
    secretWord: string, the word the user is guessing
    lettersGuessed: list, what letters have been guessed so far
    returns: boolean, True if all the letters of secretWord are in lettersGuessed;
      False otherwise
    '''
    for ch in secretWord:
      if ch not in lettersGuessed: return False
    return True

def getGuessedWord(secretWord, lettersGuessed):
    '''
    secretWord: string, the word the user is guessing
    lettersGuessed: list, what letters have been guessed so far
    returns: string, comprised of letters and underscores that represents
      what letters in secretWord have been guessed so far.
    '''
    gword = len(secretWord)*["_ "]
    for i in range(len(secretWord)):
      if secretWord[i] in lettersGuessed:
        gword[i] = secretWord[i]
    return "".join(gword)

def getAvailableLetters(lettersGuessed):
    '''
    lettersGuessed: list, what letters have been guessed so far
    returns: string, comprised of letters that represents what letters have not
      yet been guessed.
    '''
    result = ""
    for char in "abcdefghijklmnopqrstuvwxyz":
      if char not in lettersGuessed: result+= char
    return result

def hangman(secretWord):

    '''
    secretWord: string, the secret word to guess.

    Starts up an interactive game of Hangman.

    * At the start of the game, let the user know how many 
      letters the secretWord contains.

    * Ask the user to supply one guess (i.e. letter) per round.

    * The user should receive feedback immediately after each guess 
      about whether their guess appears in the computers word.

    * After each round, you should also display to the user the 
      partially guessed word so far, as well as letters that the 
      user has not yet guessed.

    Follows the other limitations detailed in the problem write-up.
    '''
    print("Welcome to the game, Hangman!")
    print("I am thinking of a word that is",len(secretWord),"letters long.")
    print("-------------")
    gNum = 8
    lettersGuessed = []
    while gNum > 0:
      print("You have",gNum,"guesses left")
      print("Available Letters:",getAvailableLetters(lettersGuessed))
      char = input("Please guess a letter: ").lower()
      if char in lettersGuessed:
        print("Oops! You've already guessed that letter:",getGuessedWord(secretWord,lettersGuessed))
        print("-------------")
        continue
      lettersGuessed.append(char)
      if char in secretWord:
        print("Good guess:",getGuessedWord(secretWord,lettersGuessed))
        if isWordGuessed(secretWord,lettersGuessed):
          print("-------------")
          print("Congratulations, you won!")
          return
      else:
        print("Oops! That letter is not in my word:",getGuessedWord(secretWord,lettersGuessed))
        gNum -= 1
      print("-------------")
    print("Sorry, you ran out of guesses. The word was", (secretWord+"."))
#==============================================
wordlist = loadWords()
secretWord = chooseWord(wordlist).lower()
hangman(secretWord)
