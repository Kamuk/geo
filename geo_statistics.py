import MySQLdb as sql
import numpy as np
from scipy.spatial.distance import pdist
from scipy.cluster.hierarchy import linkage, fcluster
import matplotlib.pyplot as plt
from shapely.geometry import Point
from shapely.geometry.polygon import Polygon

conn = sql.connect(host='localhost', user='root', passwd='password', db='geo')
cursor = conn.cursor()

radius = 0.5
meeting_threshold = 10

x1 = [(-200, -100), (-200, - 50), (-100, -100), (-100, - 50)]
x2 = [(0, 0), (100, 0), (50, 50)]
x3 = [-5, (-100, -50), (-50, -50), (0, 50), (-20, 20), (-70, 0)]
areas = np.array([x1, x2, x3])
areas_time = {}

# plt.axis()
def CheckInAreas(points, time_threshold=30):
    result = []
    for i in range(points.shape[0]):
        p = Point(points[i, 0], points[i, 1])
        for j in range(areas.shape[0]):
            area = Polygon(areas[j, :])
            if area.contain(p):
                if i in areas_time:
                    areas_time[i] += 1
                else:
                    areas_time[i] = 1
            else:
                if i in areas_time:
                    del areas_time[i]
    for name in areas_time.keys():
        if areas_time[name] >= time_threshold:
            result.append(name)
    return result



while True:
    cursor.execute("SELECT id, current_point_x, current_point_y FROM geo.trackers")

    rows = cursor.fetchall()
    ids = [row[0] for row in rows]
    conn.commit()
    if len(ids) == 0:
        continue

    points = np.array([[float(row[1]), float(row[2])] for row in rows])
    X = pdist(points)
    Y = linkage(X)
    labels = fcluster(Y, radius, depth=5)
    # print(labels)

    unique, counts = np.unique(labels, return_counts=True)
    # print(counts)

    meeting_class = [i for i in range(len(counts)) if counts[i] >= meeting_threshold]
    # print(meeting_class)

    ids_in_areas = CheckInAreas(points)
    # print(ids)


    # plt.scatter(points[:, 0], points[:, 1], c=labels, cmap='tab20c')
    # plt.show()
    # plt.clf()
    # plt.scatter(points[:, 0], points[:, 1], c=labels, cmap='tab20c')
    # plt.pause(0.1)