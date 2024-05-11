#define info(fmt, ...)                                                      \
    do                                                                      \
    {                                                                       \
        fprintf(stderr, "\x1b[1;36m[INFO]:\x1b[0m " fmt "\n", __VA_ARGS__); \
    } while (false)

#define warn(fmt, ...)                                                         \
    do                                                                         \
    {                                                                          \
        fprintf(stderr, "\x1b[1;33m[WARNING]:\x1b[0m " fmt "\n", __VA_ARGS__); \
    } while (false)

#define error(fmt, ...)                                                      \
    do                                                                       \
    {                                                                        \
        fprintf(stderr, "\x1b[1;31m[ERROR]:\x1b[0m " fmt "\n", __VA_ARGS__); \
    } while (false)

#ifdef DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) \
    do                   \
    {                    \
    } while (0)
#endif