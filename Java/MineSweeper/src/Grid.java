//A class to represent the 2D grid of buttons that may or may not contain "mines"

import java.util.Random;
import java.awt.*;
import java.awt.event.*;
import javax.swing.JButton;
import javax.swing.JOptionPane;
import java.util.Stack;

public class Grid implements ActionListener{
	
	GridElement[][] grid;
	int[][] neighborCounts;
	Container buttonGrid;
	int width, height;
	int numMines;
	Font boldFont;
	JButton reset;
	Minesweeper driver;
	
	public Grid(int width, int height, Minesweeper driver)
	{
		grid = new GridElement[width][height];
		neighborCounts = new int[width][height];
		buttonGrid = new Container();
		this.width = width;
		this.height = height;
		boldFont = new Font("Arial",Font.BOLD,14);
		reset = new JButton("Reset");
		reset.addActionListener(this);
		this.driver = driver;
		
		//initialize grid element array
		for (int i = 0; i < width; i++)
			for (int j = 0; j < width; j++)
				grid[i][j] = new GridElement(i, j);
		
		//Set up button grid
		buttonGrid.setLayout(new GridLayout(width,height));
		for (int a = 0; a < width; a++){
			for (int b = 0; b < height; b++){
				grid[a][b].setButton(new JButton());
				grid[a][b].getButton().addActionListener(this);
				grid[a][b].getButton().setBackground(Color.lightGray);
				grid[a][b].getButton().setOpaque(true);
				grid[a][b].getButton().setFont(boldFont);
				buttonGrid.add(grid[a][b].getButton());
			}
		}
	}
	
	public void createRandomMines(int numberOfMines){
		
		numMines = numberOfMines;
		
		//initializes entire grid to be free of mines
		for (GridElement[] elementRow : grid)
			for (GridElement element : elementRow)
				element.setMine(false);
		
		//reset grid counts, initializes numberOfMines randomly-placed mines
		neighborCounts = new int[width][height];
		Random random = new Random();
		int i = 0;
		while(i < numberOfMines){
			int xChoice = random.nextInt(width);
			int yChoice = random.nextInt(height);
			if(!grid[xChoice][yChoice].isMine()){
				grid[xChoice][yChoice].setMine(true);
				i++;
			}
		}
		
		//initialize neighbor counts
		for(int x = 0; x < neighborCounts.length; x++){
			for(int y = 0; y < neighborCounts[0].length; y++){
				if(!grid[x][y].isMine()){
					int neighborCount = 0;
					
					//up left
					if(x > 0 && y > 0 && grid[x-1][y-1].isMine()){
						neighborCount++;
					}
					//up
					if(y > 0 && grid[x][y-1].isMine()){
						neighborCount++;
					}
					//up right
					if(x < width-1 && y > 0 && grid[x+1][y-1].isMine()){
						neighborCount++;
					}
					//left
					if(x > 0 && grid[x-1][y].isMine()){
						neighborCount++;
					}
					//right
					if(x < width-1 && grid[x+1][y].isMine()){
						neighborCount++;
					}
					//down left
					if(x > 0 && y < height-1 && grid[x-1][y+1].isMine()){
						neighborCount++;
					}
					//down
					if(y < height-1 && grid[x][y+1].isMine()){
						neighborCount++;
					}
					//down right
					if(x < width-1 && y < height-1 && grid[x+1][y+1].isMine()){
						neighborCount++;
					}
					neighborCounts[x][y] = neighborCount;
				}
			}
		}
	}
	
	//shows the value of each button in the grid to the player, with mines clearly distinguished as red X's
	public void lostGame(){
		for(int x = 0; x < width; x++){
			for(int y = 0; y < height; y++){
				JButton currentButton = grid[x][y].getButton();
				if(currentButton.isEnabled()){
					if(!grid[x][y].isMine()){
						currentButton.setText(neighborCounts[x][y] + "");
						if(currentButton.getText().equals("0")){
							currentButton.setText("");
							currentButton.setBackground(Color.WHITE);
						}
						currentButton.setOpaque(true);
						currentButton.setEnabled(false);
					}
					else{
						currentButton.setText("X");
						currentButton.setBackground(Color.RED);
						currentButton.setOpaque(true);
						currentButton.setEnabled(false);
					}
				}
			}
		}
	}
	
	//called when a button with zero mine neighbors is clicked
	//sets of a chain reaction of clearing all other "zero" mines next to it
	public void clearZeros(GridElement original){
		Stack<GridElement> zerosToClear = new Stack<>();
		zerosToClear.push(original);
		
		while(!zerosToClear.isEmpty()){
			GridElement temp = zerosToClear.pop();
			int x = temp.getxPosition();
			int y = temp.getyPosition();
			
			//up left
			if (x > 0 && y > 0){
				JButton upLeftButton = grid[x-1][y-1].getButton();
				upLeftButton.setText(neighborCounts[x-1][y-1] + "");
				handleButton(upLeftButton);
				if(neighborCounts[x-1][y-1] == 0 && upLeftButton.isEnabled()){
					zerosToClear.push(grid[x-1][y-1]);
				}
				upLeftButton.setEnabled(false);
			}
			//up
			if (y > 0){
				JButton upButton = grid[x][y-1].getButton();
				upButton.setText(neighborCounts[x][y-1] + "");
				handleButton(upButton);
				if(neighborCounts[x][y-1] == 0 && upButton.isEnabled()){
					zerosToClear.push(grid[x][y-1]);
				}
				upButton.setEnabled(false);
			}
			//up right
			if (x < width-1 && y > 0){
				JButton upRightButton = grid[x+1][y-1].getButton();
				upRightButton.setText(neighborCounts[x+1][y-1] + "");
				handleButton(upRightButton);
				if(neighborCounts[x+1][y-1] == 0 && upRightButton.isEnabled()){
					zerosToClear.push(grid[x+1][y-1]);
				}
				upRightButton.setEnabled(false);
			}
			//left
			if (x > 0){
				JButton leftButton = grid[x-1][y].getButton();
				leftButton.setText(neighborCounts[x-1][y] + "");
				handleButton(leftButton);
				if(neighborCounts[x-1][y] == 0 && leftButton.isEnabled()){
					zerosToClear.push(grid[x-1][y]);
				}
				leftButton.setEnabled(false);
			}
			//right
			if (x < width-1){
				JButton rightButton = grid[x+1][y].getButton();
				rightButton.setText(neighborCounts[x+1][y] + "");
				handleButton(rightButton);
				if(neighborCounts[x+1][y] == 0 && rightButton.isEnabled()){
					zerosToClear.push(grid[x+1][y]);
				}
				rightButton.setEnabled(false);
			}
			//down left
			if (x > 0 && y < height-1){
				JButton downLeftButton = grid[x-1][y+1].getButton();
				downLeftButton.setText(neighborCounts[x-1][y+1] + "");
				handleButton(downLeftButton);
				if(neighborCounts[x-1][y+1] == 0 && downLeftButton.isEnabled()){
					zerosToClear.push(grid[x-1][y+1]);
				}
				downLeftButton.setEnabled(false);
			}
			//down
			if (y < height-1){
				JButton downButton = grid[x][y+1].getButton();
				downButton.setText(neighborCounts[x][y+1] + "");
				handleButton(downButton);
				if(neighborCounts[x][y+1] == 0 && downButton.isEnabled()){
					zerosToClear.push(grid[x][y+1]);
				}
				downButton.setEnabled(false);
			}
			//down right
			if (x < width-1 && y < height-1){
				JButton downRightButton = grid[x+1][y+1].getButton();
				downRightButton.setText(neighborCounts[x+1][y+1] + "");
				handleButton(downRightButton);
				if(neighborCounts[x+1][y+1] == 0 && downRightButton.isEnabled()){
					zerosToClear.push(grid[x+1][y+1]);
				}
				downRightButton.setEnabled(false);
			}
		}
	}
	
	//helper method for clearZeros()
	private void handleButton(JButton button)
	{
		if(button.getText().equals("0")){
			button.setText("");
			button.setBackground(Color.WHITE);
			button.setOpaque(true);
		} else{
			button.setBackground(Color.lightGray);
			button.setOpaque(true);
		}
	}
	
	public void actionPerformed(ActionEvent event) {
		
		//resets the game
		if(event.getSource().equals(reset)){
			for(int x = 0; x < width; x++){
				for(int y = 0; y < height; y++){
					JButton button = grid[x][y].getButton();
					button.setEnabled(true);
					button.setText("");
					button.setBackground(Color.lightGray);
					button.setOpaque(false);
				}
			}
			createRandomMines(numMines);
		}
		else{
			//checks which button was clicked in grid
			for(int x = 0; x < width; x++){
				for(int y = 0; y < height; y++){
					JButton button = grid[x][y].getButton();
					if(event.getSource().equals(button)){
						button.setForeground(Color.BLACK);
						if((event.getModifiers() & ActionEvent.CTRL_MASK) != 0){ //if control is held down while clicking
							button.setForeground(Color.RED);
							button.setText("?");
						}
						else if(grid[x][y].isMine()){
							button.setBackground(Color.RED);
							button.setOpaque(true);
							button.setText("X");
							lostGame();
						}
						else if(neighborCounts[x][y] == 0){
							button.setText(neighborCounts[x][y] + "");
							button.setEnabled(false);
							clearZeros(grid[x][y]);
							checkWin();
						}
						else{
							button.setBackground(Color.lightGray);
							button.setOpaque(true);
							button.setText(neighborCounts[x][y] + "");
							button.setEnabled(false);
							checkWin();
						}
					}
				}
			}
		}
	}
	
	public void checkWin(){
		boolean won = true;
		for(int x = 0; x < width; x++){
			for(int y = 0; y < height; y++){
				if(!grid[x][y].isMine() && grid[x][y].getButton().isEnabled()){
					won = false;
				}
			}
		}
		if(won){
			JOptionPane.showMessageDialog(driver.frame, "YOU WIN!!!");
		}
	}

}
