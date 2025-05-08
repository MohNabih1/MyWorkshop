# PLOT 1 Grouped Bar Chart Trend Analysis

"""
INSTRUCTIONS TO RUN THIS FILE AND GENERATE THE BAR CHART IMAGE

This python script generates a bar chart showing the number of highly rated movies by genre combination
across three time periods (1991–2000, 2001–2010, and 2011–2020). The output image will be saved 
as 'genre_comparison_chart_plot1.png' in the same folder as this script.

Follow these steps to run this file successfully:

1. Make sure Python is installed on your Windows system.

2. Install the required Python libraries: matplotlib and numpy
   - Open Command Prompt
   - Navigate to the folder where this file is saved
   - Then run the following command to install the libraries:
      - python -m pip install matplotlib numpy

3. Run this script using Python:
   - In the same terminal window, type:
     - python plot1.py

4. The script will:
   - Open a pop-up window displaying the bar chart
   - Save the chart as 'genre_comparison_chart_plot1.png' in the current directory

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

#final 4/15/25
# Import necessary libraries
import matplotlib.pyplot as plt  # For creating plots
import numpy as np               # For numerical operations (like creating x-axis positions)

# Data setup
periods = ['1991-2000', '2001-2010', '2011-2020']  # Time periods for comparison
genres = ['Action;Thriller', 'Adventure;Drama', 'Comedy;Romance']  # Genre combinations
values = {
    '1991-2000': [55, 56, 215],    # Number of highly rated movies in each genre during 1991–2000
    '2001-2010': [76, 141, 400],   # ...for 2001–2010
    '2011-2020': [208, 343, 590]   # ...for 2011–2020
}

# Create x-axis positions for the bars
x = np.arange(len(genres))  # [0, 1, 2]
width = 0.25  # Width of each individual bar

# Set up the figure size and resolution
plt.figure(figsize=(10, 6), dpi=200)

# Plot bars for each period at adjusted x positions
plt.bar(x - width, values['1991-2000'], width=width, label='1991-2000')    # Bars shifted left
plt.bar(x, values['2001-2010'], width=width, label='2001-2010')            # Bars centered
plt.bar(x + width, values['2011-2020'], width=width, label='2011-2020')    # Bars shifted right

# Add labels and title
plt.xlabel('Genre Combinations', fontsize=12)  # x-axis label
plt.ylabel('Number of Highly Rated Movies', fontsize=12)  # y-axis label
plt.title('Highly Rated Movies by Genre Combination and Period', fontsize=14)  # Chart title

# Set x-axis tick labels 
plt.xticks(x, genres, rotation=15)

# Add legend to differentiate the periods
plt.legend()

# Add horizontal gridlines
plt.grid(axis='y', linestyle='--', alpha=0.7)

# Automatically adjust layout 
plt.tight_layout()

# Save chart to a file genre_comparison_chart_plot1.png
plt.savefig('genre_comparison_chart_plot1.png', dpi=300)

# Display the chart
plt.show()
