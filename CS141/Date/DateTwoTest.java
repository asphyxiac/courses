public class DateTwoTest
{
   public static void main( String args[] )
   {
      Date2 date = new Date2( 12, 20, 2011 );
      System.out.println( "Incrementing day 20 times" );

      for( int i = 0; i < 20; i++ )
      {
         date.nextDay();
         System.out.printf( "%s%s\n", "The current date is: ", date.toString() );
      }

   }
}

/*
Sample Output:

Date object constructor for date 12/20/2011
Incrementing day 20 times
The current date is: 12/21/2011
The current date is: 12/22/2011
The current date is: 12/23/2011
The current date is: 12/24/2011
The current date is: 12/25/2011
The current date is: 12/26/2011
The current date is: 12/27/2011
The current date is: 12/28/2011
The current date is: 12/29/2011
The current date is: 12/30/2011
The current date is: 12/31/2011
Invalid day (32) set to 1.
Invalid month (13) set to 1.
The current date is: 1/1/2012
The current date is: 1/2/2012
The current date is: 1/3/2012
The current date is: 1/4/2012
The current date is: 1/5/2012
The current date is: 1/6/2012
The current date is: 1/7/2012
The current date is: 1/8/2012
The current date is: 1/9/2012
Press any key to continue . . .

*/