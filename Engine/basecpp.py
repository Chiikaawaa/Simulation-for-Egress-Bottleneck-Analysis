import matplotlib.pyplot as pt
import particle
import seaborn as sns
import time

n_particle = 25
Dp         = 1.0
Dt         = 0.0001
bounds     = (-5,5)
radius     = 0.25

p = particle.Particle(n_particle, Dp, Dt, radius, bounds)
n_steps = 1000
start_time = time.perf_counter()
p.simulate(n_steps)
end_time = time.perf_counter()
print(f"compute time for {n_steps} steps and {n_particle} particles is {end_time - start_time:.2f} seconds")
final_positions = p.get_positions()
final_x = [p[0] for p in final_positions]
final_y = [p[1] for p in final_positions]

pt.figure(figsize = (10,10))

pt.subplot(1,2,1)
sns.histplot(final_x, bins=100)

pt.subplot(1,2,2)
sns.histplot(final_y, bins=100)

pt.tight_layout()
pt.show()
pt.savefig("histogram.png", dpi=150, bbox_inches="tight")
