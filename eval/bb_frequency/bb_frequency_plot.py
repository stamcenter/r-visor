import matplotlib.pyplot as plt
from collections import defaultdict
import re

# file_path = 'bb_frequency_logs_dhrystone'
file_path = './../bb_frequency_logs'
plt.rcParams.update({'font.size': 16})


with open(file_path, 'r') as file:
    data = file.readlines()

# Initialize a defaultdict to store frequencies by address bins
address_bins = defaultdict(int)

# Set the bin size
# bin_size = 256
bin_size = 512

# Parse the input data
min_address = float('inf')
max_address = float('-inf')

for line in data:
    match = re.match(r"Basic Block: ([0-9a-f]+), frequency: (\d+)", line)
    if match:
        address = int(match.group(1), 16)  # Convert hex to int
        frequency = int(match.group(2))
        bin_address = (address // bin_size) * bin_size  # Group by larger bins
        address_bins[bin_address] += frequency
        min_address = min(min_address, bin_address)
        max_address = max(max_address, bin_address)

# Filter out bins with frequency less than 1
# filtered_bins = {bin_addr: freq for bin_addr, freq in address_bins.items() if freq >= 1000}
filtered_bins = {bin_addr: freq for bin_addr, freq in address_bins.items() if freq > 50}

rail_color = '#3F6BB9'
rail_plus_color = '#7DB958'
dynamorio_color = '#996CAB'

# Prepare data for histogram
bins_to_plot = sorted(filtered_bins.keys())
frequencies = [filtered_bins[bin] for bin in bins_to_plot]
bin_labels = [f'{hex(bin)}-{hex(bin+bin_size-1)}' for bin in bins_to_plot]

# Plot the histogram
# plt.figure(figsize=(18, 6))
plt.rcParams.update({'font.size': 26})
fig, ax = plt.subplots(figsize=(18, 6))
# plt.bar(range(len(bins_to_plot)), [freq/1000 for freq in frequencies], width=0.8)
ax.bar(range(len(bins_to_plot)), [freq for freq in frequencies], width=0.8, color=rail_color, zorder=3)
ax.set_xlabel(f'Basic Block Address Range (binned by {bin_size} bytes)')
ax.set_ylabel(r'Frequency')
# plt.title('Histogram of Basic Block Frequencies (Frequency >= 1)')
# plt.grid(True)

ax.grid(axis='y', zorder=0)
ax.spines[['right', 'top']].set_visible(False)
# ax.set_xticklabels(bin_labels, rotation=45, ha='right')  # Rotate labels by 45 degrees
ax.set_xticks(range(len(bins_to_plot)))
ax.set_xticklabels(bin_labels, rotation=45, ha='right')  # Rotate labels by 45 degrees
# ax.tight_layout()
plt.savefig("bb_frequency_sha.pdf", format="pdf", bbox_inches="tight")
# plt.savefig("bb_frequency_fib.png", format="png", bbox_inches="tight")
plt.show()