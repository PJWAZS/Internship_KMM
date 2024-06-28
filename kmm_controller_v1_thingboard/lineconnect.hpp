#include <TridentTD_LineNotify.h>
#define LINE_TOKEN "W8AMbd0oZhIDxuf9OSXHI0patBEqktAKeVvgEM7VB34"

void Line_start() {
  LINE.setToken(LINE_TOKEN);
  LINE.notifySticker("Test",446,1989);
}
