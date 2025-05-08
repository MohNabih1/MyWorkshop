import java.io.IOException; // For handling IO exceptions
import javax.naming.Context; // (Unused) Java naming context, not needed for Hadoop jobs - 4/9/25 discussed w/ teammates

// Necessary Hadoop packages
import org.apache.hadoop.conf.Configuration; // For setting job configuration
import org.apache.hadoop.fs.Path; // For specifying input/output file paths
import org.apache.hadoop.io.IntWritable; // Hadoop wrapper for int 
import org.apache.hadoop.io.LongWritable; // Hadoop wrapper for long 
import org.apache.hadoop.io.Text; // Hadoop wrapper for string 
import org.apache.hadoop.mapreduce.Job; // Represents MapReduce job
import org.apache.hadoop.mapreduce.Mapper; // Base class for Mapper
import org.apache.hadoop.mapreduce.Reducer; // Base class for Reducer
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat; // For file input
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat; // For file output

public class MapReduce 
{
    // Custom Mapper class to filter and count genre combinations per decade
    public static class GenreMapper extends Mapper<LongWritable, Text, Text, IntWritable> 
    {
        private final static IntWritable counterValue = new IntWritable(1); // Constant writable for count of 1
        private Text genreKey = new Text(); // Output key that stores decade and genre combination

        @Override
        public void map(LongWritable offset, Text line, Context context) throws IOException, InterruptedException 
        {
            // Split input line into fields based on semicolon
            String[] dataTokens = line.toString().split(";");

            // Make sure there are enough fields to parse
            if (dataTokens.length < 6) 
            {
                return; // skip malformed lines
            }

            // Extract and normalize relevant fields
            String titleType = dataTokens[1].trim().toLowerCase(); // "movie"
            String yearStr    = dataTokens[3].trim();              // release year
            String ratingStr  = dataTokens[4].trim();              // average rating
            String genreList  = dataTokens[5].trim().toLowerCase(); // genre list like Comedy,Romance

            // Proceed only if type is 'movie'
            if (!titleType.equals("movie"))
            {
                return; // skip if not a movie
            }

            try 
            {
                // Parse year and rating
                int year = Integer.parseInt(yearStr);
                double rating = Double.parseDouble(ratingStr);

                // Filter on rating (only include ratings >= 7.0)
                if (rating < 7.0) 
                {
                    return; // skip if rating too low
                }

                String yearRange = ""; // Variable to hold the period tag

                // Determine the correct decade bucket
                if (year >= 1991 && year <= 2000) 
                {
                    yearRange = "[1991-2000]";
                } 
                else if (year >= 2001 && year <= 2010) 
                {
                    yearRange = "[2001-2010]";
                } 
                else if (year >= 2011 && year <= 2020) 
                {
                    yearRange = "[2011-2020]";
                } 
                else 
                {
                    return; // skip if year is outside these 3 periods
                }

                // Genre matching — must contain BOTH genres in combination

                if (genreList.contains("action") && genreList.contains("thriller")) {
                    genreKey.set(yearRange + ",Action;Thriller"); // Format: [period],Genre1;Genre2
                    context.write(genreKey, counterValue); // Emit key value pair to reducer
                }

                if (genreList.contains("adventure") && genreList.contains("drama")) {
                    genreKey.set(yearRange + ",Adventure;Drama");
                    context.write(genreKey, counterValue);
                }

                if (genreList.contains("comedy") && genreList.contains("romance")) {
                    genreKey.set(yearRange + ",Comedy;Romance");
                    context.write(genreKey, counterValue);
                }

            } 
            catch (NumberFormatException e) 
            {
                // skip lines with bad numbers (year or rating not valid numbers)
            }
        }
    }

    // Reducer class to sum up counts for each key
    public static class GenreReducer extends Reducer<Text, IntWritable, Text, IntWritable> 
    {
        @Override
        public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException 
        {
            int count = 0; // running total
            for (IntWritable entry : values) 
            {
                count += entry.get(); // add up all counts
            }
            context.write(key, new IntWritable(count)); // emit final count
        }
    }

    // Main function that configures and runs MapReduce job
    public static void main(String[] args) throws Exception 
    {
        Configuration jobConf = new Configuration(); // Hadoop job configuration
        Job mapReduceJob = Job.getInstance(jobConf, "Highly Rated Genre Combinations"); // Create new job

        mapReduceJob.setJarByClass(MapReduce.class); // Set main class for the job
        mapReduceJob.setMapperClass(GenreMapper.class); // Set custom Mapper
        mapReduceJob.setReducerClass(GenreReducer.class); // Set custom Reducer

        mapReduceJob.setOutputKeyClass(Text.class); // Output key type
        mapReduceJob.setOutputValueClass(IntWritable.class); // Output value type

        FileInputFormat.setInputPaths(mapReduceJob, new Path(args[0])); // Set input path
        FileOutputFormat.setOutputPath(mapReduceJob, new Path(args[1])); // Set output path

        boolean success = mapReduceJob.waitForCompletion(true); // Run job and wait for result
        if (success) 
        {
            System.exit(0); // Exit with 0 if job succeeded
        } else 
        {
            System.exit(1); // Exit with 1 if job failed
        }
    }
}