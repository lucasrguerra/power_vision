# ESP32 Power Analyzer

## Descrição

O ESP32 Power Analyzer é um projeto de código aberto destinado ao desenvolvimento de um analisador de energia de baixo custo utilizando o microcontrolador ESP32-S3. Este analisador é capaz de medir e calcular uma variedade de parâmetros elétricos em sistemas monofásicos, fornecendo informações cruciais para análise e monitoramento de consumo de energia.

## Funcionalidades

O ESP32 Power Analyzer atualmente tem na teoria as seguintes funcionalidades:

- **Tensão True RMS (Vac)**: Medição precisa da tensão eficaz do sistema.
- **Corrente True RMS (A)**: Medição precisa da corrente eficaz do sistema.
- **Potência Aparente (kVA)**: Cálculo da potência aparente do sistema.
- **Potência Ativa (kW)**: Cálculo da potência ativa consumida pelo sistema.
- **Cosseno φ**: Cálculo do cosseno do ângulo de fase.
- **Distorção Harmônica Total (%)**: Avaliação da distorção harmônica no sistema.
- **Fator de Potência (%)**: Determinação do fator de potência do sistema.
- **Potência de Distorção (DkVA)**: Cálculo da potência de distorção do sistema.
- **Potência Reativa (kVAR)**: Cálculo da potência reativa do sistema.

## Estrutura do Projeto

O projeto está organizado da seguinte forma:

- **`src/`**: Contém o código fonte principal do analisador de energia, que vai no Microcontrolador.
- **`theory (Python)/`**: Esta pasta contém implementações em Python das fórmulas utilizadas antes de serem integradas ao código principal do microcontrolador.
- **`docs/`**: (**_Em desenvolvimento 🛠️_**) Documentação do projeto, incluindo manuais de usuário, especificações técnicas e outras informações relevantes.

## Como Contribuir

Sinta-se à vontade para contribuir com o projeto! Você pode:

- Reportar problemas encontrados.
- Propor melhorias e novas funcionalidades.
- Enviar correções ou otimizações de código.
- Compartilhar suas experiências e ideias.

## Como Utilizar

Para utilizar o ESP32 Power Analyzer, siga estas etapas:

1. Clone o repositório para o seu ambiente de desenvolvimento com o PlatformIO instalado.
2. Instale as dependências necessárias:
    - kosme/arduinoFFT@^1.6.2
3. Compile e carregue o código no seu ESP32-S3.
4. Monte o circuito conforme o esquemático disponível na pasta `docs/`.
4. Conecte o analisador de energia ao sistema que deseja analisar.
5. Inicie o analisador e visualize os resultados pela interface serial.

## Créditos

Este projeto é atualmente mantido por [Lucas Rayan](https://github.com/lucasrguerra).

## Licença

Este projeto está licenciado sob a Licença MIT. Consulte o arquivo [LICENSE](./LICENSE) para obter mais detalhes.