# Documentação do ESP32 Power Analyzer

Bem-vindo à documentação oficial do ESP32 Power Analyzer. Este documento fornece informações detalhadas sobre como usar, configurar e estender o analisador de energia de baixo custo desenvolvido com o microcontrolador ESP32-S3.

## Conteúdo

1. [Introdução](#introdução)
2. [Configuração do Ambiente de Desenvolvimento](#configuração-do-ambiente-de-desenvolvimento)
3. [Diagrama esquemático](#diagrama-esquemático)
4. [Matemática por trás do projeto](#matemática-por-trás-do-projeto)

## Introdução

O ESP32 Power Analyzer é um projeto de código aberto que visa fornecer uma solução acessível e eficaz para análise de energia em sistemas elétricos. Ele busca oferecer uma variedade de funcionalidades para medição e cálculo de parâmetros elétricos, como tensão, corrente, potência ativa, potência reativa, fator de potência e muito mais com um custo acessível. O projeto é baseado no microcontrolador ESP32-S3 e utiliza sensores de corrente e tensão populares e de baixo custo para realizar as medições.

## Configuração do Ambiente de Desenvolvimento

Para configurar o ambiente de desenvolvimento e começar a trabalhar com o ESP32 Power Analyzer, basta ter o [PlatformIO](https://platformio.org) instalado em seu ambiente de desenvolvimento. Eu recomendo usar o [VSCode](https://code.visualstudio.com) com a extensão PlatformIO.

## Diagrama esquemático

Em breve...

## Matemática por trás do projeto

O ESP32 Power Analyzer utiliza uma série de fórmulas e conceitos matemáticos para calcular com precisão os parâmetros elétricos do sistema. Abaixo estão as explicações sobre sua aplicação e implementação em Python.

### Tensão True RMS (Vac)

A tensão eficaz é uma medida da média quadrática (Ou Root Mean Square - RMS) da tensão em um sistema. Ela é o valor da real da tensão que está sendo aplicada ao sistema. A fórmula para calcular a tensão eficaz é dada por:


```python
integral_da_tensão = 0

for amostra in amostras_da_tensao:
    integral_da_tensao += amostra ** 2

tensao_eficaz = sqrt(integral_da_tensao / len(amostras_da_tensao))
```

### Corrente True RMS (A)

Assim como na tensão eficaz, a corrente eficaz também é uma medida da média quadrática da corrente em um sistema. Ela é o valor da corrente que está passando ao sistema. A fórmula para calcular a corrente eficaz é dada por:

```python
integral_da_corrente = 0

for amostra in amostras_da_corrente:
    integral_da_corrente += amostra ** 2

corrente_eficaz = sqrt(integral_da_corrente / len(amostras_da_corrente))
```

### Potência Aparente (kVA)

A potência aparente é a medida da potência total em um sistema. Ela é o produto da tensão eficaz e da corrente eficaz. A fórmula para calcular a potência aparente é dada por:

```python
potencia_aparente = tensao_eficaz * corrente_eficaz
```

### Potência Ativa (kW)

A potência ativa é a medida da potência que executa trabalho em um sistema. Ela é dada pela média do produto da tensão e corrente em um sistema. A fórmula para calcular a potência ativa é dada por:

```python
integral_da_potencia_ativa = 0

for i in range(len(amostras_da_tensao)):
    integral_da_potencia_ativa += amostras_da_tensao[i] * amostras_da_corrente[i]

potencia_ativa = integral_da_potencia_ativa / len(amostras_da_tensao)
```

Perceba que nesse caso é muito importante que a quantidade de amostras de tensão e corrente seja a mesma.

### Cosseno φ

O cosseno do ângulo de fase é uma medida da defasagem entre a tensão e a corrente em um sistema. Ele é dado pela razão entre a potência ativa e a potência aparente. A fórmula para calcular o cosseno do ângulo de fase é dada por:

```python
cosseno_phi = potencia_ativa / potencia_aparente
```

### Distorção Harmônica Total (%)

A distorção harmônica total é uma medida da distorção harmônica em um sistema. Para calcular ela, é necessário transformar as amostras de corrente do domínio do tempo para o domínio da frequência e calcular a raiz quadrada da soma dos quadrados das amplitudes das componentes harmônicas. Nesse projeto, eu utilizei a Transformada Rápida de Fourier (FFT) para realizar essa transformação. A fórmula para calcular a distorção harmônica total é dada por:

```python
corrente_no_dominio_da_frequencia = fft(amostras_da_corrente_no_dominio_do_tempo)
harmonicas = corrente_no_dominio_da_frequencia[2: numero_de_amotras // 2]
amplitude_das_harmonicas = []

for harmonica in harmonicas:
    amplitude_da_harmonica = (2 / numero_de_amostras) * abs(harmonica)
    amplitude_das_harmonicas.append(amplitude_da_harmonica)

integral_da_distorcao_harmonica = 0

for amplitude in range(quantidade_de_harmonicas_analisadas):
    integral_da_distorcao_harmonica += amplitude ** 2

distorcao_harmonica_total = sqrt(integral_da_distorcao_harmonica) / corrente_eficaz
```

Perceba que eu eliminei as 2 primeiras componentes harmônicas, pois elas são a componente DC (frequência 0) e a componente fundamental (frequência do sistema. Nos testes 60Hz), respectivamente. E também, eu dividi por 2 o número de amostras, pois a FFT retorna a transformada de Fourier simétrica, ou seja, a segunda metade dos valores é um espelho da primeira metade.

### Fator de Potência

O fator de potência é uma medida da eficiência energética de um sistema. Ele indica a quantidade de energia que está sendo utilizada para realizar trabalho em um sistema. Normalmente, o fator de potência é igual ao cosseno do ângulo de fase. Mas nesse caso, eu utilizei uma fórmula que também leva em consideração a distorção harmônica total. A fórmula para calcular o fator de potência é dada por:

```python
fator_de_potencia = cosseno_phi / sqrt(1 + distorcao_harmonica_total ** 2)
```

### Potência de Distorsão (DkVA)

A potência de distorção é a medida da potência que é perdida devido à distorção harmônica em um sistema. Ela é dada pela seguinte fórmula:

```python
potencia_de_distorsao = sqrt((potencia_aparente ** 2) * (distorcao_harmonica_total ** 2 / (1 + distorcao_harmonica_total ** 2)))
```

### Potência Reativa (kVAR)

A potência reativa é a medida da potência que é utilizada para manutenção dos campos elétricos e magnéticos em um sistema. Ela é dada pela seguinte fórmula:

```python
potencia_reativa = sqrt(potencia_aparente ** 2 - (potencia_ativa ** 2 + potencia_de_distorsao ** 2))
```