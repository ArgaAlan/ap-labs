int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);


int main()
{
    infof("Hello World %f %s %d %c", 15.5, "hola", 14, 71);
    warnf("Hello World %f %s %d %c", 15.5, "hola", 14, 71);
    errorf("Hello World %f %s %d %c", 15.5, "hola", 14, 71);
    panicf("Hello World %f %s %d %c", 15.5, "hola", 14, 71);
    return 0;
}
