import numpy as np
import time
import MySQLdb as sql
from time_converter import get_current_time

conn = sql.connect(host='localhost', user='root', passwd='password', db='geo')
conn.autocommit(True)
cursor = conn.cursor()

def GeoSimulator(n_objects, xlim, ylim, velocity, timesleep):
    x0 = np.random.random((n_objects, 1)) * (xlim[1] - xlim[0]) + xlim[0]
    y0 = np.random.random((n_objects, 1)) * (ylim[1] - ylim[0]) + ylim[0]
    points = np.hstack((x0, y0))
    while True:
        dmove = (2 * np.random.random((n_objects, 2)) - 1) * velocity
        points += dmove
        current_time = get_current_time()
        cursor.executemany("INSERT INTO geo_data (tracker_id, point_x, point_y, dt_created) VALUES (%s, %s, %s, %s)",
                           [(i + 1, np.round(points[i, 0], 6), np.round(points[i, 1], 6), current_time) for i in range(n_objects)])
        time.sleep(timesleep)

GeoSimulator(50, (-200, 200), (-100, 100), 10, 0.1)

conn.close()
