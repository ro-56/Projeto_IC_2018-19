### Definição:
Esse programa busca uma solução para o problema de *CRP* utilizando uma matriz dos períodos de plantio.

-- | P1 | P2 | P3 | ... | Pn
-- | -- | -- | -- | -- | -- 
T1 | 1 | 1 | 0 | 2 | 2
T2 | 2 | 2 | 1 | 1 | 0
... | 3 | 3 | 3 | 3 | 0
Tn | 0 | 2 | 2 | 1 | 1


### Restrições:
- Demanda _precisa_ ser atendida ("multa" para demanda não atendida).
- Sequência de mesma espécie **não** pode ocorrer.
- Sazonalidade **deve** ser obedecida.
		

### Dados de entrada:
- Produtividade (ton/hec).
- Área dos terrenos (hec).
- Tempo de produção (períodos).
- Demanda (ton).
- Lucratividade (u.m./ton).
- Número de terrenos.
- Número de períodos.
- Número de espécies.
	

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
