import java.util.Scanner

public class BinConvert
{
   public static void main( String args[] )
   {
   
      int binary;
      int decimal = 0;
      int n;
      int multiplier = 1;
      
      System.out.println( "Please enter a binary integer" );
      binary = Math.abs(input.nextInt());
      
      n = binary;
      
      while ( n > 0)
      {
         decimal = decimal + ((n%10)*multiplier);
         n = n/10;
         multiplier = multiplier*2
      }
      
      System.out.println( "The decimal equivalent of " + binary + " is " + decimal + ".");
   }
}