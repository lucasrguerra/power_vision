input_ac_voltage = 380
input_load_resistor = 270000
output_load_resistor = 680

raise_of_two = 2 ** 0.5
input_current = input_ac_voltage / input_load_resistor
output_ac_voltage = input_current * output_load_resistor
output_ac_voltage_peak = output_ac_voltage * raise_of_two
output_ac_voltage_peak_to_peak = output_ac_voltage_peak * 2
transformation_ratio = input_ac_voltage / output_ac_voltage

print()
print(f"Primary current: {input_current:.5f} A")
print(f"Secondary rms voltage: {output_ac_voltage:.5f} Vac")
print(f"Secondary peak voltage: {output_ac_voltage_peak:.5f} Vac")
print(f"Secondary peak-to-peak voltage: {output_ac_voltage_peak_to_peak:.5f} Vdc")
print(f"Transformation ratio: {transformation_ratio:.5f}")
print()