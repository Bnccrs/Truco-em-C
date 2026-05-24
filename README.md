# 🃏 Truco em C - Protótipo do Simulador

Este é um projeto em desenvolvimento de um simulador do clássico jogo de cartas **Truco** feito em linguagem C. O objetivo atual é estruturar a lógica interna do jogo, incluindo a distribuição de cartas, definição de equipes, embaralhamento e a identificação do "Vira" e das "Manilhas".

---

## Status

**Em Desenvolvimento** Este código é um protótipo inicial. Estamos atualizando o repositório constantemente para implementar a lógica completa das rodadas, inteligência artificial (se aplicável), apostas de tentos ("Truco!") e a interface de jogabilidade.

---

## 🛠️ Funcionalidades Atuais (Protótipo)

O estado atual do código já gerencia com sucesso as seguintes etapas mecânicas:
* **Baralho de Truco:** Criação de um deck "limpo" com 40 cartas (removendo 8, 9, 10 e Curingas).
* **Algoritmo de Embaralhamento:** Implementação do método *Fisher-Yates* para garantir um embaralhamento pseudo-aleatório justo.
* **Divisão de Equipes:** Alocação automática de 4 jogadores divididos em 2 equipes (Time A e Time B).
* **Distribuição de Cartas:** Entrega de 3 cartas por jogador a partir do topo do deck.
* **Mecânica do Vira:** Sorteio da carta que define a força da rodada e identificação automática das manilhas baseadas no valor correspondente.

---

## Como Funciona a Lógica Interna

Para facilitar a manipulação, as cartas são tratadas numericamente de `0` a `39`:
* **Faces (`card_id / 4`):** Mapeadas na ordem tradicional de força do truco (`4, 5, 6, 7, Q, J, K, A, 2, 3`).
* **Naipes (`card_id % 4`):** Mapeados seguindo a ordem de relevância (`o` - Ouros, `e` - Espadas, `c` - Copas, `p` - Pica-fumo/Paus).

---

## Como Executar o Protótipo

Se você quiser testar a geração do deck e a distribuição das cartas atual, siga os passos abaixo:

### Pré-requisitos
Você precisará de um compilador C (como o `gcc`) instalado na sua máquina.

### Passo a Passo

1. **Clone o repositório:**
   ```bash
   git clone [https://github.com/SEU-USUARIO/NOME-DO-REPOSITORIO.git](https://github.com/SEU-USUARIO/NOME-DO-REPOSITORIO.git)
   cd NOME-DO-REPOSITORIO
