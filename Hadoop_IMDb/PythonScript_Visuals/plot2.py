# PLOT 2 Line Chart Trend Analysis

"""
INSTRUCTIONS TO RUN THIS FILE AND GENERATE THE LINE CHART IMAGE

This Python script generates a line chart showing the trend of highly rated movies 
over time across three genre combinations: Action;Thriller, Adventure;Drama, and Comedy;Romance. 
It compares the number of movies in each genre across three time periods: 
1991–2000, 2001–2010, and 2011–2020.

Follow these steps to run this file successfully:

1. Make sure Python is installed on your Windows system.

2. Install the required Python libraries: matplotlib and numpy
   - Open Command Prompt
   - Navigate to the folder where this file is saved
   - Then run the following command to install the libraries:
      - python -m pip install matplotlib numpy

3. Run this script using Python:
   - In the same terminal window, type:
     - python plot2.py

4. The script will:
   - Open a pop-up window displaying the bar chart
   - Save the chart as 'genre_trend_line_chart_plot2.png' in the current directory

If you don’t see the chart or image, double-check that:
   - You’re in the correct folder (Downloads)
   - You’ve installed matplotlib and numpy
   - You’re using regular Command Prompt

! If you plan to run this script using WSL (Ubuntu on Windows Subsystem for Linux):

1. Open your WSL terminal by typing 'wsl' in Command Prompt or opening Ubuntu directly.

2. Make sure Python 3 and pip are installed:
   - sudo apt update
   - sudo apt install python3 python3-pip

3. Install the required libraries inside WSL:
   - pip3 install matplotlib numpy

"""

# Import the matplotlib library for plotting
import matplotlib.pyplot as plt

# Define the time periods
periods = ['1991-2000', '2001-2010', '2011-2020']

# Define the number of highly rated movies per genre combination across the periods
action_thriller = [55, 76, 208]
adventure_drama = [56, 141, 343]
comedy_romance = [215, 400, 590]

# Create a new figure with specified size and resolution
plt.figure(figsize=(10, 6), dpi=150)

# Plot each genre combination as a separate line on the graph
plt.plot(periods, action_thriller, marker='o', label='Action;Thriller')     # Line for Action;Thriller
plt.plot(periods, adventure_drama, marker='s', label='Adventure;Drama')     # Line for Adventure;Drama
plt.plot(periods, comedy_romance, marker='^', label='Comedy;Romance')       # Line for Comedy;Romance

# Set the title and axis labels
plt.title('Highly Rated Movies Over Time by Genre Combination', fontsize=14)
plt.xlabel('Time Period', fontsize=12)
plt.ylabel('Number of Highly Rated Movies', fontsize=12)

# Add grid lines 
plt.grid(True, linestyle='--', alpha=0.7)

# Show legend 
plt.legend()

# Adjust layout
plt.tight_layout()

# Save the plot as an image file
plt.savefig('genre_trend_line_chart_plot2.png', dpi=300)

# display the chart
plt.show()
