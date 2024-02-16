from random import random
import math

def pure_sine_wave(amplitude, frequency, degree, sample_rate):
    return [amplitude * math.sin(2 * math.pi * frequency * i / sample_rate + math.radians(degree)) for i in range(sample_rate)]

def insert_noise(wave, noise_amplitude):
    return [sample + noise_amplitude * (2 * random() - 1) for sample in wave]

def insert_harmonics(wave, harmonics_amplitudes):
    for i in range(len(wave)):
        for j in range(len(harmonics_amplitudes)):
            wave[i] += harmonics_amplitudes[j] * math.sin(2 * math.pi * (j + 1) * (i + 1) / len(wave))
    return wave