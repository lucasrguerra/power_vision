base_frequency = 80000000
divider = 1

for i in range(11, 100):
    sample_rate = (60 + (i * 60)) * 2
    frequency = (base_frequency / divider) / sample_rate
    microseconds = 1000000 / sample_rate
    rest = microseconds % 1
    if rest > 0.95 or rest < 0.05:
        print(f"Sample rate: {sample_rate}Hz / {i} / {int(frequency)} / {microseconds}us / {rest}")