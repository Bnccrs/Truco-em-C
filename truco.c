#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

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

void shuffle(int *p);
int translate_suit(int card_id);
int translate_face(int card_id);
void sort(int matrix, char *sorted_faces[]);
int is_valid_card(char *card, struct player *player, char *faces[], char *suits[], int round_power[]);
void round_winner(int round_power[], struct team *points_A, struct team *points_B);

int main(){
    int cards[40];
    int max_points = 12;
    
    char *faces[] = {"4", "5", "6", "7", "Q", "J", "K", "A", "2", "3"};
    char *suits[] = {"o", "e", "c", "p"};
    
    char round_cards[4][4]; 
    int round_power[4];

    struct team team_0 = {0, 0, 0};
    struct team team_1 = {1, 0, 0};

    int starter = 0;
    srand(time(NULL)); 

    while(team_0.total_score < max_points && team_1.total_score < max_points){
        
        team_0.points = 0;
        team_1.points = 0;

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
        printf(
            "O Vira e: %s%s\n", 
            faces[translate_face(matrix)], 
            suits[translate_suit(matrix)]
        );
        printf("A Manilha (mais forte) e: %s\n", sorted_faces[9]);
        printf("==================================\n\n");

        for(int i = 0; i < 4; i++){
            printf(
                "Player %d (Time %c): Cards: ", i+1, 
                (players[i].team_id == 0) ? 'A' : 'B'
            );
            players[i].id_player = i+1;
            
            printf(
                "%s%s, ", 
                faces[translate_face(players[i].cards[0])], 
                suits[translate_suit(players[i].cards[0])]
            );
            printf(
                "%s%s, ", 
                faces[translate_face(players[i].cards[1])], 
                suits[translate_suit(players[i].cards[1])]
            );
            printf(
                "%s%s\n", 
                faces[translate_face(players[i].cards[2])], 
                suits[translate_suit(players[i].cards[2])]
            );
        }  
        
        

        while(team_0.points < 2 && team_1.points < 2){

            for(int i = 0; i < 4; i++) round_power[i] = -1;

            for(int i = 0;  i < 4 ; i++){
                int current_player = (starter+i) % 4;
                int valid = 0;
 
                while(valid == 0){
                    printf("Carta Jogador %d válida: ", players[current_player].id_player);
                    scanf("%3s", round_cards[current_player]); 
                    valid = is_valid_card(round_cards[current_player], 
                        &players[current_player], faces, suits, round_power);
                    if(!valid) {
                        printf("Carta inválida ou já jogada! Tente novamente.\n");
                    }
                }
            }

            round_winner(round_power, &team_0, &team_1);
            printf("Placar Parcial da Mão: Time A: %d, Time B: %d\n", team_0.points, team_1.points);

            if (starter == 3) {
                starter = 0;
            } else{
                starter++;
            }
            
        }

        if(team_0.points >= 2) {
            team_0.total_score++;
            printf("\n>>> Time A ganhou a mão! <<<\n");
        } else {
            team_1.total_score++;
            printf("\n>>> Time B ganhou a mão! <<<\n");
        }
        printf("PLACAR GERAL: Time A: %d | Time B: %d\n", 
            team_0.total_score, team_1.total_score
        );
    }

    printf("\nFIM DE JOGO! os ganhadores veceram os patos %s!\n", 
        (team_0.total_score >= max_points) ? "Time A" : "Time B"
    );
    return 0;
}

void shuffle(int *p){ 
    for(int i = 39; i > 0; i--){
        int r = rand() % (i + 1); 
        int temp = p[i];
        p[i] = p[r];
        p[r] = temp;
    } 
}

void sort(int matrix, char *sorted_faces[]){
    int vira_index = translate_face(matrix);
    int manilha_index = (vira_index + 1) % 10;
    char *manilha_face = sorted_faces[manilha_index];
    
    for(int i = manilha_index; i < 9; i++){
        sorted_faces[i] = sorted_faces[i+1];
    }
    sorted_faces[9] = manilha_face;
}

int translate_face(int card_id) { return card_id / 4; }
int translate_suit(int card_id) { return card_id % 4; }

int is_valid_card(char *card, struct player *player, char *faces[], char *suits[], int round_power[]){
    char face_recebida = card[0];
    char suit_recebida = card[1];

    for(int i = 0; i < 3; i++){
        if(player->cards[i] == -1) continue;

        int idx_face = translate_face(player->cards[i]);
        int idx_suit = translate_suit(player->cards[i]);

        char face_player = faces[idx_face][0]; 
        char naipe_player = suits[idx_suit][0]; 
 
        if(face_recebida == face_player && suit_recebida == naipe_player){
            round_power[(player->id_player - 1)] = player->cards[i];
            
            player->cards[i] = -1; 
            return 1;
        }
    }
    return 0;
}
    
void round_winner(int round_power[], struct team *points_A, struct team *points_B){  
    int p = -1;
    int id_player = -1;

    for(int i = 0; i < 4; i++){
        if(round_power[i] > p){
            p = round_power[i];
            id_player = i;
        }
    }

    printf("\n----------------------------------\n");
    if(id_player % 2 == 0){
        printf("Time A venceu a rodada.\n");
        points_A->points++;
    } else {
        printf("Time B venceu a rodada.\n");
        points_B->points++;
    }
    printf("----------------------------------\n");
}

// rodar o starter pelo winner 
// total_score até 12, então reiniciar partida, o primeiro a chegar em 12 ganha o campeonato
// opção de truco, truco vale 3, retruco vale 6, vale 9, vale 12
// opção de aceitar ou recusar o truco, se recusar perde a rodada, se aceitar o valor do truco aumenta e o jogo continua
// truco na mão de 11 não é permitido, o jogador só pode pedir truco se tiver 10 pontos ou menos, quem pedir truco, perde o campeonato
// ultima alteração do programa: exibir completamente o valor das cartas do jogo, e não char, o naipe e a face, ex: 4 de ouros, 5 de espadas, etc.
