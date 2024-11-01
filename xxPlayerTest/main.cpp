#include "xxPlayer/xxPlayer.h"

int main(){
    xxplayer player("D:/Videos/ss.mp4");

    int ret = player.open();

    if (ret)
    {
        printf("Player open fail \n");
        return -1;
    }

    player.play();
    std::this_thread::sleep_for(std::chrono::seconds(1));


    // ...wait
    player.seek(3);
    std::this_thread::sleep_for(std::chrono::seconds(1));


    player.pause();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // ...wait
    
    // player.play();
    // std::this_thread::sleep_for(std::chrono::seconds(1));

    // player.pause();
    // std::this_thread::sleep_for(std::chrono::seconds(1));

    player.play();
    std::this_thread::sleep_for(std::chrono::seconds(60));
    // ...wait

    player.stop();


}