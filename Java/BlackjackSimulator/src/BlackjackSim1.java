/**
 * This class tests the probabilities of a player playing alone.
 * It tests all combinations of player's strategies.
 */
public class BlackjackSim1 {

	public static void main(String[] args) {
		
		Hand hand = new Hand();
		Strategy strategy = new Strategy();
		
		strategy.setStandOnSoft(true);
		printProbsFor(strategy, hand);
		strategy.setStandOnSoft(false);
		printProbsFor(strategy, hand);
		
	}
	
	public static void printProbsFor(Strategy strategy, Hand hand){
		int count, sumBust = 0;
		double probabilityOfGoingBust;
		int numSimulations = 5000;
		for (int SOV = 11; SOV<=20; SOV++){
			
			strategy.setStandOnValue(SOV);
			
			// reset the bust counter
			sumBust = 0;
			
			// run a number of simulations
			for (count = 1; count <= numSimulations; count++) {
				hand.initialize();
				runSimulation(strategy, hand);
				if (hand.isBust()){
					sumBust++;
				}
			}
			
			// print out probability
			probabilityOfGoingBust = (double) sumBust / (double) numSimulations;
			System.out.println("Stand on soft: " + strategy.getStandOnSoft() + ", stand on value: " + strategy.getStandOnValue() + 
					", and the probability of going bust with this strategy is: " + probabilityOfGoingBust);
		}
	}
	
	public static void runSimulation(Strategy strategy, Hand hand) {
		while (!hand.isBust() && (hand.getScore() < strategy.getStandOnValue() || 
				    			  strategy.getStandOnSoft() && hand.getScore() == strategy.getStandOnValue())){
			hand.playNextCard();
		}
	}
}
