//Representation of an Edge in a map

public class Edge {

	private Vertex origin;
	private Vertex destination;
	private double weight;
	
	public Edge(Vertex origin, Vertex destination, double weight){
		this.origin = origin;
		this.destination = destination;
		this.weight = weight;
	}
	
	public String toString(){
		return "(" + origin.toString() + " --> " + destination.toString() + ", " + weight + ")";
	}
	
	//getters and setters:
	public Vertex getOrigin() {
		return origin;
	}
	public void setOrigin(Vertex origin) {
		this.origin = origin;
	}
	public Vertex getDestination() {
		return destination;
	}
	public void setDestination(Vertex destination) {
		this.destination = destination;
	}
	public double getWeight() {
		return weight;
	}
	public void setWeight(double weight) { this.weight = weight;}
}
