# PLOT 3 Combined Pie Chart Trend Analysis

"""
INSTRUCTIONS TO RUN THIS FILE AND GENERATE THE COMBINED PIE CHART IMAGE

This Python script generates a single figure containing three pie charts, each representing
the distribution of highly rated movies across three genre combinations:
Action;Thriller, Adventure;Drama, and Comedy;Romance.

The three pie charts correspond to these time periods:
- 1991–2000
- 2001–2010
- 2011–2020

All three charts are displayed in a single pop-up window and saved together as a single image file:
- 'combined_genre_pie_charts_plot3.png'

Follow these steps to run this file successfully:

1. Make sure Python is installed on your Windows system.

2. Install the required Python library: matplotlib
   - Open Command Prompt
   - Navigate to the folder where this file is saved
   - Then run:
     - python -m pip install matplotlib

3. Run this script using Python:
   - In the same terminal window, type:
     - python plot3.py

4. The script will:
   - Open a single pop-up window displaying all three pie charts side-by-side
   - Save the image as 'combined_genre_pie_charts_plot3.png' in the same directory

If you don’t see the chart or image, double-check that:
   - You’re in the correct folder (e.g., Downloads)
   - You’ve installed matplotlib
   - You’re using Command Prompt

! If you plan to run this script using WSL (Ubuntu on Windows Subsystem for Linux):

1. Open your WSL terminal by typing 'wsl' in Command Prompt or opening Ubuntu directly.

2. Make sure Python 3 and pip are installed:
   - sudo apt update
   - sudo apt install python3 python3-pip

3. Install the required library inside WSL:
   - pip3 install matplotlib

4. Image will be saved as 'combined_genre_pie_charts_plot3.png' in the same directory as running your python script.
"""

# Import the matplotlib library
import matplotlib.pyplot as plt

# Number of highly rated movies per genre combination for each time period
data = {
    '1991-2000': [55, 56, 215],
    '2001-2010': [76, 141, 400],
    '2011-2020': [208, 343, 590]
}

# Labels and consistent color palette for all pie charts
labels = ['Action;Thriller', 'Adventure;Drama', 'Comedy;Romance']
colors = ['blue', 'orange', 'green']

# Create a 1x3 subplot layout
fig, axes = plt.subplots(1, 3, figsize=(18, 6))  # 1 row, 3 columns

# Loop through the data and create each pie chart in a subplot
for ax, (period, sizes) in zip(axes, data.items()):
    ax.pie(sizes, labels=labels, autopct='%1.1f%%', startangle=140, colors=colors)
    ax.set_title(f'{period}', fontsize=12)

# Add main title for whole figure
plt.suptitle('Genre Distribution of Highly Rated Movies by Time Period', fontsize=16)

# Adjust layout
plt.tight_layout(rect=[0, 0, 1, 0.95])

# Save combined figure
plt.savefig('combined_genre_pie_charts_plot3.png', dpi=300, bbox_inches='tight')

# Show combined figure
plt.show()
