import random
import pathlib
from itertools import cycle

# 22.80;53.00;2020;September;Sunday;20


def get_temp(month):

	"""
	Get temperature based on month

	"""

	temp = 0.0
	l, h = 0, 0 

	if month == "January":
		l, h = -20, 3
	if month == "February":
		l, h = -20, 0
	if month == "March":
		l, h = -18, 8
	if month == "April":
		l, h = -10, 10
	if month == "May":
		l, h = 6, 25
	if month == "June":
		l, h = 13, 30
	if month == "July":
		l, h = 15, 40
	if month == "August":
		l, h = 13, 35
	if month == "September":
		l, h = 10, 25
	if month == "October":
		l, h = 0, 18
	if month == "November":
		l, h = -5, 15
	if month == "December":
		l, h = -10, 8

	return round(random.uniform(l, h), 1)

def main(yrs):

	x = 0

	path = pathlib.Path(__file__).parent.absolute()
	file_name = str(path) + "/dummydata.txt"

	range_days = 372 * 4 * yrs # every year has 372 days, and there are 4 records each year

	hours = ("0.00", "6.00", "12.00", "18.00")
	days = ("Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday")
	ndays = tuple((x for x in range(1, 32)))
	months = ("January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December")
	years = ("2020", "2021")

	hcycle = cycle(hours)
	dcycle = cycle(days)
	ndcycle = cycle(ndays)
	mcycle = cycle(months)
	ycycle = cycle(years)

	with open(file_name, mode='a') as f:

		d = next(dcycle)
		nd = next(ndcycle)
		y = next(ycycle)
		m = next(mcycle)
		h = next(hcycle)

		for _ in range(range_days):
			x += 1

			temp = get_temp(m)
			humid = round(random.uniform(0, 100), 1)

			h = next(hcycle)

			to_write = f"{str(temp)};{str(humid)};{str(y)}.{str(m)}.{str(nd)};{str(d)};{str(h)}\n"

			if h == "0.00":
				d = next(dcycle)
				nd = next(ndcycle)

			if nd == 31 and h == "0.00":
				m = next(mcycle)

			if m == "January" and h == "0.00" and nd == 1:
				y = next(ycycle)

			f.write(to_write)
			print(m, x, nd)

	return file_name


if __name__ == '__main__':
	path = main(2)
	print(f"File written at {path}")
