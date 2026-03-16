#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#define MAX_SONGS 100

typedef struct {
    char name[50];
    char place[50];
    int age;
    int is_creator; // 1 = yes, 0 = no
} User;


// Struct to store song details
typedef struct {
    char artist[50];
    char song[50];
    char language[20];
    int year;
    char genre[20];
    int plays;  // This is for tracking how many times a song has been played
} Song;

Song songs[MAX_SONGS];  // Array to store song data
int total_songs = 0;  // Track the total number of songs in the array

void save_user_info(User u);
void save_user_info(User u) {
    FILE *file = fopen("users.csv", "a");  // Open in append mode
    if (file == NULL) {
        printf("Error: Could not open users.csv\n");
        return;
    }

    // Write user data in CSV format
    fprintf(file, "%s,%s,%d,%d\n", u.name, u.place, u.age, u.is_creator);

    fclose(file);
}


int ask_user(){
    
    int get_reco;
    printf("\nDo you want some music recommendations? (1 for Yes / 0 for No): ");
    scanf("%d", &get_reco);
    getchar();  // clear newline

    if (get_reco == 0) {
        printf("Thanks for contributing to our BEATBINGE! See you soon:)\n");
        return 0;
    }
    return 1;
}

void show_trending_songs() {
    printf("\nTrending Songs Right Now:\n");

    // Display top trending songs, could be sorted by plays or popularity
    for (int i = 1; i < 6 && i < total_songs; i++){
        // You can sort these by the number of plays or choose to display randomly
        printf("%d. %s by %s\n", i, songs[i].song, songs[i].artist);
    }
}

// Function to read the songs from the CSV file and store them in the songs array
void load_songs() {
    FILE *file = fopen("songs.csv", "r");
    if (file == NULL) {
        printf("Error: Could not open the file.\n");
        return;
    }

    char line[256];
    // Read each line from the CSV file and store song data
    while (fgets(line, sizeof(line), file)) {
        // Split each line by commas to extract the song's attributes
        char *token = strtok(line, ",");
        strcpy(songs[total_songs].artist, token);

        token = strtok(NULL, ",");
        strcpy(songs[total_songs].song, token);

        token = strtok(NULL, ",");
        strcpy(songs[total_songs].language, token);

        token = strtok(NULL, ",");
        songs[total_songs].year = atoi(token);

        token = strtok(NULL, ",");
        strcpy(songs[total_songs].genre, token);

        token = strtok(NULL, ",");
        songs[total_songs].plays = atoi(token);

        total_songs++;
    }

    fclose(file);
}

void add_to_playlist(int selected_index) {
    FILE *playlist = fopen("playlist.csv", "a");
    if (playlist == NULL) {
        printf("Error: Could not open playlist.csv\n");
        return;
    }

    // Write song details to playlist file
    Song s = songs[selected_index];
    fprintf(playlist, "%s,%s,%s,%d,%s,%d\n", 
            s.artist, s.song, s.language, s.year, s.genre, s.plays);

    fclose(playlist);

    }

// Function to recommend songs based on user preferences
void recommend_songs(const char *language, const char *artist, int year) {
    int found = 0;
    int matched_indexes[100]; // to store indexes of matching songs

    printf("\nRecommended Songs:\n");

    for (int i = 0; i < total_songs; i++) {
        if ((strcmp(language, "") == 0 || strcmp(songs[i].language, language) == 0) &&
            (strcmp(artist, "") == 0 || strcmp(songs[i].artist, artist) == 0) &&
            (year == 0 || songs[i].year == year)) {

            matched_indexes[found] = i;  // store original index
            printf("%d. %s\n", found + 1, songs[i].song);
            found++;
        }
    }

    if (found == 0) {
        printf("No songs match your preferences.\n");
        int trending_choice;
        printf("Would you like to explore trending songs? (1 for Yes / 0 for No): ");
        scanf("%d", &trending_choice);
        if (trending_choice == 1) {
            show_trending_songs();  // Call function to display trending songs
        }
        else{
            printf("Thankyou for using BEATBINGE!");
        }
    return;
    }

    // Ask if user wants to add to playlist
    int choice;
    printf("\nDo you want to add any song to your playlist? (1 for Yes / 0 for No): ");
    scanf("%d", &choice);
    getchar(); // clear newline

    if (choice == 1) {
        int song_choice;
        printf("Enter the number of the song you want to add: ");
        scanf("%d", &song_choice);
        getchar();

        if (song_choice >= 1 && song_choice <= found) {
            int original_index = matched_indexes[song_choice - 1];
            add_to_playlist(original_index);
            printf("Song added to your playlist!\nThankyou for using BEATBINGE!\n");
        } else {
            printf("Invalid song number.\n");
        }
    }
    else{
    printf("Thankyou for using BEATBINGE!");
    }
}

User get_user_info() {
    User u;
    int valid_name = 0;
while (!valid_name) {
    printf("Enter your name: ");
    scanf(" %[^\n]", u.name);

    valid_name = 1;  // Assume valid initially
    for (int i = 0; u.name[i] != '\0'; i++) {
        if (!(isalpha(u.name[i]) || u.name[i] == ' ')) {
            valid_name = 0;
            printf("Error: Enter a valid name (letters and spaces only).\n");
            break;
        }
    }
}

    int valid_place = 0;
while (!valid_place) {
    printf("Enter your place: ");
    scanf(" %[^\n]", u.place);

    valid_place = 1;  // Assume valid
    for (int i = 0; u.place[i] != '\0'; i++) {
        if (!(isalpha(u.place[i]) || u.place[i] == ' ')) {
            valid_place = 0;
            printf("Error: Enter a valid place name (letters and spaces only).\n");
            break;
        }
    }
}


    char age_input[10];
int valid_age = 0;

while (getchar() != '\n'); // clear leftover newline
while (!valid_age) {
    printf("Enter your age: ");
    fgets(age_input, sizeof(age_input), stdin);

    if (sscanf(age_input, "%d", &u.age) == 1 && u.age > 0) {
        valid_age = 1;
    } else {
        printf("Please enter a valid age (numbers only).\n");
    }
}


    printf("Are you a creator? (1 for Yes / 0 for No): ");
    scanf("%d", &u.is_creator);
    getchar();

    return u;
}

void add_song_to_library() {
    FILE *file = fopen("songs.csv", "a");
    if (file == NULL) {
        printf("Error: Could not open songs.csv\n");
        return;
    }

    Song new_song;

    printf("\n Enter details of the song you want to add:\n");

    printf("Artist name: ");
    scanf(" %[^\n]", new_song.artist);

    printf("Song title: ");
    scanf(" %[^\n]", new_song.song);

    printf("Language: ");
    scanf(" %[^\n]", new_song.language);

    printf("Year of release: ");
    scanf("%d", &new_song.year);

    printf("Genre: ");
    scanf(" %[^\n]", new_song.genre);

    new_song.plays = 0; // New songs start with 0 plays

    // Save to CSV
    fprintf(file, "%s,%s,%s,%d,%s,%d\n", 
            new_song.artist, new_song.song, new_song.language, new_song.year, new_song.genre, new_song.plays);

    fclose(file);

    printf("Song added to the library successfully!\n");
}



int main() {

    printf("        ------BEATBINGE------        \n");
    User current_user = get_user_info();
    save_user_info(current_user);

    printf("\nWelcome to BEATBINGE %s!!\n", current_user.name);
    if (current_user.is_creator)
        printf(" Glad to know you are a creator.You can add your music to BEATBINGE!! \n");
    else
        printf(" Let's find some songs you'll enjoy!\n");

        if (current_user.is_creator) {
            char choice;
            printf("\nDo you want to add a song to the library? (y/n): ");
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                add_song_to_library();
                if (!ask_user()) {
                    return 0;  // Exit the program if user doesn't want recommendations
                }
            }
            else{
                if (!ask_user()) {
                    return 0;  // Exit the program if user doesn't want recommendations
                }
            }
        }
        
    // Load songs from the CSV
    load_songs();

    
    char preferred_language[20];
    char preferred_artist[50];
    int preferred_year;

    printf("\nEnter preferred language : ");
    fgets(preferred_language, sizeof(preferred_language), stdin);
    preferred_language[strcspn(preferred_language, "\n")] = '\0';   // Allow space in input

    printf("Enter preferred artist : ");
    fgets(preferred_artist, sizeof(preferred_artist), stdin);
    preferred_artist[strcspn(preferred_artist, "\n")] = '\0';

    printf("Enter preferred year : ");
    scanf("%d", &preferred_year);

    // Call the recommend_songs function to show results based on user input
    recommend_songs(preferred_language, preferred_artist, preferred_year);
    return 0;

}
    
    
