import math

for x in range(360):
	sin_x = math.sin(math.radians(x))
	sin_x = sin_x * 127 + 127
	print(round(sin_x), end=', ')
	