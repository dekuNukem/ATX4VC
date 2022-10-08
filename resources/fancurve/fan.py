# 0 to 100, or 0 to 501?
def temp_c_to_fan_percent(raw_12b_data):
	if raw_12b_data == -1: # no probe installed
		return 100;
	temp_c = raw_12b_data / 16
	if temp_c <= 25:
		return 20
	if temp_c >= 50:
		return 100;
	return 3.2*temp_c - 60

def fan_percent_to_timer_value(percent):
	if percent >= 100:
		return 501
	if percent <= 0:
		return 0
	return percent * 5;

dec_c_to_timer_dict = {}

for x in range(-2048, 2047):
	deg_c = x / 16
	fan_percent = temp_c_to_fan_percent(x)
	timer_value = int(fan_percent_to_timer_value(fan_percent))
	print(x, deg_c, fan_percent, timer_value)

	dec_c_to_timer_dict[int(deg_c)] = timer_value


print(dec_c_to_timer_dict)