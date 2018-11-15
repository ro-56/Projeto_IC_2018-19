### Uso:
- `make`: Processa os arquivos e cria um programa executável.
- `./run`: Executa o programa executável.
- `make clean`: Limpa a pasta, deixando apenas os arquivos principais, o `.txt` e o programa executável.
- `make clear`: Limpa todos os arquivos extras, até os `.txt` para gráficos e o executável.
- `make reset`: Limpa a pasta com `make clear` e processa os arquivos com o `make`.
- `make reboot`: Limpa a pasta com `make reset` e executa o programa com `./run`.

### Definição:
- Busca uma solução para o problema de CRP utilizando listas ligadas

### Restrições:
- Demanda precisa ser atendida ("multa" para demanda não atendida)
- Sequencia de mesma planta não pode ocorrer
- Sazonalidade deve ser obedecida
		

### Dados de entrada:
- Produtividade (ton/hec)
- Area dos terrenos (hec)
- Tempo de produção (períodos)
- Demanda (ton)
- Lucratividade (u.m./ton)
- Número de terrenos
- Número de períodos
- Número de espécies
	

### Exemplo de "Entrada.txt":
```
PERIODOS_POR_ANO:
12

PERIODOS:
24

ESPECIES:
9

TERRENOS:
10

AREA_TERRENOS(hec):
1 1 1 1 1 1 1 1 1 1

TEMPO_DE_PRODUCAO(periodos):
2 1 3 1 2 3 4 1 2  

DEMANDA(ton):
10 10 10 10 10 10 10 10 10 

LUCRATIVIDADE(u.m./ton):
4 6 2 10 15 7 11 8 5 

PRODUTIVIDADE(ton/hec):
4 3 1 2 2 2 1 4 2

PERIODO_DE_PLANTIO_[Ei,Ti]:
10 3
7 12
5 12
2 10
5 12
9 3
5 12
4 7
2 8
```