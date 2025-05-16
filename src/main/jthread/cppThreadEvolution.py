import matplotlib.pyplot as plt
import numpy as np

# Enhanced Data with C++14 and C++26 (future)
years = [2011, 2014, 2017, 2020, 2023, 2026]
labels = [
    "C++11\nFundamental Multithreading Introduced\n(std::thread, mutexes, condition_variable, atomic,\nasync, future, promise)",
    "C++14\nConcurrency Refinements\n(shared_timed_mutex, chrono literals e.g., 100ms)",
    "C++17\nEnhanced Synchronization & Parallelism\n(scoped_lock, shared_mutex, parallel algorithms\nwith std::execution policies)",
    "C++20\nMajor Concurrency Additions\n(Coroutines [co_await, co_yield], jthread & stop_token,\natomic_ref, barrier, latch, counting_semaphore)",
    "C++23\natomic enhancements (floating-point, trivially copyable structs),\nsync_wait (coroutine/future sync), expected, stacktrace",
    "C++26\nPlanned Advanced Features (proposals)\n(Hazard Pointers, RCU, Executor model, Networking TS)",
]
colors = ["#4c72b0", "#937860", "#55a868", "#c44e52", "#8c564b", "#e377c2"]
y_offsets = [2.5, 1.75, 2.5, 1.75, 2.5, 1.75]

# Plot
fig, ax = plt.subplots(figsize=(18, 7))
ax.set_xlim(2009, 2028)
ax.set_ylim(0, 3)
ax.axis("off")
ax.set_facecolor("#f9f9f9")

# Wavy timeline curve
x_curve = np.linspace(2010, 2027, 500)
y_curve = 1 + 0.05 * np.sin(0.6 * np.pi * (x_curve - 2010))
ax.plot(x_curve, y_curve, color="black", linewidth=3)

# Y position for each milestone on curve
y_positions = 1 + 0.05 * np.sin(0.6 * np.pi * (np.array(years) - 2010))

# Milestones + connector lines
for year, label, color, y_offset, y_base in zip(years, labels, colors, y_offsets, y_positions):
    ax.vlines(
        x=year,
        ymin=y_base,
        ymax=y_offset - 0.45,
        color=color,
        linestyle="dotted",
        linewidth=2,
    )
    ax.plot(
        year,
        y_base,
        "o",
        markersize=14,
        color=color,
        markeredgecolor="black",
        markeredgewidth=1.5,
    )
    ax.text(
        year,
        y_offset + 0.05,
        f"{int(year)}",
        ha="center",
        va="bottom",
        fontsize=14,
        weight="bold",
        color="#222222",
    )
    ax.text(
        year,
        y_offset - 0.02,
        label,
        ha="center",
        va="top",
        fontsize=11,
        wrap=True,
        color="#222222",
    )

plt.tight_layout()
plt.show()

fig.savefig("cpp_multithreading_timeline_wavy.png", dpi=300, bbox_inches="tight")
