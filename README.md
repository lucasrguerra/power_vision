# ESP32 Power Analyzer



## Descrição

O ESP32 Power Analyzer é um projeto de código aberto destinado ao desenvolvimento de um analisador de qualidade da energia elétrica em tempo real utilizando um hardware de baixo custo com o microcontrolador ESP32-S3. Este analisador é capaz de medir e calcular uma variedade de parâmetros elétricos em sistemas monofásicos e trifásicos, fornecendo informações cruciais para análise e monitoramento de consumo de energia.

O projeto foi desenvolvido como Trabalho de Conclusão de Curso (TCC) do curso de Ciência da Computação da Universidade Federal Rural de Pernmabuco (UFRPE) e orientado pelo Prof. Dr. Abner Corrêa Barros. O projeto tem como objetivo principal a implementação de um analisador de energia elétrica de baixo custo e alta precisão, que possa ser utilizado em residências, empresas e indústrias para monitoramento e análise de consumo de energia.



## Funcionalidades

O ESP32 Power Analyzer atualmente tem na teoria as seguintes funcionalidades:

- **Frequência (Hz)**: Medição da frequência do sistema.
- **Tensão True RMS (Vac)**: Medição precisa da tensão eficaz do sistema.
- **Corrente True RMS (A)**: Medição precisa da corrente eficaz do sistema.
- **Potência Aparente (kVA)**: Cálculo da potência aparente do sistema.
- **Distorção Harmônica Total (%)**: Avaliação da distorção harmônica no sistema.
- **Cosseno φ**: Cálculo do cosseno do ângulo de fase.
- **Fator de Potência (%)**: Determinação do fator de potência do sistema.
- **Potência Ativa (kW)**: Cálculo da potência ativa consumida pelo sistema.
- **Potência Reativa (kVAr)**: Cálculo da potência reativa do sistema.
- **Potência de Distorção (kVAD)**: Cálculo da potência de distorção do sistema.



## Limites de medição

Assim como qualquer equipamento de medição, o ESP32 Power Analyzer possui limites de medição para cada parâmetro elétrico. Abaixo estão os limites de medição que este projeto visa alcançar:

Tensão de Linha: 0-820 Vac  
Tensão de Fase: 0-475 Vac  
Corrente: 0-1000 A  
Frequência: 45-65 Hz  



## Estrutura do Projeto

O projeto está organizado da seguinte forma:

- **[`src/`](./src/)**: Contém o código fonte principal do analisador de energia, que vai no Microcontrolador. (Em desenvolvimento 🛠️)
- **[`docs/`](./docs/)**: Documentação do projeto, incluindo manuais de usuário, especificações técnicas e outras informações relevantes.
- **[`theory/`](./theory/)**: Esta pasta contém implementações em Python das fórmulas utilizadas antes de serem integradas ao código principal do microcontrolador.
- **[`boards/`](./boards/)**: Contém informações sobre as placas desenvolvidas para o projeto e os arquivos de fabricação no formato Gerber.



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
4. Monte o circuito conforme o esquemático disponível na pasta [`docs/`](./docs/).
4. Conecte o analisador de energia ao sistema que deseja analisar.
5. Inicie o analisador e visualize os resultados pela interface serial.



## Créditos

Este projeto foi criador por [Lucas Rayan](https://github.com/lucasrguerra) (vulgo "eu") e continua sendo mantido por ele.



## Agradecimentos

A minha mãe, que sempre lutou muito para que eu e minha irmã tivéssemos uma boa educação, e que sempre me apoiou em todas as minhas decisões. E que é sem dúvida a pessoa mais importante da minha vida.

Ao [Prof. Dr. Abner Corrêa Barros](https://www.linkedin.com/in/abner-barros-5b86409/), meu orientador, que sempre esteve disposto a ajudar e doar seu tempo e conhecimento para contribuir com o desenvolvimento do projeto.

A minha grande amiga e companheira de vida, [Vitória das Dores](https://github.com/VitoriaDasDores), que sempre esteve ao meu lado me apoiando e me ajudando a superar todos os desafios.

Ao meu grande amigo e grande Engenheiro Eletrônico, [João Pontes](https://github.com/JoaoPonTechS), que sempre esteve disposto a ajudar e ouvir minhas ideias e dúvidas.



## Licença

Este projeto está licenciado sob a Licença MIT. Consulte o arquivo [LICENSE](./LICENSE) para obter mais detalhes.