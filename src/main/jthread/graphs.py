import matplotlib.pyplot as plt

# Data
years = [2011, 2011, 2011, 2017, 2020, 2023]
labels = [
    "C++11\nstd::thread introduced\n(manual join/detach)",
    "C++11\nstd::mutex introduced\n(locking primitives)",
    "C++11\nstd::condition_variable\n(synchronization)",
    "C++17\nminor improvements\n(std::scoped_lock etc.)",
    "C++20\nstd::jthread introduced\n(auto join + stop_token)",
    "C++23\nminor threading updates\n(std::atomic_ref etc.)"
]
colors = ['#4c72b0', '#dd8452', '#55a868', '#937860', '#55a868', '#c44e52']

# Slight horizontal staggering for 2011 to avoid overlap
adjusted_years = [2011 - 0.4, 2011, 2011 + 0.4, 2017, 2020, 2023]

# Y offsets: 2011 labels lowered with double spacing
y_offsets = [2.5,2 , 1.5, 1.7, 2.3, 2.3]

# Plot
fig, ax = plt.subplots(figsize=(12, 6))
ax.set_xlim(2009, 2024)
ax.set_ylim(0, 3)
ax.axis('off')

# Timeline line
ax.hlines(y=1, xmin=2010, xmax=2024, color='gray', linewidth=2)

# Milestones
for year, label, color, y_offset in zip(adjusted_years, labels, colors, y_offsets):
    ax.plot(year, 1, 'o', markersize=12, color=color)
    ax.text(year, y_offset + 0.05, f"{int(round(year))}",
            ha='center', va='bottom', fontsize=12, weight='bold')
    ax.text(year, y_offset - 0.02, label,
            ha='center', va='top', fontsize=9, wrap=True)

plt.tight_layout()
plt.show()
