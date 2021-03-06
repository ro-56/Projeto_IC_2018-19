
### Definição:
Busca uma solução para o problema de CRP com múltiplos objetivos utilizando listas ligadas.

### Configurações
 - **Iniciação**: Plantas são adicinadas nos terrenos em ordem, completando-os totalmente antes de avançar para o próximo.
 - **Mutação**: Um terreno aleatório é substituido por outro totalmente aleatório.

### Restrições:
- Demanda precisa ser atendida ("multa" para demanda não atendida).
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

FAMILIAS:
1 2 3 4 5 6 7 8 9

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
