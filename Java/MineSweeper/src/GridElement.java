//A class to represent one button in the Minesweeper grid

import javax.swing.JButton;

public class GridElement {
	
	private int xPosition, yPosition;
	private JButton button;
	private boolean isMine;
	
	GridElement(int x, int y)
	{
		xPosition = x;
		yPosition = y;
		button = new JButton();
		isMine = false;
	}
	
	public int getxPosition() {
		return xPosition;
	}

	public void setxPosition(int xPosition) {
		this.xPosition = xPosition;
	}

	public int getyPosition() {
		return yPosition;
	}

	public void setyPosition(int yPosition) {
		this.yPosition = yPosition;
	}

	public JButton getButton() {
		return button;
	}

	public void setButton(JButton button) {
		this.button = button;
	}

	public boolean isMine() {
		return isMine;
	}

	public void setMine(boolean isMine) {
		this.isMine = isMine;
	}
}
