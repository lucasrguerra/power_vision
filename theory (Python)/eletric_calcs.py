def rms(wave):
    integral = 0
    for i in range(len(wave)):
        integral += wave[i] ** 2
    
    return (integral / len(wave)) ** 0.5

def apparent_power(voltage, current):
    return voltage * current

def active_power(voltage_wave, current_wave):
    num_samples = len(voltage_wave)
    integral = 0
    for i in range(num_samples):
        integral += voltage_wave[i] * current_wave[i]
    
    return integral / num_samples

def reactive_power(voltage_wave, current_wave):
    num_samples = len(voltage_wave)
    lag = int(num_samples / 4)
    current_wave_shifted = []
    for i in range(num_samples):
        if i < (lag * 3):
            current_wave_shifted.append(current_wave[i + lag])
        elif i >= (lag * 3):
            current_wave_shifted.append(current_wave[i - (lag * 3)])

    return active_power(voltage_wave, current_wave_shifted)

def power_factor(active_power, apparent_power):
    return active_power / apparent_power

def load_type(power_factor, reactive_power):
    if power_factor > 0.999:
        return "Resistive"
    else:
        if reactive_power < 0:
            return "Capacitive"
        else:
            return "Inductive"
        
def frequency(wave):
    num_samples = len(wave)
    period = 0
    for i in range(num_samples):
        if wave[i] > 0 and wave[i - 1] < 0:
            period += 1
            