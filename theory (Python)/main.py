import matplotlib.pyplot as plt
import eletric_calcs as ec
import data

def to2decimal(value):
    int_value = int(value * 100)
    return int_value / 100

voltage_amplitude = 311
current_amplitude = 70
frequency = 1
sample_rate = 2 ** 9
noise_amplitude = 10
type = "single"
#type = "two"
#type = "three"

if type == "single":
    voltage_wave = data.pure_sine_wave(voltage_amplitude, frequency, 0, sample_rate)
    current_wave = data.pure_sine_wave(current_amplitude, frequency, 4, sample_rate)

    voltage = ec.rms(voltage_wave)
    current = ec.rms(current_wave)
    apparent_power = ec.apparent_power(voltage, current)
    active_power = ec.active_power(voltage_wave, current_wave)
    reactive_power = ec.reactive_power(voltage_wave, current_wave)
    power_factor = ec.power_factor(active_power, apparent_power)
    print(power_factor)
    load_type = ec.load_type(power_factor, reactive_power)

    print()
    print("Voltage: ", to2decimal(voltage), " Vac")
    print("Current: ", to2decimal(current), " A")
    print("Apparent Power: ", to2decimal(apparent_power / 1000), " kVA")
    print("Active Power: ", to2decimal(active_power / 1000), " kW")
    print("Reactive Power: ", to2decimal(reactive_power / 1000), " kVAR")
    print("Power Factor: ", to2decimal(power_factor))
    print("Load Type: ", load_type)
    print()

    plt.plot(voltage_wave)
    plt.plot(current_wave)
    #plt.show()