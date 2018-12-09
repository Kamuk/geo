import datetime

def totimestamp(dt, epoch=datetime.datetime(1970, 1, 1)):
    td = dt - epoch
    return int(1 * (td.microseconds + (td.seconds + td.days * 86400) * 10**6) / 10**6)

def get_current_time():
    timenow = datetime.datetime.utcnow()
    return totimestamp(timenow)