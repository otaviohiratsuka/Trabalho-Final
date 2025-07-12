# Sistema de Recomendação de Filmes 🎥
<div align="center">
    <img src="./img/recF.png" width="400">
</div>

## INTRODUÇÃO

<p>No cenário digital atual, a vasta quantidade de informações e opções disponíveis pode ser esmagadora para os usuários. Plataformas de streaming de vídeo, lojas de e-commerce e redes sociais enfrentam o desafio de apresentar conteúdo relevante e personalizado em meio a um volume gigantesco de dados. Nesse contexto, sistemas de recomendação emergem como ferramentas indispensáveis, atuando como filtros inteligentes para guiar os usuários através de um universo de possibilidades.

A capacidade de prever o interesse de um usuário por um determinado item — seja um filme, um produto ou um artigo — é fundamental para aprimorar a experiência digital, aumentar o engajamento e impulsionar a descoberta de novos conteúdos. Essa previsão é influenciada por uma variedade de fatores, incluindo o histórico de consumo do usuário, suas interações com outros itens, o comportamento de usuários com gostos semelhantes e até mesmo as características intrínsecas dos próprios itens.

Além de otimizar a experiência individual do usuário, as consequências de um sistema de recomendação eficaz vão além da satisfação pessoal, impactando diretamente o sucesso das plataformas digitais. Ao conectar usuários a conteúdos que realmente os engajam, esses sistemas contribuem para o aumento do tempo de permanência, a fidelização de clientes e a geração de receita. A modelagem precisa do comportamento e das preferências do usuário é, portanto, uma ferramenta vital, e deve ser integrada de forma inteligente no design de qualquer plataforma que almeje oferecer uma experiência personalizada. Essa abordagem permite a identificação de tendências de consumo e a aplicação de estratégias proativas para aprimorar a relevância das recomendações.</p>


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

## 🧪 METODOLOGIA

Nesta seção, apresentamos a metodologia adotada no desenvolvimento do projeto, detalhando as etapas de pré-processamento dos dados e os algoritmos de recomendação utilizados. Discutimos os critérios que motivaram essas escolhas, bem como as alterações realizadas ao longo do processo, que impactaram diretamente o desempenho e influenciaram na decisão final.

### VISÃO GERAL

**O projeto é estruturado em três etapas principais que determinam seu desempenho:**

1. **Pré-processamento de dados**:  
   Processamento inicial do arquivo `ratings.csv` do MovieLens para construção de uma base de dados consistente, aplicando filtros de qualidade e removendo inconsistências.

2. **Cálculo de similaridades**:  
   Implementação do algoritmo de recomendação baseado na Similaridade de Jaccard, responsável por analisar padrões de avaliação e identificar relações entre usuários e filmes.

3. **Geração de recomendações**:  
   Produção das recomendações personalizadas com base nas similaridades calculadas, formatadas conforme os requisitos de saída do projeto.

### 1 - PRÉ-PROCESSAMENTO
#### FLUXO GERAL

<div align="center">
    <img src="./img/Fluxograma.png" width="400">
</div>

### Jaccard
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


1. Leitura de Perfis
   
    Como o começo do processo do algoritmo foi feita uma função [lerPerfis(const string& caminho)](https://github.com/otaviohiratsuka/Trabalho-Final/blob/af82099ee590dcaf3b7018e25b30ab74b846c4e1/src/Jaccard.cpp#L21-L58). Que tem o objetivo de ler um arquivo de perfis de usuários, onde cada linha representa os filmes que um usuários assistiu, e transformar em um `unordered_map<int, vector<int>>`.
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

2. Função Jaccard
   
   Para implementar a similaridade de jaccard usamos a função [double jaccard](https://github.com/otaviohiratsuka/Trabalho-Final/blob/af82099ee590dcaf3b7018e25b30ab74b846c4e1/src/Jaccard.cpp#L60-L89). Essa função calcula a similaridade de jaccard entre dois usuários com base nos filmes que assistiram. Dois vetores ordenados de `int`, representando filmes assistidos por dois usuários são as entradas e o valor `double` entre 0 e 1, indicando a similaridade Jaccard é a saída.
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

3. Processar Chunk

Para processar um subconjunto (chunk) dos exploradores, calcular similaridade, gerar recomendação e salvar os resultados em arquivo temporário, foi criado a função [processarChunk](https://github.com/otaviohiratsuka/Trabalho-Final/blob/af82099ee590dcaf3b7018e25b30ab74b846c4e1/src/Jaccard.cpp#L90-L188). O `exploradoresVec` é o vetor de pares `<uid, filmes>` dos usuários a serem recomendados. `PerfisVec` são todos os usuários com seus filmes. `startIdx`, `endIdx` é o intervalo de índices para esse processo. `tempFileName` é o nome do arquivo onde o processo salvará o resultado.
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

4. Recomendação usando Jaccard

No [recomendarJaccard()](https://github.com/otaviohiratsuka/Trabalho-Final/blob/af82099ee590dcaf3b7018e25b30ab74b846c4e1/src/Jaccard.cpp#L189-L266), o objetivo é controlar a leitura dos arquivos, paralelizar o processo usando `fork()`, e combinar os resultados em um arquivo final.

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

Finalizando, para unir os resultados a função junta todos os arquivos temporários no arquivo de saída final.

```
for (const string& tempFileName : arquivosTemp) {
    ifstream tempFile(tempFileName);
    while (getline(tempFile, linha)) out << linha << "\n";
}
```


## ANÁLISES E CONCLUSÕES

  
## COMPILAÇÃO E EXECUÇÃO
</hr>
A algorítmo disponibilizado possui um arquivo Makefile que realiza todo o procedimento de compilação e execução. Para tanto, temos as seguintes diretrizes de execução:

| Comando                |  Função                                                                                           |                     
| -----------------------| ------------------------------------------------------------------------------------------------- |
|  `make clean`          | Apaga a última compilação realizada contida na pasta build                                        |
|  `make`              	 | Compila o código-fonte conforme as regras definidas no Makefile | 
|  `make run`            | Executa a compilação do programa utilizando o gcc, e o resultado vai para a pasta build, além de em seguida executar o programa da pasta build após a realização da compilação |

## AUTORES

Esse trabalho foi realizado pelos seguintes integrantes.

[Fabricio Quintilhiano;](https://github.com/F-Quintilhiano)

[Jean Pedro Oliveira;](https://github.com/Jeanaraga)

[Lorena Ávila;](https://github.com/Lorenaavila20)

[Otávio Hiratsuka;](https://github.com/otaviohiratsuka)

Alunos do curso de Engenharia da Computação no [CEFET-MG](https://www.cefetmg.br)




















  
</div>
