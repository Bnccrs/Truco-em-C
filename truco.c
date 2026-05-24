#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct player {
    int cards[3];
    int team;
};

struct team {
    int points;
    int value;
};

void shuffle(int *p);

void card_suit(int card_id, int matrix);

int main(){
    int cards[40];

    int matrix;
    
    struct player players[4]; 

    for(int i = 0; i < 40; i++){ 
        cards[i] = i; 
    }

    srand(time(NULL)); 
    shuffle(cards);

    int top_deck = 0; 

    for(int i = 0; i < 4; i++){
        if (i == 0 || i == 2) {
            players[i].team = 1; 
        } else {
            players[i].team = 2;
        }

        for(int j = 0; j < 3; j++){
            players[i].cards[j] = cards[top_deck];
            top_deck++;
        }
        
    }

    matrix = cards[top_deck];

    for(int i = 0; i < 4; i++){
        printf("Player %d (Time %c): Cards: ", i, (players[i].team == 1) ? 'A' : 'B');
        
        card_suit(players[i].cards[0], matrix);
        printf(", ");
        card_suit(players[i].cards[1], matrix);
        printf(", ");
        card_suit(players[i].cards[2], matrix);
        printf("\n");
    }

    printf("O vira é: ");
    card_suit(matrix, matrix);
    printf("\n");
    printf("As manilha é: ");
    card_suit(matrix+1, matrix);
    card_suit(matrix+2, matrix);
    card_suit(matrix+3, matrix);
    card_suit(matrix+4, matrix);
    printf("\n");

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


void card_suit(int card_id, int matrix){//ordenar poder de cartas

    char *faces[] = {"4", "5", "6", "7", "Q", "J", "K", "A", "2", "3", "\0"};
    char *suits[] = {"o", "e", "c", "p"};

    for(int i = 0; *faces[i] == '\0'; i++){
        char *z;
        if(i == (matrix/4) + 1){
            z = faces[i];
            for(int j = i+1; *faces[j] == '\0'; j++){
                faces[j-1] = faces[j];
            }
            faces[9] = z;
        }
    }

    int face = card_id / 4;
    int suit = card_id % 4;

    printf("%s%s ", faces[face], suits[suit]);
}