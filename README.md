# Sistema de Recomendação de Filmes 🎥
<div align="center">
    <img src="./img/recF.png" width="400">
</div>

## 📚 Sumário
- [🌟 Introdução](#introdução)
- [🎯 Objetivos](#-objetivos)
- [🛠️ Regras de Implementação](#-regras-de-implementação)
- [🗂️ Estrutura do Projeto](#-estrutura-do-projeto)
- [🧪 Metodologia](#-metodologia)
  - [Visão Geral](#visão-geral)
  - [1 - Pré-processamento](#1---pré-processamento)
  - [2 - Jaccard](#2---jaccard)
- [📊 Análises e Conclusões](#-análises-e-conclusões)
  - [Escolha da Métrica de Similaridade](#escolha-da-métrica-de-similaridade-índice-de-jaccard)
  - [Resultados](#resultados)
  - [📈 Análise de Complexidade](#-análise-de-complexidade)
  - [✅ Conclusões Gerais](#-conclusões-gerais)
  - [🖥️ Ambiente de Testes](#-ambiente-de-testes)
- [🚀 Compilação e Execução](#-compilação-e-execução)
- [👥 Autores](#-autores)
- [📚 Referências](#-referências)


## 🌟INTRODUÇÃO

No cenário digital atual, a vasta quantidade de informações e opções disponíveis pode ser esmagadora para os usuários. Plataformas de streaming de vídeo, lojas de e-commerce e redes sociais enfrentam o desafio de apresentar conteúdo relevante e personalizado em meio a um volume gigantesco de dados. Nesse contexto, sistemas de recomendação emergem como ferramentas indispensáveis, atuando como filtros inteligentes para guiar os usuários através de um universo de possibilidades.

A capacidade de prever o interesse de um usuário por um determinado item — seja um filme, um produto ou um artigo — é fundamental para aprimorar a experiência digital, aumentar o engajamento e impulsionar a descoberta de novos conteúdos. Essa previsão é influenciada por uma variedade de fatores, incluindo o histórico de consumo do usuário, suas interações com outros itens, o comportamento de usuários com gostos semelhantes e até mesmo as características intrínsecas dos próprios itens.

Além de otimizar a experiência individual do usuário, as consequências de um sistema de recomendação eficaz vão além da satisfação pessoal, impactando diretamente o sucesso das plataformas digitais. Ao conectar usuários a conteúdos que realmente os engajam, esses sistemas contribuem para o aumento do tempo de permanência, a fidelização de clientes e a geração de receita. A modelagem precisa do comportamento e das preferências do usuário é, portanto, uma ferramenta vital, e deve ser integrada de forma inteligente no design de qualquer plataforma que almeje oferecer uma experiência personalizada. Essa abordagem permite a identificação de tendências de consumo e a aplicação de estratégias proativas para aprimorar a relevância das recomendações.


## 🎯 OBJETIVOS

O trabalho final da disciplina de Algoritmos e Estruturas de Dados I, proposto pelo professor Michel Pires, tem como objetivo a construção de um **sistema de recomendação de filmes** utilizando a base de dados **MovieLens 25M**.

O sistema deve ser capaz de, a partir do **perfil dos usuários e das características dos filmes avaliados**, sugerir **filmes similares** que possam ser do interesse de um determinado usuário. Para isso, é necessário realizar um **pré-processamento robusto da base de dados** e aplicar **algoritmos de recomendação baseados em similaridade**.

Entre as etapas principais estão:

* **Pré-processamento da base `ratings.csv`** para filtrar e organizar os dados;
* Escolha e implementação de um **algoritmo de recomendação**, como:

  * Similaridade do cosseno;
  * Distância Euclidiana;
  * Índice de Jaccard;
* Geração das recomendações para usuários definidos no arquivo `explore.dat`;
* Escrita das recomendações no arquivo `output.dat` em formato exigido.

---

### 🛠️ REGRAS DE IMPLEMENTAÇÃO

O professor definiu algumas **regras obrigatórias** que devem ser seguidas na implementação:

* O **arquivo principal da base de dados** deve ser `ratings.csv`, do conjunto **MovieLens 25M**;

* O sistema deve considerar apenas:

  * Usuários com **pelo menos 50 avaliações distintas**;
  * Filmes avaliados por **pelo menos 50 usuários diferentes**;

* Avaliações duplicadas ou inconsistentes devem ser removidas;

* O arquivo `input.dat` deve ser gerado com o seguinte formato:

  ```
  usuario_id item_id1:nota1 item_id2:nota2 item_id3:nota3 ...
  ```

* Arquivos devem seguir o seguinte padrão:

| Arquivo       | Local       | Formato       | Descrição                                           |
| ------------- | ----------- | ------------- | --------------------------------------------------- |
| `input.dat`   | `datasets/` | Texto (UTF-8) | Base processada de avaliações por usuário           |
| `explore.dat` | `datasets/` | Texto (UTF-8) | Lista de usuários para os quais gerar recomendações |
| `output.dat`  | `outcome/`  | Texto (UTF-8) | Saída com as recomendações para cada usuário        |


* A compilação deve ser feita usando um `Makefile` com as regras `make`, `make run` e `make clean`;
* O sistema deve compilar e executar corretamente no **Ubuntu 24.04 LTS com `g++ 13+`**;

---

## 🗂️ ESTRUTURA DO PROJETO


```
ProjetoRecomendacao/
│
├── datasets/
│   ├── explore.dat       # Arquivo de usuários para gerar recomendações
│   ├── input.dat         # Saída do pré-processamento (formato usuário filme:nota)
│   ├── movies.csv        # Metadados dos filmes (opcional para recomendações avançadas)
│   └── ratings.csv       # Base de dados principal (avaliações brutas)
│
├── docs/
│   └── Fluxograma.png    # Diagrama do fluxo do sistema
│
├── include/
│   ├── jaccard/
│   │   ├── CalculadorSimilaridade.hpp
│   │   ├── GerenciadorProcessos.hpp
│   │   ├── Jaccard.hpp
│   │   ├── LeitorPerfis.hpp
│   │   └── ProcessadorRecomendacoes.hpp
│   │
│   └── preprocess/
│   |    ├── agrupar.hpp
│   |    ├── ler.hpp
│   |    └── preprocess.hpp
|   |
│   └── config.h
│
├── src/
│   ├── jaccard/
│   │   ├── CalculadorSimilaridade.cpp
│   │   ├── GerenciadorProcessos.cpp
│   │   ├── Jaccard.cpp
│   │   ├── LeitorPerfis.cpp
│   │   └── ProcessadorRecomendacoes.cpp
│   │
│   ├── preprocess/
│   │   ├── agrupar.cpp
│   │   ├── ler.cpp
│   │   └── preprocess.cpp
│   │
│   └── main.cpp          # Ponto de entrada do programa
│
├── outcome/
│   └── output.dat        # Resultado final das recomendações
│
├── Makefile              # Contém regras para compilar e executar
└── README.md             # Documentação do projeto (instruções, requisitos)
```


## 🧪 METODOLOGIA

Nesta seção, apresentamos a metodologia adotada no desenvolvimento do projeto, detalhando as etapas de pré-processamento dos dados e os algoritmos de recomendação utilizados. Discutimos os critérios que motivaram essas escolhas, bem como as alterações realizadas ao longo do processo, que impactaram diretamente o desempenho e influenciaram na decisão final.

### VISÃO GERAL

**O projeto é estruturado em duas etapas principais que determinam seu desempenho:**

1. **Pré-processamento de dados**:  
   Processamento inicial do arquivo `ratings.csv` do MovieLens para construção de uma base de dados consistente, aplicando filtros de qualidade e removendo inconsistências.

2. **Cálculo de similaridades**:  
   Implementação do algoritmo de recomendação baseado na Similaridade de Jaccard, responsável por analisar padrões de avaliação e identificar relações entre usuários e filmes.


### 1 - PRÉ-PROCESSAMENTO
#### FLUXO GERAL

<div align="center">
    <img src="./img/Fluxograma.png" >
</div>


### **ETAPAS DO PRÉ-PROCESSAMENTO**

**a) LEITURA E REMOÇÃO DE DUPLICATAS**  
A gente fez a leitura dos dados do `ratings.csv` usando `mmap` - essa opção é muito mais rápida que `fstream` porque mapeia o arquivo direto na memória e acessa com ponteiros, ficando até 10x mais rápido. Depois fizemos um parsing manual pra pegar só os dados importantes e transformar em objetos `Avaliacao` (tipo `(1, 122, 3.5)`).  

A remoção de duplicatas antes usava `unordered_set`, mas dava muito overhead por causa das colisões do hash. Quando mudamos pra `sort + unique`, melhorou 10x porque evita as colisões e aproveita melhor o cache da CPU.

**b) FILTRAGEM**  
No `ler.cpp` contamos quantas vezes cada usuário e filme aparecia usando `unordered_map` - a gente escolheu isso porque a busca é O(1) e a pré-alocação (`reserve()`) evita redimensionamentos. Depois filtramos só os usuários com 50+ avaliações e filmes avaliados por 50+ pessoas, usando `unordered_set` aqui pra consulta rápida.

**c) AGRUPAMENTO**  
Essa parte pega as avaliações filtradas e junta tudo por usuário. Usamos `unordered_map` de novo porque inserir e buscar é O(1), então fica eficiente. No final, cada usuário vira uma linha com todos seus filmes e notas, tipo:  
`123 456:4.0 789:3.5`  

**d) ESCRITA PARALELIZADA**  
Essa foi a única parte que paralelizamos, porque a escrita em disco é o gargalo. Pensamos em usar OpenMP, mas como tem dependência entre os chunks, optamos por `fork()` - que é um paralelismo "manual".  

*Detalhe importante*: O formato do arquivo (`usuario_id filme:nota`) tem que ser exatamente como o professor pediu, senão o algoritmo de recomendação não funciona depois. A parte do fork ficou meio complicada porque precisa dividir os dados em pedaços iguais e juntar depois, mas valeu a pena pelo ganho de velocidade.

**e) FUNÇÃO PREPROCESSAR - O COORDENADOR DO PROCESSO**  

Essa função é o "cérebro" que organiza todo o pré-processamento. Ela faz um passo a passo bem definido:  

1. **Preparação**:  
   Cria dois `unordered_map` (um pra usuários, outro pra filmes) e já faz uma pré-alocação inteligente de memória baseada no `maxLinhas`. Isso evita que o programa fique realocando memória toda hora e deixa tudo mais rápido.

2. **Leitura e Filtragem Inicial**:  
   Chama a função `lerAvaliacoes` que a gente já explicou - ela lê o CSV, remove duplicatas e já vai contando quantas vezes cada usuário e filme aparecem.  

3. **Filtragem Final**:  
   Usa `filtrarPorContagemMinima` pra pegar só os usuários que avaliaram 50+ filmes e os filmes que foram avaliados por 50+ pessoas. Aqui a gente usa `unordered_set` pra conseguir verificar rapidamente se um usuário ou filme é válido.

4. **Agrupamento e Escrita**:  
   Junta tudo com `agruparAvaliacoesPorUsuario` (que transforma as avaliações no formato usuário → lista de filmes) e finalmente escreve no arquivo `input.dat` usando a escrita paralelizada com `fork()`.

*Por que é importante*:  
- Ela garante que todas as etapas acontecem na ordem certa  
- Controla o fluxo de dados entre as funções  
- Mostra um resumo no final (quantos usuários foram processados)  
- A pré-alocação de memória aqui evita gargalos nas etapas seguintes  

*Observação*: O parâmetro `maxLinhas` é útil pra testar com subsets dos dados - a gente pode rodar só nas primeiras 1000 linhas pra ver se tá tudo funcionando antes de processar o arquivo completo.


### 2 - JACCARD
O índice de Jaccard mede a similaridade entre dois conjuntos. A fórmula é:

$$
J(A,B) = \frac{|A \cap B|}{|A \cup B|}
$$

Ou seja:
* **Numerador:** número de elementos em comum.
* **Denominador:** número total de elementos únicos nos dois conjuntos.

Exemplo:

```
A = {1, 2, 3}
B = {2, 3, 4}

A ∩ B = {2, 3} → tamanho = 2  
A ∪ B = {1, 2, 3, 4} → tamanho = 4

J(A, B) = 2 / 4 = 0.5
```

 **a)Leitura de Perfis**
   
Como o começo do processo do algoritmo foi feita uma função [lerPerfis(const string& caminho)](https://github.com/otaviohiratsuka/Trabalho-Final/blob/bc3c98c0894718d50e1f9653d5e06b386fa5c1a6/src/jaccard/LeitorPerfis.cpp#L7-L44). Que tem o objetivo de ler um arquivo de perfis de usuários, onde cada linha representa os filmes que um usuários assistiu, e transformar em um `unordered_map<int, vector<int>>`.
   * Entrada -> `caminho`: nome do arquivo (ex: ratings.csv).
   * Saída -> Um `Perfil`, que é um alias para:
     ```
        using Perfil = unordered_map<int, vector<int>>;
     ```

**O que a função faz?**

A função lê cada linha e extrai o `uid` e os filmes assistidos (`filmeId: rating` -> pega só o `filmeId`). Depois ordena e remove duplicatas de filmes e salva no `unordered_map`

```
    while (getline(arq, linha)){
        if (linha.empty()) continue;
        
        stringstream ss(linha);
        int uid;
        ss >> uid;


        string par;
        vector<int> filmes;
        while(ss >> par){
            size_t pos = par.find(":");
            if (pos != string::npos){
                try {
                    int filmeId = stoi(par.substr(0, pos));
                    filmes.push_back(filmeId);
                } catch (const exception&) {
                    continue;
                }
            }
        }
        
        sort(filmes.begin(), filmes.end());
        filmes.erase(unique(filmes.begin(), filmes.end()), filmes.end());
        
        perfis[uid] = move(filmes);
    }
```
É importante citar o uso de `vector<int>` ordenado ao invés do `unordered_set`, que permite calcular interseção e união em tempo linear O(m + n) usando duas pontas (`i`, `j`), dentro da função `jaccard`. O `unordered_set` se mostra mais lento para interseções frequentes porque exige hashing e alocação dinâmica.

**b) Função Jaccard**
   
   Para implementar a similaridade de jaccard usamos a função [double jaccard](https://github.com/otaviohiratsuka/Trabalho-Final/blob/af82099ee590dcaf3b7018e25b30ab74b846c4e1/src/Jaccard.cpp#L60-L89](https://github.com/otaviohiratsuka/Trabalho-Final/blob/a5f28f1879b5d8163d932deb9de3c34e6646a2d8/src/jaccard/CalculadorSimilaridade.cpp#L4)). Essa função calcula a similaridade de jaccard entre dois usuários com base nos filmes que assistiram. Dois vetores ordenados de `int`, representando filmes assistidos por dois usuários são as entradas e o valor `double` entre 0 e 1, indicando a similaridade Jaccard é a saída.
   As duas variáveis (`i`, `j`) são usadas para percorrer os vetores ordenados. Após isso, é contado quantos filmes estão em comum (**intersecção**). E calcula a união com: $uniao = |a| + |b| - interssec$

   Retornando:

$$
   J(a,b) = \frac{intersec}{uniao}
$$

```
while (i < a.size() && j < b.size()) {
    if (a[i] == b[j]) intersec++;
    else if (a[i] < b[j]) i++;
    else j++;
}
```
Nota-se que existe um cálculo eficiente da interseção com ponteiros duplos. Isso trás vantagens para algoritmo, pois utiliza o fato de os vetores estarem ordenados para calcular interseção sem estrutura adicional, sem `set`, sem `map`, sem `find()`. Se mostrando muito mais rápido do que comparar elementos individualmente ou usar estruturas como `std::set_intersection`. Assim, tornando o cálculo de Jaccard tão rápido quanto possível com vetores ordenados.

**c) Processar Chunk**

Para processar um subconjunto (chunk) dos exploradores, calcular similaridade, gerar recomendação e salvar os resultados em arquivo temporário, foi criado a função [processarChunk](https://github.com/otaviohiratsuka/Trabalho-Final/blob/a5f28f1879b5d8163d932deb9de3c34e6646a2d8/src/jaccard/ProcessadorRecomendacoes.cpp#L9). O `exploradoresVec` é o vetor de pares `<uid, filmes>` dos usuários a serem recomendados. `PerfisVec` são todos os usuários com seus filmes. `startIdx`, `endIdx` é o intervalo de índices para esse processo. `tempFileName` é o nome do arquivo onde o processo salvará o resultado.
Na saída o `tempFileName` escreve as recomendações no formato:
```
UID filme1 filme2 filme3 ...
```
A função para cada explorador, compara com todos os perfis, calcula jaccard e mantém os top 10 mais semelhantes (`topK`).

```
    for (int idx = startIdx; idx < endIdx; ++idx) {
        const auto& [uidExplorador, filmesExplorador] = exploradoresVec[idx];
        
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> topK;
        
        for(const auto& [uid, filmes] : perfisVec){
            if (uid == uidExplorador) continue;
            
            double sim = jaccard(filmesExplorador, filmes);
```

Agora, para os top-10 semelhantes é usado: `filmeScore[filme] += similaridade;`, que recomenda os filmes que o explorado **ainda não viu** e o peso do filme depende da similaridade com o vizinho. Depois a função pega os top-10 filmes com maior score  `priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> topFilmes;`, usa o heap para manter os 10 melhores e salva recomendações no arquivo temporário.

O uso de `priority_queue` para manter top-K elementos entrega alguns bônus como Evita ordenar todos os candidatos (custo **O(n log n)**) — só mantém os 10 melhores (`K = 10`) com custo **O(log K)** por inserção. Usado duas vezes: Para armazenar os top-K usuários mais similares e para armazenar os top-10 filmes mais recomendados. O resultado disso é que Minimiza o custo de ordenação e maximiza a eficiência na seleção dos melhores vizinhos e melhores filmes.

Nessa mesma função também usamos a filtragem pro similaridade mínima (`if (sim >= MIN_SIMILARITY)`) que ignora comparações com usuários muito diferentes. Isso evita populamento da heap com valores irrelevantes e economiza tempo e memória. Isso trás uma redução de custo computacional com comparações inúteis.

**d) Recomendação usando Jaccard**

No [recomendarJaccard()](https://github.com/otaviohiratsuka/Trabalho-Final/blob/bc3c98c0894718d50e1f9653d5e06b386fa5c1a6/src/jaccard/Jaccard.cpp#L9-L28](https://github.com/otaviohiratsuka/Trabalho-Final/blob/a5f28f1879b5d8163d932deb9de3c34e6646a2d8/src/jaccard/Jaccard.cpp#L9)), o objetivo é controlar a leitura dos arquivos, paralelizar o processo usando `fork()`, e combinar os resultados em um arquivo final.

* `caminhoInput`: perfis base.
* `caminhoExplore`: perfis dos exploradores.
* `caminhoOutput`: arquivo de saída final com as recomendações.
* E cria um arquivo com recomendações de filmes por usuário.

Na leitura, carrega os dois arquivos em unordered_map.
```
auto perfis = lerPerfis(caminhoInput);
auto exploradores = lerPerfis(caminhoExplore);
```
E converte para `vector<pair<>>`: `for (const auto& [uid, filmes] : exploradores) exploradoresVec.emplace_back(uid, filmes);
`, assim facilita a indexação e divisão de dados.

Para a melhor otimização do código, usamos o paralelismo com o `fork()`, que divide o vetor de exploradores em `P` partes (onde `P = núcleos da CPU`) e com isso cada processo executa `processaChunk(...)` com seu pedaço. E os resultados são salvos em arquivos temporários separados.
```
pid_t pid = fork();
if (pid == 0) {
    processarChunk(...);
    exit(0);
}
```

As vantagens do `fork()` são claras, é notório que a carga de trabalho é dividida proporcionalmente entre os núcleos disponíveis. Cada processo cuida de um pedaço dos exploradores, sem compartilhamento de memória (evita race conditions). 
O ganho de performance proporcional ao número de núcleos da máquina (`P`). Complexidade total reduzida de `O(E x U x m)` para `O((E x U x m) / P). Onde:

* `E` = número de exploradores
* `U` = número de usuários no dataset
* `m` = tamanho médio de filmes por usuário
* `K`= número de vizinhos

Para cada explorador, é comparado com `U` usuários -> **O(U x m)**. Atualiza top-k -> **O(log K)**. Percorre os filmes dos top-K → **O(K × m)**. Ordena os top 10 → **O(log 10)**. 

**Total por explorador:**
`O(U × m + K × m + log K) ≈ O(U × m)` (pois `K` é constante)

**Total geral (sem paralelização):**
`O(E × U × m)`

**Com paralelização (`P` processos):**
`O((E × U × m) / P)`


Ainda na função `recomendarJaccard` é perceptivel que o algoritmo evita recomputar dados com conversão inicial de `unordered_map` para `vector<pair<>>`. Ao converter para `vector`, é possivel indexar diretamente (`[i]`), o que é mais rápido que iterar `unordered_map`, também melhora o desempenho do paralelismo, pois os dados são contíguos na memória. 

Com essa otimizações é entregue o menor overhead de acesso e melhor cache-locality durante execução. 


Finalizando, para unir os resultados a função junta todos os arquivos temporários no arquivo de saída final.

```
for (const string& tempFileName : arquivosTemp) {
    ifstream tempFile(tempFileName);
    while (getline(tempFile, linha)) out << linha << "\n";
}
```

O salvamento assíncrono em arquivos temporários mostra que cada processo escreve seu resultado em um arquivo temporário isolado e evita problemas de concorrência na escrita e dispensa locks. A execução paralela mais segura e simples, sem riscos de race conditions em disco.



---

## 📊 ANÁLISES E CONCLUSÕES
O desenvolvimento deste sistema de recomendação de filmes não se limitou à implementação das funcionalidades, mas também envolveu uma profunda análise comparativa das diferentes métricas de similaridade e uma avaliação rigorosa do desempenho. As decisões tomadas ao longo do projeto foram guiadas pela busca por eficiência computacional, qualidade das recomendações e robustez geral do sistema.

### Escolha da Métrica de Similaridade: Índice de Jaccard

Conforme detalhado na seção de Metodologia, diversas métricas de similaridade foram consideradas para este projeto, incluindo a Distância Euclidiana, Similaridade do Cosseno e o Índice de Jaccard. Após extensivos testes e análises dos resultados preliminares, o Índice de Jaccard foi o escolhido para a implementação final do algoritmo de recomendação.</p>

### RESULTADOS
<p>Para avaliar a eficiência computacional do sistema de recomendação, que processou 102.492 usuários, o algoritmo foi executado múltiplas vezes. Os tempos médios de execução, juntamente com o uso de memória, foram calculados para garantir uma análise consistente do desempenho, mitigando variações pontuais do ambiente de execução.

Os resultados médios obtidos são os seguintes:
* Tempo Médio de Pré-processamento: 5041.33 ms

* Tempo Médio de Recomendação: 2858 ms

* Tempo Médio Total: 7899.67 ms

* Uso Médio de Memória (VmRSS): 281656 kB

Esses resultados indicam uma boa eficiência computacional, especialmente na etapa de recomendação, que é a parte central do algoritmo. A capacidade do sistema de lidar com volumes consideráveis de dados em um tempo aceitável e com um consumo de memória gerenciável demonstra a robustez da implementação.

### 📈 ANÁLISE DE COMPLEXIDADE
Entender a complexidade computacional é fundamental para prever como o sistema se comportará com o aumento do volume de dados e para identificar possíveis gargalos. Esta análise foca no tempo de execução e uso de memória (espaço), utilizando a notação Big O (O maiúsculo) para descrever a taxa de crescimento.

**1 - PRÉ-PROCESSAMENTO**

O pré-processamento tem complexidade **O(N log N)** no pior caso, devido à ordenação para remoção de duplicatas (`sort + unique`). As outras etapas são lineares:  

- Leitura e parsing: **O(N)**  
- Contagem de usuários/filmes: **O(N)**  
- Filtragem com hash maps: **O(1)** por operação  
- Agrupamento: **O(N)**  

A escrita paralelizada divide o trabalho em **P processos**, reduzindo o tempo real, mas mantendo a complexidade total. A pré-alocação de memória (`reserve()`) evita operações O(N) de realocação dinâmica.  

*Resumo*: Eficiente para grandes volumes de dados (como o MovieLens 25M), com operações otimizadas para evitar gargalos.

**2 - RECOMENDAÇÃO**

Variáveis Importantes:
* `N` -> Total de perfis (usuários no arquivo base)
* `E` -> Número de exploradores (usuários a recomendar)
* `m` -> Média de filmes por usuário
* `U` -> Igual a `N` (perfis contra os quais se compara)
* `K` -> Quantidade de vizinhos mais similares (fixo: 10)
* `P` -> Número de processos (paralelos com `fork()`)

  - `lerPerfis`: Chamada duas vezes (para base e exploradores) = `O(N × m log m) + O(E × m log m)
≈ O((N + E) × m log m)`
  - `processarChunk`: `O((E × U × m) / P)`
  - `recomendarJaccard`: `O(E)`
 
**Complexidade Total de** `Jaccard.cpp`:

$$
    O\Big((N + E) \cdot m \ \log \ m + \frac{E \cdot N \cdot m }{P}\Big)
$$

Observação: **99% do tempo está no** `processarChunk`, resto se mostra mais leve

### ✅ CONCLUSÕES GERAIS

A complexidade Final do Sistema (pré-processamento + recomendação):

$$
O \Big(N \ \log \ N + (N + E) \cdot m \ \log \ m + \frac{E \cdot N \cdot m}{P}\Big)
$$ 

**Conclusão Técnica**

O sistema de recomendação desenvolvido apresenta um custo computacional escalável, mas com um gargalo principal concentrado no cálculo de similaridade Jaccard entre cada usuário explorador e todos os perfis da base. Mesmo com a implementação de paralelismo via fork(), esse custo pode crescer consideravelmente com o aumento do número de usuários (`U` e `E`) e da quantidade máxima de filmes por perfil (`m`).

**Possíveis Próximas Otimizações**

Se quiser escalar para milhões de usuários ou usar em tempo real, considere:
* Redução do espaço de comparação (ex: comparar só com um subconjunto amostrado dos perfis).
* Indexação aproximada:
  - MinHash + LSH para estimar Jaccard rapidamente
  - Reduz custo para sublinear (dependendo da qualidade esperada)
* Paralelismo com `std::thread` ou `OpenMP` (evita overhead de `fork()` e I/O de arquivos)
* Cálculo incremental: se os perfis mudam pouco, recompute parcialmente.


### 🖥️ AMBIENTE DE TESTES
<p>Os testes de desempenho e a execução do sistema foram realizados no seguinte ambiente: 
    
* **Sistema Operacional:** Ubuntu 24.04.1 LTS (Noble Numbat)
* **Kernel Linux:** 6.11.0-29-generic
* **Processador (CPU):** 11th Gen Intel(R) Core(TM) i5-11400H @ 2.70GHz (12 núcleos)
* **Arquitetura:** x86_64
* **Memória RAM:** 7.5 GiB
* **Dataset:** MovieLens 25M (utilizando um subconjunto de 102.492 usuários para os testes de performance)

</p>

## 🚀 COMPILAÇÃO E EXECUÇÃO
</hr>
A algorítmo disponibilizado possui um arquivo Makefile que realiza todo o procedimento de compilação e execução. Para tanto, temos as seguintes diretrizes de execução:

| Comando                |  Função                                                                                           |                     
| -----------------------| ------------------------------------------------------------------------------------------------- |
|  `make clean`          | Apaga a última compilação realizada contida na pasta build                                        |
|  `make`              	 | Compila o código-fonte conforme as regras definidas no Makefile | 
|  `make run`            | Executa a compilação do programa utilizando o gcc, e o resultado vai para a pasta build, além de em seguida executar o programa da pasta build após a realização da compilação |

## 👥 AUTORES

Esse trabalho foi realizado pelos seguintes integrantes.

[Fabricio Quintilhiano;](https://github.com/F-Quintilhiano)

[Jean Pedro Oliveira;](https://github.com/Jeanaraga)

[Lorena Ávila;](https://github.com/Lorenaavila20)

[Otávio Hiratsuka;](https://github.com/otaviohiratsuka)

Alunos do curso de Engenharia da Computação no [CEFET-MG](https://www.cefetmg.br)


## 📚 REFERÊNCIAS

- GROUMLENS. *MovieLens 25M Dataset*. Disponível em: [https://grouplens.org/datasets/movielens/25m/](https://grouplens.org/datasets/movielens/25m/). Acesso em: 13 jul. 2025.

- SCIENCEDIRECT. *Jaccard Similarity – Tópicos em Ciência da Computação*. Disponível em: [https://www-sciencedirect-com.translate.goog/topics/computer-science/jaccard-similarity](https://www-sciencedirect-com.translate.goog/topics/computer-science/jaccard-similarity?_x_tr_sl=en&_x_tr_tl=pt&_x_tr_hl=pt&_x_tr_pto=tc). Acesso em: 13 jul. 2025.

- RESNICK, Paul et al. *GroupLens: An Open Architecture for Collaborative Filtering of Netnews*. In: *Proceedings of the 1994 ACM Conference on Computer Supported Cooperative Work (CSCW)*. 1994. p. 175–186. Disponível em: [https://dl.acm.org/doi/10.1145/192844.192905](https://dl.acm.org/doi/10.1145/192844.192905). Acesso em: 13 jul. 2025.

- CPPREFERENCE. *std::priority_queue*. Disponível em: [https://en.cppreference.com/w/cpp/container/priority_queue](https://en.cppreference.com/w/cpp/container/priority_queue). Acesso em: 13 jul. 2025.

- MAN7. *fork(2) – Linux Manual Pages*. Disponível em: [https://man7.org/linux/man-pages/man2/fork.2.html](https://man7.org/linux/man-pages/man2/fork.2.html). Acesso em: 13 jul. 2025.

- MAN7. *mmap(2) – Linux Manual Pages*. Disponível em: [https://man7.org/linux/man-pages/man2/mmap.2.html](https://man7.org/linux/man-pages/man2/mmap.2.html). Acesso em: 13 jul. 2025.

</div>
