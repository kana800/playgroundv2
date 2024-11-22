import re

numbers = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]

if __name__ == "__main__":

    calibration_value = 0
#   PART 1
#    with open("day_1_input.txt", "r") as f:
#        for char_arr in f:
#            firstdigit = re.search("[1-9]", char_arr.strip())
#            seconddigit = re.search("\d(?=\D*$)", char_arr.strip())
#            if (seconddigit is None): 
#                number = firstdigit.group(0) + firstdigit.group(0)
#            else:
#                number = firstdigit.group(0) + seconddigit.group(0)
#            calibration_value += int(number)
#   PART 2
    with open("day_1_input.txt", "r") as f:
        for char_arr in f:
            number_arr = []
            for i,c in enumerate(char_arr):
                if c.isdigit():
                    number_arr.append(c)
                for d,val in enumerate(numbers):
                    if char_arr[i:].startswith(val):
                        number_arr.append(str(d + 1))
            score = int(number_arr[0] + number_arr[-1])
            calibration_value += score
