/*
 * This class has several tests used to ensure the DateTime class is running properly.
 * It does not cover all possible test cases, but covers a few (including tests for leap days).
 */

import static org.junit.Assert.*;
import org.junit.Test;

public class DateTimeTester {

	public DateTime dateTime;
	
	@Test
	public void toStringTest()
	{
		dateTime = new DateTime(5, 6, 7, 8, 9, 10);
		assertEquals("5-06-07 08:09:10", dateTime.toString());
	}
	
	@Test
	public void copyConstructorTest()
	{
		dateTime = new DateTime(5, 6, 7, 8, 9, 10);
		DateTime dateTimeCopy = new DateTime(dateTime);
		assertEquals(dateTime.getYear(), dateTimeCopy.getYear());
		assertEquals(dateTime.getMonth(), dateTimeCopy.getMonth());
		assertEquals(dateTime.getDay(), dateTimeCopy.getDay());
		assertEquals(dateTime.getHour(), dateTimeCopy.getHour());
		assertEquals(dateTime.getMinute(), dateTimeCopy.getMinute());
		assertEquals(dateTime.getSecond(), dateTimeCopy.getSecond());
	}

	@Test
	public void threeArgConstructorTest()
	{
		dateTime = new DateTime(1, 2, 3);
		assertEquals(1, dateTime.getYear());
		assertEquals(2, dateTime.getMonth());
		assertEquals(3, dateTime.getDay());
		assertEquals(0, dateTime.getHour());
		assertEquals(0, dateTime.getMinute());
		assertEquals(0, dateTime.getSecond());
	}
	
	@Test
	public void fiveArgConstructorTest()
	{
		dateTime = new DateTime(1, 2, 3, 4, 5);
		assertEquals(1, dateTime.getYear());
		assertEquals(2, dateTime.getMonth());
		assertEquals(3, dateTime.getDay());
		assertEquals(4, dateTime.getHour());
		assertEquals(5, dateTime.getMinute());
		assertEquals(0, dateTime.getSecond());

	}
	
	@Test
	public void sixArgConstructorTest()
	{
		dateTime = new DateTime(5, 6, 7, 8, 9, 10);
		assertEquals(5, dateTime.getYear());
		assertEquals(6, dateTime.getMonth());
		assertEquals(7, dateTime.getDay());
		assertEquals(8, dateTime.getHour());
		assertEquals(9, dateTime.getMinute());
		assertEquals(10, dateTime.getSecond());
	}
	
	@Test
	public void addOneYearTest()
	{
		dateTime = new DateTime(2019, 1, 1);
		DateTime dt2 = dateTime.addYear(1);
		DateTime exp = new DateTime(2020, 1, 1);
		assertEquals(exp.toString(), dt2.toString());
	}
	
	@Test
	public void addYearFromLeapDayTest()
	{
		dateTime = new DateTime(2012, 2, 29);
		DateTime dateTime2 = dateTime.addYear(1);
		DateTime exp = new DateTime(2013, 3, 1);
		assertEquals(exp.toString(), dateTime2.toString());
	}

	@Test
	public void subtractYearFromLeapDayTest()
	{
		dateTime = new DateTime(2012, 2, 29);
		DateTime dt2 = dateTime.addYear(-1);
		DateTime exp = new DateTime(2011, 2, 28);
		assertEquals(exp.toString(), dt2.toString());
	}
	
	@Test
	public void addMonthTest()
	{
		dateTime = new DateTime(2017, 2, 16);
		DateTime dt2 = dateTime.addMonth(4);
		DateTime exp = new DateTime(2017, 6, 16);
		assertEquals(exp.toString(), dt2.toString());
	}
	
	
}


