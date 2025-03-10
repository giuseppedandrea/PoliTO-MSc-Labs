/*
TEMPO(ID_TEMPO, DATA, GIORNO, MESE, ANNO, PRIMARY KEY(ID_TEMPO));
TARIFFA(ID_TAR, TIPO_TARIFFA, PRIMARY KEY(ID_TAR));
LUOGO(ID_LUOGO, CITTA, PROVINCIA, REGIONE, PRIMARY KEY(ID_LUOGO));
FATTI(ID_TEMPO, ID_TAR, ID_LUOGO_CHIAMANTE, ID_LUOGO_CHIAMATO, PREZZO, CHIAMATE, PRIMARY KEY(ID_TEMPO, ID_TAR,ID_LUOGO_CHIAMANTE,ID_LUOGO_CHIAMATO));
*/

/*
Selezionare l’incasso totale per ogni tipo tariffa e per ogni anno. Selezionare inoltre l'incasso totale
complessivo, l'incasso totale per ogni tipo di tariffa indifferentemente dall’anno, e l'incasso totale per
ogni anno indifferentemente dal tipo di tariffa.
*/
SELECT      TIPO_TARIFFA, ANNO, SUM(PREZZO) AS INCASSO_TOTALE_TARIFFA_ANNO,
            SUM(SUM(PREZZO)) OVER () AS INCASSO_TOTALE,
            SUM(SUM(PREZZO)) OVER (PARTITION BY TIPO_TARIFFA) AS INCASSO_TOTALE_TARIFFA,
            SUM(SUM(PREZZO)) OVER (PARTITION BY ANNO) AS INCASSO_TOTALE_ANNO
FROM        FATTI F, TARIFFA TA, TEMPO TE
WHERE       F.ID_TAR = TA.ID_TAR AND F.ID_TEMPO = TE.ID_TEMPO
GROUP BY    TIPO_TARIFFA, ANNO

/*
Per ogni mese, selezionare il numero di chiamate totali e l'incasso totale. Utilizzando la funzione
RANK(), associare ad ogni mese un numero che identifica la posizione del mese all'interno dei mesi in
funzione dell'incasso totale effettuato (1 per il mese che ha incassato di più, 2 per il secondo mese,
ecc.)
*/
SELECT      MESE, SUM(CHIAMATE) AS CHIAMATE_TOTALI, SUM(PREZZO) INCASSO_TOTALE,
            RANK() OVER (ORDER BY SUM(PREZZO) DESC) AS POSIZIONE_MESE
FROM        FATTI F, TEMPO T
WHERE       F.ID_TEMPO = T.ID_TEMPO
GROUP BY    MESE

/*
Selezionare per ogni mese dell’anno 2003 il numero di chiamate totali. Utilizzando la funzione RANK(),
associare ad ogni mese un numero che identifica la posizione del mese all'interno dei vari mesi
dell’anno 2003 in funzione del numero di chiamate totali (1 per il mese con più telefonate, 2 per il
secondo mese, ecc.)
*/
SELECT      MESE, SUM(CHIAMATE) AS CHIAMATE_TOTALI,
            RANK() OVER (ORDER BY SUM(CHIAMATE) DESC) AS POSIZIONE_MESE
FROM        FATTI F, TEMPO T
WHERE       F.ID_TEMPO = T.ID_TEMPO AND ANNO = 2003
GROUP BY    MESE

/*
Selezionare per ogni data del mese di luglio 2003 l’incasso totale, e la media giornaliera degli incassi
delle chiamate effettuate negli ultimi tre giorni.
*/
SELECT      DATA, SUM(PREZZO) INCASSO_TOTALE,
            AVG(SUM(PREZZO)) OVER (ORDER BY DATA
                                   RANGE BETWEEN INTERVAL '2' DAY PRECEDING AND CURRENT ROW) AS MEDIA_GIORNALIERA
FROM        FATTI F, TEMPO T
WHERE       F.ID_TEMPO = T.ID_TEMPO AND MESE = '7-2003'
GROUP BY    DATA

/*
Selezionare per ogni mese, l'incasso del mese e l’incasso cumulativo dall’inizio dell’anno.
*/
SELECT      MESE, ANNO, SUM(PREZZO) INCASSO_TOTALE,
            SUM(SUM(PREZZO)) OVER (PARTITION BY ANNO
                                   ORDER BY MESE
                                   ROWS UNBOUNDED PRECEDING) AS INCASSO_CUMULATIVO
FROM        FATTI F, TEMPO T
WHERE       F.ID_TEMPO = T.ID_TEMPO
GROUP BY    MESE, ANNO

/*
Basandosi sulle cardinalità delle tabelle del data warehouse (tabelle dei fatti e tabelle delle
dimensioni), decidere quali viste materializzate possono essere definite al fine di ridurre i tempi di
risposta per le interrogazioni appena risolte.
Provare a creare alcune delle viste materializzate ritenute utili e verificare se le viste create vengono
effettivamente utilizzate dal DBMS analizzando il piano di esecuzione generato per ogni
interrogazione.
*/

CREATE MATERIALIZED VIEW GROUPBY_MESE_ANNO
BUILD IMMEDIATE
REFRESH COMPLETE ON DEMAND
ENABLE QUERY REWRITE
AS
    SELECT      MESE, ANNO, SUM(CHIAMATE) AS CHIAMATE_TOTALI, SUM(PREZZO) INCASSO_TOTALE
    FROM        FATTI F, TEMPO T
    WHERE       F.ID_TEMPO = T.ID_TEMPO
    GROUP BY    MESE, ANNO;
