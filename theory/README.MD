# Teoria do Power Vision

Bem-vindo à documentação da teoria por trás do Power Vision. Este documento fornece informações detalhadas sobre a matemática e os cálculos utilizados no projeto para medir e calcular parâmetros elétricos, como tensão, corrente, potência ativa, potência reativa, fator de potência e muito mais.



## Componentes de Hardware

### Resistores para a corrente (Rcurr)

Para calcular a corrente do sistema, é necessário utilizar resistores de baixa resistência, e essa resistência é calculada levando em consideração a tensão máxima que pode ser medida pelo microcontrolador com uma margem de erro. Para simplificar os cálculos, o Jupyter Notebook [calculate_limits_of_current.ipynb](./calculate_limits_of_current.ipynb) foi criado para calcular os parâmetros de acordo com os resistores utilizados.

| Corrente Máxima do Sistema | Relação do Sensor | Corrente no Secundário | Tensão Pico a Pico no Secundário | Resistência (±1%) | Relação da Transformação |
| --- | --- | --- | --- | --- | --- |
| 1000A | 3000:1 | 0.33333A | 2.3094V  | 2Ω   | 1500    |
| 750A  | 3000:1 | 0.25A    | 2.33827V | 2.7Ω | 1111.11 |
| 500A  | 3000:1 | 0.16667A | 2.48261V | 4.3Ω | 697.674 |
| 300A  | 3000:1 | 0.1A     | 2.35559V | 6.8Ω | 441.176 |
| 200A  | 3000:1 | 0.06667A | 2.3094V  | 10Ω  | 300     |
| 120A  | 3000:1 | 0.04A    | 2.49415V | 18Ω  | 166.667 |
| 80A   | 3000:1 | 0.02667A | 2.49415V | 27Ω  | 111.111 |
| 100A  | 2000:1 | 0.05A    | 2.07846V | 12Ω  | 166.667 |
| 20A   | 2000:1 | 0.01A    | 2.35559V | 68Ω  | 29.4118 |




### Resistores para a tensão (Rvolt)

Para calcular a tensão do sistema, é necessário utilizar resistores de baixa e alta resistência, e essa resistência é calculada levando em consideração a tensão máxima que pode ser medida pelo microcontrolador com uma margem de erro e respeitando os limites do transfomador utilizado. Para simplificar os cálculos, o Jupyter Notebook [calculate_limits_of_voltage.ipynb](./calculate_limits_of_voltage.ipynb) foi criado para calcular os parâmetros de acordo com os resistores utilizados.

_**Nota:** Os valores de resistência foram calculados considerando o transformador de corrente ZMPT101B com uma relação de transformação de 2mA:2mA._

#### Resistores para o Primário (RPvolt) do Transformador de Tensão

| Tensão do Sistema | Tensão Máxima do Sistema | Corrente no Transformador | Resistência RPvolt (±1%) | Tensão Pico a Pico no Secundário | Relação da Transformação |
| --- | --- | --- | --- | --- | --- | --- |
| 440Vac | 475Vac | 0.00161A | 510kΩ | 2.40294V | 1186.05 |
| 380Vac | 440Vac | 0.00162A | 470kΩ | 2.41532V | 1093.02 |
| 220Vac | 275Vac | 0.00159A | 300kΩ | 2.365V   | 697.674 |
| 127Vac | 175Vac | 0.00168A | 180kΩ | 2.50833V | 418.605 |



## Matemática por trás do projeto

O Power Vision utiliza uma série de fórmulas e conceitos matemáticos para calcular com precisão os parâmetros elétricos do sistema. Abaixo estão alguns dados sobre os cálculos utilizados no projeto e as suas versões convertidas em funções na linguagem de programação Python.

_**Nota:** As fórmulas e cálculos a utilizados consideram que você tenha a mesma quantidade de amostras da tensão e da corrente. E que essa quantidade de amostras sejam um multiplo de 2, para ficilitar a utilização da FFT._



### Filtragem do sinal

Para remover os ruídos do sinal de corrente e tensão, é utilizado um filtro digital de média móvel. A função para calcular a média móvel é dada por:

```python
def filtro_media_movel(sinal, janela):
    sinal_filtrado = []
    fator_de_normalizacao = 1.0 / janela
    quantidade_de_amostras = len(sinal)

    for i in range(quantidade_de_amostras):
        inicio = max(0, i - (janela // 2))
        fim = min(quantidade_de_amostras, i + (janela // 2))
        media = sum(sinal[inicio:fim]) * fator_de_normalizacao
        sinal_filtrado.append(media)

    return sinal_filtrado
```



### Frequência (Hz)

A frequência é a quantidade de ciclos por segundo em um sinal elétrico. A função para calcular a frequência é dada por:

```python
def frequencia(sinal, milissegundos_entre_amostras):
    quantidade_de_amostras = len(sinal)
    duracoes_dos_ciclos = []
    momento_da_ultima_amostra = 0

    for i in range(quantidade_de_amostras):
        amostra = sinal[i]

        if amostra > 0 :
            momento_atual = i * milissegundos_entre_amostras
            if momento_da_ultima_amostra >= 0 and sinal[i - 1] < 0:
                duracoes_dos_ciclos.append(momento_atual - momento_da_ultima_amostra)
                momento_da_ultima_amostra = momento_atual

    return 1000 / (sum(duracoes_dos_ciclos) / len(duracoes_dos_ciclos))
    
```



### Tensão True RMS (Vac)

A tensão eficaz é a calculada pela raiz quadrada da média dos quadrados das amostras do sinal de tensão, em inglês conhecida como Root Mean Square (RMS). A função para calcular a tensão eficaz é dada por:

```python
def tensao_rms(sinal_da_tensao):
    return math.sqrt(sum([amostra ** 2 for amostra in sinal_da_tensao]) / len(sinal_da_tensao))
```



### Corrente True RMS (A)

A corrente eficaz é a calculada pela raiz quadrada da média dos quadrados das amostras do sinal de corrente, ou em inglês, Root Mean Square (RMS). A função para calcular a corrente eficaz é dada por:

```python
def corrente_rms(sinal_da_corrente):
    return math.sqrt(sum([amostra ** 2 for amostra in sinal_da_corrente]) / len(sinal_da_corrente))
```



### Potência Aparente (VA)

A potência aparente é a potência total em um circuito elétrico, e é dada pelo produto da tensão eficaz e da corrente eficaz. A função para calcular a potência aparente é dada por:

```python
def potencia_aparente(tensao_eficaz, corrente_eficaz):
    return tensao_eficaz * corrente_eficaz
```



### Distorção Harmônica Total (THD)

A distorção harmônica total é uma medida da distorção harmônica em um sinal elétrico. E no projeto é calculada pelo sinal da corrente. Para fazer a análisa da distorção harmônica total, é necessário transformar o sinal do domínio do tempo para o domínio da frequência, e para isso foi utilizado a Transformada Rápida de Fourier, ou em inglês, Fast Fourier Transform (FFT). A função para calcular a distorção harmônica total é dada por:

```python
def thd(sinal_da_corrente, frequencia, corrente_eficaz, quantidade_de_harmonicas_analisadas, amplitude_minima_da_harmonica):
    quantidade_de_amostras = len(sinal_da_corrente)
    fft_da_corrente = np.fft.fft(sinal_da_corrente)
    amplitudes_da_fft_da_corrente = (2 / quantidade_de_amostras) * np.abs(fft_da_corrente)
    integral_do_quadrado_das_correntes = []

    segunda_harmonica = 2
    for i in range(segunda_harmonica, quantidade_de_harmonicas_analisadas + 2):
        frequencia_da_harmonica = int(i * frequencia)
        frequecias_proximas = [
            amplitudes_da_fft_da_corrente - 2,
            amplitudes_da_fft_da_corrente - 1,
            amplitudes_da_fft_da_corrente,
            amplitudes_da_fft_da_corrente + 1,
            amplitudes_da_fft_da_corrente + 2
        ]
        hamonica_real = max(frequencias_proximas)

        if hamonica_real > amplitude_minima_da_harmonica:
            integral_do_quadrado_das_correntes.append(hamonica_real ** 2)

    return math.sqrt(integral_do_quadrado_das_correntes) / corrente_eficaz
```



### Potência Ativa (W)

A potência ativa é a potência que se converte em trabalho útil, e é dada pela soma do produto das amostras do sinal de tensão e corrente dividido pelo número de amostras. A função para calcular a potência ativa é dada por:

```python
def potencia_ativa(tensao, corrente):
    return sum([tensao[i] * corrente[i] for i in range(len(tensao))]) / len(tensao)
```



### Cosseno da Fase (cos(θ))

O cosseno da fase é a medida do deslocamento angular entre a tensão e a corrente em um circuito elétrico. A função para calcular o cosseno da fase é dada por:

```python
def cosseno_da_fase(potenca_ativa, potencia_aparente):
    return potencia_ativa / potencia_aparente
```



### Fator de Potência (FP)

O fator de potência é a medida da eficiência de um circuito elétrico, e é dada pelo cosseno da fase. A função para calcular o fator de potência é dada por:

```python
def fator_de_potencia(cosseno_da_fase, distorcao_harmonica_total):
    return cosseno_da_fase / math.sqrt(1 + distorcao_harmonica_total ** 2)
```



### Potência Reativa (VAr)

A potência reativa é a potência que não se converte em trabalho útil, mas é necessária para manter os campos eletricos e magnéticos em um circuito elétrico. A função para calcular a potência reativa é dada por:

```python
def potencia_reativa(potencia_aparente, fator_de_potencia):
    return math.sin(math.acos(fator_de_potencia)) * potencia_aparente
```



### Potência de Distorsão (VAD)

A potência de distorção é a potência que é perdida devido a distorção harmônica em um circuito elétrico. A função para calcular a potência de distorção é dada por:

```python
def potencia_de_distorsao(potencia_aparente, potencia_ativa, potencia_reativa):
    return math.sqrt(abs((potencia_aparente ** 2) - (potencia_ativa ** 2) - (potencia_reativa ** 2)))
```