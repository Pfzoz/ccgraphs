Autores: Pedro Henrique Ferreira Zoz & Felipe Fonseca Rosa

# Grafos C++

## O programa utiliza uma representação 'matricial'.

Essa representação é diferente pois em vez de utilizar vetores/listas convencionais, utiliza a classe Dicionário para facilitar algumas relações entre os vértices e arestas.

## Status de Grafo

Algumas informações são status exibidos dinâmicamente na tela, como se o grafo atual é euleriano ou não.

## Salvamento/Carregamento

É possível carregar/salvar o grafo a partir da opção 9 e 8 do menu. É possível especificar o nome do arquivo.

## Menu Interativo

O menu possuí opções diferentes para as funcionalidades do programa, como buscas, ciclo eueleriano, etc.

## Estrutura

O código fonte é dividido entre *dictionary.hpp*, header para a classe Dicionário e suas devidas funcionalidades. Tem-se o *ccgraphs.hpp*, header para a estrutura e funcionalidade dos grafos, e por fim *graphs.cc*, o programa principal que contém o fluxo principal do processo geral da aplicação.