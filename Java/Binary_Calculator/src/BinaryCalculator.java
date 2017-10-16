//A program that can add, subtract, multiply, or divide binary numbers using only logical operations
//Created for CompOrg in order to fully understand how computer hardware performs arithmetic operations on binary numbers
//see README for details

import java.util.Scanner;
import java.util.ArrayList;
import static java.lang.Math.*;


public class BinaryCalculator
{
	
	public static void main(String[] args)
	{		
		System.out.println("Welcome to the Binary Calculator. Please enter an arithmetic operation of the type: \n<binary operand 1> <operator> <binary operand 2>");
		System.out.println("For example, 0001 + 0001, 01110111 / 00111101");
		System.out.println("__________________________________________");
		Scanner in = new Scanner(System.in);
		while(true){
			String arg1;
			if(in.hasNext()){
				arg1 = in.next();
				if(arg1.equalsIgnoreCase("QUIT")){
					break;
				}
			}
			else {
				break;
			}
			String operator = "?", arg2 = "0";
			if(in.hasNext()){
				operator = in.next();
			}
			if(in.hasNext()){
				arg2 = in.next();
			}
			if(arg1.length() != arg2.length()){
				System.err.println("ERROR: '" + arg1 + "' and '" + arg2 + "' are not the same length.");
				continue;
			}
			if(arg1.length() > 64 || arg2.length() > 64)
			{
				System.err.println("ERROR: operands must be 64 bits or fewer.");
				continue;
			}
			
			ArrayList<Boolean> firstNumber = createArrayListFromString(arg1);
			long firstNumberLong = createLongFromArrayList(firstNumber);
			
			ArrayList<Boolean> secondNumber = createArrayListFromString(arg2);
			long secondNumberLong = createLongFromArrayList(secondNumber);
			
			if((operator.equals("/")) && secondNumberLong == 0)
			{
				System.out.println(firstNumberLong + " " + operator + " " + secondNumberLong + " = ERROR");
				continue;
			}
			
			ArrayList<Boolean> answer = null;
			ArrayList<Boolean> remainder = null;
			
			switch (operator){
			case "+":
				answer = binaryAdd(firstNumber,secondNumber);
				break;
			case "-":
				answer = binarySub(firstNumber,secondNumber);
				break;
			case "*":
				answer = binaryMult(firstNumber,secondNumber);
				break;
			case "/":
				Object[] objects = binaryDiv(firstNumber,secondNumber);
				answer = (ArrayList<Boolean>) objects[0];
				remainder = (ArrayList<Boolean>) objects[1];
				break;
			default:
				System.out.println("Invalid operator");
				System.exit(0);
			}
			
			System.out.print(firstNumberLong + " " + operator + " " + secondNumberLong + " = " + createLongFromArrayList(answer));
			if (operator.equals("/"))
				System.out.println("R" + createLongFromArrayList(remainder));
			else
				System.out.println();
		}
		in.close();
	}
	
	static ArrayList<Boolean> createArrayListFromString(String str)
	{
		int length = str.length();
		ArrayList<Boolean> list = new ArrayList<Boolean>();
		for (int i=length-1; i>=0; i--)
		{
			char c = str.charAt(i);
			if (c == '0')
			{
				list.add(false);
			}
			else if (c == '1')
			{
				list.add(true);
			}
			else
			{
				System.out.println("Numbers must be in binary.");
				System.exit(0);
			}
		}
		return list;
	}
	
	static String createStringFromArrayList(ArrayList<Boolean> list)
	{
		int length = list.size();
		StringBuilder sb = new StringBuilder();
		for (int i=length-1; i>=0; i--)
		{
			if (list.get(i))
			{
				sb.append('1');
			}
			else
			{
				sb.append('0');
			}
		}
		return sb.toString();
	}
	
	static long createLongFromArrayList(ArrayList<Boolean> list)
	{
		long returnValue = 0;
		int length = list.size();
		for (int i=0; i<length-1; i++)
		{
			if (list.get(i))
				returnValue += pow(2,i);
		}
		if (list.get(length-1))
			returnValue -= pow(2,length-1);
		return returnValue;
	}
	
	static ArrayList<Boolean> binaryAdd(ArrayList<Boolean> firstNumber, ArrayList<Boolean> secondNumber)
	{
		ArrayList<Boolean> answer = new ArrayList<Boolean>();
		int size = firstNumber.size();
		Boolean carry = false, a = false, b = false;
		
		for (int i = 0; i < size; i++)
		{
			a = firstNumber.get(i);
			b = secondNumber.get(i);
			if((!carry && !a && b) || (!carry && a && !b) || (carry && !a && !b))
			{
				answer.add(true);
				carry = false;
			}
			else if((!carry && a && b) || (carry && !a && b) || (carry && a && !b))
			{
				answer.add(false);
				carry = true;
			}
			else if(carry && a && b)
			{
				answer.add(true);
				carry = true;
			}
			else if(!carry && !a && !b)
			{
				answer.add(false);
				carry = false;
			}
			else
				System.out.println("binary add logical error");
		}
		return answer;
	}
	
	static ArrayList<Boolean> twosComplement(ArrayList<Boolean> original)
	{
		int length = original.size();
		ArrayList<Boolean> newList = new ArrayList<Boolean>();
		
		//reverse bits
		for (int i = 0; i < length; i++)
		{
			if(original.get(i))
				newList.add(false);
			else
				newList.add(true);
		}
		
		//add 1
		ArrayList<Boolean> one = new ArrayList<Boolean>();
		one.add(true);
		for (int i = 1; i < length; i++)
		{
			one.add(false);
		}
		newList = binaryAdd(newList,one);
		return newList;
	}
	
	static ArrayList<Boolean> binarySub(ArrayList<Boolean> firstNumber,ArrayList<Boolean> secondNumber)
	{
		secondNumber = twosComplement(secondNumber);
		return binaryAdd(firstNumber,secondNumber);
	}
	
	static ArrayList<Boolean> binaryMult(ArrayList<Boolean> multiplicand, ArrayList<Boolean> multiplier)
	{
		int length = multiplicand.size();
		Boolean multiplicandIsNegative = false, multiplierIsNegative = false;

		if(multiplicand.get(length-1))
		{
			multiplicandIsNegative = true;
			multiplicand = twosComplement(multiplicand);
		}
		if(multiplier.get(length-1))
		{
			multiplierIsNegative = true;
			multiplier = twosComplement(multiplier);
		}
		
		//double the size of multiplicand
		for (int i = 0; i < length; i++)
			multiplicand.add(length, false);
		
		//initialize product
		ArrayList<Boolean> product = new ArrayList<Boolean>();
		for (int i = 0; i < (length*2); i++)
			product.add(false);
		
		for (int i = 0; i < length; i++)
		{
			if(multiplier.get(0))
				product = binaryAdd(product, multiplicand);
			
			//left shift multiplicand
			multiplicand.remove((length*2)-1);
			multiplicand.add(0, false);
			
			//right shift multiplier
			multiplier.remove(0);
			multiplier.add(false);
		}
		
		if((multiplicandIsNegative && !multiplierIsNegative) || (multiplierIsNegative && !multiplicandIsNegative))
			product = twosComplement(product);
		
		ArrayList<Boolean> halfSizeProduct = new ArrayList<Boolean>();
		for(int i = 0; i < length; i++)
		{
			halfSizeProduct.add(product.get(i));
		}
		return halfSizeProduct;
	}
	
	static Object[] binaryDiv(ArrayList<Boolean> dividend, ArrayList<Boolean> divisor)
	{
		Object[] out = new Object [2];
		int length = dividend.size();
		
		Boolean dividendIsNegative = false, divisorIsNegative = false;

		if(dividend.get(length-1))
		{
			dividendIsNegative = true;
			dividend = twosComplement(dividend);
		}
		if(divisor.get(length-1))
		{
			divisorIsNegative = true;
			divisor = twosComplement(divisor);
		}
		
		//initialize remainder with dividend and double the size, filling left half with zeros
		ArrayList<Boolean> remainder = dividend;
		for (int i = 0; i < length; i++)
			remainder.add(length, false);
		
		//initialize quotient
		ArrayList<Boolean> quotient = new ArrayList<Boolean>();
		for (int i = 0; i < length; i++)
			quotient.add(false);
		
		//double the size of divisor, filling right half with zeros
		for (int i = 0; i < length; i++)
			divisor.add(0, false);
		
		for (int i = 0; i < length+1; i++)
		{
			remainder = binarySub(remainder, divisor);
			
			//if remainder went negative
			if(remainder.get((length*2)-1))
			{
				remainder = binaryAdd(remainder, divisor);
				
				//shift quotient left, setting the new least significant bit to 0
				quotient.remove(length-1);
				quotient.add(0, false);
			}
			else
			{
				//shift quotient left, setting the new least significant bit to 1
				quotient.remove(length-1);
				quotient.add(0, true);
			}
			
			
			
			//shift the divisor right
			divisor.remove(0);
			divisor.add(false);
		}
		
		if((dividendIsNegative && !divisorIsNegative) || (divisorIsNegative && !dividendIsNegative))
			quotient = twosComplement(quotient);
		if(dividendIsNegative)
			remainder = twosComplement(remainder);
		
		ArrayList<Boolean> halfSizeRemainder = new ArrayList<Boolean>();
		for(int i = 0; i < length; i++)
		{
			halfSizeRemainder.add(remainder.get(i));
		}
		
		out[0] = quotient;
		out[1] = halfSizeRemainder;
		
		return out;
	}
}
