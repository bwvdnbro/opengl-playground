import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as pl

if __name__ == "__main__":
  pixels = None
  with open("grid_export.pgm", "r") as handle:
    lines = handle.readlines()
    nx, ny = [int(x) for x in lines[2].split()]
    pixels = [0]*nx*ny
    i = 0
    for line in lines[4:]:
      for p in line[:-1]:
        pixels[i] = int(p)
        i += 1
    pixels = np.array(pixels).reshape((nx,ny))

  pl.imshow(pixels)

  pl.tight_layout()
  pl.axis("off")
  pl.savefig("grid_export.png", dpi=300, bbox_inches="tight")
