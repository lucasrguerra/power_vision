from scipy.stats import linregress
import generate_data
import numpy as np



reference_voltage = 1.65
raise_of_three = 3 ** 0.5
number_of_points = 100
system_frequency = 60
quantity_of_samples = 1000
sample_rate = 2160

current_levels = [
    [3000, 1000, 2],
    [3000, 750, 2.7],
    [3000, 500, 4.3],
    [3000, 300, 6.8],
    [3000, 200, 10],
    [3000, 120, 18],
    [3000, 80, 27],
    [2000, 100, 12],
    [2000, 20, 68]
]



print("\n")
for level in current_levels:
    current_amplitude = level[1] * raise_of_three
    current_input = generate_data.pure_sine_wave(quantity_of_samples, current_amplitude, system_frequency, 0, sample_rate)
    voltage_output = []
    adc_values = []

    def calculate_data(current_ratio, input_current, feedback_resistor):
        transformer_current = input_current / current_ratio
        output_voltage = transformer_current * feedback_resistor
        output_peak_voltage = output_voltage * raise_of_three
        return output_peak_voltage

    for current in current_input:
        output = calculate_data(level[0], current, level[2])
        voltage_output.append(output)
        adc_values.append(int(output * 1000))

    slope, intercept, r_value, p_value, std_err = linregress(adc_values, current_input)
    equation = equation = f"y = {slope:.3f}x + {intercept:.3f}"
    print(f"Calibration equation for {level[1]}A: {equation}")
print("\n")
