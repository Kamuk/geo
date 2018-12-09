import numpy as np
import MySQLdb as sql
from time_converter import get_current_time

conn = sql.connect(host='localhost', user='root', passwd='password', db='geo')
cursor = conn.cursor()

while True:
    try:
        cursor.execute("SELECT id, tracker_id, point_x, point_y, MIN(dt_created) FROM geo.geo_data group by tracker_id")

        rows = cursor.fetchall()
        ids = [row[0] for row in rows]
        if len(ids) == 0:
            conn.commit()
            continue

        cursor.execute("DELETE FROM geo.geo_data WHERE id IN " + str(tuple(ids)))

        current_time = get_current_time()
        rows_without_ids = [row[1:] + (current_time,) for row in rows]
        cursor.executemany("REPLACE INTO trackers (id, current_point_x, current_point_y, dt_created, dt_modified) VALUES (%s, %s, %s, %s, %s)", rows_without_ids)
        conn.commit()
    except:
        print('Rollback!')
        conn.rollback()

conn.close()