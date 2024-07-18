from scipy.stats import linregress
import generate_data



reference_voltage = 1.65
raise_of_three = 3 ** 0.5
number_of_points = 100
system_frequency = 60
quantity_of_samples = 1000
sample_rate = 2160

voltage_levels = [
    [440, 510000, 430],
    [380, 470000, 430],
    [220, 300000, 430],
    [127, 180000, 430]
]



print("\n")
for level in voltage_levels:
    voltage_amplitude = level[0] * raise_of_three
    voltage_input = generate_data.pure_sine_wave(quantity_of_samples, voltage_amplitude, system_frequency, 0, sample_rate)
    voltage_output = []
    adc_values = []

    def calculate_data(input_voltage, shunt_resistor, feedback_resistor):
        transformer_current = input_voltage / shunt_resistor
        output_voltage = transformer_current * feedback_resistor
        output_peak_voltage = output_voltage + reference_voltage
        return output_peak_voltage

    for voltage in voltage_input:
        output = calculate_data(voltage, level[1], level[2])
        voltage_output.append(output)
        adc_values.append(int(output * 1000))

    slope, intercept, r_value, p_value, std_err = linregress(adc_values, voltage_input)
    equation = equation = f"y = {slope:.3f}x + {intercept:.3f}"
    print(f"Calibration equation for {level[0]}Vac: {equation}")
print("\n")
