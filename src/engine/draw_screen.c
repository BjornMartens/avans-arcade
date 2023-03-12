#include "draw_screen.h"

uint8_t hh_world_to_tile(vec2 pos){
	
    FILE* level = fopen("../test/bin/test_map.bin", "rb"); /* open binary file */
    if (!level) { /* check if file opened successfully */
        fprintf(stderr, "Error: Failed to open file.\n");
        return 0;
    }
	int index = (pos.y + pos.x);
	fseek(level, (index * sizeof(int)) + sizeof(int), SEEK_SET);
	int* tile = (int*)malloc(sizeof(int));
   fread(tile, sizeof(int), 1, level); // read 1 tile from binary

	fclose(level);
	int val = *tile;
	free(tile);
	return val;
}


// remeber old value to know which part to update.
vec2 previousViewport = { .x = 0, .y = 0 };
void hh_draw_screen(vec_cor viewport){
	if (viewport.x == previousViewport.x && viewport.y == previousViewport.y) return;
	
	hh_ppu_update_aux((hh_s_ppu_loc_aux){
		.bg_shift_x = viewport.x*HH_PPU_SPRITE_WIDTH,
		.bg_shift_y = viewport.y*HH_PPU_SPRITE_HEIGHT,
		.fg_fetch	= 0,
		.sysreset	= 0,
	});

	// update previous viewport values
	previousViewport = viewport;
}

void hh_setup_screen(){
	//(HH_map_size_X*HH_map_size_Y)
	int size = 3200; // max X = 40 en max Y = 80
	FILE* level = fopen("../test/bin/test_map.bin", "rb"); /* open binary file */
	if (!level) { /* check if file opened successfully */
		fprintf(stderr, "Error: Failed to open file.\n");
		return;
	}
	fseek(level, (0* sizeof(int)) + sizeof(int), SEEK_SET);
	int* tile = (int*)malloc(size*sizeof(int));
   fread(tile, sizeof(int), size, level); // read 1 tile from binary

	fclose(level);

	for(int BAM_index = 0; BAM_index < size; BAM_index++){
		hh_ppu_update_background(BAM_index, (hh_s_ppu_loc_bam_entry){
			.horizontal_flip = false,
			.vertical_flip   = false,
			.palette_index   = tile[BAM_index]+1,
			.tilemap_index   = tile[BAM_index],
		});
	}
	free(tile);
}