from subprocess import run as ExecuteCommand
from time import sleep
from sys import argv as arguments
from datetime import datetime

def GetMeasurementsFromPMIC():
    output_raw = ExecuteCommand(["vcgencmd", "pmic_read_adc"], capture_output = True)
    output = output_raw.stdout.decode().split("\n")

    return output

def GetVoltageFromCPU(pmic_read_adc_output):
    measurement_line = pmic_read_adc_output[19]
    voltage = float(measurement_line.split('=')[1][0:5])

    return voltage

def GetAmpereFromCPU(pmic_read_adc_output):
    measurement_line = pmic_read_adc_output[7]
    ampere = float(measurement_line.split('=')[1][0:5])

    return ampere

if len(arguments) > 2:
    print("pmc [filename]")
    print("[file]: OPTIONAL, specifies the file the data should be stored at for later use - WARNING, IT WILL REPLACE ANY EXISTING FILE WITH THE SAME NAME")
    exit(-1)

output_file = None

if len(arguments) == 2:
    output_file = open(arguments[1], "w")

while (True):
    measurements = GetMeasurementsFromPMIC()
    voltage = GetVoltageFromCPU(measurements)
    ampere = GetAmpereFromCPU(measurements)
    wattage = voltage * ampere

    print(f"Time: {datetime.now()}")
    print(f"Voltage: {voltage}")
    print(f"Ampere: {ampere}")
    print(f"Wattage: {wattage}")
    print()

    if output_file is not None:
        output_file.write(f"Time: {datetime.now()}\n")
        output_file.write(f"Voltage: {voltage}\n")
        output_file.write(f"Ampere: {ampere}\n")
        output_file.write(f"Wattage: {wattage}\n")
        output_file.write("\n")

    sleep(1)