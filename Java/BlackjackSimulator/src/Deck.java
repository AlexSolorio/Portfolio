import	java.util.Random;

/** Deck is a static class (all its method are static) used to model a
 * deck of cards.
 *
 * This implementation models a deck of cards as a never-ending
 * stream of random cards.  It is not modeled after the standard 52 card deck
 * 
 */
public class Deck
{
	private final static Random _random = new Random();
	private final static Card[] _values = Card.values();

	//This method returns a randomly selected object from the Card enumeration.
	public static Card nextCard()
	{
		return _values[_random.nextInt(_values.length)];
	}
}