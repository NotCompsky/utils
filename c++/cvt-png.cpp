#include <cassert> // assert
#include <iostream>
#include <png.h>

typedef unsigned char uchar;
typedef unsigned int uint32_t;
typedef int int32_t;




void proc_img(char* fp){
	const int32_t n_bitplanes = 8;
	uchar* img_data;
	
	png_color_16p png_bg;
	int32_t png_srgb_intent;
	double png_gamma;
	
	
	
	
	/* Load PNG file into array */
	FILE* png_file = fopen(fp, "rb");
	
	uchar png_sig[8];
	
	fread(png_sig, 1, 8, png_file);
	if (!png_check_sig(png_sig, 8)){
		std::cerr << "Bad signature in file `" << fp << "`" << std::endl;
		abort();
	}
	
	auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	if (!png_ptr)
		// Could not allocate memory
		abort();
  
	auto png_info_ptr = png_create_info_struct(png_ptr);
	
	if (!png_info_ptr){
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		abort();
	}
	
	png_init_io(png_ptr, png_file);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, png_info_ptr);
	
	uint32_t w;
	uint32_t h;
	int32_t bit_depth;
	int32_t colour_type;
	
	png_get_IHDR(png_ptr, png_info_ptr, &w, &h, &bit_depth, &colour_type, NULL, NULL, NULL);
	
	assert(bit_depth == n_bitplanes);
	assert(colour_type == PNG_COLOR_TYPE_RGB);
	
	png_get_bKGD(png_ptr, png_info_ptr, &png_bg);
	png_get_gAMA(png_ptr, png_info_ptr, &png_gamma);
	png_get_sRGB(png_ptr, png_info_ptr, &png_srgb_intent);
	
	uint32_t rowbytes;
	
	png_read_update_info(png_ptr, png_info_ptr);
	
	rowbytes = png_get_rowbytes(png_ptr, png_info_ptr);
	
	assert(png_get_channels(png_ptr, png_info_ptr) == 3);
	
	if ((img_data = (uchar*)malloc(rowbytes*h)) == NULL){
		png_destroy_read_struct(&png_ptr, &png_info_ptr, NULL);
		abort();
	}
	
	uchar* row_ptrs[h];
	for (uint32_t i=0; i<h; ++i)
		row_ptrs[i] = img_data + i*rowbytes;
	
	png_read_image(png_ptr, row_ptrs);
	
	
	
	
	
	
	png_file = fopen(fp, "wb");
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	if (!png_file){
		std::cerr << "Cannot write to file" << std::endl;
		abort();
	}
	if (!png_ptr){
		std::cerr << "png_create_write_struct failed" << std::endl;
		abort();
	}
	
	png_info_ptr = png_create_info_struct(png_ptr);
	
	if (!png_info_ptr){
		std::cerr << "png_create_info_struct failed" << std::endl;
		abort();
	}
	
	if (setjmp(png_jmpbuf(png_ptr))){
		std::cerr << "png_init_io failed" << std::endl;
		abort();
	}
	
	png_init_io(png_ptr, png_file);
	
	if (setjmp(png_jmpbuf(png_ptr))){
		std::cerr << "png_set_IHDR failed" << std::endl;
		abort();
	}
	
	png_set_bKGD(png_ptr, png_info_ptr, png_bg);
	
	std::cout << "png_gamma " << +png_gamma << std::endl;
	
	//png_set_sRGB(png_ptr, png_info_ptr, png_srgb_intent);
	
	png_set_IHDR(png_ptr, png_info_ptr, w, h, n_bitplanes, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	
	png_write_info(png_ptr, png_info_ptr);
	
	if (setjmp(png_jmpbuf(png_ptr))){
		std::cerr << "png_write_image failed" << std::endl;
		abort();
	}
	
	png_write_image(png_ptr, row_ptrs);
	
	if (setjmp(png_jmpbuf(png_ptr))){
		std::cerr << "png_write_end failed" << std::endl;
		abort();
	}
	
	png_write_end(png_ptr, NULL);
}







int main(int argc, char* argv[]){
	for (uint16_t i=1; i<argc; ++i)
		proc_img(argv[i]);
}
