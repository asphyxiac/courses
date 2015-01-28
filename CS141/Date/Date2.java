public class Date2
{
   private int month; // 1-12
   private int day; // 1-31 based on month
   private int year; // any year

   // constructor: call checkMonth to confirm proper value for month;
   // call checkDay to confirm proper value for day
   public Date2( int theMonth, int theDay, int theYear )
   {
      month = checkMonth( theMonth ); // validate month
      year = checkYear( theYear ); // could validate year
      day = checkDay( theDay ); // validate day

      System.out.printf(
         "Date object constructor for date %s\n", this );
   } // end Date constructor

   //increment the day by one
   public void nextDay()
   {
      int dayPlus = ++day;
      int monthPlus = month;
      int yearPlus = year;
      int testDay = checkDay( dayPlus );

      if( testDay == 1 )
      {
         ++monthPlus;
         int testMonth = checkMonth( monthPlus );
         if( testMonth == 1 )
         {
            ++yearPlus;
            year = yearPlus;
            month = testMonth;
            day = testDay;
         }
         else
         {
            month = monthPlus;
            day = testDay;
         }
      }
      else
         day = dayPlus;
   }// end method nextDay

   // utility method to confirm proper month value
   private int checkMonth( int testMonth )
   {
      if ( testMonth > 0 && testMonth <= 12 ) // validate month
         return testMonth;
      else // month is invalid
      {
         System.out.printf(
            "Invalid month (%d) set to 1.\n", testMonth );
         return 1; // maintain object in consistent state
      } // end else
   } // end method checkMonth

   // utility method to confirm proper day value based on month and year
   private int checkDay( int testDay )
   {
      int[] daysPerMonth =
         { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

      // check if day in range for month
      if ( testDay > 0 && testDay <= daysPerMonth[ month ] )
         return testDay;

      // check for leap year
      if ( month == 2 && testDay == 29 && ( year % 400 == 0 ||
           ( year % 4 == 0 && year % 100 != 0 ) ) )
         return testDay;

      System.out.printf( "Invalid day (%d) set to 1.\n", testDay );
      return 1;  // maintain object in consistent state
   } // end method checkDay

   // utility method to confirm proper year value
   private int checkYear( int testYear )
   {
      if ( testYear >= 1582 ) // validate year
         return testYear;
      else // year is invalid
      {
         System.out.printf(
            "Invalid year (%d) set to 1582, the year the Gregorian Calendar was established.", testYear );
         return 1582; // maintain object in consistent state
      } // end else
   } // end method checkYear

   // utility method to confirm proper day value based on month and year

   // return a String of the form month/day/year
   public String toString()
   {
      return String.format( "%d/%d/%d", month, day, year );
   } // end method toString

} // end class Date