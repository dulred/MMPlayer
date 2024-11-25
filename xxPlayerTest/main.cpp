#include "xxPlayer/xxPlayer.h"

int main(){
    xxplayer player("D:/videos/ss.mp4");

    int ret = player.open();

    if (ret)
    {
        printf("Player open fail \n");
        return -1;
    }

    // int64_t duration = 100167000;  // 视频时长为 100167000 微秒
    
    // // 将微秒转换为秒
    // double duration_in_seconds = (double)duration / AV_TIME_BASE;

    // printf("Duration in seconds: %f\n", duration_in_seconds);  // 输出: 100.167000 秒

    


    std::this_thread::sleep_for(std::chrono::seconds(10));

    player.pause();

    std::this_thread::sleep_for(std::chrono::seconds(20));
    // // // // ...wait

    player.play();

    std::this_thread::sleep_for(std::chrono::seconds(20));

    player.seek(10);
  

    std::this_thread::sleep_for(std::chrono::seconds(12000));
    // ...wait

    player.stop();


}