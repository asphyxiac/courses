// Tests class Date.
public class DateTest
{
   public static void main( String args[] )
   {
      Date date = new Date( 9, 15, 2012 );
      Date date2 = new Date( "October", 31, 2012 );
      Date date3 = new Date( 60, 2000 );

      System.out.println( "Now printing dates for the first date object.");
      System.out.println( "The date in MM/DD/YYYY format is: " + date.printDDMMYY() );
      System.out.println( "The date in DDD/YYYY format is: " + date.printDDDYY() );
      System.out.println( "The date in Month DD, YYYY format is: "+ date.printMonDDYY() );

      System.out.println( "Now printing dates for the second date object.");
      System.out.println( "The date in MM/DD/YYYY format is: " + date2.printDDMMYY() );
      System.out.println( "The date in DDD/YYYY format is: " + date2.printDDDYY() );
      System.out.println( "The date in Month DD, YYYY format is: " + date2.printMonDDYY() );

      System.out.println( "Now printing dates for the last date object.");
      System.out.println( "The date in MM/DD/YYYY format is: " + date3.printDDMMYY() );
      System.out.println( "The date in DDD/YYYY format is: " + date3.printDDDYY() );
      System.out.println( "The date in Month DD, YYYY format is: " + date3.printMonDDYY() );
	
   }
}
/*
Sample output:
Now printing dates for the first date object.
The date in MM/DD/YYYY format is: 9/15/2012
The date in DDD/YYYY format is: 258/2012
The date in Month DD, YYYY format is: September 15, 2012
Now printing dates for the second date object.
The date in MM/DD/YYYY format is: 10/31/2012
The date in DDD/YYYY format is: 304/2012
The date in Month DD, YYYY format is: October 31, 2012
Now printing dates for the last date object.
The date in MM/DD/YYYY format is: 2/29/2000
The date in DDD/YYYY format is: 60/2000
The date in Month DD, YYYY format is: February 29, 2000
*/