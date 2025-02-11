#ifndef reveal_h
#define reveal_h

typedef struct {
    bool show_all;
    bool show_details;
    char *path;
} Reveal_Struct;

void Myreveal_cmd(char *input);
Reveal_Struct parse_Myreveal_cmd(char *input);
char* resolve_path(char *input_path);
struct dirent** Direc_read(char *myreslov_path, bool show_all, int *entry_count);
void sort_MyEntries_lexicographically(struct dirent **MyEntries, int count);
void display_MyEntries(struct dirent **MyEntries, int count, bool show_details, char *myreslov_path);
void delete_freeMyEntries(struct dirent **MyEntries, int count);
void Permsns_Cnvt(mode_t mode, char *prmsns);

void Display_Details(struct stat file_stat, const char *name);



#endif









