import java.util.Scanner;



public class Palindrome
{
   public static void main( String args[] )
   {
      Scanner input = new Scanner( System.in );

      int palindrome;
      
      System.out.println( "Please enter a 5 digit integer");
     
      palindrome = Math.abs(input.nextInt()); // Taking absolute value of input
      
      while ( palindrome<10000 || palindrome>99999 ) // Testing for 5 digit integer
      {
         System.out.println( "Please enter a 5 digit integer");
         palindrome = Math.abs(input.nextInt());
      } // Endwhile
      
      int n = palindrome;
      
      int digitFive = n%10;
      n = n/10;
      int digitFour = n%10;
      n = n/10;
      int digitThree = n%10;
      n = n/10;
      int digitTwo = n%10;
      n = n/10;
      int digitOne = n%10;

      if ( (digitFive == digitOne) && (digitFour == digitTwo) )
         System.out.println( palindrome + " is a palindrome!" );
      else
         System.out.println( palindrome + "is not a palindrome!");
      
      
      
      
   }
}