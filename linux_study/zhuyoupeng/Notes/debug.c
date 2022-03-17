
#define DEBUG

#ifndef DEBUG
#define sim_wifi(x...) printf(x)
#else
#define sim_wifi(x...) do {} while(0)
#endif