#define TO_SRGB(_Linear_Color) (pow(_Linear_Color, 1.0 / 2.2))
#define TO_LINEAR(_SRGB_Color) (pow(_SRGB_Color, 2.2))