// Alex Solorio | Project 3 | 4/18/16 
//This class creates a forest object that holds an array of vegetation plots with values 
//DRY, FIRE, or VEGETATION. It has methods that determine if fire will spread in the array.

package project3;
import net.datastructures.*;

public class Forest {

	private int width;
	private int height;
	private int[][] array;
	private final int DRY = 0;
	private final int VEGETATION = 1;
	private final int FIRE = 2;
	
	public Forest(int width, int height, double probabilityOfVegetation)
	{
		this.width = width;
		this.height = height;
		array = new int[width][height];
		
		for (int i = 0; i < width; i++){
			for (int j = 0; j < height; j++){
				double random = Math.random();
				if (random <= probabilityOfVegetation)
					array[i][j] = VEGETATION;
				else
					array[i][j] = DRY;
			}
		}
	}
	
	public String toString()
	{
		StringBuilder sb = new StringBuilder();
		sb.append("(");
		for (int j = 0; j < height; j++){
			for (int i = 0; i < width; i++){
				sb.append(array[i][j]);
			}
			if (!(j == height-1))
				sb.append(", ");
		}
		sb.append(")");
		return sb.toString();
	}
	
	//tests if fire will spread from the top of the forest to the bottom using the depth first search algorithm (stack data structure)
	public boolean depthFirstSearch()
	{
		LinkedStack<CellIndices> cellsToExplore = new LinkedStack<>();
		for (int x = 0; x < width; x++){
			if (array[x][0] == VEGETATION){
				cellsToExplore.push(new CellIndices(x,0));				//goes through top row and pushes all vegetation cells onto stack
			}
		}
		CellIndices currentCell;
		while (!(cellsToExplore.isEmpty())){
			currentCell = cellsToExplore.pop();
			array[currentCell.x][currentCell.y] = FIRE;
			if (currentCell.y == height-1){
				return true;											//if the current cell is in the bottom row, return true (fire has spread)
			}
			if (currentCell.y < height-1 && (array[currentCell.x][currentCell.y+1] == VEGETATION)){
				cellsToExplore.push(new CellIndices(currentCell.x,currentCell.y+1));	//if the cell above the currentCell is vegetation, push it onto the stack
			}
			if (currentCell.x > 0 && (array[currentCell.x-1][currentCell.y] == VEGETATION)){
				cellsToExplore.push(new CellIndices(currentCell.x-1,currentCell.y));	//if the cell left of the currentCell is vegetation, push it onto the stack
			}
			if (currentCell.x < width-1 && (array[currentCell.x+1][currentCell.y] == VEGETATION)){
				cellsToExplore.push(new CellIndices(currentCell.x+1,currentCell.y));	//if the cell right of the currentCell is vegetation, push it onto the stack
			}
			if (currentCell.y > 0 && (array[currentCell.x][currentCell.y-1] == VEGETATION)){
				cellsToExplore.push(new CellIndices(currentCell.x,currentCell.y-1));	//if the cell below the currentCell is vegetation, push it onto the stack
			}
		}
		return false;		//if it doesn't make it to the bottom, return false
	}
	
	//tests if fire will spread from the top of the forest to the bottom using the breadth first search algorithm (queue data structure)
	public boolean breadthFirstSearch()
	{
		LinkedQueue<CellIndices> cellsToExplore = new LinkedQueue<>();
		for (int x = 0; x < width; x++){
			if (array[x][0] == VEGETATION){
				cellsToExplore.enqueue(new CellIndices(x,0));				//goes through top row and enqueues all vegetation cells onto queue
			}
		}
		CellIndices currentCell;
		while (!(cellsToExplore.isEmpty())){
			
			currentCell = cellsToExplore.dequeue();
			array[currentCell.x][currentCell.y] = FIRE;
			if (currentCell.y == height-1){
				return true;											//if the current cell is in the bottom row, return true (fire has spread)
			}
			if (currentCell.y < height-1 && (array[currentCell.x][currentCell.y+1] == VEGETATION)){
				cellsToExplore.enqueue(new CellIndices(currentCell.x,currentCell.y+1));	//if the cell above the currentCell is vegetation, enqueue it onto the queue
			}
			if (currentCell.x > 0 && (array[currentCell.x-1][currentCell.y] == VEGETATION)){
				cellsToExplore.enqueue(new CellIndices(currentCell.x-1,currentCell.y));	//if the cell left of the currentCell is vegetation, enqueue it onto the queue
			}
			if (currentCell.x < width-1 && (array[currentCell.x+1][currentCell.y] == VEGETATION)){
				cellsToExplore.enqueue(new CellIndices(currentCell.x+1,currentCell.y));	//if the cell right of the currentCell is vegetation, enqueue it onto the queue
			}
			if (currentCell.y > 0 && (array[currentCell.x][currentCell.y-1] == VEGETATION)){
				cellsToExplore.enqueue(new CellIndices(currentCell.x,currentCell.y-1));	//if the cell below the currentCell is vegetation, enqueue it onto the queue
			}
		}
		return false;		//if it doesn't make it to the bottom, return false
	}
	
	private class CellIndices{  //sub-class that stores indices of a given cell
		
		private int x;
		private int y;
		
		public CellIndices(int x, int y){
			this.x = x;
			this.y = y;
		}
		
		public String toString(){
			return "("+x+","+y+")";
		}
	}	
}
