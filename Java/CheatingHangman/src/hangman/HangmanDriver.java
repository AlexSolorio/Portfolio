/* Alex Solorio | Project 5 | May 26, 2016
 * Creates and plays an instance of HangmanGame
 */
package hangman;

import java.io.FileNotFoundException;

public class HangmanDriver {

	public static void main(String[] args) throws FileNotFoundException {

		HangmanGame game = new HangmanGame();
		game.play();
		
	}

}
