#include <TridentTD_LineNotify.h>
#define LINE_TOKEN "njQdk3GDFyxywKjw4vMialnlbW776i1KkHqFQfEGNtj"

void Line_start() {
  LINE.setToken(LINE_TOKEN);
  LINE.notifySticker("เปิดระบบการควบคุมอาคารสำนักงาน 1",446,1989);
}