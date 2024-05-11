# FYMC

---

author: FYMC\
date: 2024\
title: ARCHY CPU instruction set architecture

---

# Introduction

Ceci est une version modifiée du jeu d\'instruction fournie dans le
sujet. Le document a été traduit en français et des éléments y ont été
ajoutés afin de correspondre aux modifications apportées lors de la
réalisation du projet. Auteur original : yaspr.

# Spécifications

## Format binaire

Les fichiers binaires que nous utilisons sont divisés en trois parties distinctes. Tout d'abord, il y a le header, qui constitue la première section et présente la structure suivante. Ensuite, il y a la section appelée "data", où sont stockées les valeurs constantes comme les chaînes de caractères. Enfin, la dernière section, appelée "code", contient le code machine.

| Taille en octets | Description                  |
|------------------|------------------------------|
| 8 octets         | Magic Number (ARCHY0.0)      |
| 8 octets         | Taille du header             |
| 8 octets         | Adresse de la section data   |
| 8 octets         | Taille de la section data    |
| 8 octets         | Adresse de la section code   |
| 8 octets         | Taille de la section code    |
| 8 octets         | Taille totale du fichier     |


## Types de données

-   **u64**: Entier non-signé de 64 bits
-   **i64**: Entier signé de 64 bits
-   **f64**: Nombre à virgule flottante double précision
-   **ascii**: Caractère ASCII non-signé de 8 bits

## Registres

### Scalaires

-   **U**: Registres scalaires pour entiers non-signés et caractères
    ASCII
-   **S**: Registres scalaires pour entiers signés
-   **F**: Registres scalaires pour nombres à virgule flottante double
    précision

### Vectoriels

-   **V**: Registres scalaires (512 bits) pour entiers non-signés et
    caractères ASCII
-   **T**: Registres scalaires (512 bits) pour entiers signés
-   **G**: Registres scalaires (512 bits) pour pour nombres à virgule
    flottante double précision

### Système

| Position du bit   | 7        | 6        | 5        | 4         | 3        | 2         | 1    | 0    |
|-------------------|----------|----------|----------|-----------|----------|-----------|------|------|
| Opération         | "\"==\"" | "\"!=\"" | "\"\>\"" | "\"\>=\"" | "\"\<\"" | "\"\<=\"" |  ?   | ZERO |


-   **CF**: Un drapeau de comparaison de 8 bits contenant retenant
    l\'état après une instruction de comparaison et utilisé pour les
    instructions conditionnelles.

Le tableau ci-dessus montre l\'opérateur représenté par chaque bit. Si
un bit est à 1, cela implique que l\'opérateur a été satisfé par la
dernière comparaison. Le bit à la position **0** est mis à 1 lorsque le
résultat d\'une opération vaut 0. Le bit à la position **1** est mis à 1
dans les cas suivants :

-   soustraction non signée où le premier opérande est inférieur au
    second
-   le second opérande d\'une division ou d\'un modulo est nul


-   **IP**: Un pointeur d\'instruction de 64 bits. Il pointe l\'adresse
    de l\'instruction en cours d\'exécution.

## Instructions

Une instruction est encodée sous 32 bits dans le format suivant :

``` example

[ 8 bits ][ 9 bits ][ 5 bits ][ 5 bits ][ 5 bits ]
  opcode   "unused"    dst       src1      src2

```

Les 8 premiers bits sont utilisés pour encoder l\'opcode de
l\'instruction. Les 9 bits suivants sont utilisés pour donner des
informations supplémentaires dans certaines instructions. Les 15 bits
restants sont utilisés pour représenter les indices des registres
utilisés par l\'instruction.

### Memoire

1.  [**Loads**]

    Une instruction de chargement (\"load\") est formattée de la manière
    suivante : **opcode reg, (base, décalage \[, offset\])**. Le dernier
    offset est optionnel et doit être une valeur immédiate.

    1. **Scalaires**

   | Opcode | Opérandes                     | Description                                                                                                                   |
   |--------|-------------------------------|-------------------------------------------------------------------------------------------------------------------------------|
   | loadu  | U2, (U0, U1 \[, OFFSET\])    | Charge la valeur scalaire non-signée (ou 8 caractères ASCII) depuis la mémoire située en **U0 + U1 + OFFSET** dans le registre **U2**. |
   | loads  | S0, (U0, U1 \[, OFFSET\])    | Charge la valeur scalaire signée depuis la mémoire située en **U0 + U1 + OFFSET** dans le registre **S0**.                   |
   | loadf  | F0, (U0, U1 \[, OFFSET\])    | Charge la valeur scalaire flottante depuis la mémoire située en **U0 + U1 + OFFSET** dans le registre **F0**.               |

2. **Vectoriels**

   | Opcode | Operands                     | Description                                                                                                                              |
   |--------|------------------------------|------------------------------------------------------------------------------------------------------------------------------------------|
   | loadv  | V0, (U0, U1 \[, OFFSET\])   | Charge un bloc de 8 valeurs scalaires non-signées consécutives (ou 64 caractères ASCII) depuis la mémoire située en **U0 + U1 + OFFSET** dans le registre **V0**. |
   | loadt  | T0, (U0, U1 \[, OFFSET\])   | Charge un bloc de 8 valeurs scalaires signées consécutives depuis la mémoire située en **U0 + U1 + OFFSET** dans le registre **T0**.  |
   | loadg  | G0, (U0, U1 \[, OFFSET\])   | Charge un bloc de 8 valeurs flottantes signées consécutives depuis la mémoire située en **U0 + U1 + OFFSET** dans le registre **T0**. |


2.  [**Stores**]

    Une instruction de stockage (\"store\") est formattée de la manière
    suivante : **opcode (base, décalage \[, offset\]), reg**. Le dernier
    offset est optionnel et doit être une valeur immédiate.

    1. **Scalar**

   | Opcode | Opérandes                     | Description                                                                                                                    |
   |--------|-------------------------------|--------------------------------------------------------------------------------------------------------------------------------|
   | storeu | (U0, U1 \[, OFFSET\]), U2    | Stocke la valeur scalaire non-signée (ou 8 caractères ASCII) contenue dans le registre **U2** vers la mémoire située en **U0 + U1 + OFFSET**. |
   | stores | (U0, U1 \[, OFFSET\]), S0    | Stocke la valeur scalaire signée contenue dans le registre **S0** vers la mémoire située en **U0 + U1 + OFFSET**.              |
   | stores | (U0, U1 \[, OFFSET\]), F0    | Stocke la valeur scalaire flottante contenue dans le registre **F0** vers la mémoire située en **U0 + U1 + OFFSET**.          |

2. **Vector**

   | Opcode | Opérandes                     | Description                                                                                                         |
   |--------|-------------------------------|---------------------------------------------------------------------------------------------------------------------|
   | storev | (U0, U1 \[, OFFSET\]), V0    | Stocke le contenu du registre **V0** dans un block de mémoire situé en **U0 + U1 + OFFSET**.                       |
   | storet | (U0, U1 \[, OFFSET\]), T0    | Stocke le contenu du registre **T0** dans un block de mémoire situé en **U0 + U1 + OFFSET**.                       |
   | storeg | (U0, U1 \[, OFFSET\]), G0    | Stocke le contenu du registre **G0** dans un block de mémoire situé en **U0 + U1 + OFFSET**.                       |


### Déplacement de données et conversion de types

1.  [**Data movement**]

| Opcode | Opérandes          | Description                                                                                |
|--------|--------------------|--------------------------------------------------------------------------------------------|
| mov    | ?, ?               | Déplace les bits d'un registre à un autre (peu importe leurs types)                       |
| movu   | U0, U1             | U0 = U1                                                                                    |
| movs   | S0, S1             | S0 = S1                                                                                    |
| movf   | F0, F1             | F0 = F1                                                                                    |
| movv   | V0, V1             | V0 = V1                                                                                    |
| movt   | T0, T1             | T0 = T1                                                                                    |
| movg   | G0, G1             | G0 = G1                                                                                    |
| movui  | U0, IMM.           | U0 = IMM.                                                                                  |
| movsi  | S0, IMM.           | S0 = IMM.                                                                                  |
| movfi  | F0, IMM.           | F0 = IMM.                                                                                  |
| movvi  | V0, { 8 x IMM. }   | V0 = { IMM1., IMM2., IMM3, ... }                                                           |
| movti  | T0, { 8 x IMM. }   | T0 = { IMM1., IMM2., IMM3, ... }                                                           |
| movgi  | G0, { 8 x IMM. }   | G0 = { IMM1., IMM2., IMM3, ... }                                                           |



2.  [**Type conversion**]

| Opcode | Opérandes | Description                                      |
|--------|-----------|--------------------------------------------------|
| cvtus  | S0, U0    | Convertit un entier non signé en entier signé.  |
| cvtsu  | U0, S0    | Convertit un entier signé en entier non signé.  |
| cvtuf  | F0, U0    | Convertit un entier non signé en flottant.      |
| cvtfu  | U0, F0    | Convertit un flottant en entier non signé.      |
| cvtsf  | F0, S0    | Convertit un entier signé en flottant.          |
| cvtfs  | S0, F0    | Convertit un flottant en entier signé.          |

### Arithmétique et logique pour les entiers non signés

| Opcode | Opérandes  | Description                   |
|--------|------------|-------------------------------|
| addu   | U0, U1, U2 | U0 = U1 + U2                  |
| subu   | U0, U1, U2 | U0 = U1 - U2                  |
| mulu   | U0, U1, U2 | U0 = U1 * U2                  |
| divu   | U0, U1, U2 | U0 = U1 / U2                  |
| modu   | U0, U1, U2 | U0 = U1 % U2                  |
| fmau   | U0, U1, U2 | U0 = U0 + U1 * U2             |
| sqrtu  | U0, U1     | U0 = sqrtu(U1)                |
| logu   | U0, U1     | U0 = logu(U1)                 |
| incu   | U0         | U0 = U0 + 1                   |
| decu   | U0         | U0 = U0 - 1                   |
| andu   | U0, U1, U2 | U0 = U1 et U2                 |
| oru    | U0, U1, U2 | U0 = U1 ou U2                 |
| xoru   | U0, U1, U2 | U0 = U1 xor U2                |
| shlu   | U0, U1, U2 | U0 = U1 << U2                 |
| shru   | U0, U1, U2 | U0 = U1 >> U2                 |
| rolu   | U0, U1, U2 | U0 = U1 << U2 \| U1 >> U2     |
| popcntu| U0, U1     | U0 = popcount(U1)             |
| lmbu   | U0, U1     | U0 = leftmostbit(U1)         |

### Arithmétique et logique pour les entiers signés

| Opcode | Opérandes  | Description                   |
|--------|------------|-------------------------------|
| adds   | S0, S1, S2 | S0 = S1 + S2                  |
| subs   | S0, S1, S2 | S0 = S1 - S2                  |
| muls   | S0, S1, S2 | S0 = S1 * S2                  |
| divs   | S0, S1, S2 | S0 = S1 / S2                  |
| mods   | S0, S1, S2 | S0 = S1 % S2                  |
| fmas   | S0, S1, S2 | S0 = S0 + S1 * S2             |
| sqrts  | S0, S1     | S0 = sqrts(S1)                |
| logs   | S0, S1     | S0 = logs(S1)                 |
| ands   | S0, S1, S2 | S0 = S1 et S2                 |
| ors    | S0, S1, S2 | S0 = S1 ou S2                 |
| xors   | S0, S1, S2 | S0 = S1 xor S2                |
| shls   | S0, S1, S2 | S0 = S1 << S2                 |
| shrs   | S0, S1, S2 | S0 = S1 >> S2                 |
| rols   | S0, S1, S2 | S0 = S1 >> S2 \| S1 << S2     |
| popcnts| S0, S1     | S0 = popcount(S1)             |
| lmbs   | S0, S1     | S0 = leftmostbit(S1)         |

### Arithmétique pour les nombres à virgule flottante

| Opcode | Opérandes  | Description                   |
|--------|------------|-------------------------------|
| addf   | F0, F1, F2 | F0 = F1 + F2                  |
| subf   | F0, F1, F2 | F0 = F1 - F2                  |
| mulf   | F0, F1, F2 | F0 = F1 * F2                  |
| divf   | F0, F1, F2 | F0 = F1 / F2                  |
| fmaf   | F0, F1, F2 | F0 = F0 + F1 * F2             |
| sqrtf  | F0, F1     | F0 = sqrtf(F1)                |
| logf   | F0, F1     | F0 = logf(F1)                 |

### Comparaison et contrôle de flux

[**Comparaison**]

| Opcode | Opérandes | Description                                             |
|--------|-----------|---------------------------------------------------------|
| cmpu   | U0, U1    | Met à jour les bits du drapeau **CF** comme indiqué ci-dessous. |
| cmps   | S0, S1    | Met à jour les bits du drapeau **CF** comme indiqué ci-dessous. |
| cmpf   | F0, F1    | Met à jour les bits du drapeau **CF** comme indiqué ci-dessous. |

L'instruction de comparaison met à jour le drapeau **CF** de la manière suivante :

| Condition                                      |
|-----------------------------------------------|
| CF[7] = (U0 == U1) ? 1 : 0                    |
| CF[6] = (U0 != U1) ? 1 : 0                    |
| CF[5] = (U0 > U1) ? 1 : 0                     |
| CF[4] = (U0 >= U1) ? 1 : 0                    |
| CF[3] = (U0 < U1) ? 1 : 0                     |
| CF[2] = (U0 <= U1) ? 1 : 0                    |



[**Contrôle de flux**]

| Opcode | Opérandes | Description                                         |
|--------|-----------|-----------------------------------------------------|
| je     | ADRESSE   | Saute vers ADRESSE si le 7ème bit du drapeau **CF** est à 1. |
| jne    | ADRESSE   | Saute vers ADRESSE si le 6ème bit du drapeau **CF** est à 1. |
| jg     | ADRESSE   | Saute vers ADRESSE si le 5ème bit du drapeau **CF** est à 1. |
| jge    | ADRESSE   | Saute vers ADRESSE si le 4ème bit du drapeau **CF** est à 1. |
| jl     | ADRESSE   | Saute vers ADRESSE si le 3ème bit du drapeau **CF** est à 1. |
| jle    | ADRESSE   | Saute vers ADRESSE si le 2ème bit du drapeau **CF** est à 1. |
| jz     | ADRESSE   | Saute vers ADRESSE si le bit 0 du drapeau **CF** est à 1.  |
| jnz    | ADRESSE   | Saute vers ADRESSE si le bit 0 du drapeau **CF** n'est pas à 1. |

### Entrée/Sortie

| Opcode | Opérandes | Description                                                   |
|--------|-----------|---------------------------------------------------------------|
| outu   | U0        | Affiche l'entier non signé contenu dans **U0**.              |
| outs   | S0        | Affiche l'entier signé contenu dans **S0**.                  |
| outf   | F0        | Affiche le flottant contenu dans **F0**.                     |
| outa   | U0        | Affiche le caractère ASCII stocké dans les 8 bits de poids faible de **U0**. |
| outb   | U0        | Affiche un flux de caractères ASCII à partir de l'adresse mémoire stockée dans **U0**. S'arrête en rencontrant un 0. |
| outx   | U0        | Affiche le contenu de **U0** en hexadécimal.                 |

### Fin du programme

| Opcode | Opérandes | Description                               |
|--------|-----------|-------------------------------------------|
| hlt    | NONE      | Termine l'exécution du programme.         |


## Assembleur

Un fichier assembleur est constitué de deux sections : la section
**data** et la section **code**.

### Règles d\'écriture :

1.  Section \"data\" :

    Déclarée avec \"data:\" ; il ne doit y avoir sur la ligne de
    déclaration que \"data:\", des espaces / tabulations et des
    commentaires

    Format :

    ``` asm
    TYPE NOM VALEUR
    ```

2.  Section \"data\" :

    Déclarée avec \"code:\" ; il ne doit y avoir sur la ligne de
    déclaration que \"codes:\", des espaces / tabulations et des
    commentaires

    Format :

    ``` asm
    OPERANDE ARGUMENTS
    ```

3.  Règles supplémentaires :

    -   @ : symbole pour demander l\'adresse d\'une variable
    -   \$ : symbole pour demander la valeur d\'une variable
    -   \# : symbole de commentaire ; ce qui le suit sera ignoré
        jusqu\'à la fin de la ligne
    -   Les opcodes et les registres ne sont pas sensibles à la casse
    -   Les noms de variables sont sensibles à la casse
    -   La section \"data\" doit être déclarée avant la section \"code\"
        (ou ne pas être déclarée du tout)
    -   Il ne peut y avoir au plus qu\'une section \"data\" et il doit y
        avoir exactement une section \"code\"

4.  Exemples :

    Programme \"Hello World\" :

    ``` asm

      # This is an assembly hello world program

      # The data section holding the program's constants
    data:
      ascii msg "Hello, World!\n"

      # The code section
    code:
      movui u0, @msg
      outb u0
      hlt

    ```

    Générateur d\'une suite de Fibonacci :

    ``` asm

      # Sample program demonstrating how to calculate the Fibonacci series
    data:
      u64 n 10

    code:
      movui u0, 0    # Iteration index
      movui u1, $n   # Number of iterations

      # First two terms of the series
      movui u2, 1
      movui u3, 1

    _loop_entry:
      addu u4, u2, u3   # Calculate the term's value
      outu u4           # Print the value

      # Swap and update the values for next iteration
      movu u2, u3
      movu u3, u4

      incu u0
      cmpu u0, u1
      jl  _loop_entry
      hlt

    ```

# Futur - parallélisme dans le fichier

## Header parallèle

## Header

| Taille en octets | Description                  |
|------------------|------------------------------|
| 8 octets         | Magic Number (ARCHY0.0)      |
| 8 octets         | Taille du header             |
| 8 octets         | Adresse de la section data   |
| 8 octets         | Taille de la section data    |
| 8 octets         | Adresse de la section code   |
| 8 octets         | Taille de la section code    |
| 8 octets         | Adresse de la section threads|
| 8 octets         | Taille de la section threads |
| 8 octets         | Nombre de cœurs requis       |
| 8 octets         | Taille totale du fichier     |

## Threads Section

Cette section contient les adresses des sections parallèles ainsi que les cœurs qui y sont attribués.

| Taille en octets | Description                  |
|------------------|------------------------------|
| 8 octets         | Adresse de la section parallèle |
| 8 octets         | Nombre de cœurs attribués à cette section |

## Instructions liées au parallélisme

| Opcode          | Opérandes   | Description                                                                                       |
|-----------------|-------------|---------------------------------------------------------------------------------------------------|
| parallel_on    | MASK        | Déclare le début d'une zone parallèle, qui utilisera au plus les cœurs indiqués par MASK.       |
| parallel_off   | NONE        | Déclare la fin d'une zone parallèle.                                                             |
| thread_on      | MASK        | Déclare le début d'une section parallèle, qui sera exécutée par les cœurs indiqués par MASK.    |
| thread_off     | NONE        | Déclare la fin d'une section parallèle.                                                          |
| lock_on        | U0          | Tente de prendre le mutex numéro **U0** (reste bloqué jusqu'à obtention du mutex).               |
| lock_off       | U0          | Relâche le mutex numéro **U0**.                                                                  |
| get_core       | U0          | Récupère le numéro du cœur et le stocke dans **U0**.                                             |


## Exemple

Programme de calcule de dotprod sur deux cœurs :

``` asm

# Sample code for computing a dotprod of two arrays
data:
    u64 a 0            # Memory address of array a
    u64 b 200000       # Memory address of array b
    u64 core_number 2
    u64 c 300000       # Memory to store f0

code:
    movui u0, 0       # Array index
    movui u1, 10000   # Number of array elements boundary
    movui u2, 8       # sizeof(f64)
    mulu  u1, u1, u2  # Array size in bytes (loop boundary)
    movfi f0, 0.0     # Accumulator

    movui u3, @a
    movui u4, @b
    movui u20, $c

    # Create two parallel regions

    parallel_on 0b11

    thread_on 0b11

_address_to_jump:

    # Checking if it's core 0 or 1

    get_core u31
    movui u30 0
    cmpu u31, u30
    jne _case_core_two

    # Changing end of array for core 0
    movui u29 $core_number
    divu u1 u1 u29
    cmpu u1 u1
    je _start_work

_case_core_two:

    # Changing first element for core 1
    movui u29 $core_number
    divu u0 u1 u29

_start_work:

    movfi f1, 1.1     # Initializer for a
    movfi f2, 2.2     # Initializer for b

    # Initialize arrays
_init_loop:

    storef (u3, u0), f1
    storef (u4, u0), f2

    addu u0, u0, u2
    cmpu u0, u1
    jl _init_loop

    movui u0, 0

    # Compute dotprod
_dotprod_loop:

    loadf f1, (u3, u0)    # Load 8-byte DP FP value from a's memory
    loadf f2, (u4, u0)    # Load 8-byte DP FP value from b's memory

    fmaf f0, f1, f2       # f0 += f1 * f2

    addu u0, u0, u2       # u0 += 8 bytes
    cmpu u0, u1
    jl _dotprod_loop
    storef (u20, u31), f1

    thread_off

    parallel_off

    movui u0 0
    movui u1 1
    movui u20 $core_number
    movfi f1 0.0

_add_two:
    loadf f2 (u20, u0)
    addf f1, f1, f2
    addu u0, u0, u1
    cmpu u0, u20
    jl _add_two

    outf f0

    hlt

```
