from matplotlib import pyplot
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

points = np.loadtxt("intersect.txt", dtype='f', delimiter=' ')
print(points)

polygon = np.array([[2.5, 1.5, 4], [2.5, 2.5, 4], [2.5, 2.5, 6], [2.5, 1.5, 6], [2.5, 1.5, 4]], np.float)
path = np.array([[1, 1, 5], [1, 5, 5]], np.int32)


fig = pyplot.figure()
ax = Axes3D(fig)

ax.scatter(points[:,0], points[:,1], points[:,2])
ax.plot(polygon[:,0], polygon[:,1], polygon[:,2])
ax.plot(path[:,0], path[:,1], path[:,2])
pyplot.show()

