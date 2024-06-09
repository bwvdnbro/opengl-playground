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

pl.plot(positions[:,0], positions[:,1], "ro-")
pl.plot(positions[:,2], positions[:,3], "bo-")
pl.xlim(0., 100.)
pl.ylim(0., 50.)
pl.gca().axis("off")
pl.tight_layout()

pl.savefig("output.png", bbox_inches="tight")
