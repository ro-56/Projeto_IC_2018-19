
### Definição:
Busca uma solução para o problema de CRP utilizando listas ligadas para codificar o individuo e matriz para a população mista durante as gerações.

### Configurações
 - **Iniciação**: Plantas são adicinadas no terreno com menor tempo livre.
 - **Mutação**: Um terreno aleatório é substituido por outro totalmente aleatório.

### Restrições:
- Demanda precisa ser atendida ("multa" para demanda não atendida).
- Plantas no mesmo período e em terrenos adjacentes penalizam a função objetivo
- Sequencia de plantas da mesma familia não pode ocorrer.
- Sazonalidade deve ser obedecida.
		

### Dados de entrada:
- Produtividade (ton/hec)
- Area dos terrenos (hec)
- Tempo de produção (períodos)
- Familias
- Demanda (ton)
- Lucratividade (u.m./ton)
- Número de terrenos
- Número de períodos
- Número de espécies
- Matriz de adjacência
- Número de arestas na Matriz de Adjacência
	

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
1 2 1 1 4 3 1 2 1 3

TEMPO_DE_PRODUCAO(periodos):
2 3 2 5 2 3 4 3 2

FAMILIAS:
1 2 3 4 5 6 7 8 9

DEMANDA(ton):
30 20 10 10 20 10 40 30 10

LUCRATIVIDADE(u.m./ton):
4 6 2 10 15 7 11 8 5 

PRODUTIVIDADE(ton/hec):
4 3 1 2 2 2 1 4 2

PERIODO_DE_PLANTIO_[Ei,Ti]:
10 3
12 7
5 12
1 10
5 12
9 3
5 12
1 7
2 8

NUMERO_DE_LIGACOES_MATRIZ_ADJACENCIA:
21

MATRIZ_DE_INCIDENCIA_TERRENOS_ADJACENTES:
1 2
1 4
1 5
1 6
2 3
2 4
2 9
3 4
3 7
3 9
3 10
4 5
4 10
5 6
5 7
5 10
6 7
7 8
7 9
7 10
8 9
```
