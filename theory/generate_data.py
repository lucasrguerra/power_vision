from random import random
import math

def pure_sine_wave(number_of_samples, amplitude, frequency, phase, sample_rate):
    return [amplitude * math.sin(2 * math.pi * frequency * i / sample_rate + math.radians(phase)) for i in range(number_of_samples)]

def add_noise(wave, noise_amplitude):
    return [sample + noise_amplitude * (2 * random() - 1) for sample in wave]

def add_harmonic(wave, harmonic_amplitude, harmonic_frequency):
    sample_rate = len(wave)
    return [sample + harmonic_amplitude * math.sin(2 * math.pi * harmonic_frequency * i / sample_rate) for i, sample in enumerate(wave)]