/*
 * A class to represent a specific date and time.
 * There are methods for adding/subtracting any number of years, months, days, hours, minutes, or seconds 
 * to a given DateTime object.
 * This project was an assignment for my Introduction to Java Coding course at the University of Denver.
 */
public class DateTime {
	
	private int year;
	private int month;
	private int day;
	private int hour;
	private int minute;
	private int second;
	
	public DateTime (DateTime that){
		year = that.getYear();
		month = that.getMonth();
		day = that.getDay();
		hour = that.getHour();
		minute = that.getMinute();
		second = that.getSecond();
	}
	
	public DateTime(int year, int month, int day){
		this(year, month, day, 0, 0, 0);
	}
	
	public DateTime(int year, int month, int day, int hour, int minute){
		this(year, month, day, hour, minute, 0);
	}
	
	public DateTime(int year, int month, int day, int hour, int minute, int second){
		if (year < 0){
			throw new IllegalArgumentException (String.format("invalid year: %d", year));
		}
		if (month < 1 || month > 12){
			throw new IllegalArgumentException (String.format("invalid month: %d", month));
		}
		else if (month == 4 || month == 9 || month == 6 || month == 11){
			if (day < 1 || day > 30){
				throw new IllegalArgumentException (String.format("invalid day: %d", day));
			}
		}
		else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12){
			if (day < 1 || day > 31){
				throw new IllegalArgumentException (String.format("invalid day: %d", day));
			}
		}
		else if (month == 2){
			if (isLeapYear(year) && (day < 1 || day > 29)){
				throw new IllegalArgumentException (String.format("invalid day: %d", day));
			}
			else if (!isLeapYear(year) && (day < 1 || day > 28))
				throw new IllegalArgumentException (String.format("invalid day: %d", day));
		}
		if (hour < 0 || hour > 23){
			throw new IllegalArgumentException (String.format("invalid hour: %d", hour));
		}
		if (minute < 0 || minute > 59){
			throw new IllegalArgumentException (String.format("invalid minute: %d", minute));
		}
		if (second < 0 || second > 59){
			throw new IllegalArgumentException (String.format("invalid second: %d", second));
		}
		this.year=year;
		this.month=month;
		this.day=day;
		this.hour=hour;
		this.minute=minute;
		this.second=second;
	}
	public int getYear(){
		return year;
	}
	public int getMonth(){
		return month;
	}
	public int getDay(){
		return day;
	}
	public int getHour(){
		return hour;
	}
	public int getMinute(){
		return minute;
	}
	public int getSecond(){
		return second;
	}
	
	public DateTime addYear(int n){
		return addMonth(12*n);
	}
	public DateTime addMonth(int n){
		int numOfSecondsToAdd = 0, numSecondsInDay = 24*60*60;
		int month = this.month;
		int year = this.year;
		if(n>0){
			for (int count = 1; count <= n; count++){
				
				if (month == 4 || month == 9 || month == 6 || month == 11)
					numOfSecondsToAdd += 30*numSecondsInDay;
				else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
					numOfSecondsToAdd += 31*numSecondsInDay;
				else if (month == 2){
					if(isLeapYear(year))
						numOfSecondsToAdd += 29*numSecondsInDay;
					else
						numOfSecondsToAdd += 28*numSecondsInDay;
				}
				month++;
				if (month == 13){
					year++;
					month = 1;
				}
			}
		}
		else if(n<0){
			for (int count = 1; count <= -n; count++){
				
				if (month == 4 || month == 9 || month == 6 || month == 11)
					numOfSecondsToAdd -= 30*numSecondsInDay;
				else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
					numOfSecondsToAdd -= 31*numSecondsInDay;
				else if (month == 2){
					if(isLeapYear(year))
						numOfSecondsToAdd -= 29*numSecondsInDay;
					else
						numOfSecondsToAdd -= 28*numSecondsInDay;
				}
				month--;
				if(month == 0){
					year--;
					month = 12;
				}
			}
		}
		return addSecond(numOfSecondsToAdd);
	}
	public DateTime addDay(int n){
		return addSecond(n*86400);
	}
	public DateTime addHour(int n){
		return addSecond(n*3600);
	}
	public DateTime addMinute(int n){
		return addSecond(n*60);
	}
	public DateTime addSecond(int n){
		
		int year = this.getYear();
		int month = this.getMonth();
		int day = this.getDay();
		int hour = this.getHour();
		int minute = this.getMinute();
		int second = this.getSecond();
		if (n>0){
			for (int count = 1; count <=n; count++){
				if (second == 59){
					minute++;
					second = 0;
					if (minute == 60){
						hour++;
						minute = 0;
						if (hour == 24){
							day++;
							hour = 0;
							if ((month == 4 || month == 9 || month == 6 || month == 11) && day == 31){
								month++;
								day = 1;
							}
							else if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day == 32){
								month++;
								day = 1;
								if (month == 13){
									year++;
									month = 1;
								}
							}
							else if(month == 2){
								if(isLeapYear(year) && day == 30){
									month++;
									day = 1;
								}
								else{
									if (!(isLeapYear(year)) && day == 29){
										month++;
										day = 1;
									}
								}
							}
						}
					}
				}
				else{
					second++;
				}
			}
		}
		else if (n<0){
			for (int count = 1; count <= -n; count++){
				if (second == 0){
					minute--;
					second = 59;
					if (minute == -1){
						hour--;
						minute = 59;
						if (hour == -1){
							day--;
							hour = 23;
							if (day == 0){
								if(month == 5 || month == 10 || month == 7 || month == 12){
									month--;
									day = 30;
								}
								else if(month == 2 || month == 4 || month == 6 || month == 8 || month == 9 || month == 11 || month == 1){
									month--;
									day = 31;
									if(month == 0){
										year--;
										month = 12;
									}
								}
								else if(month == 3){
									if(isLeapYear(year)){
										month--;
										day = 29;
									}
									else{
										month--;
										day = 28;
									}
								}
							}	
						}
					}
				}
				else{
					second--;
				}
			}
		}
		DateTime newDateTime = new DateTime(year, month, day, hour, minute, second);
		return newDateTime;
	}
	
	public int compareTo(DateTime that){
		if (this.getYear() < that.getYear()){
			return -1;
		}
		else if (this.getYear() > that.getYear()){
			return 1;
		}
		else{
			if (this.getMonth() < that.getMonth()){
				return -1;
			}
			else if (this.getMonth() > that.getMonth()){
				return 1;
			}
			else{
				if (this.getDay() < that.getDay()){
					return -1;
				}
				else if (this.getDay() > that.getDay()){
					return 1;
				}
				else{
					if (this.getHour() < that.getHour()){
						return -1;
					}
					else if (this.getHour() > that.getHour()){
						return 1;
					}
					else{
						if (this.getMinute() < that.getMinute()){
							return -1;
						}
						else if (this.getMinute() > that.getMinute()){
							return 1;
						}
						else{
							if (this.getSecond() < that.getSecond()){
								return -1;
							}
							else if (this.getSecond() > that.getSecond()){
								return 1;
							}
							else{
								return 0;
							}
						}
					}
				}
			}
		}
	}
	
	public boolean equals(DateTime that){
		if (that == null){
			return false;
		}
		else if (this.getYear() == that.getYear() && this.getMonth() == that.getMonth() && 
			this.getDay() == that.getDay() && this.getHour() == that.getHour() &&
			this.getMinute() == that.getMinute() && this.getSecond() == that.getSecond()){
			return true;
		}
		else{
			return false;
		}
	}
	
	public String toString(){
		return String.format("%d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
	}
	
	private static boolean isLeapYear(int year){
		
		if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))
			return true;
		return false;
	}
}
