/*作者::XNY
**引用::Opencv3.49
**平台::x86_64
**系统::Linux/Windows10
**版本::2.5
**时间::2020.2.12
*/
#include "oled_absd.h"

int main(void)
{
    GRAY_DEAL();      /* 灰度处理视屏->二值化图像帧->取点 */
    Pot_buf_deal();   /* 阴码/阳码 */
    print_file();     /* 格式化输出 */

    cout<<"deal finsh!\n";
    getchar();
    return 0;
}
