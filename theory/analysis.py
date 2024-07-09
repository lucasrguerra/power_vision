import matplotlib.pyplot as plt
import generate_data
import numpy as np
import math



# Data parameters
voltage_amplitude = 311.14
current_amplitude = 70.71
system_frequency = 60.2
quantity_of_samples = 1024
sample_rate = 256 * 60



# Generate the voltage and current data
voltage_wave = generate_data.pure_sine_wave(quantity_of_samples, voltage_amplitude, system_frequency, 0, sample_rate)
voltage_wave = generate_data.add_noise(voltage_wave, 5)

current_wave = generate_data.pure_sine_wave(quantity_of_samples, current_amplitude, system_frequency, 20, sample_rate)
current_wave = generate_data.add_noise(current_wave, 1)
#current_wave = generate_data.add_harmonic(current_wave, 20, 2 * system_frequency)
#current_wave = generate_data.add_harmonic(current_wave, 25, 3 * system_frequency)



# Function to calculate data
def main():

    # Function to filter the data
    def moving_average_filter(data, n=3):
        filtered_data = []
        normalization_factor = 1.0 / n
        number_of_samples = len(data)

        for i in range(number_of_samples):
            start = max(0, i - (n // 2))
            end = min(number_of_samples, i + (n // 2) + 1)
            average = sum(data[start:end]) * normalization_factor
            filtered_data.append(average)

        return filtered_data



    voltage_samples = moving_average_filter(voltage_wave)
    current_samples = moving_average_filter(current_wave)
    number_of_samples = len(voltage_samples)
    ms_per_sample = 1000 / (number_of_samples * 15)



    voltage_rms_integral = 0
    current_rms_integral = 0
    active_power_integral = 0
    duration_of_cycles = []
    last_time_start_cycle = 0
    


    for i in range(number_of_samples):
        voltage_sample = voltage_samples[i]
        current_sample = current_samples[i]

        voltage_rms_integral += voltage_sample ** 2
        current_rms_integral += current_sample ** 2
        active_power_integral += voltage_sample * current_sample

        if i > 0:
            actual_time = i * ms_per_sample
            if voltage_sample >= 0 and voltage_samples[i - 1] < 0:
                duration_of_cycles.append(actual_time - last_time_start_cycle)
                last_time_start_cycle = actual_time



    frequency = 1000 / (sum(duration_of_cycles) / len(duration_of_cycles))
    voltage_rms = (voltage_rms_integral / number_of_samples) ** 0.5
    current_rms = (current_rms_integral / number_of_samples) ** 0.5
    apparent_power = voltage_rms * current_rms
    active_power = active_power_integral / number_of_samples
    cosine_phi = active_power / apparent_power



    current_fft = np.fft.fft(current_samples)
    current_fft_amplitudes = (2 / number_of_samples) * np.abs(current_fft)
    harmonic_currents_integral = 0
    harmonics = []



    for i in range(2, 14):
        frequency_of_harmonic = int(i * frequency)
        nearby_harmonics = [
            current_fft_amplitudes[frequency_of_harmonic - 2],
            current_fft_amplitudes[frequency_of_harmonic - 1],
            current_fft_amplitudes[frequency_of_harmonic],
            current_fft_amplitudes[frequency_of_harmonic + 1],
            current_fft_amplitudes[frequency_of_harmonic + 2],
        ]
        real_harmonic = max(nearby_harmonics)

        if real_harmonic > 0.5:
            harmonic_currents_integral += real_harmonic ** 2
            harmonics.append(real_harmonic)



    total_harmonic_distortion = (harmonic_currents_integral ** 0.5) / current_rms
    total_harmonic_distortion_squared = total_harmonic_distortion ** 2
    power_factor = cosine_phi / ((1 + total_harmonic_distortion_squared) ** 0.5)
    reactive_power = math.sin(math.acos(power_factor)) * apparent_power
    distortion_power = abs((apparent_power ** 2) - (active_power ** 2) - (reactive_power ** 2)) ** 0.5



    # Print the results
    print()
    print(f"Frequency: {frequency:.2f} Hz")
    print(f"Voltage RMS: {voltage_rms:.2f} Vac")
    print(f"Current RMS: {current_rms:.2f} A")
    print(f"Apparent Power: {(apparent_power / 1000):.2f} kVA")
    print(f"THD: {(total_harmonic_distortion * 100):.3f} %")
    print(f"Cosine Phi: {cosine_phi:.3f}")
    print(f"Power Factor: {power_factor:.3f}")
    print(f"Active Power: {(active_power / 1000):.2f} kW")
    print(f"Reactive Power: {(reactive_power / 1000):.2f} kVAr")
    print(f"Distortion Power: {(distortion_power / 1000):.2f} kVAD")
    print()

    plt.plot(voltage_samples, label="Voltage")
    plt.plot(current_samples, label="Current")
    plt.plot(current_fft_amplitudes, label="Current FFT")
    plt.legend()
    #plt.show()

main()
