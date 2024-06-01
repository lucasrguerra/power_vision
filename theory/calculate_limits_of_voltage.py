input_ac_voltage = 475
input_load_resistor = 270000
secondary_resistor = 560
raise_of_two = 2 ** 0.5



primary_current = input_ac_voltage / input_load_resistor
secundary_rms_voltage = secondary_resistor * primary_current
secondary_peak_voltage = secundary_rms_voltage * raise_of_two
secundary_peak_to_peak_voltage = secondary_peak_voltage * 2
transformation_ratio = input_ac_voltage / secundary_rms_voltage



print()
print(f"Primary current: {primary_current:.5f} A")
print(f"Secondary rms voltage: {secundary_rms_voltage:.5f} Vac")
print(f"Secondary peak voltage: {secondary_peak_voltage:.5f} V")
print(f"Secondary peak to peak voltage: {secundary_peak_to_peak_voltage:.5f} V")
print(f"Transformation ratio: {transformation_ratio:.5f}")
print()