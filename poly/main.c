// #include <stdio.h>
// #include "poly.h"

// static void poly_test(const char* a, const char* b)
// {
// 	poly_t*		p;
// 	poly_t*		q;
// 	poly_t*		r;

// 	printf("Begin polynomial test of (%s) * (%s)\n", a, b);

// 	p = new_poly_from_string(a);
// 	q = new_poly_from_string(b);

// 	print_poly(p);
// 	print_poly(q);

// 	r = mul(p, q);

// 	printf("Added: ");
// 	print_poly(r);

// 	free_poly(p);
// 	free_poly(q);
// 	free_poly(r);

// 	printf("End polynomial test of (%s) * (%s)\n", a, b);
// }

// int main(void)
// {
// 	poly_test("x^34 - 7x + 1", "3x + 2");
// 	putchar('\n');
// 	// poly_test("x^10000000 + 2", "2x^2 + 3x + 4");

// 	return 0;
// }

#include <stdio.h>
#include "poly.h"

static void poly_test(const char* a, const char* b)
{
	poly_t*		p;
	poly_t*		q;
	poly_t*		r;

	printf("Begin polynomial test of (%s) * (%s)\n", a, b);

	p = new_poly_from_string(a);
	q = new_poly_from_string(b);

	print_poly(p);
	print_poly(q);

	r = mul(p, q);

	print_poly(r);

	free_poly(p);
	free_poly(q);
	free_poly(r);

	printf("End polynomial test of (%s) * (%s)\n", a, b);
}

int main(void)
{
	poly_test("-656x^764 - 847x^732 + 422x^709 + 541x^649 - 934x^569 + 478x^516 + 679x^492 + 797x^425 + 568x^348 + 353x^291 - 469x^206 + 552x^112 + 326x^89 - 737x^42 + 316", "386x^69 + 417");
	// poly_test("x^2 - 7x + 1", "3x + 2");
	// putchar('\n');
	// poly_test("x^10000000 + 2", "2x^2 + 3x + 4");

	return 0;
}
