
public class Strategy {

	Hand hand = new Hand();
	private int standOnValue = 17;
	private boolean standOnSoft;
	
	public boolean isBust(){
		return hand.isBust();
	}
	
	public int getScore(){
		return hand.getScore();
	}
	
	public void play(){
		while (getScore() < standOnValue){
			hand.playNextCard();
		}
	}
	
	public void setStandOnSoft(boolean flag){
		standOnSoft = flag;
	}
	
	public void setStandOnValue(int value){
		standOnValue = value;
	}
	
	public boolean getStandOnSoft(){
		return standOnSoft;
	}
	
	public int getStandOnValue(){
		return standOnValue;
	}
	
}
