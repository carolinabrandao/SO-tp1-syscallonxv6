static unsigned long next = 1;

int random(void) { // linear congruential generator
    next = next * 1103515245 + 12345;
    return (unsigned)(next / 65536) % 32768;
}
