import sys

if len(sys.argv) <= 1:
	print(__file__, 'text_file')
	exit()

text_file = open(sys.argv[1], encoding="utf8")

def make_section(text):
	text = text.lstrip("## ").replace('\r', '').replace('\n', '').strip()
	link = text.lower().replace('.', '')
	result = ''
	for letter in link:
		if letter.isalnum():
			result += letter
		else:
			result += '-'
	result = result.strip('-')
	print(f'* [{text}](#{result})\n')

print("## Table of Contents\n")

for line in text_file:
	if 'table of content' in line.lower():
		continue
	if line.startswith("## "):
		make_section(line)

text_file.close()