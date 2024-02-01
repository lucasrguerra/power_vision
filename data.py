import matplotlib.pyplot as plt
from random import random
import math

def pure_sine_wave(amplitude, frequency, degree, sample_rate, duration):
    num_samples = int(sample_rate * duration)
    return [amplitude * math.sin(2 * math.pi * frequency * t / sample_rate + math.radians(degree)) for t in range(num_samples)]

def insert_noise(wave, noise_amplitude):
    return [sample + noise_amplitude * (2 * random() - 1) for sample in wave]