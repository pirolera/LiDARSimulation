from matplotlib import pyplot
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import json
import sys


def plotConfig(ax, configFile):
    '''Plot the structures and the vehicle path, as defined in the config file'''

    path = np.empty((0,4))

    with open(configFile) as json_file:  
        data = json.load(json_file)

        #get vehicle path
        for waypoint in data['vehiclePath']:
            path = np.append(path, np.array([[waypoint['t'], waypoint['x'], waypoint['y'], waypoint['z']]]), axis=0)

        #plot structures
        for structure in data['structures']:
            polygon = np.empty((0,3))
            for point in structure:
                polygon = np.append(polygon, np.array([[point['x'], point['y'], point['z']]]), axis=0)
            polygon = np.append(polygon, np.array([polygon[0,:]]), axis=0)
            ax.plot(polygon[:,0], polygon[:,1], polygon[:,2])

    ax.plot(path[:,1], path[:,2], path[:,3], 'r-*')


def plotOutput(ax, outputFile):
    ''' Plot the beam intersection points, as outputted by the simulator'''

    points = np.empty((0,3))

    with open(outputFile) as json_file:  
        data = json.load(json_file)

        #get intersection points
        for line in data:
            for beam in line['beams']:
                if 'point' in beam:
                    point = beam['point']
                    points = np.append(points, np.array([[point['x'], point['y'], point['z']]]), axis=0)
    ax.scatter(points[:,0], points[:,1], points[:,2])


def plotData(configFile, outputFile):
    '''Create complete plot with configuration and sensor output'''
    
    fig = pyplot.figure()
    ax = Axes3D(fig)

    plotConfig(ax, configFile)
    plotOutput(ax, outputFile)

    pyplot.show()


if __name__ == "__main__":
    if ( len( sys.argv ) != 3 ):
        print("ERROR, need to pass config file and output file")
        sys.exit(-1)

    configFile = sys.argv[1]
    outputFile = sys.argv[2]

    plotData(configFile, outputFile)

