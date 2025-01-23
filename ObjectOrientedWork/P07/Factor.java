import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.math.BigInteger;

public class Factor {
    public static List<PrimeFactors> solutions = new ArrayList<>();
    public static String[] bigints = null;
    public static int numThreads = 1;

    public static void main(String[] args) {
        if(args.length < 2 || args[0].equals("-h")) usage();

        try {
            numThreads = Integer.parseInt(args[0]);
            bigints = Arrays.copyOfRange(args, 1, args.length);
        } catch(NumberFormatException e) {
            numThreads = 1;
            bigints = args;
      }

    // creating threads
        Thread[] threads = new Thread[numThreads];
        int totalBigInts = bigints.length;
        int chunkSize = totalBigInts / numThreads;
        int remainder = totalBigInts % numThreads;

        for (int t = 0; t < numThreads; t++) {
              int start = t * chunkSize + Math.min(t, remainder);
              int end = start + chunkSize + (t < remainder ? 1 : 0);

            

//  final variables for lambda
            final int threadNum = t;
            final int startIndex = start;
            final int endIndex = end;

            threads[t] = new Thread(() -> solve(threadNum, startIndex, endIndex));
            threads[t].start();
      }

     // finishing threads
        for (Thread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                System.err.println("Thread interrupted: " + e.getMessage());
            }
        }

     // print out the solutions 
        for (PrimeFactors solution : solutions)
            System.out.println(solution);
    }
   public static void solve(int threadNumber, int firstIndex, int lastIndexPlusOne) {
   System.out.println("Thread " + threadNumber + " solving " 
                         + firstIndex + "-" + (lastIndexPlusOne-1));
        for (int i = firstIndex; i < lastIndexPlusOne; ++i) {
            try {
                PrimeFactors pf = new PrimeFactors(new BigInteger(bigints[i]));
                synchronized (solutions) {
                    solutions.add(pf);
                }
            } catch (Exception e) {
                System.err.println("Failed " + bigints[i] + ": " + e);
            }
        }
    }

    public static void usage() {
      System.err.println("usage: java Factor [#threads] bigint [bigint...]");
       System.exit(-1);
    }
}

