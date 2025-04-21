#include <IOS.hpp>

void setDiscordPresence() {

    int fd = IOS_Open("/dev/dolphin", 0);
    if (fd < 0)
		return;

    ioctlv clientVector[1] = { 0 };
    char clientID[] = "1315834746445758465";
    clientVector[0].data = clientID;
    clientVector[0].len = sizeof(clientID);
    int setclient = IOS_Ioctlv(fd, 0x7, 1, 0, clientVector);
	
    ioctlv statusVector[10] = { 0 };

    char state[] = "Dolphin Emulator";
	char details[] = "THE New Super Mario Bros. Wii Mod";

	char largeImageKey[] = "realism";
	char largeImageText[] = "Realism and the Freelous Worlds";

    char smallImageKey[] = "dolphinicon";
    char smallImageText[] = "Dolphin is an emulator for the Gamecube and the Wii.";

    statusVector[0].data = details;
    statusVector[0].len = sizeof(details);
    statusVector[1].data = state;
	statusVector[1].len = sizeof(state);
    statusVector[2].data = largeImageKey;
    statusVector[2].len = sizeof(largeImageKey);
    statusVector[3].data = largeImageText;
    statusVector[3].len = sizeof(largeImageText);
    statusVector[4].data = smallImageKey;
	statusVector[4].len = sizeof(smallImageKey);
	statusVector[5].data = smallImageText;
	statusVector[5].len = sizeof(smallImageText);
    u64 number0 = 0;
	statusVector[6].data = &number0;
	statusVector[7].data = &number0;
    statusVector[8].data = &number0;
	statusVector[9].data = &number0;
	
	int setpresence = IOS_Ioctlv(fd, 0x8, 10, 0, (void *)statusVector);

    return;
}