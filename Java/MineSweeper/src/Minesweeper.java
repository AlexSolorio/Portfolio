//My java version of the classic Minesweeper game

import java.awt.*;
import javax.swing.JFrame;

public class Minesweeper{
	
	private final int GRID_WIDTH = 25;
	private final int GRID_HEIGHT = 25;
	private final int FRAME_WIDTH = 700;
	private final int FRAME_HEIGHT = 800;
	private final int NUMBER_OF_MINES = 75;
	
	JFrame frame;
	Grid minesweeperGrid;
	
	public Minesweeper(){
		minesweeperGrid = new Grid(GRID_WIDTH, GRID_HEIGHT, this);
		frame = new JFrame("Minesweeper");
		frame.setSize(FRAME_WIDTH, FRAME_HEIGHT);
		frame.setLayout(new BorderLayout());
		frame.add(minesweeperGrid.reset, BorderLayout.NORTH);		
		
		frame.add(minesweeperGrid.buttonGrid, BorderLayout.CENTER);
		minesweeperGrid.createRandomMines(NUMBER_OF_MINES);
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
	
	public static void main(String[] args) {
		new Minesweeper();
	}
	
}
