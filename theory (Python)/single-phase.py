import matplotlib.pyplot as plt
import numpy as np
import data

voltage_amplitude = 311.2
current_amplitude = 70
frequency = 1
sample_rate = (2 ** 9) * frequency
noise_amplitude = 10


def main():
    voltage_wave = data.pure_sine_wave(voltage_amplitude, frequency, 0, sample_rate)
    #voltage_wave = data.insert_noise(voltage_wave, noise_amplitude)
    current_wave = data.pure_sine_wave(current_amplitude, frequency, -8, sample_rate)
    #current_wave = data.insert_noise(current_wave, noise_amplitude)
    current_wave = data.insert_harmonics(current_wave, [0.01 * current_amplitude] * 15)


    voltage_integral = 0
    current_integral = 0
    active_power_integral = 0

    for i in range(sample_rate):
        voltage_data = voltage_wave[i]
        current_data = current_wave[i]

        voltage_integral += voltage_data ** 2
        current_integral += current_data ** 2
        active_power_integral += voltage_data * current_data

    voltage_rms = (voltage_integral / sample_rate) ** 0.5
    current_rms = (current_integral / sample_rate) ** 0.5
    active_power = active_power_integral / sample_rate

    current_fft = np.fft.fft(current_wave)[1:sample_rate // 2]
    current_fft_amplitudes = (2 / sample_rate) * np.abs(current_fft)
    harmonic_currents_integral = 0
    for i in range(15):
        harmonic_current = current_fft_amplitudes[i + 1] ** 2
        harmonic_currents_integral += harmonic_current
    total_harmonic_distortion = (harmonic_currents_integral ** 0.5) / current_rms

    apparent_power = voltage_rms * current_rms
    cos_phi = active_power / apparent_power

    total_harmonic_distortion_squared = total_harmonic_distortion ** 2
    apparent_power_squared = apparent_power ** 2
    power_factor = cos_phi / (1 + total_harmonic_distortion_squared) ** 0.5
    distortion_power = (apparent_power_squared * (total_harmonic_distortion_squared / (1 - total_harmonic_distortion_squared))) ** 0.5
    reactive_power = (apparent_power_squared - (active_power ** 2 - distortion_power ** 2)) ** 0.5
    

    print()
    print(f"Voltage: {voltage_rms:.2f} Vac")
    print(f"Current: {current_rms:.2f} Aac")
    print(f"Apparent Power: {(apparent_power / 1000):.2f} kVA")
    print(f"Active Power: {(active_power / 1000):.2f} kW")
    print(f"Cosφ: {cos_phi:.2f}")
    print(f"THD: {total_harmonic_distortion:.2f} %")
    print(f"Power Factor: {power_factor:.2f} %")
    print(f"Distortion Power: {(distortion_power / 1000):.2f} DkVA")
    print(f"Reactive Power: {(reactive_power / 1000):.2f} kVAR")
    print()

    plt.plot(voltage_wave)
    plt.plot(current_wave)
    plt.show()

main()
