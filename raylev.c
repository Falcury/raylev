#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;

#pragma pack(push,1)
typedef struct event_t {
    u32 DES0;
    u32 DES4;
    u32 DES8;
    u32 ETA;
    u32 commands;
    u32 command_labels;
    u16 field_18;
    u16 field_1A;
    u32 field_1C;
    u16 field_20;
    u16 field_22;
    s32 field_24; // is_event_linked? (?)
    s32 xpos;
    s32 ypos;
    u32 field_30;
    u16 field_34;
    u16 field_36;
    u16 field_38;
    u16 field_3A;
    u16 field_3C;
    u16 field_3E;
    s16 xspeed;
    s16 yspeed;
    u16 field_44;
    s16 next_command_index;
    s16 command_par;
    s16 command_par2; // action (?)
    u16 field_4C;
    u16 field_4E;
    u16 field_50;
    s16 command_par4;
    u16 field_54;
    s16 command_par3; // timer (?)
    u16 field_58;
    u16 field_5A;
    u16 field_5C;
    u16 field_5E;
    u16 obj_type;
    u8 field_62; // some_tile_type (?)
    u8 field_63;
    u8 field_64;
    u8 field_65;
    u8 field_66;
    u8 field_67;
    s8 offset_bx;
    s8 offset_by;
    s8 anim_index;
    u8 field_6B;
    u8 subetat;
    u8 etat;
    u8 spawn_subetat;
    u8 spawn_etat;
    u8 curr_command;
    u8 field_71;
    u8 field_72;
    u8 field_73; // etat updated (?)
    u8 offset_hy;
    u8 follow_sprite;
    u8 hitp;
    u8 spawn_hitp;
    u8 group;
    u8 hit_sprite;
    u8 field_7A;
    u8 field_7B;
    u8 field_7C;
    u8 field_7D;
    u8 field_7E;
    u8 field_7F;
    u8 field_80;
    u8 movement_mode;
    u8 field_82;
    u8 field_83;
} event_t;

typedef struct rgb_t {
    u8 red, green, blue;
} rgb_t;

typedef struct color_table_t {
    rgb_t rgb[256];
} color_table_t;

typedef struct maptile_t {
    u16 texture_id;
    u8 tile_type;
    u8 field_3;
    u8 transparency;
    u8 field_5;
} maptile_t;

typedef struct x_texture_t {
    u8 unk[256];
} x_texture_t;

typedef struct lev_t {
    // Header block
    u32 event_block_offset;
    u32 texture_block_offset;
    u16 level_width;
    u16 level_height;
    color_table_t color_tables[3];

    // Map block
    u8 unk_always_2;
    maptile_t* map_tiles;
    u8 unk_map_byte;
    u8 bg_image;
    u32 bg_DES;

    // X Mode Textures block
    u32 num_x_textures;
    u32 x_unk_table_size;
    x_texture_t* x_textures;
    u8 x_textures_checksum;
    u32 x_indices[1200];
    u8* x_unk_table;
    u8 x_unk_checksum;
    u32 x_unk_offsets[1200];

    // Normal Mode Textures block
    u32 texture_offsets[1200];
    u32 num_textures;
    u32 num_opaque_textures;
    u32 texture_data_size;
    u8* textures;
    u8 textures_checksum;

    // Events block
    u16 num_events;
    u16* event_links;
    event_t* events;
    // @Todo unfinished

} lev_t;

#pragma pack(pop)

FILE* logfile;

static inline void report(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(logfile, fmt, args);
    vprintf(fmt, args);
    va_end(args);
}

#define to_csv_int(x) fprintf(csvfile, "%d,", x)
#define to_csv_uint(x) fprintf(csvfile, "%u,", x)

void event_bodies_create_csv(lev_t* lev, const char* lev_filename) {

    int num_events = lev->num_events;
    event_t* base_event = lev->events;

    char filename[256];
    snprintf(filename, 256, "%s.events.csv", lev_filename);

    FILE* csvfile = fopen(filename, "w");
    fprintf(csvfile, "id,DES0,"
                        "DES4,"
                        "DES8,"
                        "ETA,"
                        "commands,"
                        "command_labels,"
                        "field_18,"
                        "field_1A,"
                        "field_1C,"
                        "field_20,"
                        "field_22,"
                        "field_24,"
                        "xpos,"
                        "ypos,"
                        "field_30,"
                        "field_34,"
                        "field_36,"
                        "field_38,"
                        "field_3A,"
                        "field_3C,"
                        "field_3E,"
                        "xspeed,"
                        "yspeed,"
                        "field_44,"
                        "next_command_index,"
                        "command_par,"
                        "command_par2,"
                        "field_4C,"
                        "field_4E,"
                        "field_50,"
                        "command_par4,"
                        "field_54,"
                        "command_par3,"
                        "field_58,"
                        "field_5A,"
                        "field_5C,"
                        "field_5E,"
                        "obj_type,"
                        "field_62,"
                        "field_63,"
                        "field_64,"
                        "field_65,"
                        "field_66,"
                        "field_67,"
                        "offset_bx,"
                        "offset_by,"
                        "anim_index,"
                        "field_6B,"
                        "subetat,"
                        "etat,"
                        "spawn_subetat,"
                        "spawn_etat,"
                        "curr_command,"
                        "field_71,"
                        "field_72,"
                        "field_73,"
                        "offset_hy,"
                        "follow_sprite,"
                        "hitp,"
                        "spawn_hitp,"
                        "group,"
                        "hit_sprite,"
                        "field_7A,"
                        "field_7B,"
                        "field_7C,"
                        "field_7D,"
                        "field_7E,"
                        "field_7F,"
                        "field_80,"
                        "movement_mode,"
                        "field_82,"
                        "field_83,\n");

    for (int i = 0; i < num_events; ++i) {

        event_t* e = &base_event[i];

        fprintf(csvfile, "%d,", i);
        to_csv_int(e->DES0);
        to_csv_int(e->DES4);
        to_csv_int(e->DES8);
        to_csv_int(e->ETA);
        to_csv_uint(e->commands);
        to_csv_uint(e->command_labels);
        to_csv_int(e->field_18);
        to_csv_int(e->field_1A);
        to_csv_int(e->field_1C);
        to_csv_int(e->field_20);
        to_csv_int(e->field_22);
        to_csv_int(e->field_24);
        to_csv_int(e->xpos);
        to_csv_int(e->ypos);
        to_csv_int(e->field_30);
        to_csv_int(e->field_34);
        to_csv_int(e->field_36);
        to_csv_int(e->field_38);
        to_csv_int(e->field_3A);
        to_csv_int(e->field_3C);
        to_csv_int(e->field_3E);
        to_csv_int(e->xspeed);
        to_csv_int(e->yspeed);
        to_csv_int(e->field_44);
        to_csv_int(e->next_command_index);
        to_csv_int(e->command_par);
        to_csv_int(e->command_par2);
        to_csv_int(e->field_4C);
        to_csv_int(e->field_4E);
        to_csv_int(e->field_50);
        to_csv_int(e->command_par4);
        to_csv_int(e->field_54);
        to_csv_int(e->command_par3);
        to_csv_int(e->field_58);
        to_csv_int(e->field_5A);
        to_csv_int(e->field_5C);
        to_csv_int(e->field_5E);
        to_csv_int(e->obj_type);
        to_csv_int(e->field_62);
        to_csv_int(e->field_63);
        to_csv_int(e->field_64);
        to_csv_int(e->field_65);
        to_csv_int(e->field_66);
        to_csv_int(e->field_67);
        to_csv_int(e->offset_bx);
        to_csv_int(e->offset_by);
        to_csv_int(e->anim_index);
        to_csv_int(e->field_6B);
        to_csv_int(e->subetat);
        to_csv_int(e->etat);
        to_csv_int(e->spawn_subetat);
        to_csv_int(e->spawn_etat);
        to_csv_int(e->curr_command);
        to_csv_int(e->field_71);
        to_csv_int(e->field_72);
        to_csv_int(e->field_73);
        to_csv_int(e->offset_hy);
        to_csv_int(e->follow_sprite);
        to_csv_int(e->hitp);
        to_csv_int(e->spawn_hitp);
        to_csv_int(e->group);
        to_csv_int(e->hit_sprite);
        to_csv_int(e->field_7A);
        to_csv_int(e->field_7B);
        to_csv_int(e->field_7C);
        to_csv_int(e->field_7D);
        to_csv_int(e->field_7E);
        to_csv_int(e->field_7F);
        to_csv_int(e->field_80);
        to_csv_int(e->movement_mode);
        to_csv_int(e->field_82);
        to_csv_int(e->field_83);
        fprintf(csvfile, "\n");
    }

    fclose(csvfile);
}

void destruct_lev(lev_t* lev) {
    free(lev->map_tiles);
    free(lev->x_textures);
    free(lev->x_unk_table);
    free(lev->textures);
    free(lev->event_links);
    free(lev->events);
}


#define process_read(x) fread(&x, sizeof(x), 1, fd);

#define process_read_log_int(x) \
    process_read(x); \
    report(#x " = %d\n", x);

#define process_read_log_array(x) \
    process_read(x); \
    report(#x ": %d bytes read\n", sizeof(x));

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("Usage: raylev [LEV_FILENAME] [...]\n");
        return 0;
    }

    logfile = fopen("raylev.log", "w");

    for (int i = 1; i < argc; ++i) {

        char* filename = argv[i];

        FILE* fd = fopen(filename, "rb");
        if (fd == NULL) {
            perror(filename);
            return -1;
        }

        lev_t lev;

        report("Reading .LEV file '%s'...\n", filename);

        // Header
        report("\nHeader block:\n");
        process_read_log_int(lev.event_block_offset);
        process_read_log_int(lev.texture_block_offset);
        process_read_log_int(lev.level_width);
        process_read_log_int(lev.level_height);
        process_read_log_array(lev.color_tables);
        process_read_log_int(lev.unk_always_2);
        report("current file position is at %d\n", (int) ftell(fd));

        // Map
        printf("\nMap block:\n");
        size_t num_tiles = lev.level_width * lev.level_height;
        lev.map_tiles = (maptile_t*) malloc(num_tiles * sizeof(maptile_t));
        fread(lev.map_tiles, sizeof(maptile_t), num_tiles, fd);
        report("lev.map_tiles: %d tiles read\n", num_tiles);

        process_read_log_int(lev.unk_map_byte);
        process_read_log_int(lev.bg_image);
        process_read_log_int(lev.bg_DES);
        report("current file position is at %d\n", (int) ftell(fd));

        // X Mode Textures
        report("\nX Mode Textures block:\n");
        process_read_log_int(lev.num_x_textures);
        process_read_log_int(lev.x_unk_table_size);

        lev.x_textures = (x_texture_t*) malloc(lev.num_x_textures * sizeof(x_texture_t));
        fread(lev.x_textures, sizeof(x_texture_t), lev.num_x_textures, fd);
        report("lev.x_textures: %d textures read\n", lev.num_x_textures);

        process_read_log_int(lev.x_textures_checksum);
        process_read_log_array(lev.x_indices);

        lev.x_unk_table = (u8*) malloc(lev.x_unk_table_size);
        fread(lev.x_unk_table, lev.x_unk_table_size, 1, fd);
        report("lev.x_unk_table: %d bytes read\n", lev.x_unk_table_size);

        process_read_log_int(lev.x_unk_checksum);
        process_read_log_array(lev.x_unk_offsets);
        report("current file position is at %d\n", (int) ftell(fd));

        // Normal Mode Textures
        report("\nNormal Mode Textures block:\n");
        process_read_log_array(lev.texture_offsets);
        process_read_log_int(lev.num_textures);
        process_read_log_int(lev.num_opaque_textures);
        process_read_log_int(lev.texture_data_size);

        lev.textures = (u8*) malloc(lev.texture_data_size);
        fread(lev.textures, lev.texture_data_size, 1, fd);
        report("lev.textures: %d bytes read\n", lev.texture_data_size);

        process_read_log_int(lev.textures_checksum);
        report("current file position is at %d\n", (int) ftell(fd));

        // Events
        report("\nEvents block:\n");
        process_read_log_int(lev.num_events);
        const u16 num_events = lev.num_events;

        lev.event_links = (u16*) malloc(num_events * sizeof(u16));
        fread(lev.event_links, sizeof(u16), num_events, fd);

        lev.events = (event_t*) malloc(num_events * sizeof(event_t));
        fread(lev.events, sizeof(event_t), num_events, fd);

        event_bodies_create_csv(&lev, filename);

        fclose(fd);
        destruct_lev(&lev);
    }
    fclose(logfile);
    return 0;
}
