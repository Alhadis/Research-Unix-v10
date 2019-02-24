#define sendCode(c) sendInt(c)
#define sendCommand(c) sendInt(c)
#define sendUnsignedLong(l) sendUnsignedInt(l)
#define sendLong(l) sendInt(l)
#define sendShort(s) sendInt(s)

int recChar();
int recInt();
Point recPoint();
Rectangle recRectangle();
Texture *recToTexture();
