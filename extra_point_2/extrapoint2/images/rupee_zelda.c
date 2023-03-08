static const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[10 * 18 * 2 + 1];
} rupee_zelda = {
  10, 18, 2,
  "\377\377\377\377\377\377\377\377\000\000\000\000\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\000\000\377\377\242\017\000\000\377\377\377\377\377\377\377"
  "\377\377\377\000\000\377\377\377\377\242\017\242\017\000\000\377\377\377\377\377\377"
  "\000\000\377\377\377\377\377\377\242\017\242\017\242\017\000\000\377\377\000\000\377\377"
  "\242\017\377\377\242\017\000\000\242\017\000\000\242\017\000\000\000\000\377\377\377\377\242"
  "\017\242\017\242\017\000\000\242\017\242\017\000\000\000\000\377\377\377\377\242\017\242\017"
  "\242\017\000\000\242\017\242\017\000\000\000\000\377\377\377\377\242\017\242\017\242\017\000"
  "\000\242\017\242\017\000\000\000\000\377\377\377\377\242\017\242\017\242\017\000\000\242\017"
  "\242\017\000\000\000\000\377\377\377\377\242\017\242\017\242\017\000\000\242\017\242\017\000"
  "\000\000\000\377\377\377\377\242\017\242\017\242\017\000\000\242\017\242\017\000\000\000\000\377"
  "\377\377\377\242\017\242\017\242\017\000\000\242\017\242\017\000\000\000\000\377\377\242\017"
  "\377\377\242\017\242\017\000\000\242\017\242\017\000\000\000\000\242\017\242\017\242\017\377"
  "\377\000\000\377\377\000\000\242\017\000\000\377\377\000\000\242\017\242\017\242\017\242\017\242"
  "\017\242\017\000\000\377\377\377\377\377\377\000\000\242\017\242\017\242\017\242\017\000"
  "\000\377\377\377\377\377\377\377\377\377\377\000\000\242\017\242\017\000\000\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\000\000\000\000\377\377\377\377"
  "\377\377\377\377",
};
