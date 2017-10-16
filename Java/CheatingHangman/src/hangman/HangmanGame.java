/* 
 * Alex Solorio | Project 5 | May 26, 2016
 */

package hangman;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.*;

public class HangmanGame {

	//instance variables
	private boolean showWords;
	private int wordLength, guesses;
	private StringBuilder currentWord;
	private LinkedList<Character> guessedLetters;
	private ArrayList<String> wordsRemaining;
	private HashMap<Integer,ArrayList<String>> allWords;
	private Character letter;
	private String blankCharacter = "_";
	HashMap<String,ArrayList<String>> wordFamilies;
	
	public HangmanGame(){}
	
	//only public method
	public void play() throws FileNotFoundException{
		guessedLetters = new LinkedList<>();
		allWords = new HashMap<>(); //key = length of word
		createHashMapFromFile("dictionary.txt", allWords);
		askInitialQuestions();
		currentWord = new StringBuilder();
		for (int i = 0; i < wordLength; i++)
			currentWord.append(blankCharacter);
		while (guesses > 0){
			if (showWords){
				printAvailableWords();
			}
			printCurrentWord();
			printGuessedLetters();
			printGuessesLeft();
			askForGuess();
			createWordFamilies();
			keepMostCommonWordFamily();
			checkIfWon();
			System.out.println();
		}
		youLose();
	}
	
	//********************HELPER METHODS********************//
	
	//initially scans all the words from the dictionary into a hashmap with word length as a key and an arraylist of strings as the values
	private void createHashMapFromFile(String fileName, HashMap<Integer,ArrayList<String>> map) throws FileNotFoundException{
		Scanner info = new Scanner(new FileInputStream(fileName));
		
		//iterates through each line
		while (info.hasNext()){
			String word = info.nextLine().toUpperCase();
			
			//if there are no previous words with the same length as the word, it adds a new key value pair
			if (map.get(word.length()) == null){
				ArrayList<String> list = new ArrayList<>();
				list.add(word);
				map.put(word.length(), list);
			}
			
			//otherwise it just adds the word to the correct key value pair
			else{
				map.get(word.length()).add(word);
			}
		}
		info.close();
	}
	
	//helper method for getting keyboard input after asking a question
	private String getResponse(){
		@SuppressWarnings("resource")
		Scanner keyboard = new Scanner(System.in);
		String answer = keyboard.nextLine();
		return answer;
	}
	
	//asks the first three questions to set up the game
	private void askInitialQuestions(){
		System.out.println("Welcome to Alex's Hangman Game.");
		System.out.println("What word length (number of letters) would you like to play with?");
		wordLength = Integer.parseInt(getResponse());
		while(!allWords.containsKey(wordLength)){
			System.out.println("There must exist a word with this word length in the dictionary. Try again: ");
			wordLength = Integer.parseInt(getResponse());
		}
		
		System.out.println("How many guesses would you like? ");
		int response = Integer.parseInt(getResponse());
		while (response < 1 || response > 14){
			System.out.println("Guesses must be greater than 0 and less than 15. Try again: ");
			response = Integer.parseInt(getResponse());
		}
		guesses = response;
		
		System.out.println("Do you want to show available words? ");
		String answer = getResponse();
		while (!(answer.equalsIgnoreCase("yes") || (answer.equalsIgnoreCase("no")))){
			System.out.println("Must type either \"yes\" or \"no\". Do you want to show available words?");
			answer = getResponse();
		}
		if (answer.equalsIgnoreCase("yes"))
			showWords = true;
		else
			showWords = false;
		wordsRemaining = allWords.get(wordLength);
	}
	
	//prints the words that remain as options
	private void printAvailableWords(){
		StringBuilder sb = new StringBuilder();
		sb.append("Available words: [");
		for (String s : wordsRemaining){
			sb.append(s.toUpperCase() + ", ");
		}
		sb.delete(sb.length() - 2, sb.length());
		sb.append("]");
		System.out.println(sb.toString());
	}
	
	//prints the current word with dashes depicting unknown letters
	private void printCurrentWord(){
		System.out.println(currentWord.toString());
	}
	
	//prints the alphabetized list of letters already guessed
	private void printGuessedLetters(){
		if (guessedLetters.size() != 0)
			System.out.println("Letters already guessed: " + guessedLetters.toString());
		else
			System.out.println();
	}
	
	//asks for the user's next letter guess, ensuring that the guess is one character long, in the alphabet, and not already guessed
	private void askForGuess(){
		System.out.println("Enter your guess: ");
		String answer = getResponse();
		letter = answer.toUpperCase().charAt(0);
		
		while (	answer.length() != 1 || 
				guessedLetters.toString().contains(letter.toString()) || 
				!isInAlphabet(letter)){
			if (answer.length() != 1)
				System.out.println("Guess must be a single letter. Try again: ");
			else if (guessedLetters.toString().contains(letter.toString()))
				System.out.println("Letter may not be already guessed. Try again: ");
			else if (!isInAlphabet(letter))
				System.out.println("Guessed letter must be in the alphabet. Try again: ");
			answer = getResponse();
			letter = answer.toUpperCase().charAt(0);
		}
		insertGuessedLetter(letter);
	}
	
	//checks if a given character is in the a-z alphabet
	private boolean isInAlphabet(Character letter){
		if (letter == 'A' || letter == 'B' || letter == 'C' || letter == 'D' || letter == 'E' || letter == 'F' || 
			letter == 'G' || letter == 'H' || letter == 'I' || letter == 'J' || letter == 'K' || letter == 'L' || 
			letter == 'M' || letter == 'N' || letter == 'O' || letter == 'P' || letter == 'Q' || letter == 'R' || 
			letter == 'S' || letter == 'T' || letter == 'U' || letter == 'V' || letter == 'W' || letter == 'X' || 
			letter == 'Y' || letter == 'Z'){
			return true;
		}
		return false;
	}
	
	//makes a HashMap of word families depending on how many occurrences of the guessed letter they have and where those occurrences are
	private void createWordFamilies(){
		wordFamilies = new HashMap<>();
		for (String str : wordsRemaining){
			StringBuilder builder = new StringBuilder();
			
			//goes through each letter of each word. if the guessed letter is at a position, it adds a 1 to that position of a new StringBuilder.
			//otherwise, it adds a 0. 
			for (int i = 0; i < wordLength; i++){
				if (str.charAt(i) == letter.charValue())
					builder.append("1");
				else
					builder.append("0");
			}
			String family = builder.toString();
			
			//creates word families depending on the new String made up of 0's and 1's
			if (wordFamilies.containsKey(family)){
				wordFamilies.get(family).add(str);
			}
			else{
				wordFamilies.put(family, new ArrayList<String>());
				wordFamilies.get(family).add(str);
			}
		}
	}
	
	//sorts through all the word families and keeps the one with the most elements. 
	//Subtracts a guess if the largest family was the one with no occurrences of the guessed letter
	private void keepMostCommonWordFamily(){
		Collection<ArrayList<String>> collection = wordFamilies.values();
		ArrayList<String> familyToKeep = new ArrayList<>();
		for (ArrayList<String> family : collection){
			if (family.size() > familyToKeep.size())
				familyToKeep = family;
		}
		wordsRemaining = familyToKeep;
		
		boolean lostAGuess = true;
		for (String str : wordsRemaining){
			if (str.indexOf(letter) != -1)
				lostAGuess = false;
		}
		if (lostAGuess){
			System.out.println("Sorry, there are no " + letter + "'s in the word.");
			guesses--;
		}
		else{
			appendCurrentWord();
		}
	}
	
	//changes the current word by changing '-'s to the guessed letter, where applicable
	public void appendCurrentWord(){
		String sampleWord = wordsRemaining.get(0);
		for (int i = 0; i < currentWord.length(); i++){
			if (sampleWord.charAt(i) == letter)
				currentWord.setCharAt(i, letter);
		}
	}
	
	//prints the number of guesses remaining
	public void printGuessesLeft(){
		if (guesses > 1)
			System.out.println("You have " + guesses + " guesses remaining.");
		else
			System.out.println("You have " + guesses + " guess remaining.");
	}
	
	//checks for any dashes in the current word. if none are found, the user won.
	public void checkIfWon() throws FileNotFoundException{
		if (currentWord.indexOf(blankCharacter) == -1){
			System.out.println("Congratulations, you won!");
			System.out.println("The word was " + currentWord);
			playAgainQuestion();
		}
	}
	
	//asks user if they want to play again. if yes, it calls the play method. if no, it exits the program.
	private void playAgainQuestion() throws FileNotFoundException{
		System.out.println("Would you like to play again? (yes/no)");
		Scanner keyboard = new Scanner (System.in);
		String answer = keyboard.nextLine().toLowerCase();
		if (answer.equals("yes")){
			play();
		}
		else if(answer.equals("no")){
			System.out.println("Thanks for playing!");
			System.exit(0);
		}
		else{
			System.out.println("Invalid answer. Please try again.");
			playAgainQuestion();
		}
	}
	
	//tells the user they lost and shows what the "correct" word was
	private void youLose() throws FileNotFoundException{
		System.out.println("You lose! No Guesses Remaining.");
		System.out.println("The word was " + wordsRemaining.get(0) + ".");
		playAgainQuestion();
	}
	
	//adds the guessed letter to the linked list of guessed letters, ensuring it is placed in alphabetical order
	private void insertGuessedLetter(Character c){
		if (guessedLetters.size() == 0){
			guessedLetters.add(c);
			return;
		}			
		for (int i = 0; i < guessedLetters.size(); i++){
			if (c.compareTo(guessedLetters.get(i)) < 0){
				guessedLetters.add(i, c);
				return;
			}
			if (i == guessedLetters.size()-1){
				guessedLetters.add(c);
				return;
			}
		}
	}
}
