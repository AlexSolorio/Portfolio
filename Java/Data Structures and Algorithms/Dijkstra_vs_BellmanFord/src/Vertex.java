//Representation of a Vertex in a map

public class Vertex implements Comparable<Vertex>{

	private String name;
	private double distance;
	private Vertex predecessor;
	
	public Vertex(String name){
		this.name = name;
	}
	
	public String toString(){
		return name;
	}
	
	//getters and setters:
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public double getDistance() {
		return distance;
	}
	public void setDistance(double distance) {
		this.distance = distance;
	}
	public Vertex getPredecessor() {
		return predecessor;
	}
	public void setPredecessor(Vertex predecessor) {
		this.predecessor = predecessor;
	}

	public int compareTo(Vertex v) {
		return 0;
	}
	
}
