// Alex Solorio | Project 3 | 4/18/16 | FireProbability.class | This driver class uses two helper methods in order 
// to compute the highest vegetation probability that results in a probability of fire spread that's less than 50%

package project3;

public class FireProbability {

	public static final int NUM_TRIALS = 10000;
	private static int forestDimX = 20;
	private static int forestDimY = 20;
	
	//computes the probability that the forest object spreads fire  for a fixed vegetation probability
	public static double probabilityOfSpread(double probabilityOfVegetation)	
	{
		int count = 0;
		for (int i = 0; i < NUM_TRIALS; i++){
			Forest forest = new Forest(forestDimX, forestDimY,probabilityOfVegetation);
			if(forest.breadthFirstSearch()){
				count++;
			}
		}
		return count / (double)NUM_TRIALS;
	}
	
	//computes the highest value of vegetation probability that spreads fire at a probability below 0.5
	public static double highestVegetationProbability()	
	{
		double low = 0;
		double high = 1;
		double p = 0;
		double probabilityOfForestBurning;
		for (int i = 0; i < 20; i++){					//uses a binary search to find the vegetation probability
			p = (high + low) / 2.0;
			probabilityOfForestBurning = probabilityOfSpread(p);
			if (probabilityOfForestBurning > 0.5){
				high = p;
			}
			else{
				low = p;
			}
			System.out.print(".");
		}
		System.out.println();
		return p;
	}
	
	public static void main(String[] args){
		
		System.out.println("Highest vegetation probability that spreads fire less than half of the time:");
		System.out.println(highestVegetationProbability());
	}
}
