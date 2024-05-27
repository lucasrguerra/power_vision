current_ratio = 3000
load_current = 1000
secundary_resistor = 3.3
raise_of_two = 2 ** 0.5



secundary_current = load_current / current_ratio
secundary_rms_voltage = secundary_resistor * secundary_current
secundary_peak_voltage = secundary_rms_voltage * raise_of_two
secundary_peak_to_peak_voltage = secundary_peak_voltage * 2



print()
print(f"Secundary current: {secundary_current:.5f} A")
print(f"Secundary rms voltage: {secundary_rms_voltage:.5f} Vac")
print(f"Secundary peak voltage: {secundary_peak_voltage:.5f} Vac")
print(f"Secundary peak to peak voltage: {secundary_peak_to_peak_voltage:.5f} Vac")
print()