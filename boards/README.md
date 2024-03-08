# Placas do projeto

## Descrição

Nesta pasta estão todos os arquivos das placas desenvolvidas para este projeto utilizando o software KiCAD. Dentre esses arquivos, estão o esquemático, o layout da placa e seus arquivos de fabricação no formato Gerber.

## Versões Disponíveis

- **[`v1.0`](./v1.0/)**: Essa é a terceira versão da placa e no modelo THT (Through-Hole Technology).
- **[`v0.7`](./v0.7/)**: Essa é a segunda versão da placa e no modelo SMD (Surface-Mount Device).
- **[`v0.5`](./v0.5/)**: Essa é a primeira versão da placa e no modelo THT (Through-Hole Technology).

## Dados das Placas

### Placa v1.0 - 143,510 x 73,025 mm (Atual)

<img alt="Placa v1.0"
    src="./v1.0/board.png"
    height="350">

Essa placa foi desenvolvida para ser utilizada com componentes THT (Through-Hole Technology) e possui um layout mais compacto e organizado que as anteriores, além de ter a conexão entre o ESP32-S3 e o módulo de interrupção de níveis de corrente. E os níveis de corrente dessa placa são 30A e 100A, possibilitando uma leiura mais precisa e adequada ao sensor utilizado, especialmente em baixas correntes.

### Placa v0.7 - 90,170 x 82,423 mm

<img alt="Placa v0.7"
    src="./v0.7/board.png"
    height="350">

Essa placa foi desenvolvida para ser utilizada com componentes SMD (Surface-Mount Device) e possui um layout mais compacto e organizado que a primeira versão. Porém, ele não possui a seleção de níveis de corrente, lendo apenas na escala de 100A.

### Placa v0.5 - 119,380 x 116,840 mm

<img alt="Placa v0.5"
    src="./v0.5/board.png"
    height="350">

Essa placa foi desenvolvida para ser utilizada com componentes THT (Through-Hole Technology) e possui um layout menos compacto e organizado. Além disso, ela possui a seleção de níveis de corrente, lendo na escala de 50A e 200A.
