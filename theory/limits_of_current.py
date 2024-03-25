current_input = 0.004
output_current_resistor = 270

raise_of_two = 2 ** 0.5
output_voltage = current_input * output_current_resistor
output_voltage_peak = output_voltage * raise_of_two
output_voltage_peak_to_peak = output_voltage_peak * 2
current_transformation_ratio = current_input / output_voltage

print()
print(f"Output rms voltage: {output_voltage:.5f} Vac")
print(f"Output peak voltage: {output_voltage_peak:.5f} Vac")
print(f"Output peak-to-peak voltage: {output_voltage_peak_to_peak:.5f} Vdc")
print(f"Current transformation ratio: {current_transformation_ratio:.5f}")
print()