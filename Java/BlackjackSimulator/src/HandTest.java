public class HandTest
{
	private static void check(String message, boolean value)
	{
		if(!value){
			System.err.println("FAILED: " + message);
		}
		else {
			System.err.println("PASSED: " + message);
		}
	}

	public static void main(String[] args)
	{
		Hand hand = new Hand();

		// test initial conditions
		check("hand.getScore() == 0", hand.getScore() == 0);
		check("hand.getSize() == 0", hand.getSize() == 0);
		check("hand.isSoft() == false", hand.isSoft() == false);
		check("hand.isBust() == false", hand.isBust() == false);

		// play a 4, 3, 2
		hand.updateScore(Card.FOUR);
		check("hand.getScore() == 4", hand.getScore() == 4);
		check("hand.getSize() == 1", hand.getSize() == 1);
		check("hand.isSoft() == false", hand.isSoft() == false);
		check("hand.isBust() == false", hand.isBust() == false);

		hand.updateScore(Card.THREE);
		check("hand.getScore() == 7", hand.getScore() == 7);
		check("hand.getSize() == 2", hand.getSize() == 2);
		check("hand.isSoft() == false", hand.isSoft() == false);
		check("hand.isBust() == false", hand.isBust() == false);

		hand.updateScore(Card.TWO);
		check("hand.getScore() == 9", hand.getScore() == 9);
		check("hand.getSize() == 3", hand.getSize() == 3);
		check("hand.isSoft() == false", hand.isSoft() == false);
		check("hand.isBust() == false", hand.isBust() == false);

		// now play an ace, should result in a soft 20
		hand.updateScore(Card.ACE);
		check("hand.getScore() == 20", hand.getScore() == 20);
		check("hand.getSize() == 4", hand.getSize() == 4);
		check("hand.isSoft() == true", hand.isSoft() == true);
		check("hand.isBust() == false", hand.isBust() == false);

		// now play a JACK, it should result in a hard 20
		hand.updateScore(Card.JACK);
		check("hand.getScore() == 20", hand.getScore() == 20);
		check("hand.getSize() == 5", hand.getSize() == 5);
		check("hand.isSoft() == false", hand.isSoft() == false);
		check("hand.isBust() == false", hand.isBust() == false);

		// now play a 2, it should result in being bust
		hand.updateScore(Card.TWO);
		check("hand.getScore() == 22", hand.getScore() == 22);
		check("hand.getSize() == 6", hand.getSize() == 6);
		check("hand.isSoft() == false", hand.isSoft() == false);
		check("hand.isBust() == true", hand.isBust() == true);

		// now create a new hand and initialize it
		hand = new Hand();
		hand.initialize();
		check("hand.getSize() == 2", hand.getSize() == 2);
		check("hand.isBust() == false", hand.isBust() == false);
	}
}