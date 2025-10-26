"""
You are given the following information, but you may prefer to do some research for yourself.

    1 Jan 1900 was a Monday.
    Thirty days has September,
    April, June and November.
    All the rest have thirty-one,
    Saving February alone,
    Which has twenty-eight, rain or shine.
    And on leap years, twenty-nine.
    A leap year occurs on any year evenly divisible by 4, but not on a century unless it is divisible by 400.

How many Sundays fell on the first of the month during the twentieth century (1 Jan 1901 to 31 Dec 2000)?

ref -> https://projecteuler.net/problem=19
"""

month_nrm =     [31,28,31,30,31,30,31,31,30,31,30,31];
month_leap =    [31,29,31,30,31,30,31,31,30,31,30,31];

"""
returns the end day
given the month and 
start date;
example:
    getEndDay(month,startday)
    getEndDay(1,1);
        Jan and Sunday
    ret -> Tue (3)
"""
def getEndDate(month, startday):
    return ((month_nrm[month - 1] - 29) + startday) % 7 

if __name__ == "__main__":

    day = 2  # 1 Jan 1901 = Tuesday
    count = 0
    for year in range(1901, 2001):
        for month in [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]:
            if day == 0:
                count += 1
            days = month
            if ((month == 28) and 
                ((year % 4 == 0)
                 or (year % 400 == 0))):
                days = 29
            day = (day + days) % 7
    print(count)
