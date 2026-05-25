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

int is_valid_card(char *card, struct player *player, char *faces[], char *suits[]);

int round_winner(char *round_cards[], struct team *score_team);

int main(){
    int cards[40];
    int max_points = 12;
    
    char *faces[] = {"4", "5", "6", "7", "Q", "J", "K", "A", "2", "3"};
    char *suits[] = {"o", "e", "c", "p"};
    char *round_cards[4];

    struct team team_0 = {0, 0, 0};
    struct team team_1 = {1, 0, 0};

    int starter = 0;

    while(team_0.total_score < max_points && team_1.total_score < max_points){
        
        char *sorted_faces[10];
        for(int i = 0; i < 10; i++){
            sorted_faces[i] = faces[i];
        }
        
        int matrix;
        struct player players[4]; 

        for(int i = 0; i < 40; i++){ 
            cards[i] = i; 
        }

        srand(time(NULL)); 
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

        printf(
            "O Vira e: %s%s\n", 
            faces[translate_face(matrix)], 
            suits[translate_suit(matrix)]
        );

        printf("A Manilha (mais forte) e: %s\n\n", sorted_faces[9]);

        for(int i = 0; i < 4; i++){
            printf("Player %d (Time %c): Cards: ", i+1, (players[i].team_id == 0) ? 'A' : 'B');
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
        
        starter = (starter == 3) ? 0 : starter++;

        //rodada
        while(team_0.points != 2 || team_1.points != 2){
            //Validar se a carta escolhida no scanf é válida, ou seja, se o jogador tem a carta e se ela não foi jogada ainda.

            for(int i = 0;  i < 3 ; i++){
                int current_player = (starter+i) % 4;
                int valid = 0;
                
                while(valid == 0){
                    printf("Carta Jogador %d válida: ", players[current_player].id_player);
                    scanf("%s", round_cards[current_player] );
                    valid = is_valid_card(round_cards[current_player], &players[current_player], faces, suits);
                }
                printf("Carta valida.");
                
            }
        }
    }



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

int translate_face(int card_id){
    return card_id / 4;
}

int translate_suit(int card_id){
    return card_id % 4;
}


//função que valida carta jogada, se ela existe ou não na mão do jogador e se ela já foi jogada ou não
int is_valid_card(char *card, struct player *player, char *faces[], char *suits[]){

    char face_recebida = card[0];
    char suit_recebida = card[1];

    //verifica se o player tem carta e se carta existe:
    for(int i = 0; i < 3; i++){

        int idx_face = translate_face(player->cards[i]);
        int idx_suit = translate_suit(player->cards[i]);

        char face_player = faces[idx_face][0]; 
        char naipe_player = suits[idx_suit][0]; 
 
        if(face_recebida == face_player && suit_recebida == naipe_player){
            return 1;
        }
    }
    return 0;
}

//função maior carta do round, comparar as cartas jogadas e retornar o time vencedor da rodada
int round_winner(char *round_cards[], struct team *score_team){
    
}
