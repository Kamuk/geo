import MySQLdb as sql
import numpy as np
from scipy.spatial.distance import pdist
from scipy.cluster.hierarchy import linkage, fcluster
import matplotlib.pyplot as plt

conn = sql.connect(host='localhost', user='root', passwd='password', db='geo')
cursor = conn.cursor()

radius = 0.5
meeting_threshold = 10

plt.axis()

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

    # plt.scatter(points[:, 0], points[:, 1], c=labels, cmap='tab20c')
    # plt.show()
    plt.clf()
    plt.scatter(points[:, 0], points[:, 1], c=labels, cmap='tab20c')
    plt.pause(0.1)