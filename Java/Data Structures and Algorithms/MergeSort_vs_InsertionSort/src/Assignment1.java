import java.util.ArrayList;
import java.util.Collections;
import java.util.Scanner;
import java.io.*;
/**
 * CS2370 Programming Assignment 1
 *
 * @author Alex Solorio, alex.solorio@du.edu, Student # 873186312
 *
 * Compares the timings of Merge Sort and Insertion Sort with different
 * values of n, and also with random, ascending, and descending values.
 *
 */

public class Assignment1 {

	public static void main(String[] args) throws FileNotFoundException{
		
		//create ArrayList of file names
		Scanner fileNames = new Scanner(new File("p1alldata.txt"));
		ArrayList<String> files = new ArrayList<>();
		while (fileNames.hasNext()){
			files.add(fileNames.next());
		}
		fileNames.close();
		
		PrintWriter writer = new PrintWriter("output.txt");
		writer.println("\"n\",\"Sort\",\"CPU Seconds\"");

		final int RANDOM_ORDER = 0;
		final int ASCENDING_ORDER = 1;
		final int DESCENDING_ORDER = 2;
		
		//loop to test timings for random-ordered, ascending-ordered, and descending-ordered ArrayLists
		for (int m = RANDOM_ORDER; m <= DESCENDING_ORDER; m++){
			
			//Main loop for reading file names and computing average sorting times
			for (String str : files){
				ArrayList<String> input = readArrayFromFile(new File(str));
				int n = input.size();
				ArrayList<String> output = new ArrayList<>();
				for (int i = 0; i < n; i++)
					output.add("");
				int timingIterations = (256 * 1024) / n;
				
				//compute average time to copy array of size n
				CpuTimer timer = new CpuTimer();
				for (int i = 0; i < timingIterations; i++){
					makeCopy(input,output);
				}
				double avgCopyTime = timer.getElapsedCpuTime() / (double)timingIterations;
				
				//puts input in ascending or descending order depending on the value of m
				if (m == ASCENDING_ORDER){
					insertionSort(input,input);
				}
				else if (m == DESCENDING_ORDER){
					insertionSort(input,input);
					Collections.reverse(input);
				}
				
				//compute average time to sort array of size n using InsertionSort
				CpuTimer insertTimer = new CpuTimer();
				for (int i = 0; i < timingIterations; i++){
					insertionSort(input,output);
				}
				double avgInsertionSortTime = (insertTimer.getElapsedCpuTime() / (double)timingIterations) - avgCopyTime;
				if (!isSorted(output)){
					System.out.println("Insertion sort error: Output not sorted properly");
					System.exit(0);
				}
				
				//print CSV text to output depending on value of m
				if (m == RANDOM_ORDER)
					writer.println(n + ",\"IR\"," + avgInsertionSortTime);
				else if (m == ASCENDING_ORDER)
					writer.println(n + ",\"IA\"," + avgInsertionSortTime);
				else if (m == DESCENDING_ORDER)
					writer.println(n + ",\"ID\"," + avgInsertionSortTime);
				
				
				//compute average time to sort array of size n using MergeSort
				CpuTimer mergeTimer = new CpuTimer();
				for (int i = 0; i < timingIterations; i++){
					makeCopy(input,output);
					mergeSort(output,0,output.size()-1);
				}
				double avgMergeSortTime = (mergeTimer.getElapsedCpuTime() / (double)timingIterations) - avgCopyTime;
				if (!isSorted(output)){
					System.out.println("Merge sort error: Output not sorted properly");
					System.exit(0);
				}
				
				//print CSV text to output depending on value of m
				if (m == RANDOM_ORDER)
					writer.println(n + ",\"MR\"," + avgMergeSortTime);
				else if (m == ASCENDING_ORDER)
					writer.println(n + ",\"MA\"," + avgMergeSortTime);
				else if (m == DESCENDING_ORDER)
					writer.println(n + ",\"MD\"," + avgMergeSortTime);
			}
		}
		writer.close();
	}
  
	//returns an ArrayList<String> object containing each line of the file passed in
	public static ArrayList<String> readArrayFromFile(File file) throws FileNotFoundException{
		ArrayList<String> list = new ArrayList<>();
		Scanner input = new Scanner(file);
		while (input.hasNext()){
			list.add(input.next());
		}
		input.close();
		return list;
	}
	
	//copies array input to array output
	public static void makeCopy(ArrayList<String> input, ArrayList<String> output){
		for(int i = 0; i < input.size(); i++){
			output.set(i, input.get(i));
		}
	}
	  
	//checks if an array is sorted in ascending order
	public static boolean isSorted(ArrayList<String> input){
		for (int i = 1; i < input.size(); i++){
			if (input.get(i).compareTo(input.get(i-1)) < 0){
				return false;
			}
		}
		return true;
	}
	  
	//sorts an ArrayList using the insertionSort algorithm
	public static void insertionSort(ArrayList<String> input, ArrayList<String> output){
		makeCopy(input,output);
		for(int j = 1; j < output.size(); j++){
			String key = output.get(j);
			
			//Insert output[j] into the sorted sequence output[1..j-1]
			int i = j-1;
			while ((i >= 0) && (output.get(i).compareTo(key) > 0)){
				output.set(i+1, output.get(i));
				i = i-1;
			}
			output.set(i+1, key);
		}
	}
	  
	//sorts an ArrayList using the mergeSort algorithm
	public static void mergeSort(ArrayList<String> output, int start, int end){
		if (start < end){
			int mid = (start + end) / 2;
            mergeSort(output, start, mid);
            mergeSort(output, mid + 1, end);
            merge(output, start, mid, end);
		}
	}

	//helper method for the mergeSort algorithm
	private static void merge(ArrayList<String> A, int start, int mid, int end) {
		int n1 = mid - start + 1;
		int n2 = end - mid;
		ArrayList<String> L = new ArrayList<>();
		ArrayList<String> R = new ArrayList<>();
		for (int i = 0; i < n1; i++){
			L.add(A.get(start + i));
		}
		for (int j = 0; j < n2; j++){
			R.add(A.get(mid + 1 + j));
		}
		int i = 0;
		int j = 0;
		for (int k = start; k <= end; k++){
			if (i == L.size()){
				while (j != R.size()){
					A.set(k, R.get(j));
					j++;
					k++;
				}
				break;
			}
			else if (j == R.size()){
				while (i != L.size()){
					A.set(k, L.get(i));
					i++;
					k++;
				}
				break;
			}
			else if (L.get(i).compareTo(R.get(j)) < 0){
				A.set(k, L.get(i));
				i++;
			}
			else{
				A.set(k, R.get(j));
				j++;
			}
		}
	}	
}
