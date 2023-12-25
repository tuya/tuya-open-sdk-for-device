#include "tal_api.h"
#include "tkl_output.h"
#include "tal_cli.h"



int main(int argc, char *argv[])
{
    tal_log_init(TAL_LOG_LEVEL_DEBUG,  1024, (TAL_LOG_OUTPUT_CB)tkl_log_output);
    PR_DEBUG ("hello world\r\n");

    int cnt = 0;
    for (cnt = 0; cnt < 10; cnt++) {
        switch (cnt)
        {
        case 1:
            PR_DEBUG("cnt is %d", cnt);
            break;
        default:
            continue;;
        }

    }

    PR_DEBUG("cnt is %d", cnt);
    while (1) {
        tal_system_sleep(10);
    }
}