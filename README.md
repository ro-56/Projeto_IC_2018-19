# Projeto de Iniciação científica (2018-19)

Projeto de iniciação científica para o período de jul/2018 à jul/2019.

Este projeto consiste em encontrar soluções para o Problema de Rotação de Culturas (Crop Rotation Problem, ou CRP) a partir de determinadas restrições, como demanda ou sazonalidade.

Cada grupo desse projeto representa uma diferente análise, seja por uma diferença do método de resolução aplicado ou pela adição ou alteração de rstrições no problema inicial.


## Componentes
- `01` : Problema analisado utilizando uma matriz das espécies a serem plantadas em determinados períodos.

- `02` : O mesmo problema do item anterior, mas utilizando Listas Ligadas para a resoluçõo do problema e com a adição da restrição de famílias de espécies.
  - **Iniciação**: Plantas são adicinadas no terreno com menor tempo livre.
  - **Mutação**: Um terreno aleatório é substituido por outro totalmente aleatório. 

- `03` : O mesmo problema do item anterior, mas utilizando Listas Ligadas para a resoluçõo do problema e com a adição da restrição de famílias de espécies.
  - **Iniciação**: Plantas são adicinadas nos terrenos em ordem, completando-os totalmente antes de avançar para o próximo.
  - **Mutação**: Um terreno aleatório é substituido por outro totalmente aleatório.


## Uso:

Segue os comanados utilizados para executar os programas contidos nesse trabalho:

- `make`: Processa os arquivos e cria um programa executável.
- `./run`: Executa o programa executável.
- `make clean`: Limpa a pasta, deixando apenas os arquivos principais, o `.txt` e o programa executável.
- `make clear`: Limpa todos os arquivos extras, até os `.txt` para gráficos e o executável.
- `make reset`: Limpa a pasta com `make clear` e processa os arquivos com o `make`.
- `make reboot`: Limpa a pasta com `make reset` e executa o programa com `./run`.