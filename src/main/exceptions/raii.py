import matplotlib.pyplot as plt
import matplotlib.patches as patches

fig, ax = plt.subplots(figsize=(12, 3))

# Define phases
phases = ['Object Created', 'Resource Acquired', 'Work Done', 'Object Exits Scope', 'Resource Released']
colors = ['lightblue', 'lightgreen', 'lightyellow', 'salmon', 'lightcoral']
positions = [0, 2.5, 5, 7.5, 10]

# Draw rectangles with dotted outline
for i, (pos, phase) in enumerate(zip(positions, phases)):
    rect = patches.FancyBboxPatch((pos, 0), 2.1, 1.1,
                                   boxstyle="round,pad=0.1",
                                   facecolor=colors[i],
                                   edgecolor='black',
                                   linewidth=1,
                                   linestyle='dotted')
    ax.add_patch(rect)
    ax.text(pos + 1.1, 0.6, phase, ha='center', va='center', fontsize=14,)

# Draw arrows between stages
for i in range(len(positions) - 1):
    ax.annotate('', xy=(positions[i+1], 0.6), xytext=(positions[i] + 2.2, 0.6),
                arrowprops=dict(arrowstyle="->", lw=3, color='black'))

# Annotate object creation start
ax.annotate('Object Construction Begins',
            xy=(0 + 1.1, 1.3), xytext=(0 + 1.1, 1.9),
            ha='center', fontsize=14, color='blue',
            arrowprops=dict(arrowstyle="-[", lw=2, color='blue'))

# Annotate constructor (resource acquired)
ax.annotate('Constructor Body',
            xy=(2.5 + 1.1, 1.3), xytext=(2.5 + 1.1, 2.6),
            ha='center', fontsize=14, color='green',
            arrowprops=dict(arrowstyle="-[", lw=2, color='green'))

# Annotate destructor
ax.annotate('Destructor Body',
            xy=(10 + 1.1, 1.3), xytext=(10 + 1.1, 1.9),
            ha='center', fontsize=14, color='red',
            arrowprops=dict(arrowstyle="-[", lw=2, color='red'))

# Formatting
ax.set_xlim(-0.5, 13)
ax.set_ylim(-0.5, 2.2)
ax.axis('off')
# ax.set_title('RAII Lifecycle: Scope-Based Resource Management', fontsize=16)

plt.tight_layout()
plt.show()
