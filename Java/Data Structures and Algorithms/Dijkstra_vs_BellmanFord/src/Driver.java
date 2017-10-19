import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Scanner;

/**
 * CS2370 Programming Assignment 3
 *
 * @author Alex Solorio, alex.solorio@du.edu, Student # 873186312
 *
 * Compares the timings of MODIFIED-DIJKSTRA() and BELLMAN-FORD() with different sized graphs.
 *
 */

public class Driver 
{

	public static void main(String[] args) throws FileNotFoundException 
	{
		//create ArrayList of file names
		Scanner fileNames = new Scanner(new File("p3datalist.txt"));
		ArrayList<String> files = new ArrayList<>();
		while (fileNames.hasNext()){
			files.add(fileNames.next());
		}
		fileNames.close();
		
		//write first line of output to p3output.txt
		PrintWriter writer = new PrintWriter("p3output.txt");
		writer.println("\"v\",\"e\",\"Algorithm\",\"CPU Seconds\",\"File\"");
		
		//loop through all file names and calculate average time spent for each algorithm
		for (String fileName : files)
		{
			//create all needed data structures
			HashMap<String,Vertex> vertices = new HashMap<>();
			HashMap<String,Edge> edges = new HashMap<>();
			HashMap<Vertex,ArrayList<Vertex>> adjacencyList = new HashMap<>();
			Vertex s;
			
			Scanner fileScanner  = new Scanner(new File(fileName));
			
			//add start vertex to the vertices HashMap
			String startVertexName = fileScanner.next();
			s = new Vertex(startVertexName);
			vertices.put(startVertexName, s);
			adjacencyList.put(s, new ArrayList<Vertex>());
			fileScanner.nextLine();
			
			//add all other vertices to the vertices HashMap
			while(fileScanner.hasNextLine())
			{
				String vertexName = fileScanner.next();
				Vertex u = new Vertex(vertexName);
				vertices.put(vertexName, u);
				adjacencyList.put(u, new ArrayList<Vertex>());
				fileScanner.nextLine();
			}
			fileScanner.close();
			
			//add all edges to the edges HashMap, add each edge to the adjacencyList HashMap
			Scanner fileScanner2 = new Scanner(new File(fileName));
			while(fileScanner2.hasNextLine())
			{
				String line = fileScanner2.nextLine();
				Scanner lineScanner = new Scanner(line);
				String start = lineScanner.next();
				Vertex origin = vertices.get(start);
				while (lineScanner.hasNext())
				{
					String end = lineScanner.next();
					Vertex destination = vertices.get(end);
					double weight = lineScanner.nextDouble();
					edges.put(start + "," + end, new Edge(origin,destination,weight));		//could possibly map two different edges with the same key???
					adjacencyList.get(origin).add(destination);
				}
				lineScanner.close();
			}
			fileScanner2.close();
			
			int timingIterations = 10000 / vertices.size();
			if (timingIterations == 0)
				timingIterations = 1;
			
			//compute and print to file average Dijkstra time for given map
			CpuTimer dijkstraTimer = new CpuTimer();
			for (int i = 0; i < timingIterations; i++)
			{
				modifiedDijkstra(vertices,edges,adjacencyList,s);
			}
			double dijkstraTotalTime = dijkstraTimer.getElapsedCpuTime();
			double avgDijkstraTime = dijkstraTotalTime / timingIterations;
			writer.println(vertices.size() + "," + edges.size() + ",\"D\"," + avgDijkstraTime + ",\"" + fileName + "\"");
			makeOutputFile(vertices,edges,fileName,true);
			
			//compute and print to file average BellmanFord time for given map
			CpuTimer bellmanFordTimer = new CpuTimer();
			for (int i = 0; i < timingIterations; i++)
			{
				bellmanFord(vertices,edges,s);
			}
			double bellmanFordTotalTime = bellmanFordTimer.getElapsedCpuTime();
			double avgBellmanFordTime = bellmanFordTotalTime / timingIterations;
			writer.println(vertices.size() + "," + edges.size() + ",\"B\"," + avgBellmanFordTime + ",\"" + fileName + "\"");
			makeOutputFile(vertices,edges,fileName,false);
		}
		writer.close();
	}
	
	//Helper method for both algorithms
	public static void initializeSingleSource(HashMap<String,Vertex> vertices, Vertex s)
	{
		for(Map.Entry<String, Vertex> entry : vertices.entrySet())
		{
			entry.getValue().setDistance(Double.POSITIVE_INFINITY);
			entry.getValue().setPredecessor(null);
		}
		s.setDistance(0);
	}
	
	//Helper method for both algorithms
	public static boolean relax(Vertex u, Vertex v, HashMap<String,Edge> edges)
	{
		if (v.getDistance() > (u.getDistance() + weight(u,v,edges)))
		{
			v.setDistance(u.getDistance() + weight(u,v,edges));
			v.setPredecessor(u);
			return true;
		}
		return false;
	}
	
	//Returns the weight of the edge from vertex u to vertex v
	public static double weight(Vertex u, Vertex v, HashMap<String,Edge> edges)
	{
		return edges.get(u.getName() + "," + v.getName()).getWeight();
	}
	
	//Dijkstra's Algorithm
	public static void modifiedDijkstra(HashMap<String,Vertex> vertices, HashMap<String,Edge> edges, HashMap<Vertex,ArrayList<Vertex>> adjacencyList, Vertex s)
	{
		initializeSingleSource(vertices, s);
		PriorityQueue<Vertex> Q = new PriorityQueue<>();
		Q.add(s);
		while(!Q.isEmpty())
		{
			Vertex u = Q.poll();
			for (Vertex v : adjacencyList.get(u))
			{
				if (relax(u,v,edges))
				{
					if(Q.contains(v))
					{
						Q.remove(v);
					}
					Q.add(v);
				}
			}
		}
	}
	
	//Bellman Ford Algorithm
	public static boolean bellmanFord(HashMap<String,Vertex> vertices, HashMap<String,Edge> edges, Vertex s)
	{
		initializeSingleSource(vertices,s);
		for (int i = 0; i < (vertices.size()-1); i++)
		{
			for (Map.Entry<String, Edge> entry : edges.entrySet())
			{
				Vertex u = entry.getValue().getOrigin();
				Vertex v = entry.getValue().getDestination();
				relax(u,v,edges);
			}
		}
		for (Map.Entry<String, Edge> entry : edges.entrySet())
		{
			Vertex u = entry.getValue().getOrigin();
			Vertex v = entry.getValue().getDestination();
			if (v.getDistance() > (u.getDistance() + weight(u,v,edges)))
			{
				return false;
			}
		}
		return true;
	}
	
	//Creates a file containing each vertex in the map, followed by the distance of that vertex from the source 
	public static void makeOutputFile(HashMap<String,Vertex> vertices, HashMap<String,Edge> edges, String fileName, boolean isDijkstra) throws FileNotFoundException
	{
		PrintWriter writer;
		
		if (isDijkstra)
		{
			writer = new PrintWriter(fileName + ".dout");
		}
		else
		{
			writer = new PrintWriter(fileName + ".bout");
		}
		for (Map.Entry<String, Vertex> entry : vertices.entrySet())
		{
			Vertex u = entry.getValue();
			writer.println(u.getName() + "  " + u.getDistance());
		}
		writer.close();
	}
}
