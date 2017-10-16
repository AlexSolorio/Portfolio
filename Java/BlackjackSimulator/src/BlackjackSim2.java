/**
 * This class tests the probabilities of a player and dealer playing against each other.
 * It tests all combinations of player and dealer strategies.
 */
public class BlackjackSim2 {

	public static void main(String[] args) {
		
		Hand playerHand = new Hand();
		Hand dealerHand = new Hand();
		Strategy playerStrategy = new Strategy();
		Strategy dealerStrategy = new Strategy();

		System.out.println("Player SOS = true, Dealer SOS = true:");
		System.out.println();
		playerStrategy.setStandOnSoft(true);
		dealerStrategy.setStandOnSoft(true);
		printProbsFor(playerStrategy, dealerStrategy, playerHand, dealerHand);
		
		System.out.println();
		System.out.println("Player SOS = true, Dealer SOS = false:");
		System.out.println();
		playerStrategy.setStandOnSoft(true);
		dealerStrategy.setStandOnSoft(false);
		printProbsFor(playerStrategy, dealerStrategy, playerHand, dealerHand);
		
		System.out.println();
		System.out.println("Player SOS = false, Dealer SOS = true:");
		System.out.println();
		playerStrategy.setStandOnSoft(false);
		dealerStrategy.setStandOnSoft(true);
		printProbsFor(playerStrategy, dealerStrategy, playerHand, dealerHand);
		
		System.out.println();
		System.out.println("Player SOS = false, Dealer SOS = false:");
		System.out.println();
		playerStrategy.setStandOnSoft(false);
		dealerStrategy.setStandOnSoft(false);
		printProbsFor(playerStrategy, dealerStrategy, playerHand, dealerHand);
	}
	public static void printProbsFor(Strategy playerStrategy, Strategy dealerStrategy, Hand playerHand, Hand dealerHand){
		int count = 0, sumDealerWin = 0, pSOV, dSOV;
		double probabilityDealerWin;
		int numSimulations = 5000;
		for (pSOV = 16; pSOV<=18; pSOV++){
			for (dSOV = 16; dSOV<=18; dSOV++){
				playerStrategy.setStandOnValue(pSOV);
				dealerStrategy.setStandOnValue(dSOV);
				sumDealerWin = 0;
				for (count = 1; count<=numSimulations; count++){
					playerHand.initialize();
					dealerHand.initialize();
					runPlayerSimulation(playerStrategy, playerHand);
					runDealerSimulation(dealerStrategy, dealerHand);
					if (playerHand.isBust() || ((dealerHand.getScore()>playerHand.getScore()) && !dealerHand.isBust())){
					sumDealerWin++;
					}
				}
				probabilityDealerWin = (double) sumDealerWin / (double) numSimulations;
				System.out.println("player SOV: " + 
				playerStrategy.getStandOnValue() +  ". dealer SOV: " + 
				dealerStrategy.getStandOnValue() + ". Probability of Dealer win: " + probabilityDealerWin);
			}
		}
	}
	
	public static void runPlayerSimulation(Strategy playerStrategy, Hand playerHand) {
		while (!playerHand.isBust() && (playerHand.getScore() < playerStrategy.getStandOnValue() || 
				playerStrategy.getStandOnSoft() && playerHand.getScore() == playerStrategy.getStandOnValue())){
			playerHand.playNextCard();
		}
	}
	public static void runDealerSimulation(Strategy dealerStrategy, Hand dealerHand) {
		while (!dealerHand.isBust() && (dealerHand.getScore() < dealerStrategy.getStandOnValue() || 
				dealerStrategy.getStandOnSoft() && dealerHand.getScore() == dealerStrategy.getStandOnValue())){
			dealerHand.playNextCard();
		}
	}
}
