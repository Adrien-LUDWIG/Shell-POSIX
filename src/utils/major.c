#include "my_utils.h"

/**
 * @brief Initializes major
 *
 * @return struct major*
 */
struct major *major_init(void)
{
    struct major *new = malloc(sizeof(struct major));
    if (!new)
        errx(1, "major init: could not malloc struct major\n");
    new->options = 0;
    return new;
}

/**
 * @brief Frees struct major
 *
 * @param mj
 */
void major_free(struct major *mj)
{
    free(mj);
}