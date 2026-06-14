#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Structs para jogadores e times
struct player {
    int id_player;
    int cards[3];
    int team_id;
};

struct team {
    int id;
    int points;
    int total_score;
};

// Protótipos das funções
void shuffle(int *p);
int translate_suit(int card_id);
int translate_face(int card_id);
void sort(int matrix, char *sorted_faces[]);
int is_valid_card(char *card, struct player *player, char *faces[], char *suits[], int round_power[], char *sorted_faces[]);
int round_winner(int round_power[], struct team *points_A, struct team *points_B, int valor);
int truco(int *valor, struct team *time_adversario, struct team *time_atual);

int main(){
    // Define as cartas, pontos máximos, faces e naipes
    int cards[40];
    int max_points = 12;
    
    char *faces[] = {"4", "5", "6", "7", "Q", "J", "K", "A", "2", "3"};
    char *suits[] = {"o", "e", "c", "p"};
    char *suitsStyle[] = {"♦", "♠", "♥", "♣"};
    
    char round_cards[4][4]; 
    int round_power[4];

    
    struct team teams[2] = {
        {0, 0, 0},
        {1, 0, 0} 
    };

    int dealer = 0; 

    srand(time(NULL)); 

    // Loop principal do jogo 
    while(teams[0].total_score < max_points && teams[1].total_score < max_points){
        
        // MODIFICAÇÃO 2: Reset de pontos usando o vetor
        teams[0].points = 0; 
        teams[1].points = 0;

        char *sorted_faces[10]; 
        for(int i = 0; i < 10; i++){ 
            sorted_faces[i] = faces[i];
        }
        
        int matrix;
        struct player players[4]; 

        for(int i = 0; i < 40; i++){ 
            cards[i] = i; 
        }

        shuffle(cards); 

        int top_deck = 0; 

        for(int i = 0; i < 4; i++){ 
            if (i%2 == 0) {
                players[i].team_id = 0; 
            } else {
                players[i].team_id = 1; 
            }

            for(int j = 0; j < 3; j++){
                players[i].cards[j] = cards[top_deck];
                top_deck++;
            }
        }

        matrix = cards[top_deck];
        sort(matrix, sorted_faces);

        printf("\n==================================\n");
        printf("Embaralha: Jogador %d\n", dealer + 1); 
        printf("O Vira é: %s%s\n", faces[translate_face(matrix)], suitsStyle[translate_suit(matrix)]);
        printf("A Manilha é: %s\n", sorted_faces[9]);
        printf("==================================\n\n");

        for(int i = 0; i < 4; i++){
            printf("Jogador %d (Time %c): Cartas: ", i+1, (players[i].team_id == 0) ? 'A' : 'B');
            players[i].id_player = i+1;
            
            printf("%s%s, ", faces[translate_face(players[i].cards[0])], suitsStyle[translate_suit(players[i].cards[0])]);
            printf("%s%s, ", faces[translate_face(players[i].cards[1])], suitsStyle[translate_suit(players[i].cards[1])]);
            printf("%s%s\n", faces[translate_face(players[i].cards[2])], suitsStyle[translate_suit(players[i].cards[2])]);
        }  
        
        int starter = (dealer + 1) % 4;

        // MODIFICAÇÃO 3: Verificação de pontos usando o vetor
        while(teams[0].points < 2 && teams[1].points < 2){ 

            int valor_rodada = 1;
            for(int i = 0; i < 4; i++) round_power[i] = -1;

            int t = 0;

            for(int i = 0;  i < 4 ; i++){
                int current_player = (starter+i) % 4;
                int valid = 0;
                
 
                while(valid == 0){
                    printf("\n-------------------------------------------\n");
                    printf("Cartas: ");
                    
                    for(int k = 0; k < 3; k++){
                        if(players[current_player].cards[k] >= 0){
                            printf("%s%s; ", faces[translate_face(players[current_player].cards[k])], suitsStyle[translate_suit(players[current_player].cards[k])]);
                        } 
                    }
                    printf("Jogada do Jogador %d: ", players[current_player].id_player);
                    scanf("%3s", round_cards[current_player]);
                    
                    valid = is_valid_card(round_cards[current_player], &players[current_player], faces, suits, round_power, sorted_faces);
                        
                    if(valid == 0) {
                        printf("Carta invalida ou já jogada! Tente novamente.\n");
                    } else if(valid == 2){
                        int adversario_idx = (players[current_player].team_id == 0) ? 1 : 0;
                        
                        if(truco(&valor_rodada, &teams[adversario_idx], &teams[players[current_player].team_id])){
                            valid = 0; 
                        } else {
                            t = 1;
                        }
                    }

                    printf("-------------------------------------------\n");
                }
                if(t > 0){break;};
            }

            if(t == 0){
                starter = round_winner(round_power, &teams[0], &teams[1], valor_rodada);
            }
            printf("Placar do jogo: Time A: %d, Time B: %d\n", teams[0].points, teams[1].points);
        }

        // MODIFICAÇÃO 6: Atualização do placar geral
        if(teams[0].points >= 2) {
            teams[0].total_score++;
            printf("\n>>> Time A ganhou a rodada! <<<\n");
        } else {
            teams[1].total_score++;
            printf("\n>>> Time B ganhou rodada! <<<\n");
        }
        printf("PLACAR GERAL: Time A: %d | Time B: %d\n", teams[0].total_score, teams[1].total_score);

        dealer = (dealer + 1) % 4;
    }

    printf("\nFIM DE JOGO! os ganhadores veceram os patos %s!\n", (teams[0].total_score >= max_points) ? "Time A" : "Time B");
    return 0;
}

// Faz o embaralhamento das cartas 
void shuffle(int *p){ 
    for(int i = 39; i > 0; i--){
        int r = rand() % (i + 1); 
        int temp = p[i];
        p[i] = p[r];
        p[r] = temp;
    } 
}

// Ordena as faces das cartas, colocando a manilha (carta mais forte) no final do array
void sort(int matrix, char *sorted_faces[]){ 
    int vira_index = translate_face(matrix);
    int manilha_index = (vira_index + 1) % 10;
    char *manilha_face = sorted_faces[manilha_index];
    
    for(int i = manilha_index; i < 9; i++){
        sorted_faces[i] = sorted_faces[i+1];
    }
    sorted_faces[9] = manilha_face;
}

//Faz a tradução do nipe e do valor da carta retornando a sua posição correspondente 
int translate_face(int card_id) { return card_id / 4; }
int translate_suit(int card_id) { return card_id % 4; }

//Verifica se a carta jogada pelo player é valida
int is_valid_card(char *card, struct player *player, char *faces[], char *suits[], int round_power[], char *sorted_faces[]){
    char received_face = card[0];
    char received_suit = card[1];

    for(int i = 0; i < 3; i++){
        if(player->cards[i] == -1) continue;

        int idx_face = translate_face(player->cards[i]);
        int idx_suit = translate_suit(player->cards[i]);

        char player_face = faces[idx_face][0]; 
        char player_suit = suits[idx_suit][0]; 
 
        if(toupper(received_face) == toupper(player_face) && received_suit == player_suit){
            
            int base_strength = 0;
            for(int f = 0; f < 10; f++){
                if(sorted_faces[f][0] == player_face){
                    base_strength = f;
                    break;
                }
            }
            
            int final_power = base_strength * 10;
            
            if(base_strength == 9) {
                final_power += idx_suit;
            }

            round_power[(player->id_player - 1)] = final_power;
            
            player->cards[i] = -1; 
            return 1;
        } 
    }

    if(received_face == 't' || received_face == 's' || received_face == 'n' || received_face == 'd' ){
        return 2;
    }

    return 0;
}

int truco(int *valor, struct team *time_adversario, struct team *time_atual) {
    
    if (time_atual->total_score == 11) {
        printf("\n[ERRO] Voce pediu truco na mao de 11! Seu time perdeu a partida.\n");
        time_adversario->total_score = 12;
        return 0;
    }

    int proximo_valor = 0;
    if (*valor == 1) proximo_valor = 3;
    else if (*valor == 3) proximo_valor = 6;
    else if (*valor == 6) proximo_valor = 9;
    else if (*valor == 9) proximo_valor = 12;
    else {
        printf("Ja esta no valor maximo.\n");
        return 1; 
    }

    printf("\n>>> PEDIDO DE TRUCO! O jogo passara a valer %d pontos. <<<\n", proximo_valor);
    printf("Adversario, voce aceita? (s = sim / n = nao): ");
    
    char resposta;
    scanf(" %c", &resposta); 

    if (toupper(resposta) == 'S') {
        *valor = proximo_valor;
        printf("O jogo agora vale %d.\n", *valor);
        return 1; 
    } else {
        printf("Adversario correu! Seu time ganha a rodada.\n");
        time_atual->points = 2; 
        return 0; 
    }
}
    
int round_winner(int round_power[], struct team *points_A, struct team *points_B, int valor){  
    int p = -1;
    int id_player = -1;

    for(int i = 0; i < 4; i++){
        if(round_power[i] > p){
            p = round_power[i];
            id_player = i; 
        }
    }

    printf("\n");
    if(id_player % 2 == 0){
        printf("Time A venceu a rodada.\n");
        points_A->points += valor;
    } else {
        printf("Time B venceu a rodada.\n");
        points_B->points += valor;
    }

    return id_player;
}




//FEITO

// bug da manilha 4 ser pior que 3 corrigido

// na rodada: rodar o starter pelo winner implementado

// starter e dealer implementados, o starter é o próximo jogador do dealer, e o starter da próxima rodada é o vencedor da rodada anterior



// PENDENTES


// opção de truco, truco vale 3, retruco vale 6, vale 9, vale 12

// opção de aceitar ou recusar o truco, se recusar perde a rodada, se aceitar o valor do truco aumenta e o jogo continua

// truco na mão de 11 não é permitido, o jogador só pode pedir truco se tiver 10 pontos ou menos, quem pedir truco, perde o campeonato

