# Documentação do Power Vision

Bem-vindo à documentação oficial do Power Vision. Este documento fornece informações detalhadas sobre como usar, configurar e estender o analisador de energia de baixo custo desenvolvido com o microcontrolador ESP32-S3.



## Conteúdo

1. [Introdução](#introdução)
2. [Simulação do Circuito](#simulação-do-circuito)
3. [Configuração do Ambiente de Desenvolvimento](#configuração-do-ambiente-de-desenvolvimento)
4. [Componentes utilizados](#componentes-utilizados)
5. [Diagrama esquemático](#diagrama-esquemático)
6. [Placa do projeto](#placa-do-projeto)



## Introdução

O Power Vision é um projeto de código aberto que visa fornecer uma solução acessível e eficaz para análise de energia em sistemas elétricos. Ele busca oferecer uma variedade de funcionalidades para medição e cálculo de parâmetros elétricos, como tensão, corrente, potência ativa, potência reativa, fator de potência e muito mais com um custo acessível. O projeto é baseado no microcontrolador ESP32-S3 e utiliza sensores de corrente e tensão populares e de baixo custo para realizar as medições.



## Simulação do Circuito

Para simular o circuito do Power Vision, você pode usar o software [Falstad Circuit Simulator](https://www.falstad.com/circuit/). Basta abrir o arquivo [simulation.falstad](./simulation.falstad) no simulador e você poderá visualizar e simular o circuito do projeto.



## Configuração do Ambiente de Desenvolvimento

Para configurar o ambiente de desenvolvimento e começar a trabalhar com o Power Vision, basta ter o [PlatformIO](https://platformio.org) instalado em seu ambiente de desenvolvimento. Eu recomendo usar o [VSCode](https://code.visualstudio.com) com a extensão PlatformIO.

Depois de instalar o PlatformIO, basta clonar este repositório e abrir o projeto no VSCode. O PlatformIO irá instalar automaticamente todas as dependências necessárias para o projeto.



## Componentes utilizados (Considerando um sistema com 3 fases e 1 neutro)

- **1x Microcontrolador - ESP32-S3 DevKitC**
- **8x Amplificador Operacional - TL082**
- **8x Socket DIP-8**
- **4x Tranformador de corrente (não invasivo) - 3000:1 (Até 1000A)**
- **3x Tranformador de potencial - ZMPT101B**
- **1x Regulador de Tensão - LM1117T-3.3**
- **1x Fonte de Tensão - HLK-5M12**
- **6x Terminal de parafuso - 2 Pinos P5.08mm**
- **1x Microswitch - SS-12D00G4**
- **1x Capacitor de Tântalo 35v 47uF**
- **1x Capacitor de Tântalo 16v 100uF**
- **14x Diodo Zener 3V3 1W - IN4733A**
- **1x Pin Header 2.54mm (01x01P)**
- **2x Pin Header 2.54mm (01x03P)**
- **1x Pin Header 2.54mm (01x04P)**
- **1x Pin Header 2.54mm (01x06P)**
- **2x Pin Socket 2.54mm (01x22P)**
- **4x Resistor (Rcurr) - 1/4W 1%**
- **3x Resistor (RPvolt) - 1/4W 1%**
- **3x Resistor (RSvolt) - 1/4W 1%**
- **2x Resistor 10kΩ - 1/4W 0.1%**
- **7x Resistor 50Ω - 1/4W 1%**
- **1x Cartão microSD - Módulo de Cartão SD (SPI) - Opcional**



### Tabelas Importantes

#### Tipo de Sistema Elétrico (SYSTEM_TYPE)

| Tipo de Sistema | Valor da Variável (SYSTEM_TYPE) |
| --- | --- |
| Monofásico (P+N) | 0 |
| Bifásico (2P)    | 0 |
| Bifásico (2P+N)  | 1 |
| Trifásico (3P+N) | 2 |

_**Nota:** Lembre-se de mudar o valor da variável `SYSTEM_TYPE` no código do projeto para o valor correspondente na tabela acima_



#### Resistores para a corrente (Rcurr)

| Corrente Máxima (A) | Relação do Sensor | Resistência (Ω) | Valor da Variável (CURRENT_TYPE) |
| --- | --- | --- | --- |
| 1000 | 3000:1 | 2   | 0 |
| 750  | 3000:1 | 2.7 | 1 |
| 500  | 3000:1 | 4.3 | 2 |
| 300  | 3000:1 | 6.8 | 3 |
| 200  | 3000:1 | 10  | 4 |
| 120  | 3000:1 | 28  | 5 |
| 80   | 3000:1 | 27  | 6 |
| 100  | 2000:1 | 12  | 7 |
| 20   | 2000:1 | 68  | 8 |

_**Nota:** Lembre-se de mudar o valor da variável `CURRENT_TYPE` no código do projeto para o valor correspondente na tabela acima_



#### Resistores para a tensão (RPvolt)

| Tensão de Operação (Vac) | Resistência do Primário (Ω) | Valor da Variável (VOLTAGE_TYPE) |
| --- | --- | --- |
| 440 | 510k | 0 |
| 380 | 470k | 1 |
| 220 | 300k | 2 |
| 127 | 180k | 3 |

_**Nota:** Lembre-se de mudar o valor da variável `VOLTAGE_TYPE` no código do projeto para o valor correspondente na tabela acima_



## Diagrama esquemático

![Diagrama esquemático](./schematic.png)



## Placa do projeto

![Placa do projeto](./board.png)