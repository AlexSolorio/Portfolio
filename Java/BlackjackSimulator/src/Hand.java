
public class Hand {

	private int size;
	private int score;
	private boolean soft;
	
	public int getScore(){
		return score;
	}
	
	public int getSize(){
		return size;
	}
	
	public void initialize(){
		score = 0;
		Card a = Deck.nextCard();
		updateScore(a);
		Card b = Deck.nextCard();
		updateScore(b);
		size = 2;
	}
	
	public boolean isBust(){
		if (score > 21)
			return true;
		return false;
	}
	
	public boolean isSoft(){
		return soft;
	}
	
	public void playNextCard(){
		Card c = Deck.nextCard();
		updateScore(c);
	}
	
	public void updateScore(Card c){
		size++;
		switch (c){
			case TWO:
				score += 2;
				if (score > 21 && soft){
					soft = false;
					score -= 10;
				}
				break;
			case THREE:
				score += 3;
				if (score > 21 && soft){
					soft = false;
					score -= 10;
				}
				break;
			case FOUR:
				score += 4;
				if (score > 21 && soft){
					soft = false;
					score -= 10;
				}
				break;
			case FIVE:
				score += 5;
				if (score > 21 && soft){
					soft = false;
					score -= 10;
				}
				break;
			case SIX:
				score += 6;
				if (score > 21 && soft){
					soft = false;
					score -= 10;
				}
				break;
			case SEVEN:
				score += 7;
				if (score > 21 && soft){
					soft = false;
					score -= 10;
				}
				break;
			case EIGHT:
				score += 8;
				if (score > 21 && soft){
					soft = false;
					score -= 10;
				}
				break;
			case NINE:
				score += 9;
				if (score > 21 && soft){
					soft = false;
					score -= 10;
				}
				break;
			case TEN:
				score += 10;
				if (score > 21 && soft){
					soft = false;
					score -= 10;
				}
				break;
			case JACK:
				score += 10;
				if (score > 21 && soft){
					soft = false;
					score -= 10;
				}
				break;
			case QUEEN:
				score += 10;
				if (score > 21 && soft){
					soft = false;
					score -= 10;
				}
				break;
			case KING:
				score += 10;
				if (score > 21 && soft){
					soft = false;
					score -= 10;
				}
				break;
			case ACE:
				score += 11;
				if (score > 21){
					score -= 10;
				}
				else{
					soft = true;
				}
				break;
			default:
				System.out.println("ERROR: incorrect card name.");
				break;
		}
	}
}
