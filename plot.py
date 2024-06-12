import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as pl

positions = []
with open("output.txt", "r") as handle:
  started = False
  for line in handle.readlines():
    if line == "=== BEGIN ===\n":
      started = True
      continue
    if line == "=== END ===\n":
      break
    if started:
      positions.append([float(x) for x in line.split()])

positions = np.array(positions)

pl.plot(positions[:,0], positions[:,1], "r-")
pl.plot(positions[:,2], positions[:,3], "b-")
pl.plot([0., 100., 100., 0., 0.], [0., 0., 50., 50., 0.], "k-")
pl.gca().axis("off")
pl.tight_layout()

pl.savefig("output.png", bbox_inches="tight")
